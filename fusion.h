#ifndef __FUSION__H__
#define __FUSION__H__

#include <elf.h>
#include "interface.h"

int newOffset(int offset);

int debutOffset(Elf32 *e);

//int newSizeofSection(Elf32_Shdr *s1, Elf32_Shdr *s2);

Elf32 *fusionSectionSize(Elf32 * e1,Elf32 * e2);

Elf32 *fusionSectionOffset(Elf32 *e1);




#endif
