#ifndef __INTERFACE__H__
#define __INTERFACE__H__

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>





// STRUCTURE DE STOCKAGE D'INFORMATIONS de fichier 
typedef struct Elf_Structure{
		//pointeur sur l'en-tête
		Elf32_Ehdr *header;
		//pointeur sur la table des sections
		Elf32_Shdr *table_section;
		//pointeur sur la table symbole
		Elf32_Sym *table_symbole;
		//pointeur sur la table de relocation de type rel
		Elf32_Rel *table_rel;
		//pointeur sur la table de relocation de type rela
		Elf32_Rela *table_rela;
		//pointeur sur string table de sections
		char* string_table_section;
		//pointeur sur string table de symboles
		char* string_table_symbole;
		//pointeur sur string table de contenu des sections
		unsigned char* contenue_section[1024];
		//pointeur sur Secion Header Start
		Elf32_Shdr *s_h_s;
		//Nobre de sections
		int nb_Section;
		//Nombre de symboles
		int nb_Symbole;
		//Nombre de symboles dans la table de relocation de type rel
		int nb_Rel;
		//Nombre de symboles dans la table de relocation de type rela
		int nb_Rela;
		//Bool pour dire si il y a des symboles dans la table de relocation de type rel
		int relE;
		//Bool pour dire si il y a des symboles dans la table de relocation de type rela
		int relaE;
}Elf32;

#endif
