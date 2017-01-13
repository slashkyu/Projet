#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>
#include <math.h>
#include "lecture.h"
#include "fusion_paq.h"
#include "interface.h"




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
	while(i<size && ordered[i].sh_offset!=offset){
		i++;
	}
}



//juste un recherche du min et un tri par insertion



//Elf32_Shdr *ordonner
		






Elf32_Shdr *calcul_offset(Elf32 *e1, Elf32_Shdr *ordered, int size, Elf32 *e2){

		int i, offset=0; 
		Elf32_Shdr *section_header1, *section_header2;

		for(i=0; i< size-1; i++) {
			section_header1 = ordered + i;
			section_header1->sh_offset=offset;
			
			section_header2=find_section(e2,e1->string_table_section + section_header1->sh_name); //retrouver la section correspondante si elle existe
			
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


		



Elf32 *traitement(Elf32 *e1, Elf32 *e2) {

	int i, j=0, index=0, offset = 0, i_str=0;
	char *string = "";
	size_t new_size;
	// pour stoquer chaque section

	Elf32_Shdr *section_header_1;
	Elf32_Shdr *section_header_2; 
	Elf32_Shdr *section;
	Elf32 *Elf_final; 

	


	
		
		for(i = 0; i < e2->nb_Section; i++) {

			//J'UTILISE FIND SECTION et pas la primitive de Nan
			
			section_header_2 = find_section(e1, e1->string_table_section + section_header_2->sh_name);
			

			// Boucle de parcours du deuxième tableau : 
			//Si on rencontre une section ayant le même nom et le même type on s'arrête


			//Si on a pas trouvé de section correspondante
			if( section_header_1 == NULL) {
				//ajout section
				e1=ajouterSection(e1, e2, i);
				
				//Ajout section name string
				//Ici on enregistre dans une string le nom de la section qu'on ajoute.
				//On passe par une variable pour ajouter en fin de la string table tout les noms des sections ajoutées.

				new_size =sizeof(e2->string_table_section + section_header_2->sh_name);	
				
				section=find_section(e1, ".shstrtab");
				e1->string_table_section = realloc(e1->string_table_section,new_size); //On Alloue l'espace mémoire nécessaire

			

				for(i=section->sh_size; i < new_size; i++){
						e1->string_table_section[i] = e2->string_table_section[section_header_2->sh_name + j];
				}
				
				e1->nb_Section++;
				e1->header->e_shnum ++;
				
				
				//Mis à jour du offset et de la taille. 0x34 étant le décalage/adresse de .text
				section_header_1->sh_size = section_header_1->sh_size + section_header_2->sh_size ;
				
			}
		}	 	
		//Elf_final->string_table_section = realloc(Elf_final->string_table_section, sizeof(string));
		//strcpy(Elf_final->string_table_section + section_header_2->sh_name, string);
		
	
		return Elf_final;
	}




//récupération des sections et fusion

//On a préalablement lu dans les deux fichiers.

Elf32 *fusion_section_table(Elf32 *e1, Elf32 *e2){
	int i, j=0, index=0, offset=0;


	Elf32 *Elf_final; 
	
	//on fusionne dans elf final e1 et e2
	Elf_final = traitement(e1, e2);
	
	return Elf_final;


}



















