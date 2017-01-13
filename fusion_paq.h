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

Elf32 *traitement(Elf32 *e1, Elf32 *e2);

Elf32 *fusion_section_table(Elf32 *e1, Elf32 *e2);

















