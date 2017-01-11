#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>
#include <math.h>
#include "fusion_paq.h"
#include "interface.h"


 /* Elf32_Shdr *section_header_start1 = (Elf32_Shdr*)((void*)data1 + data1->e_shoff);

  Elf32_Shdr sections_string1 = section_header_start1[data1->e_shstrndx];

  Elf32_Shdr *section_header1 = (Elf32_Shdr*)((void*)data1 + data1->e_shoff);

  char *strings1 = (char*)((void*)data1 + sections_string1.sh_offset);

  strings1 + section_header_1.sh_name*/

int inserer(int element_a_inserer, int tab[], int taille_gauche)

{

    int j;

    for (j = taille_gauche; j > 0 && tab[j-1] > element_a_inserer; j--){

      tab[j] = tab[j-1];
	}
    tab[j] = element_a_inserer;
	return j;
}

int is_ordered(Elf32_Off offset,Elf32_Shdr *ordered, int size) {
	int i=0;
	while(i<size && ordered[i].offset!=offset){
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
		
		



Elf32_Shdr *calcul_offset(Elf32 e1, Elf32_Shdr *ordered, int size, Elf32_Shdr *section2){

		int i, offset=0; 
		Elf32_Shdr *section_header1, *section_header2;

		for(i=0; i< size-1; i++) {
			section_header1 = ordered + i;
			section_header1->offset=offset;
			section_header2=find_section(section2,retrouver_nom(e1, section_header1->sh_name)); //retrouver la section correspondante si elle existe
			if(section_header2 != NULL){
			offset=section_header1->sh_offset + section_header1->size + section_header2->size ;
			}
			else {
				offset=section_header1->sh_offset + section_header1->size;
			}
			if ((section_header1+1)->Align > 0 && offset%(section_header1+1)->Align != 0){
				offset = offset + (section_header1+1)->Align-offset%(section_header1+1)->Align; //offset = offset + Align - res(offset modulo Align);
			}
		}
		section_header1->offset=offset;
		return ordered;
}
				


char * retrouver_nom(Elf32 e, int sh_name){
		return e->string_table_section + i;
}



//trouve la section de type type et de nom string
Elf32_Shdr *find_section(Elf32 e, char *string){
	Elf32_Shdr *section_header;int i;

	for ( i = 0; i < data->e_shnum; i++){
		section_header = e->table_section+i;
		if((strcmp(e->string_table_section + section_header->sh_name, string)==0) ){
			//printf("strtab: %x\n",section_header->sh_offset);
			return section_header;
		}
	}
	return NULL;
}




void print_flag(Elf32_Word flags) {

 if (flags & (1u << 0)){	//écriture
    	printf("W ");
    }
    if (flags & (1u << 1)){	//allocation
    	printf("A ");
    }
    if (flags & (1u << 2)){	//exécution
    	printf("X ");
    }
    if (flags & (1u << 4)){	//fusion
    	printf("M ");
    }
    if (flags & (1u << 5)){	//chaînes
    	printf("S ");
    }
    if (flags & (1u << 6)){	//info
    	printf("I ");
    }
    if (flags & (1u << 7)){	//ordre des liens
    	printf("L ");
    }
    if (flags & (1u << 8)){	//os nonconforming
    	printf("0 ");
    }
    if (flags & (1u << 9)){	//groupe
    	printf("G ");
    }
    if (flags & (1u << 10)){	//TLS
    	printf("T ");
    }
}

void print_table(Elf32 Elf, Elf32_Ehdr *data){

int i;


	Elf32_Shdr section_header;
	
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);

	// on retrouve la string table avec le nom des sections ..
	
	Elf32_Shdr sections_string = section_header_start[data->e_shstrndx];

	char *string = (char*)((void*)data + sections_string.sh_offset);	

	for(i=0; i<data->e_shnum; i ++) {

		  section_header = Elf.table_section[i];

		printf("[%d]\n",i);

    // on affiche les infos de la section
    printf("Nom : %s\nType :", string + section_header.sh_name);	//nom
    
    print_type(section_header.sh_type);
    printf("\nFanions : ");	//flags   
    print_flag(section_header.sh_flags);
    printf("\nAdresse : 0x%08x", section_header.sh_addr);
    printf("\nDécalage : 0x%06x\n", section_header.sh_offset);
    
  }
}

Elf32 traitement(Elf32_Ehdr *data1, Elf32_Ehdr *data2, Elf32_Shdr *section_header_start1, Elf32_Shdr *section_header_start2, char * string1, char* string2, int max) {

	int i, j=0, index=0, offset = 0;
	// pour stoquer chaque section

	Elf32_Shdr *section_header_1;
	Elf32_Shdr *section_header_2; 
	Elf32 Elf_final; 

	//Init des structures

 	Elf_final.table_section = malloc(max*sizeof(Elf32_Shdr)); 



		for (i = 0; i < data1->e_shnum; i++){


			// on recupere le header de section courrant
			section_header_1 = section_header_start1+i;

			//On recherche la section dans le deuxième fichier
			section_header_2 = find_section(data2, string1 + section_header_1->sh_name);

			

			// Boucle de parcours du deuxième tableau : 
			//Si on rencontre une section ayant le même nom et le même type on s'arrête


			//Si on a pas trouvé de section correspondante
			if( section_header_2 == NULL) {
				
				//Mis à jour du offset et de la taille. 0x34 étant le décalage/adresse de .text
				Elf_final.table_section[index] = section_header_start1[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				Elf_final.table_section[index].sh_flags = section_header_start1[i].sh_flags;
				offset = offset + Elf_final.table_section[index].sh_size;
				index++;
			}
	 		else {
				j++;
				//Sinon, mise à jour de la taille et du offset en prenant compte la deuxième section
				Elf_final.table_section[index] = section_header_start1[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				Elf_final.table_section[index].sh_size = section_header_start1[i].sh_size+ section_header_start2[j].sh_size;				
				Elf_final.table_section[index].sh_flags = section_header_start1[i].sh_flags;
				Elf_final.table_section[index].sh_type = section_header_start1[i].sh_type;
				offset = offset + Elf_final.table_section[index].sh_size;
				index ++;

				//Mise à jour du lien selon le type (cf doc)
				//symtab ou dynsm
				if((Elf_final.table_section[index].sh_type == 2) || Elf_final.table_section[index].sh_type == 11) { 
					
					Elf_final.table_section[index].sh_link = data1->e_shnum;
					//insérer ici l'index + 1 du dernier symbole après l'étape 7
					
				}
				//Rela ou Rel
				else if (Elf_final.table_section[index].sh_type == 4 || Elf_final.table_section[index].sh_type == 9){
					Elf_final.table_section[index].sh_link = data1->e_shnum - 1;

				}
			}
	
		}	
		return Elf_final;
	}




//récupération des sections et fusion

//On a préalablement lu dans les deux fichiers.

void fusion_section_table(Elf32 e1, Elf32 e2)
{
	int i, j=0, index=0, offset=0, max=fmax((e1->header)->e_shnum,(e2->header)->e_shnum);

	printf("Il y a %d en-têtes de section :\n",max);

	
	//STRUCTURE DE STOCKAGE
	//Elf Elf_1;

	//Elf Elf_2;
	Elf32 Elf_final; 



	  
	// on recupere un pointeur sur le debut de la section et donc sur la premiere chaine



 
	//La boucle for sera faite pour le max, celle n'ayant pas de correspondant étant ajouté
	if (data1->e_shnum > data2->e_shnum) {

		Elf_final = traitement(data1, data2, max);

	 }
	else {

		Elf_final = traitement(data2, data1, section_header_start2,section_header_start1,string2,string1, max);
			

				
					
	
	}

	print_table(Elf_final, data2);


}


















