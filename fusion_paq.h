#ifndef __FUSION_PAQ_H__
#define __FUSION_PAQ_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>
#include <math.h>
#include "lecture.h"
#include "interface.h"



int inserer(int element_a_inserer, int tab[], int taille_gauche);

Elf32_Shdr *ordre_offset(Elf32_Ehdr *data);

Elf32_Shdr *calcul_offset(Elf32 *e1, Elf32_Shdr *ordered, int size, Elf32 *e2);

Elf32_Shdr *find_section(Elf32 *e, char *string);

void print_flag(Elf32_Word flags);

void print_table(Elf32 *Elf);

Elf32 *traitement(Elf32 *e1, Elf32 *e2);

void fusion_section_table(Elf32 *e1, Elf32 *e2);


#endif

