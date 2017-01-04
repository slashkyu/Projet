#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>

// à faire : changer variable + positionnement


// rela : section type rela

void print_ELF_Rela(Elf32_Shdr *section_Sym){
	int size = section_Sym->sh_size;
	int entsize = section_Sym->sh_entsize;
	int offset = section_Sym->sh_offset;
	
	int nbSym = size/entsize;
	char bind[12], type[12];

	Elf32_Sym *Symbole_start= (Elf32_Sym*)((void*)data +section_Sym->sh_offset); 

	Elf32_Sym *table_Symbole;
	
	for(int i=0;i<nbSym;i++){
		s = Symbole_start+i*entsize;

// CF CALCUL DOC pour le dernier attribut

		//type de repossitionnement
		switch (ELF32_R_TYPE(s->st_info)) {

			case 0 :
				strcpy(bind, "R_386_NONE";
				break;
			case 1 :
				strcpy(bind, "R_386_32";
				break;
			case 2 :
				strcpy(bind, "R_386__PC32";
				break;
			case 3 :
				strcpy(bind, "R_386_GOT32";
				break;
			case 4 :
				strcpy(bind, "R_386_PLT32";
				break;
			case 5 :
				strcpy(bind, "R_386_COPY";
				break;
			case 6 :
				strcpy(bind, "R_386_GLOB_DAT ";
				break;
			case 7 :
				strcpy(bind, "R_386_JMP_SLOT ";
				break;
			case 8 :
				strcpy(bind, "R_386_RELATIVE";
				break;
			case 9 :
				strcpy(bind, "R_386_GOTOFF ";
				break;
			case 10:
				strcpy(bind, "R_386_GOTPC";
				break;	
			default :
				printf("%s\n", err_bind);
				break;		
		}	

		//indice
		printf("décalage : %s ", s->r_offset);
		printf("Index : %s ",ELF32_R_SYM(s->st_info));

// rel : section type rel
