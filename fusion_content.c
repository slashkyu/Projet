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
#include "lecture.h"
#include "interface.h"


Elf32 fusion_content(Elf32 e1, Elf32 e2, Elf32 e3){

		int i,k, j=0;
		Elf32_Shdr *section_header_1, *section_header_2;


		// Pour toute les section du premier fichier
		for(i=0; i < (e1->header)->e_shnum; i++) {
			section_header_1 = e1->table_section + i;
			section_header_2 = find_section(e2, (e1->string_table_section + section_header_1->sh_name); //find : allez chercher fonction dans lecture.c get_section_name
			if(section_header_1.sh_type !=3) {
				if (section_header_1.sh_size != 0 ){
					unsigned char *content1 = (unsigned char*)((void*)e1->header + section_header1->sh_offset);
				}
				if(section_header_2 != NULL){
							
					if{
						if (section_header_2->sh_size != 0){
							unsigned char *content2 = (unsigned char*)((void*)e2-> + section_header_2->sh_offset);
							content1 = realloc(content1, section_header_2->size + section_header_1->size);
						
									while(k < section_header_1->size + section_header_2->sh_size && j < section_header_2.sh_size){	//boucle de récupération du contenu
										content1[k]=content2[j];
									
							}
					}
				}


						
}
