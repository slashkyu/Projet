#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>

int check_elf(Elf32_Ehdr * data)
{
  
  if (data->e_ident[EI_MAG0] != ELFMAG0 ||
      data->e_ident[EI_MAG1] != ELFMAG1 ||
      data->e_ident[EI_MAG2] != ELFMAG2 ||
      data->e_ident[EI_MAG3] != ELFMAG3)
    return (-1);

  return (0);
}

int check_arch(Elf32_Ehdr * data)
{
  if (data->e_ident[EI_CLASS] != 1)
    return (-1);

  return (0);
}

/*
typedef struct {
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint32_t   sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    uint32_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint32_t   sh_addralign;
    uint32_t   sh_entsize;
} Elf32_Shdr;*/

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
    printf("Nom : %s\nType :", strings + section_header.sh_name);	//nom
    
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
    printf("\nAdresse : 0x%08x", section_header.sh_addr);
    printf("\nDécalage : 0x%06x\n", section_header.sh_offset);
    
  }
}

int main(int argc, char *argv[])
{
	Elf32_Ehdr *data;
	char err_args [] = "USAGE: executable <binary_file>";
	char err_open [] = "Ouverture du fichier impossible";
	char err_rec [] = "Recuperation des informations du fichier impossible";
	char err_mem [] = "Chargement du fichier en memoire impossible";
	char err_elf [] = "Fichier au format non ELF";
	char err_arch [] = "Fichier à l'architecture non 32bits";
	
	struct stat file_info;
	int file_descriptor;

	if (argc != 2){//Verification nb arguments
		puts(err_args);
	}
	else{
		if ((file_descriptor = open(argv[1], O_RDONLY)) <= 0){	//Verification d'ouverture du fichier
			puts(err_open);
		}
		else{
			if (fstat(file_descriptor, &file_info) == -1){	//Verification de récupartion des infos du fichier
				puts(err_rec);
			}
			else if ((data = mmap(0, file_info.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0)) == MAP_FAILED){	//Verification du chargement en memoire du fichier
				puts(err_mem);
			}
			else{				
				if (check_elf(data))	//Verification fichier ELF
				{
					puts(err_elf);
				}
				else if (check_arch(data))	//Verification fichier 32bits
				{
					puts(err_arch);
				}
				else
				{
					print_section_table(data);	//traitement et affichage du header
				}
				munmap(data, file_info.st_size);	//Unmapping du fichier en mémoire
			}
			close(file_descriptor);	//Fermeture du fichier
		}
	}
	return (0);
}
