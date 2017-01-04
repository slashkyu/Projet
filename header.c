#include <stdio.h>
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
	printf("  Magique: ");
	for (i = 0; i < 16; i++){
		printf("%02x ", data->e_ident[i]);
	}
	
	if (data->e_ident[4] == 1)
		puts("\n  Classe:                             ELF32");
	else if (data->e_ident[4] == 2)
		puts("  Classe:                             ELF64");
	if (data->e_ident[5] == 1)
		puts("  Données:                            complément à 2, système à octets de poids faible d'abord (little endian)");
	else if (data->e_ident[5] == 2)
		puts("  Données:                            système à octets de poids fort d'abord (big endian)");

	printf("  Version:                            %x\n", data->e_ident[6]);
	printf("  OS/ABI:                             %d\n", data->e_ident[7]);
	printf("  Version ABI:                        %d\n", data->e_ident[8]);

	printf("  Type:                               ");
	switch(data->e_type) {
   		case 1  :
      			puts("Fichier repositionnable"); break;
   		case 2  :
      			puts("Fichier exécutable"); break;
  		case 3  :
      			puts("Objet partagé"); break;
  		case 4  :
      			puts("Fichier core"); break;
	}
	printf("  Code machine:                       %d\n", data->e_machine);
	printf("  Version:                            0x%0x\n", data->e_version);
	printf("  Adresse du point d'entrée:          0x%0x\n", data->e_entry);
  printf("  Début des en-têtes de programme:   %4d (octets dans le fichier)\n", data->e_phoff);
  printf("  Début des en-têtes de section:     %4d (octets dans le fichier)\n", data->e_shoff);
  printf("  Fanions:                            0x%0x\n", data->e_flags);
  printf("  Taille de cet en-tête:              %d (bytes)\n", data->e_ehsize);
  printf("  Taille de l'en-tête du programme:   %d (bytes)\n", data->e_phentsize);
  printf("  Nombre d'en-tête du programme:      %d\n", data->e_phnum);
  printf("  Taille des en-têtes de section:     %d (bytes)\n", data->e_shentsize);
  printf("  Nombre d'en-têtes de section:       %d\n", data->e_shnum);
  printf("  Table d'indexes des chaînes d'en-tête de section: %d\n", data->e_shstrndx);

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
					print_info(data);	//traitement et affichage du header
				}
				munmap(data, file_info.st_size);	//Unmapping du fichier en mémoire
			}
			close(file_descriptor);	//Fermeture du fichier
		}
	}
	return (0);
}

