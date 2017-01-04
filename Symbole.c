#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>


char err_bind[] = "Impossible de lire lien du symbole\n";

void print_sym(Elf32_Shdr *section_Sym){
	int size = section_Sym->sh_size;
	int entsize = section_Sym->sh_entsize;
	int offset = section_Sym->sh_offset;
	
	int nbSym = size/entsize;
	char bind[12], type[12];

	Elf32_Sym *Symbole_start= (Elf32_Sym*)((void*)data +section_Sym->sh_offset); 

	Elf32_Sym *table_Symbole;
	
	for(int i=0;i<nbSym;i++){
		s = Symbole_start+i*entsize;

		switch (ELF32_ST_BIND(s->st_info)) {


			case 0 :
				strcpy(bind, "LOCAL";
				break;
			case 1 :
				strcpy(bind, "GLOBAL";
				break;
			case 2 :
				strcpy(bind, "WEAK";
				break;
			case 13 :
				strcpy(bind, "LOPROC";
				break;
			case 15 :
				strcpy(bind, "HIPROC";
				break;	
			default :
				printf("%s\n", err_bind);
				break;		
		}	

		switch (ELF32_ST_TYPE(s->st_info)) {
			case 0 : 
				strcpy(type, "NOTYPE");
				break;
			case 1 : 
				strcpy(type, "OBJCET");
				break;	
			case 0 : 
				strcpy(type, "FUNC");
				break;	
			case 0 : 
				strcpy(type, "SECTION");
				break;	
			case 0 : 
				strcpy(type, "FILE");
				break;	
			case 0 : 
				strcpy(type, "LOPROC");
				break;	
			case 0 : 
				strcpy(type, "HIPROC");
				break;	
		}	

		printf("nom : %s ",s->st_name);
		printf("valeur : %s ",s->st_value);
		printf("taille : %s",s->st_size);
		printf("Lien : %s ",bind);
		printf("type : %s ",type);
		printf("Vis : %s ",s->st_other)
		printf("Ndx : %s\n",s->st_shndx);
	}
}


               

