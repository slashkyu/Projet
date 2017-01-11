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
#include "interface.h"




Elf32_Shdr *find_section(Elf32_Ehdr* data, char *string);

Elf32 traitement(Elf32_Ehdr *data1, Elf32_Ehdr *data2, Elf32_Shdr *section_header_start1, Elf32_Shdr *section_header_start2, char * string1, char* string2, int max);
void fusion_section_table(Elf32_Ehdr * data1, Elf32_Ehdr *data2);

void process ( Elf32_Ehdr *data1, Elf32_Ehdr *data2);


#endif

