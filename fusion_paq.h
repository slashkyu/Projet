
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>





// STRUCTURE DE STOCKAGE D'INFORMATIONS
typedef struct {

                Elf32_Ehdr *header;

                //pointeur sur la table des sections
                Elf32_Shdr *table_section;

                //pointeur sur la table symbole
                El32_Sym *table_Symbole;

                //pointeur sur string table
                char* string_table;
}Elf_stck;

