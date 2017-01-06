#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>

// Vérification des 4 premiers octets du nombre magique ("7f 45 4c 46")
int check_elf(Elf32_Ehdr * data)
{
  if (data->e_ident[EI_MAG0] != ELFMAG0 ||
      data->e_ident[EI_MAG1] != ELFMAG1 ||
      data->e_ident[EI_MAG2] != ELFMAG2 ||
      data->e_ident[EI_MAG3] != ELFMAG3)
    return (-1);

  return (0);
}

// Vérification de l'architecture 32bits
int check_arch(Elf32_Ehdr * data)
{
  if (data->e_ident[EI_CLASS] != 1)
    return (-1);

  return (0);
}

// Procédure d'affichage des infos du header ELF
void print_info(Elf32_Ehdr * data)
{
	int i;
	puts("En-tête ELF:");
	printf("  Magique:   ");
	for (i = 0; i < 16; i++)
		printf("%02x ", data->e_ident[i]);
	puts(" ");
	if (data->e_ident[4] == 1)
		puts("  Classe:                            ELF32");
	else if (data->e_ident[4] == 2)
		puts("  Classe:                            ELF64");
	else if (data->e_ident[4] == 0)
		puts("  Classe:                          Aucun");
	if (data->e_ident[5] == 1)
		puts("  Données:                          complément à 2, système à octets de poids faible d'abord (little endian)");
	else if (data->e_ident[5] == 2)
		puts("  Données:                          système à octets de poids fort d'abord (big endian)");
	else if (data->e_ident[5] == 0)
		puts("  Données:                          Aucun");	
	if (data->e_ident[6] == 1)
		puts("  Version:                           1 (current)");
	else if (data->e_ident[6] == 0)
		puts("  Version:                           0 (aucun)");
	printf("  OS/ABI:                            ");
	switch(data->e_ident[7]) {
   		case 0  :
      			puts("UNIX - System V"); break;
   		case 1  :
      			puts("HP-UX"); break;
   		case 2  :
      			puts("NetBSD"); break;
   		case 3  :
      			puts("Linux"); break;
   		case 6  :
      			puts("Sun Solaris"); break;
   		case 7  :
      			puts("IBM AIX"); break;
   		case 8  :
      			puts("SGI IRIX"); break;
   		case 9  :
      			puts("FreeBSD"); break;
   		case 10  :
      			puts("Compaq TRU64"); break;
   		case 11  :
      			puts("Novell Modesto"); break;
   		case 12  :
      			puts("OpenBSD"); break;
   		case 13  :
      			puts("OpenVMS"); break;
   		case 14  :
      			puts("NonStop Kernel"); break;
   		case 15  :
      			puts("AROS"); break;
   		case 16  :
      			puts("Fenix OS"); break;
   		case 17  :
      			puts("CloudABI"); break;
   		case 64  :
      			puts("ARM EABI"); break;
   		case 83  :
      			puts("Sortix"); break;
   		case 97  :
      			puts("ARM"); break;
   		case 255  :
      			puts("Standalone"); break;
   		default  :
      			printf("Autre avec le code %x\n", data->e_ident[7]); break;
	}
	printf("  Version ABI:                       %d\n", data->e_ident[8]);
	printf("  Type:                              ");
	switch(data->e_type) {
   		case 1  :
      			puts("Repositionable"); break;
   		case 2  :
      			puts("Executable"); break;
  		case 3  :
      			puts("Objet partagé"); break;
  		case 4  :
      			puts("Fichier Core"); break;
		case 0  :
      			puts("Aucun"); break;
   		default  :
      			puts("Erreur"); break;
	}
	printf("  Machine:                           ");
	switch(data->e_machine) {
   		case 0  :
      			puts("Aucune"); break;
   		case 2  :
      			puts("SPARC"); break;
   		case 3  :
      			puts("Intel 80386"); break;
   		case 4  :
      			puts("Motorola 68000"); break;
   		case 7  :
      			puts("Intel i860"); break;
   		case 8  :
      			puts("MIPS I"); break;
   		case 19  :
      			puts("Intel i960"); break;
   		case 20  :
      			puts("PowerPC"); break;
   		case 40  :
      			puts("ARM"); break;
   		case 62  :
      			puts("x64"); break;
   		default  :
      			printf("Autre avec le code %d\n", data->e_machine); break;
	}
	printf("  Version:                           0x%x\n", data->e_version);
	printf("  Adresse du point d'entrée:         0x%0x\n", data->e_entry);
	if (data->e_phoff == 0)
		printf("  Début des en-têtes de programme:   0 (octets dans le fichier)\n");
	else
		printf("  Début des en-têtes de programme:         %d (octets dans le fichier)\n", data->e_phoff);
	if (data->e_shoff == 0)
		printf("  Début des en-têtes de section:    0 (octets dans le fichier)\n");
	else
		printf("  Début des en-têtes de section:         %d (octets dans le fichier)\n", data->e_shoff);
  	printf("  Fanions:                           0x%0x\n", data->e_flags);
  	printf("  Taille de cet en-tête:             %d (bytes)\n", data->e_ehsize);
  	printf("  Taille de l'en-tête du programme:  %d (bytes)\n", data->e_phentsize);
  	printf("  Nombre d'en-tête du programme:     %d\n", data->e_phnum);
  	printf("  Taille des en-têtes de section:    %d (bytes)\n", data->e_shentsize);
  	printf("  Nombre d'en-têtes de section:      %d\n", data->e_shnum);
  	printf("  Table d'indexes des chaînes d'en-tête de section: %d\n", data->e_shstrndx);

}



