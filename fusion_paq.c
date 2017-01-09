

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>
#include <math.h>
#include "header_paq.h"



//récupération des sections et fusion

//On a préalablement lu dans les deux fichiers.

void fusion_section_table(Elf32_Ehdr * data1, Elf32_Ehdr *data2)
{
	int i, j=0, index=0, offset=0, max=fmax(data1->e_shnum,data2->e_shnum);

	printf("Il y a %d en-têtes de section :\n",max);

	// on recupere un pointeur sur le premier header de section, avec le offset correspondant
  Elf32_Shdr *section_header_start1 = (Elf32_Shdr*)((void*)data1 + data1->e_shoff);
  Elf32_Shdr *section_header_start2 = (Elf32_Shdr*)((void*)data2 + data2->e_shoff);

  // on retrouve la string table avec le nom des sections ..
  Elf32_Shdr sections_string1 = section_header_start1[data1->e_shstrndx];
  Elf32_Shdr sections_string2 = section_header_start2[data2->e_shstrndx];

	// on recupere un pointeur sur le premier header de section, avec le offset correspondant
	Elf32_Shdr *section_header1 = (Elf32_Shdr*)((void*)data1 + data1->e_shoff);
	Elf32_Shdr *section_header2 = (Elf32_Shdr*)((void*)data2 + data2->e_shoff);


	//STRUCTURE DE STOCKAGE
	//Elf Elf_1;

	//Elf Elf_2;

	Elf Elf_final; 

	//Init des structures

	Elf_final.table_section = malloc(max*sizeof(Elf32_Shdr); 

	  
	// on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
	char *strings1 = (char*)((void*)data1 + sections_string1.sh_offset);
	
	char *strings2 = (char*)((void*)data2 + sections_string2.sh_offset);

	// pour stoquer chaque section
	Elf32_Shdr section_header_1;
	Elf32_Shdr section_header_2; 
 
	//La boucle for sera faite pour le max, celle n'ayant pas de correspondant étant ajouté
	if (data1->e_shnum > data2->e_shnum) {
		for (i = 0; i < data1->e_shnum; i++){


			// on recupere le header de section courrant
			section_header_1 = section_header_start1[j];	 
			section_header_2 = section_header_start2[i];

			// Boucle de parcours du deuxième tableau : 
			//Si on rencontre une section ayant le même nom et le même type on s'arrête
	  		while (j < data2->e_shnum && !strcmp(strings1 + section_header_1->sh_name, strings2 + section_header_2->sh_name) && (section_header_1.sh_type == section_header_2.sh_type) ){

					j++;
					section_header_2 = section_header_start[j];
			
			}

			//Si on a pas trouvé de section correspondante
			if( j == data2->e_shnum) {
				
				//Mis à jour du offset et de la taille. 0x34 étant le décalage/adresse de .text
				Elf_final.table_section[index] = section_header_start1[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				offset = offset + Elf_final.table_section[index].sh_size;
				index++;
			}
	 		else {
				//Sinon, mise à jour de la taille et du offset en prenant compte la deuxième section
				Elf_final.table_section[index] = section_header_start1[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				Elf_final.table_section[index].size = section_header_start1[i].size+ section_header_start2[j].size;
				offset = offset + Elf_final.table_section[index].sh_size;

				//Mise à jour du lien selon le type (cf doc)
				//symtab ou dynsm
				if(Elf_final.table_section[index].sh_type == 2 || Elf_final.table_section[index].sh_type = 11) { 
					Elf_final.table_section[index].sh_link = data1->e_shnum;
					//insérer ici l'index + 1 du dernier symbole après l'étape 7
					
				}
				//Rela ou Rel
				else if (Elf_final.table_section[index].sh_type == 4 || Elf_final.table_section[index].sh_type = 9){
					Elf_final.table_section[index].sh_link = data1->e_shnum - 1;

				}
			}
	
		}	
	}
	else {
			for (i = 0; i < data2->e_shnum; i++){


			// on recupere le header de section courrant
			section_header_1 = section_header_start[i];	 
			section_header_2 = section_header_start[j];

			// Boucle de parcours du deuxième tableau : 
			//Si on rencontre une section ayant le même nom et le même type on s'arrête
	  		while (j < data1->e_shnum && !strcmp(strings + section_header_1.sh_name, strings + section_header_2->sh_name) && (section_header_1.sh_type == section_header_2.sh_type) ){

					j++;
					section_header_2 = section_header_start[j];
			
			}

			//Si on a pas trouvé de section correspondante
			if( j == data2->e_shnum) {
				
				//Mis à jour du offset et de la taille. 0x34 étant le décalage/adresse de .text
				Elf_final.table_section[index] = section_header_start2[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				offset = offset + Elf_final.table_section[index].sh_size;
				index++;
			}
	 		else {
				//Sinon, mise à jour de la taille et du offset en prenant compte la deuxième section
				Elf_final.table_section[index] = section_header_start2[i];
				Elf_final.table_section[index].sh_offset = 0x34 + offset;
				Elf_final.table_section[index].size = section_header_start1[i].size+ section_header_start2[j].size;
				offset = offset + Elf_final.table_section[index].sh_size;

				//Mise à jour du lien selon le type (cf doc)
				//symtab ou dynsm
				if(Elf_final.table_section[index].sh_type == 2 || Elf_final.table_section[index].sh_type = 11) { 
					Elf_final.table_section[index].sh_link = data2->e_shnum;
					//insérer ici l'index + 1 du dernier symbole après l'étape 7
					
				}
				//Rela ou Rel
				else if (Elf_final.table_section[index].sh_type == 4 || Elf_final.table_section[index].sh_type = 9){
					Elf_final.table_section[index].sh_link = data2->e_shnum - 1;

				}
			}
		
			
	
		}

	


}


