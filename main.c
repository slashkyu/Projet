#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include "interface.h"
#include "lecture.h"
#include "ecriture.h"
#include "fusion.h"

int main(int argc, char* argv[])
{
	Elf32 *e2;
	Elf32 *e;

	Elf32_Ehdr *data;
	struct stat file_info; 
	int file_descriptor = open(argv[1], O_RDONLY);
	Elf32_Ehdr *data2;
	struct stat file_info2;
	int file_descriptor2 = open(argv[2], O_RDONLY);

	if (fstat(file_descriptor, &file_info))
	{ 
		exit(1);
	}
	//Verification du chargement en memoire du fichier
	//void *mmap(void *addr, size_t len, int protection, int flags, int fildes, off_t off);
	else if ((data = get_header(file_info, file_descriptor)) == MAP_FAILED)
	{
		exit(1);
	}
	else 
	{	
		e = initELF(data);

		afficherSectionTable(e);
		e = fusionSectionSize(e,e);
		e = fusionSectionOffset(e);
		afficherSectionTable(e);

		libererELF(e);
		//libererELF(e2);
		munmap(data, file_info.st_size);
	}

	//afficherSectionTable(e);
	//afficherSectionTable(e2);
	

	

	
	close(file_descriptor);
	//close(file_descriptor2);
	
	//Fermeture du fichier
	
	return (0);

}
