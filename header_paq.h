#ifndef __HEADER_PAQ_H__
#define __HEADER_PAQ_H__

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>

// Vérification des 4 premiers octets du nombre magique ("7f 45 4c 46")
int check_elf(Elf32_Ehdr * data);

// Vérification de l'architecture 32bits
int check_arch(Elf32_Ehdr * data);

// Procédure d'affichage des infos du header ELF
void print_info(Elf32_Ehdr * data);
void print_section_table(Elf32_Ehdr * data);
void print_section(Elf32_Ehdr * data);
//ETAPES 1_2_3
void process_etapes(int file_descriptor, char sous_option);

#endif