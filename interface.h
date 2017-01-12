#ifndef __INTERFACE__H__
#define __INTERFACE__H__

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
		Elf32_Sym *table_symbole;
		
		Elf32_Rel *table_rel;

		Elf32_Rela *table_rela;
		//pointeur sur string table de sectioin
		char* string_table_section;
		//pointeur sur string table de symbole
		char* string_table_symbole;
		unsigned char* contenue_section[2048];

		Elf32_Shdr *s_h_s;//Secion Header Start pour void afficherSection(Elf32 *e)	
		int nb_Section;

		int nb_Symbole;

		int nb_Rel;

		int nb_Rela;

		int relE;

		int relaE;
}Elf32;

#endif
