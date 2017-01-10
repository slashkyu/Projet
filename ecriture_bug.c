#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include "interface.h"
#include "ecriture.h"



char *tab_type_section[14] = {"NULL", "PROGBITS", "SYMTAB" , "STRTAB", "RELA", "                                                                                                                                                                                               HASH", "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "PROCSPECSEM", "AP                                                                                                                                                                                               PINDEX"};

char * tab_classe[2]={"ELF32","ELF64"};

char * tab_type_sym[7]={"NOTYPE", "OBJCET", "FUNC","SECTION","FILE","LOPROC","HI                                                                                                                                                                                               PROC"};

char * tab_lien_sym[5]={"LOCAL","GLOBAL","WEAK","LOPROC","HIPROC"};

char *tab_donnees[2]={"little_endian","big_endian"};

char * tab_header_type[4]= {"Fichier repositionnable","Fichier exÃ©cutable","Obj                                                                                                                                                                                               et partagÃ©","Fichier core"};


void afficherHeader(Elf32 *e){

        int i;
        //Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
        /* //Nb Magique
        printf("e:  ");
        for(i=0;i<EI_NIDENT;i++){
                printf("%02x ",(e->header)->e_ident[i]);
        }       puts(" ");*/
        //header->e_ident = h->e_ident;
        printf("Classe :%s \n ", tab_classe[(e->header)->e_ident[4]-1]);
        printf("DonnÃ©es : %s\n", tab_donnees[(e->header)->e_ident[5]-1]);
        printf("  Version: %x\n", (e->header)->e_ident[6]);
        printf("  OS/ABI:%d\n", (e->header)->e_ident[7]);
        printf("  Version ABI:%d\n", (e->header)->e_ident[8]);
        printf("Type : %s \n",tab_header_type[(e->header)->e_type]);
        printf("Code Machine : %02x \n",(e->header)->e_machine);
        printf("Version %02x \n",(e->header)->e_version);
        printf("Adresse du point d'entrÃ©e%02x \n",(e->header)->e_entry);
        printf("DÃ©but des en-tÃªtes de programmes%02x \n",(e->header)->e_phoff)                                                                                                                                                                                               ;
        printf("DÃ©buts des en-tÃªtes de sections %02x \n",(e->header)->e_shoff)                                                                                                                                                                                               ;
        printf("Fanions : %02x \n",(e->header)->e_flags);
        printf("Taille en-tÃªte%02x \n",(e->header)->e_ehsize);
        printf("Taille en-tÃªte programme%02x \n",(e->header)->e_phentsize);
        printf("Nombre d'en-tÃªte de programme%02x \n",(e->header)->e_phnum);
        printf("Taille en-tÃªte de section%02x \n",(e->header)->e_shentsize);
        printf("Nombre de section : %02x \n",(e->header)->e_shnum);
        printf("Table d'indexes des chaines d'en-tÃªtes de section : %02x \n",(e                                                                                                                                                                                               ->header)->e_shstrndx);
}

void afficherSectionTable(Elf32 *e){
        int i;
        printf("Nom :   Type    Flag    Addr    Off     Size    Es      Flg Lk I                                                                                                                                                                                               nf Al");
        for(i=0;i<e->nb_Section;i++){

                printf("%02x ",(e->table_section + i)->sh_name);

                if((e->table_section + i)->sh_type >= 0x70000000 && (e->table_se                                                                                                                                                                                               ction + i)->sh_type <= 0x7fffffff){
                printf("%s\n", tab_type_section[12]);
                }  //Correspond Ã  "processor specific semantic"
        else if((e->table_section + i)->sh_type >= 0x80000000 && (e->table_secti                                                                                                                                                                                               on + i)->sh_type <= 0xffffffff){
                        printf("%s\n", tab_type_section[13]);
                }
                else {
                        printf("%s\n", tab_type_section[(e->table_section + i)->                                                                                                                                                                                               sh_type]);
                }

                printf("%02x ",(e->table_section + i)->sh_flags);
                printf("%02x ",(e->table_section + i)->sh_addr);
                printf("%08x ",(e->table_section + i)->sh_offset);
                printf("%02x ",(e->table_section + i)->sh_size);
                printf("%02x ",(e->table_section + i)->sh_link);
                printf("%02x ",(e->table_section + i)->sh_info);
                printf("%02x ",(e->table_section + i)->sh_addralign);
                //printf("T%02x \n",(e->table_section + i)->sh_entsize);
        }
}

void afficherSectionSymbole(Elf32 *e){
        int i, type, bind;
        printf("Table de symboles << .symtab >> contient %d entrÃ©es:\n",e->nb_S                                                                                                                                                                                               ymbole);
        printf("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
        for (i = 0; i < e->nb_Symbole; i++){
                printf("%6d: ",i);

                //affichage de la value
                printf("%08x ",(e->table_symbole + i)->st_value);

                //affichage de la taille
                printf("%6d ",(e->table_symbole + i)->st_size);

                //Type
                type = ELF32_ST_TYPE((e->table_symbole + i)->st_info);
                if( type == 13) {
                        printf("%-7s", tab_type_sym[5]);
                }
                else if(type == 14 ){
                        printf("%-7s", tab_type_sym[6]);
                }
                else {
                        printf("%-7s", tab_type_sym[type]);
                }

                //affichage du lien
                bind = ELF32_ST_BIND((e->table_symbole + i)->st_info);
                if( type == 13) {
                        printf("%-7s", tab_type_sym[3]);
                }
                else if(type == 15 ){
                        printf("%-7s", tab_type_sym[4]);
                }
                else {
                        printf("%-7s", tab_lien_sym[bind]);
                }


                if((e->table_symbole + i)->st_other == 0){
                        printf("DEFAULT");
                }

                if((e->table_symbole + i)->st_shndx == 0){
                        printf("UND ");
                }
                else if((e->table_symbole + i)->st_shndx == 0xfff1){
                        printf("ABS ");
                }
                else{
                        printf("%3x ",(e->table_symbole + i)->st_shndx);
                }
                printf("\n");
                //printf("%08x ",(e->table_symbole + i)->st_name);

        }
}
