#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>


void print_sym(Elf32_Shdr *section_Sym){
	int size = section_Sym->sh_size;
	int entsize = section_Sym->sh_entsize;
	int offset = section_Sym->sh_offset;
	
	int nbSym = size/entsize;

	Elf32_Sym *Symbole_start= (Elf32_Sym*)((void*)data +section_Sym->sh_offset); 

	Elf32_Sym *table_Symbole;
	
	for(int i=0;i<nbSym;i++){
		s = Symbole_start+i*entsize;
		printf("%s",s->st_name);
		printf("%s",s->st_value);
		printf("%s",s->st_size);
		printf("%s",s->st_info);
		printf("%s",s->st_other);
		printf("%s",s->st_shndx);
	}
}



