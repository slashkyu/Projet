#ifndef __LECTURE__H__
#define __LECTURE__H__
#include <elf.h>
#include "interface.h"

Elf32_Ehdr *get_header(struct stat file_info,int file_descriptor);
//get start of section table
Elf32_Shdr *get_section_table(Elf32_Ehdr * h);

//get section with the type
Elf32_Shdr *get_sectionT(Elf32_Ehdr * h,int indice);

Elf32_Shdr *get_string_section(Elf32_Ehdr * h);

char *string_section(Elf32_Ehdr * h);

//get section with the name
Elf32_Shdr *get_sectionN(Elf32_Ehdr * h, char *string);

Elf32_Sym *get_symbole_table(Elf32_Ehdr * h);

//get nb of section
int get_section_nb(Elf32_Ehdr * h);

int get_symbole_nb(Elf32_Ehdr * h);

int get_rel_nb(Elf32_Ehdr *h);

int get_rela_nb(Elf32_Ehdr *h);

Elf32_Ehdr *initHeader(Elf32_Ehdr * h);

void initSymboleTable(Elf32_Sym *s, Elf32_Sym *d); 

Elf32 *initELF(Elf32_Ehdr * data);

void libererELF(Elf32 *e);
#endif
