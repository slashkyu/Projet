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
#include "fusion_paq.h"
#include "interface.h"


/* A LIRE. je n'ai pas utiliser les primites de lecture.c sauf initELF*/

int inserer(int element_a_inserer, int tab[], int taille_gauche)

{

    int j;

    for (j = taille_gauche; j > 0 && tab[j-1] > element_a_inserer; j--){

      tab[j] = tab[j-1];
        }
    tab[j] = element_a_inserer;
        return j;
}
/*

int is_ordered(Elf32_Off offset,Elf32_Shdr *ordered, int size) {
        int i=0;
        while(i<size && ordered[i].sh_offset!=offset){
                i++;
        }
}



//juste un recherche du min et un tri par insertion

Elf32_Shdr *ordre_offset(Elf32_Ehdr *data){

        Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
        Elf32_Shdr *section_header;
        Elf32_Off offset_min==0, tab[data->e_shnum];

        int i, j;

        Elf32_Shdr *offset_order_table;
        offset_order_table=malloc(data->e_shnum * sizeof(Elf32_Shdr));

        for (i =0; i<data->e_shnum; i ++) {

                //j=1;
                section_header = section_header_start + i;
                j=insérer(section_header->sh_offset, tab, i);

                while(j<data->e_shnum-1 && section_header->sh_offset > offset_min && is_ordered(section_header->offset,offset_order_table, i){

                        j++;
                        section_header = section_header_start + j;
                }
        }


                                offset_order_table[i].sh_name=section_header->sh_name;
                                offset_order_table[i].sh_name=section_header->sh_type;
                                offset_order_table[i].sh_name=section_header->sh_size;
                                offset_order_table[i].sh_name=section_header->sh_offset;
                                offset_order_table[i].sh_name=section_header->sh_link;
                                offset_order_table[i].sh_name=section_header->sh_flags;
                                offset_order_table[i].sh_name=section_header->sh_info;
                                offset_order_table[i].sh_name=section_header->sh_addr;
                                offset_order_table[i].sh_name=section_header->sh_addralign;
                                offset_order_table[i].sh_name=section_header->sh_entsize;

*/
//Elf32_Shdr *ordonner







Elf32_Shdr *calcul_offset(Elf32 e1, Elf32_Shdr *ordered, int size, Elf32 *e2){

                int i, offset=0;
                Elf32_Shdr *section_header1, *section_header2;

                for(i=0; i< size-1; i++) {
                        section_header1 = ordered + i;
                        section_header1->sh_offset=offset;

                        section_header2=find_section(e2,retrouver_nom(e1, e2->string_table_section + section_header1->sh_name)); //retrouver la section correspondante si elle existe

                        if(section_header2 != NULL){
                        offset=section_header1->sh_offset + section_header1->sh_size + section_header2->sh_size ;
                        }
                        else {
                                offset=section_header1->sh_offset + section_header1->sh_size;
                        }
                        if ((section_header1+1)->sh_addralign > 0 && offset%(section_header1+1)->sh_addralign != 0){
                                offset = offset + (section_header1+1)->sh_addralign - offset%(section_header1+1)->sh_addralign; //offset = offset + Align - res(offset modulo Align);
                        }
                }
                section_header1->sh_offset=offset;
                return ordered;
}




//trouve la section de type type et de nom string
Elf32_Shdr *find_section(Elf32 *e, char *string){
        Elf32_Shdr *section_header;int i;

        for ( i = 0; i < e->nb_Section; i++){
                section_header = e->table_section+i;
                if((strcmp(e->string_table_section + section_header->sh_name, string)==0) ){
                        //printf("strtab: %x\n",section_header->sh_offset);
                        return section_header;
                }
        }
        return NULL;
}




void print_flag(Elf32_Word flags) {

 if (flags & (1u << 0)){        //écriture
        printf("W ");
    }
    if (flags & (1u << 1)){     //allocation
        printf("A ");
    }
    if (flags & (1u << 2)){     //exécution
        printf("X ");
    }
    if (flags & (1u << 4)){     //fusion
        printf("M ");
    }
    if (flags & (1u << 5)){     //chaînes
        printf("S ");
    }
    if (flags & (1u << 6)){     //info
        printf("I ");
    }
    if (flags & (1u << 7)){     //ordre des liens
        printf("L ");
    }
    if (flags & (1u << 8)){     //os nonconforming
        printf("0 ");
    }
    if (flags & (1u << 9)){     //groupe
        printf("G ");
    }
    if (flags & (1u << 10)){    //TLS
        printf("T ");
    }
}



void print_table(Elf32 *Elf){

int i;


        printf("Il y a %d en-têtes de section :\n",Elf->nb_Section);

        Elf32_Shdr section_header;




        for(i=0; i<Elf->nb_Section; i ++) {

                section_header = Elf->table_section[i];

                printf("[%d]\n",i);

                // on affiche les infos de la section
                printf("Nom : %s\nType :", Elf->string_table_section + section_header.sh_name); //nom

                print_type(section_header.sh_type);
                printf("\nFanions : "); //flags
                print_flag(section_header.sh_flags);
                printf("\nAdresse : 0x%08x", section_header.sh_addr);
                printf("\nDécalage : 0x%06x\n", section_header.sh_offset);

        }
}



Elf32 *traitement(Elf32 *e1, Elf32 *e2) {

        int i, j=0, index=0, offset = 0, i_str=0;
        char *string = "";
        // pour stoquer chaque section

        Elf32_Shdr *section_header_1;
        Elf32_Shdr *section_header_2;
        Elf32 *Elf_final;

        //Init des structures



                Elf_final = initELF(e1->header);

                for(i = 0; i < e2->nb_Section; i++) {

                        //J'UTILISE FIND SECTION et pas la primitive de Nan

                        section_header_2 = find_section(e1, e1->string_table_section + section_header_2->sh_name);


                        // Boucle de parcours du deuxième tableau :
                        //Si on rencontre une section ayant le même nom et le même type on s'arrête


                        //Si on a pas trouvé de section correspondante
                        if( section_header_1 == NULL) {
                                //Ajout section

                                //Ajout section name string
                                //Ici on enregistre dans une string le nom de la section qu'on ajoute.
                                //On passe par une variable pour ajouter en fin de la string table tout les noms des sections ajoutées.
                                string = realloc(string,sizeof(e2->string_table_section + section_header_2->sh_name)); //On Alloue l'espace mémoire nécessaire
                                strcat(string + i_str, e2->string_table_section + section_header_2->sh_name);   //On concaténe les deux chaines de caractères
                                Elf_final->nb_Section++;
                                Elf_final->header->e_shnum ++;
                                index++;


                                //Mis à jour du offset et de la taille. 0x34 étant le décalage/adresse de .text
                                Elf_final->table_section[index].sh_size = Elf_final->table_section[index].sh_size + section_header_2->sh_size ;
                        }
                }
                Elf_final->string_table_section = realloc(Elf_final->string_table_section, sizeof(string));
                strcpy(Elf_final->string_table_section + section_header_2->sh_name, string);


                return Elf_final;
        }




//récupération des sections et fusion

//On a préalablement lu dans les deux fichiers.

void fusion_section_table(Elf32 *e1, Elf32 *e2){
        int i, j=0, index=0, offset=0;


        Elf32 *Elf_final;

        //on fusionne dans elf final e1 et e2
        Elf_final = traitement(e1, e2);

        //on affiche la table des sections
        print_table(Elf_final);


}
