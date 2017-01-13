#ifndef __LECTURE_PAQ_H__
#define __LECTURE_PAQ_H__

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>



// Variable globale pour le choix de langue
int language;

// Procédure d'affichage des infos du header ELF
void print_header(Elf32_Ehdr * data);
void print_section_table(Elf32_Ehdr * data);
void print_section(Elf32_Ehdr * data);
void print_sym(Elf32_Ehdr * data);
//ETAPES 1_2_3_4

Elf32_Shdr find_section(Elf32_Ehdr *data, int type, char*string);

static const char *get_section_type_name(unsigned int sh_type);

void process_etapes(int file_descriptor, char sous_option);

#endif
