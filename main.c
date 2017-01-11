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


int main(int argc, char* argv[])
{
	Elf32_Ehdr *data;
	struct stat file_info; Elf32 E;
	int file_descriptor = open(argv[1], O_RDONLY);

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
		
		//printf("%02x\n",E.header->e_flags);
		//E.table_section = get_section_table(E.header);
		//printf("%02x\n",E.table_section->sh_offset);
		//initHeader(data);
		Elf32 *e = initELF(data);
		afficherHeader(initELF(data));
		afficherSectionTable(initELF(data));
		//get_symbole_table(data);
		//afficherSectionSymbole(Elf32 *e)
		afficherSectionSymbole(e);
		//get_sectionT(data,2);
		//get_sectionN(data,".symtab");
		libererELF(e);
		//get_symbole_table(data);
		//get_header_section(data,2);
		//get_section_table(initELF(data)->header);
		//get_section_nb(initELF(data)->header);
		//puts("AAAAAAAAA");
		//header(data);							
		//Unmapping du fichier en mémoire
		munmap(data, file_info.st_size);
	}
	//Fermeture du fichier
	close(file_descriptor);
	return (0);

}
