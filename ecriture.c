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
#include "ecriture.h"

void afficherHeader(Elf32 *e){
	int i;
	//Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
	printf("e:  ");
	for(i=0;i<EI_NIDENT;i++){
		printf("%02x ",(e->header)->e_ident[i]);
	}	puts(" ");
	//header->e_ident = h->e_ident;
	printf("%02x ",(e->header)->e_type);
	printf("%02x ",(e->header)->e_machine);
	printf("%02x ",(e->header)->e_version);
	printf("%02x ",(e->header)->e_entry);
	printf("%02x ",(e->header)->e_phoff);
	printf("%02x ",(e->header)->e_shoff);
	printf("%02x ",(e->header)->e_flags);
	printf("%02x ",(e->header)->e_ehsize);
	printf("%02x ",(e->header)->e_phentsize);
	printf("%02x ",(e->header)->e_phnum);
	printf("%02x ",(e->header)->e_shentsize);
	printf("%02x ",(e->header)->e_shnum);
	printf("%02x ",(e->header)->e_shstrndx);
}

void afficherSectionTable(Elf32 *e){
	int i;
	//Elf32_Shdr *section_header_start = e->table_section;
	for(i=0;i<e->nb_Section;i++){
		printf("%02x ",(e->table_section + i)->sh_name);
		printf("%08x ",(e->table_section + i)->sh_type);
		printf("%02x ",(e->table_section + i)->sh_flags);
		printf("%02x ",(e->table_section + i)->sh_addr);
		printf("%08x ",(e->table_section + i)->sh_offset);
		printf("%02x ",(e->table_section + i)->sh_size);
		printf("%02x ",(e->table_section + i)->sh_link);
		printf("%02x ",(e->table_section + i)->sh_info);
		printf("%02x ",(e->table_section + i)->sh_addralign);
		printf("%02x \n",(e->table_section + i)->sh_entsize);
	}	
}

void afficherSectionSymbole(Elf32 *e){
	int i;
	for (i = 0; i < e->nb_Symbole; i++){
		printf("%02x ",(e->table_symbole + i)->st_name);
		printf("%02x ",(e->table_symbole + i)->st_value);
		printf("%02x ",(e->table_symbole + i)->st_size);
		printf("%02x ",(e->table_symbole + i)->st_info);
		printf("%02x ",(e->table_symbole + i)->st_other);
		printf("%08x \n",(e->table_symbole + i)->st_shndx);
	}
}