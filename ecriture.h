#ifndef __ECRITURE__H__
#define __ECRITURE__H__
#include <elf.h>
#include "interface.h"
#include "lecture.h"





//Renvoie le nom de OS/ABI à partir de son code
static const char * get_osabi_name (unsigned int osabi);
//Renvoie le nom de machine à partir de son code
static char *get_machine_name (unsigned e_machine);
//Renvoient l'information supplementaire sur la machine
static void decode_ARM_machine_flags (unsigned e_flags, char buf[]);
static char *get_machine_flags (unsigned e_flags, unsigned e_machine);
//Renvoie le nom de section à partir de son code
static const char *get_section_type_name (unsigned int sh_type);
//FONCTIONS PRINCIPALES d'AFFICHAGE
//Affiche l'en-tête de la structure
void afficherHeader(Elf32 *e);
//Affiche le table de sections de la structure
void  afficherSectionTable(Elf32 *e);
//Affiche le contenu de touts sections de la structure
void afficherSection(Elf32 *e);
//Affiche le table de symboles de la structure
void afficherSectionSymbole(Elf32 *e);
//Affiche le table de relocations de la structure
void afficherReloc(Elf32 *e);

#endif
