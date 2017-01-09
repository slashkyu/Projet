#ifndef __FUSION_PAQ_H__
#define __FUSION_PAQ_H__

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>





// STRUCTURE DE STOCKAGE D'INFORMATIONS
typedef struct Elf_Structure{

		Elf32_Ehdr *header;
		
		//pointeur sur la table des sections
		Elf32_Shdr *table_section;

		//pointeur sur la table symbole
		El32_Sym *table_Symbole;

		//pointeur sur string table
		char* string_table;
}Elf_stck;

void fusion_section_table(Elf32_Ehdr * data1, Elf32_Ehdr *data2);

