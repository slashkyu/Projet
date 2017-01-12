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
#include "lecture.h"

int newOffset(int offset,int alignement){
	while(offset%alignement != 0){
		offset++;
	}
	return offset;	
}

int debutOffset(Elf32 *e){
	return e->header->e_ehsize;
}

void Insere(char * ch, char x, int p){
	int i;
	for(i=strlen(ch);i>=p;i--){
		ch[i+1]=ch[i];
	}
	ch[p]=x;
}
/*
int *newSizeofSection(Elf32_Shdr *s1, Elf32_Shdr *s2){
	int i = s1->sh_size;
	int j = s2->sh_size;
	i = i+j;
	return ()(void*)i;
}
*/
Elf32 *fusionSectionSize(Elf32 *e1,Elf32 *e2){
	int i,j,k;
	for(i=1;i<e2->nb_Section;i++){
		k=0;
		for(j=0;j<e1->nb_Section;j++){
			//printf("s2: %s  s1: %s\n",get_Nameofsection(e2, i),get_Nameofsection(e1, j));
			if(strcmp(get_Nameofsection(e2, i), get_Nameofsection(e1, j)) == 0){
				//printf("ici\n");
				//printf("%02x  %02x\n",(e1->table_section+j)->sh_size,(e2->table_section+i)->sh_size);
				(e1->table_section+j)->sh_size = (e1->table_section+j)->sh_size+(e2->table_section+i)->sh_size;
				//printf("%02x\n",(e1->table_section+j)->sh_size);
				k=1;			
			}
		}
		if(k==0){
			ajouterSection(e2, e1, i);	
		}
	}
	return e1;
}

Elf32 *fusionSectionOffset(Elf32 *e1){
	int offset = debutOffset(e1);
	int i;
	(e1->table_section+1)->sh_offset = (e1->header)->e_ehsize;
	while(((e1->table_section+1)->sh_offset)%((e1->table_section+1)->sh_addralign) != 0){
		(e1->table_section+1)->sh_offset++;
	}
	for(i=2;i<e1->nb_Section;i++){
		if((e1->table_section+i-1)->sh_size == 0){
			(e1->table_section+i)->sh_offset = (e1->table_section+i-1)->sh_offset;
		}
		else{
			(e1->table_section+i)->sh_offset = (e1->table_section+i-1)->sh_offset +(e1->table_section+i-1)->sh_size;
			//printf("%d: %d\n",i,((e1->table_section+i)->sh_offset)%((e1->table_section+i)->sh_addralign));
			while(((e1->table_section+i)->sh_offset)%((e1->table_section+i)->sh_addralign) != 0){
				//printf("%d\n",((e1->table_section+i)->sh_offset)%((e1->table_section+i)->sh_addralign));
				(e1->table_section+i)->sh_offset++;
				//printf("%d\n",((e1->table_section+i)->sh_offset)%((e1->table_section+i)->sh_addralign));
			}	
		}	
	}
	return e1;
}
