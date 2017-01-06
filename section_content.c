#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void print_section(Elf32_Ehdr * data, char *section_string)
{
  int i, section_int = -1;

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
			unsigned char *content = (unsigned char*)((void*)data + section_header.sh_offset);
			i=0;
			printf("Vidange hexadécimale de la section \" %s \":\n  Ox%08x",strings + section_header.sh_name,i);
			while(i < section_header.sh_size){	//boucle de récupération du contenu
				if (i % 4 == 0){	//Formatage
					printf(" ");
				}
				printf("%02x", *(content+i));
				i++;
				if (i % 16 == 0 && i < section_header.sh_size){	//Formatage
					printf("\n  Ox%08x",i);
				}
			}
			printf("\n");			
		}
	}	
}

int main(int argc, char *argv[])
{
	Elf32_Ehdr *data;
	char err_args [] = "USAGE: executable <binary_file> <num|name section>";
	char err_open [] = "Ouverture du fichier impossible";
	char err_rec [] = "Recuperation des informations du fichier impossible";
	char err_mem [] = "Chargement du fichier en memoire impossible";
	char err_elf [] = "Fichier au format non ELF";
	char err_arch [] = "Fichier à l'architecture non 32bits";
	
	struct stat file_info;
	int file_descriptor;

	if (argc != 3){	//Verification nb arguments
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
					print_section(data, argv[2]);	//traitement et affichage du header
				}
				munmap(data, file_info.st_size);	//Unmapping du fichier en mémoire
			}
			close(file_descriptor);	//Fermeture du fichier
		}
	}
	return (0);
}