void print_section_table(Elf32_Ehdr * data)
{
  int i;

  printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x:\n",data->e_shnum, data->e_shoff);

  // on recupere un pointeur sur le premier header de section, avec le offset correspondant
  Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);


  // on retrouve la string table avec le nom des sections ..
  Elf32_Shdr sections_string = section_header_start[data->e_shstrndx];
  
  // on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
  char *strings = (char*)((void*)data + sections_string.sh_offset);

  // pour stoquer chaque section
  Elf32_Shdr section_header;

  for (i = 0; i < data->e_shnum; i++){
  	// on recupere le header de section courrant
    section_header = section_header_start[i];
	


		printf("[%d]\n",i);

    // on affiche les infos de la section
    printf("Nom : %s\nType : ", strings + section_header.sh_name);	//nom
    
    if(section_header.sh_type == 0){	//type
    	printf("NULL");
    }
    else if(section_header.sh_type == 1){
  	printf("PROGBITS");
    }
    else if(section_header.sh_type == 2){
    	printf("SYMTAB");
    }
    else if(section_header.sh_type == 3){
     	printf("STRTAB");
    }
    else if(section_header.sh_type == 4){
     	printf("RELA");
    }
    else if(section_header.sh_type == 5){
     	printf("HASH");
    }
    else if(section_header.sh_type == 6){
     	printf("DYNAMIC");
    }
    else if(section_header.sh_type == 7){
     	printf("NOTE");
    }
    else if(section_header.sh_type == 8){
     	printf("NOBITS");
    }
    else if(section_header.sh_type == 9){
     	printf("REL");
    }
    else if(section_header.sh_type == 10){
     	printf("SHLIB");
    }
    else if(section_header.sh_type == 11){
    	printf("DYNSYM");
    }
    else if(section_header.sh_type >= 0x70000000 && section_header.sh_type <= 0x7fffffff){
     	printf("PROCSPECSEM");
    }  //Correspond à "processor specific semantic"  
    else if(section_header.sh_type >= 0x80000000 && section_header.sh_type <= 0xffffffff){
     	printf("APPINDEX");
    }
    
    printf("\nFanions : ");	//flags
    if (section_header.sh_flags & (1u << 0)){	//écriture
    	printf("W ");
    }
    if (section_header.sh_flags & (1u << 1)){	//allocation
    	printf("A ");
    }
    if (section_header.sh_flags & (1u << 2)){	//exécution
    	printf("X ");
    }
    if (section_header.sh_flags & (1u << 4)){	//fusion
    	printf("M ");
    }
    if (section_header.sh_flags & (1u << 5)){	//chaînes
    	printf("S ");
    }
    if (section_header.sh_flags & (1u << 6)){	//info
    	printf("I ");
    }
    if (section_header.sh_flags & (1u << 7)){	//ordre des liens
    	printf("L ");
    }
    if (section_header.sh_flags & (1u << 8)){	//os nonconforming
    	printf("0 ");
    }
    if (section_header.sh_flags & (1u << 9)){	//groupe
    	printf("G ");
    }
    if (section_header.sh_flags & (1u << 10)){	//TLS
    	printf("T ");
    }
    printf("\nAdresse : 0x%08x", section_header.sh_addr);	//adresse
    
    printf("\nDécalage : 0x%06x", section_header.sh_offset);	//offset
    
    printf("\nTaille : 0x%06x", section_header.sh_size); //taille
    
    printf("\nLien : %d", section_header.sh_link);	//Lien
    
    printf("\nInfo : %d", section_header.sh_info);	//Info
    
    printf("\nAlignement d'adresse : ");
    if (section_header.sh_addralign != 0 && section_header.sh_addralign != 1){
    	printf("%d", section_header.sh_addralign); //Alignement adresse
    }
    else{
    	printf("non");
    }
    
    printf("\nEntrée de taille fixe : ");
    if (section_header.sh_entsize != 0){
    	 printf("%d (octets)\n\n", section_header.sh_entsize);
    }
    else{
    	printf("non\n\n");
    }
  }
}

