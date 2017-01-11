#ifndef __LECTURE__H__
#define __LECTURE__H__
#include <elf.h>
#include "interface.h"

Elf32_Ehdr *get_header(struct stat file_info,int file_descriptor);
//get start of section table
Elf32_Shdr *get_section_table(Elf32_Ehdr * h);
//get section with the type
Elf32_Shdr *get_sectionT(Elf32_Ehdr * h,int indice);

int sectionE(Elf32_Ehdr * h,int indice);
//get section with the name
Elf32_Shdr *get_sectionN(Elf32_Ehdr * h, char *string);

Elf32_Shdr *get_string_section(Elf32_Ehdr * h);

char *string_section(Elf32_Ehdr * h);

Elf32_Shdr *get_string_symbole(Elf32_Ehdr * h);

char *string_symbole(Elf32_Ehdr * h);

Elf32_Sym *get_symbole_table(Elf32_Ehdr * h);

Elf32_Rel *get_rel_table(Elf32_Ehdr * h);

Elf32_Rela *get_rela_table(Elf32_Ehdr * h);

int get_section_nb(Elf32_Ehdr * h);

int get_symbole_nb(Elf32_Ehdr * h);

int get_rel_nb(Elf32_Ehdr *h);

int get_rela_nb(Elf32_Ehdr *h);

Elf32_Ehdr *initHeader(Elf32_Ehdr * h);

void initSectionTable(Elf32_Shdr *s, Elf32_Shdr *d);

void initSymboleTable(Elf32_Sym *s, Elf32_Sym *d);

void initRelTable(Elf32_Rel *s, Elf32_Rel *d);

void initRelaTable(Elf32_Rela *s, Elf32_Rela *d);

Elf32 *initELF(Elf32_Ehdr * data);

void libererELF(Elf32 *e);

#endif
