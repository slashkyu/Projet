#ifndef __ECRITURE__H__
#define __ECRITURE__H__
#include <elf.h>
#include "interface.h"
#include "lecture.h"

static const char* get_flag(int flags);

void afficherHeader(Elf32 *e);

void  afficherSectionTable(Elf32 *e);

void afficherSectionSymbole(Elf32 *e);

#endif