void print_section(Elf32_Ehdr * data)
{
  int i, section_int = -1;
  unsigned char hex; //%hu

  ////////////////////////
	char section_string[20];
	puts("Entrez section name : ");	
	scanf("%s",section_string);
	//section_string = ".symtab";
	


  ////////////////////////

  // on recupere un pointeur sur le premier header de section, avec le offset correspondant
  Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);

  // on retrouve la string table avec le nom des sections ..
  Elf32_Shdr sections_string = section_header_start[data->e_shstrndx];
  
  // on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
  char *strings = (char*)((void*)data + sections_string.sh_offset);

  // pour stoquer chaque section
  Elf32_Shdr section_header;
  
  //lecture de la section choisie
  
  if(section_string[0] == '0'){
  	section_int=0;
  	section_header = section_header_start[section_int];
  }  
  else if(atoi(section_string) < data->e_shnum && atoi(section_string) > 0){	//si la section est choisie par son numéro
  	section_int = atoi(section_string);
  	section_header = section_header_start[section_int];
  }
  else{	//si la section est choisie par son nom
  	i=1;
  	section_header = section_header_start[i];
  	while (strstr(strings + section_header.sh_name, section_string) == NULL && i < data->e_shnum){
  		i++;
  		section_header = section_header_start[i];
	  }  	
	  if (i < data ->e_shnum){
	  	section_int = i;
	  }
  }
  
  if (section_int == -1){	//erreur d'argument
  	printf("Il n'y a pas de section %s dans ce fichier.\n", section_string);
  }
	else{
		if (section_header.sh_size == 0){	//La section a une taille vide
			printf("La section \" %s \" n'a pas de données à être videngé.\n",strings + section_header.sh_name);
		}
		else{	//hexdump de la section
			printf("Vidange hexadécimale de la section \" %s \":\n",strings + section_header.sh_name);
			i=0;
			while(i < section_header.sh_size){
				if (i % 4 == 0){	//Formatage
					printf(" ");
				}
				printf("%x", section_header.sh_offset+i);	//On souhaite afficher le contenu, or on a ici les adresses
				i++;
				if (i % 16 == 0){	//Formatage
					printf("\n");
				}
			}			
		}
	}	
}


//ETAPES 1_2_3
void process_etapes(int file_descriptor, char sous_option)
{


	Elf32_Ehdr *data;
	char err_rec [] = "Recuperation des informations du fichier impossible";
	char err_mem [] = "Chargement du fichier en memoire impossible";
	char err_elf [] = "Pas format propre";
	char err_arch [] = "Mauvais architecture";

	struct stat file_info;

	//Verification de récupartion des infos du fichier
	//int fstat(int fd, struct stat *buf);
	//printf("%d\n",fstat(file_descriptor, &file_info)); //-> 0
	if (fstat(file_descriptor, &file_info) == -1)
	{
		puts(err_rec);
	}
	//Verification du chargement en memoire du fichier
	//void *mmap(void *addr, size_t len, int protection, int flags, int fildes, off_t off);
	else if ((data = mmap(0, file_info.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0)) == MAP_FAILED)
	{
		puts(err_mem);
	}
	else
	{							
		//Verification fichier ELF
		if (check_elf(data))
		{
			puts(err_elf);
		}
		//Verification fichier 32bits
		else if (check_arch(data))
		{
			puts(err_arch);
		}
		//Affichage des etapes
		else
		{
			
		//STRUCTURE DE STOCKAGE
		//Elf.header = data



			switch (sous_option)
				{
					case 'a' : //all
					case 'b' : //header
						print_info(data);
						if (sous_option!='a')
							break;
					case 'c' : //section table
						print_section_table(data);
						if (sous_option!='a')
						break;
					case 'd' : //section
						print_section(data);
						if (sous_option!='a')
							break;
				}			
			#if 0	//SOUS-option est lu depuis stdin!
			char sous_option;
			char mess_sopt [] = "Choisissez un sous-otption : \na - all\nb - header\nc - section table\nd - section";
			char err_sopt [] = "Sous-option mal choisi!";
			while (1)
			{
				puts(mess_sopt);
				scanf(" %c", &sous_option);
				switch (sous_option)
				{
					case 'a' : //all
					case 'b' : //header
						print_info(data);
						if (sous_option!='a')
							break;
					case 'c' : //section table
						print_section_table(data);
						if (sous_option!='a')
						break;
					case 'd' : //section
						print_section(data);
						if (sous_option!='a')
							break;
					default : //erreur
						puts(err_sopt);
						if (sous_option!='a')
							continue;
				}
				break;
			}
			#endif
		}
		//Unmapping du fichier en mémoire
		munmap(data, file_info.st_size);
	}
	//Fermeture du fichier
	close(file_descriptor);

}
