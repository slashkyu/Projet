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
	printf("  OS/ABI:                             %d\n", data->e_ident[7]);// switch case
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
  //int fd;
  Elf32_Ehdr *data;
  //struct stat file_infos;
	///////////////////////////////////////////////////////
	char err_args [] = "USAGE: executable <binary_file>";
	char err_open [] = "Ouverture du fichier impossible";
	char err_rec [] = "Recuperation des informations du fichier impossible";
	char err_mem [] = "Chargement du fichier en memoire impossible";
	char err_elf [] = "Pas format propre";
	char err_arch [] = "Mauvais architecture";
	//struct stat { 
        //       dev_t     st_dev;     /* ID of device containing file */ 
        //       ino_t     st_ino;     /* inode number */ 
        //       mode_t    st_mode;    /* protection */ 
        //       nlink_t   st_nlink;   /* number of hard links */ 
        //       uid_t     st_uid;     /* user ID of owner */ 
        //       gid_t     st_gid;     /* group ID of owner */ 
        //       dev_t     st_rdev;    /* device ID (if special file) */ 
        //       off_t     st_size;    /* total size, in bytes */ 
        //       blksize_t st_blksize; /* blocksize for file system I/O */ 
        //       blkcnt_t  st_blocks;  /* number of 512B blocks allocated */ 
        //       time_t    st_atime;   /* time of last access */ 
        //       time_t    st_mtime;   /* time of last modification */ 
        //       time_t    st_ctime;   /* time of last status change */ 
        //   };	
	struct stat file_info;
	int file_descriptor;


	if (argc != 2)
	{
		puts(err_args);
	}
	else
	{
		//int open(const char *path, int oflags);
		//Returns the file descriptor for the new file. 
		//The file descriptor returned is the smallest integer > 0 that is still available. 
		//If a negative value is returned, then there was an error opening the file.
		if ((file_descriptor = open(argv[1], O_RDONLY)) <= 0)
		{
			puts(err_open);
		}
		else
		{
			//int fstat(int fd, struct stat *buf);
			//printf("%d\n",fstat(file_descriptor, &file_info)); //-> 0
			if (fstat(file_descriptor, &file_info) == -1)
			{
				puts(err_rec);
			}
			//void *mmap(void *addr, size_t len, int protection, int flags, int fildes, off_t off);
			else if ((data = mmap(0, file_info.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0)) == MAP_FAILED)
			{
				puts(err_mem);
			}
			else
			{
				//puts("POINT 1");							
				if (check_elf(data))
				{
					puts("POINT 2");puts(err_elf);puts("POINT 3");
				}
				else if (check_arch(data))
				{
					puts("POINT 4");puts(err_arch);puts("POINT 5");
				}
				else
				{
					//puts("POINT 6");
					print_info(data);//puts("POINT 7");
				}
				munmap(data, file_info.st_size);
			}
			close(file_descriptor);
		}
	}
	return (0);
}
	///////////////////////////////////////////////////////

