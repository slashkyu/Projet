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
	printf("-----HEADER-----\n");
	for(i=0;i<EI_NIDENT;i++){
		printf("%02x ",(e->header)->e_ident[i]);
	}	puts(" ");
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
	printf("-----SYMBOLE-----\n");
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


void afficherSectionRel(Elf32 *e){
	printf("-----REL-----\n");
	int i;
	for (i = 0; i < e->nb_Rel; i++){
		printf("r2: %02x ",(e->table_rel + i)->r_offset);
		printf("%02x \n",(e->table_rel + i)->r_info);
	}
}
/*
void afficherSectionRela(Elf32 *e){
	int i;
	for (i = 0; i < e->nb_Rela; i++){
		printf("%02x ",(e->table_rela + i)->r_offset);
		printf("%02x ",(e->table_rela + i)->r_info);
		printf("%02x ",(e->table_rela + i)->r_addend);
	}
}
*/

