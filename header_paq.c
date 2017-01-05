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
      			printf("Autre avec le code %x\n", data->e_ident[6]); break;
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


void process_header(int file_descriptor)
{
  	Elf32_Ehdr *data;
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
		//Affichage du header
		else
		{
			print_info(data);
		}
		//Unmapping du fichier en mémoire
		munmap(data, file_info.st_size);
	}
	//Fermeture du fichier
	close(file_descriptor);

}

