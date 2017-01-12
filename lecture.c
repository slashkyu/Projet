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



//get addresse of header of file elf
Elf32_Ehdr *get_header(struct stat file_info,int file_descriptor){
	Elf32_Ehdr * h = mmap(0, file_info.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0);
	return h;
}

//get start of section table
Elf32_Shdr *get_section_table(Elf32_Ehdr * h){
	Elf32_Shdr *s = (Elf32_Shdr*)((void*)h + h->e_shoff);
	return s;
}

//get section with the type
Elf32_Shdr *get_sectionT(Elf32_Ehdr * h,int indice){
	Elf32_Shdr *start = get_section_table(h);
	Elf32_Shdr *s;
	for (int i = 0; i < h->e_shnum; i++){
		s = start+i;
		if(s->sh_type == indice){
			return s;
		}
	}
	//return NULL;
}

int sectionE(Elf32_Ehdr * h,int indice){
	Elf32_Shdr *start = get_section_table(h);
	Elf32_Shdr *s;
	int f = 0;
	for (int i = 0; i < h->e_shnum; i++){
		s = start+i;
		if(s->sh_type != indice){
			f++;
		}
	}
	if(f == h->e_shnum){
		return 0;
	}
	else{
		return 1;
	}
}

//get section with the name
Elf32_Shdr *get_sectionN(Elf32_Ehdr * h, char *string){
	Elf32_Shdr *section_header_start = get_section_table(h);
	Elf32_Shdr *section_header;
	//printf("%02x",section_header_start->sh_offset);
	int i;
	char *strings = string_section(h);
	//printf("%s",strings+7);
	for ( i = 0; i < h->e_shnum; i++){
		section_header = section_header_start+i;
		if(strcmp(strings + section_header->sh_name, string)==0){
			return section_header;
			//printf("sym%02x",section_header->sh_offset);
		}
	}
	//return NULL;
}



Elf32_Shdr *get_string_section(Elf32_Ehdr * h){
	Elf32_Shdr *section_header_start = get_section_table(h);
	Elf32_Shdr *s = section_header_start + h->e_shstrndx;
	return s;
}

char *string_section(Elf32_Ehdr * h){
	char *strings = (char*)((void*)h + get_string_section(h)->sh_offset);
	return strings;
}

Elf32_Shdr *get_string_symbole(Elf32_Ehdr * h){
	Elf32_Shdr *s = (Elf32_Shdr*)((void*)h + get_sectionN(h,".strtab")->sh_offset);
	//printf("add1: %02x",s->sh_size);
	return s;
}

char *string_symbole(Elf32_Ehdr * h){
	char *strings = (char*)((void*)h + get_string_symbole(h)->sh_offset);
	return strings;
}

Elf32_Sym *get_symbole_table(Elf32_Ehdr * h){
	Elf32_Sym *s = (Elf32_Sym*)((void*)h + get_sectionT(h,2)->sh_offset);
	return s;
}

Elf32_Rel *get_rel_table(Elf32_Ehdr * h){
	Elf32_Rel *s = (Elf32_Rel*)((void*)h + get_sectionT(h,9)->sh_offset);
	return s;
}

Elf32_Rela *get_rela_table(Elf32_Ehdr * h){
	Elf32_Rela *s = (Elf32_Rela*)((void*)h + get_sectionT(h,4)->sh_offset);
	return s;
}

int get_section_nb(Elf32_Ehdr * h){
	int nb = h->e_shnum;
}

int get_symbole_nb(Elf32_Ehdr * h){
	Elf32_Shdr *s = get_sectionT(h,2);
	int nb = (s->sh_size)/(s->sh_entsize);
}

int get_rel_nb(Elf32_Ehdr *h){
	Elf32_Shdr *s = get_sectionT(h,9);
	int nb = (s->sh_size)/(s->sh_entsize);
}

int get_rela_nb(Elf32_Ehdr *h){
	Elf32_Shdr *s = get_sectionT(h,4);
	int nb = (s->sh_size)/(s->sh_entsize);
}

Elf32_Ehdr *initHeader(Elf32_Ehdr * h){
	int i;
	Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
	if(header == NULL) exit(-1);
	for(i=0;i<EI_NIDENT;i++){
		header->e_ident[i] = h->e_ident[i];
	}	
	header->e_type = h->e_type;
	header->e_machine = h->e_machine;
	header->e_version = h->e_version;
	header->e_entry = h->e_entry;
	header->e_phoff = h->e_phoff;
	header->e_shoff = h->e_shoff;
	header->e_flags = h->e_flags;
	header->e_ehsize = h->e_ehsize;
	header->e_phentsize = h->e_phentsize;
	header->e_phnum = h->e_phnum;
	header->e_shentsize = h->e_shentsize;
	header->e_shnum = h->e_shnum;
	header->e_shstrndx = h->e_shstrndx;
	return header;
}

void initSectionTable(Elf32_Shdr *s, Elf32_Shdr *d) {
	d->sh_name =  s->sh_name;
	d->sh_type =  s->sh_type;
	d->sh_flags =  s->sh_flags;
	d->sh_addr =  s->sh_addr;
	d->sh_offset =  s->sh_offset;
	d->sh_size =  s->sh_size;
	d->sh_link =  s->sh_link;
	d->sh_info =  s->sh_info;
	d->sh_addralign =  s->sh_addralign;
	d->sh_entsize =  s->sh_entsize;
}

void initSymboleTable(Elf32_Sym *s, Elf32_Sym *d) {
	d->st_name =  s->st_name;
	d->st_value =  s->st_value;
	d->st_size =  s->st_size;
	d->st_info =  s->st_info;
	d->st_other =  s->st_other;
	d->st_shndx =  s->st_shndx;
}

void initRelTable(Elf32_Rel *s, Elf32_Rel *d) {
	d->r_offset =  s->r_offset;
	d->r_info =  s->r_info;
}

void initRelaTable(Elf32_Rela *s, Elf32_Rela *d) {
	d->r_offset =  s->r_offset;
	d->r_info =  s->r_info;
	d->r_addend = s->r_addend;
}

//Elf32 *

Elf32 *initELF(Elf32_Ehdr * data){
	int i,exite;

	Elf32* e = malloc(sizeof(Elf32));

	//init header
	e->header = initHeader(data);

	//init section table
	Elf32_Shdr *section_header_start = get_section_table(data);
	int nbSec = get_section_nb(data);
	e->table_section = malloc(sizeof(Elf32_Shdr) * nbSec);
	for (i = 0; i < nbSec; i++){
		initSectionTable(section_header_start+i, e->table_section + i);
	}

	//init symbole table
	Elf32_Sym *symbole_header_start = get_symbole_table(data);
	int nbSym = get_symbole_nb(data);
	e->table_symbole = malloc(sizeof(Elf32_Sym) * nbSym);
	for (i = 0; i < nbSym; i++){
		initSymboleTable(symbole_header_start+i, e->table_symbole + i);
	}

	//init rel table

	Elf32_Rel *rel_header_start = get_rel_table(data);
	exite = sectionE(data,9);
	//printf("%d\n",exite);
	e->relE = exite;
	if(exite==1){
		int nbRel = get_rel_nb(data);
		e->nb_Rel = nbRel;
		e->table_rel = malloc(sizeof(Elf32_Rel) * nbRel);
		for (i = 0; i < nbRel; i++){
			initRelTable(rel_header_start+i, e->table_rel + i);
		}
	}
	/*
	//init rela table
	Elf32_Rela *rela_header_start = get_rela_table(data);
	exite = sectionE(data,4);
	//printf("%d\n",exite);
	e->relaE = exite;
	if(exite==1){
		int nbRela = get_rela_nb(data);
		e->nb_Rela = nbRela;
		e->table_rela = malloc(sizeof(Elf32_Rela) * nbRela);
		for (i = 0; i < nbRela; i++){
			initRelaTable(rela_header_start+i, e->table_rela + i);
		}
	}
	*/
	//valgrind
	//init nb section
	e->nb_Section = nbSec;
	//init nb symbole
	e->nb_Symbole = nbSym;
	//init string section
	char* cSec = string_section(data);
	e->string_table_section = cSec;
/*
	for(int i; i<10; i++){
		printf("%s\n",cSec+((get_section_table(data)+i)->sh_name));
	}
*/
	//printf("%d\n",exite);

	//init string symbole
	char* cSym = string_symbole(data);
	e->string_table_symbole = cSym;
/*
	for(i; i<10; i++){
		printf("%s\n",cSym+((get_symbole_table(data)+i)->st_name));
	}
	//printf("%d\n",exite);
*/
	//init tout les sections (contenue)
	e->s_h_s = get_section_table(data);
	int grom,z;Elf32_Shdr s_h;
	for (grom=0;grom<nbSec;grom++)
	{
		s_h = e->s_h_s[grom];
		e->contenue_section[grom]=(unsigned char*)((void*)data + s_h.sh_offset);
	}
	
	return e;
}

void libererELF(Elf32 *e){
	int i;
	free(e->header);
	free(e->table_section);
	free(e->table_symbole);
	free(e->table_rel);
	free(e);
}

Elf32* ajouterSection(Elf32* s, Elf32 *d, int indice){
		d->header->e_shnum ++;
		d->nb_Section ++;
		d->header->e_shentsize = d->header->e_shentsize + (s->table_section + indice)->sh_size;
		d->table_section = realloc(d->table_section, sizeof(Elf32_Shdr) * s->nb_Section);
		initSectionTable(s->table_section + indice, d->table_section+(d->nb_Section-1));		
		return d;
}

Elf32_Shdr *get_sectionwithName(Elf32 *e, char *string){
	Elf32_Shdr *section_header;
	int i;
	for ( i = 0; i < (e->header)->e_shnum; i++){
		section_header = e->table_section+i;
		if(strcmp(e->string_table_section + (e->table_section+i)->sh_name, string)==0){
			return section_header;
		}
	}
}

char *get_Nameofsection(Elf32 *e, int indice){
	return e->string_table_section + (e->table_section+indice)->sh_name;
}


Elf32 *ajouterSymbole(Elf32 *s, Elf32 *d, int indice){
		int nbS = d->nb_Symbole;
		d->nb_Symbole ++;
		//Elf32_Shdr *sec;
		get_sectionwithName(s, ".symtab")->sh_size = get_sectionwithName(s, ".symtab")->sh_size+sizeof(Elf32_Sym);
		d->table_symbole = realloc(d->table_symbole, sizeof(Elf32_Sym) * d->nb_Symbole);
		initSymboleTable(s->table_symbole + indice, d->table_symbole+(d->nb_Symbole-1));		
		return d;
}

/*
int main(int argc, char* argv[])
{
	Elf32_Ehdr *data;
	struct stat file_info; Elf32 E;
	int file_descriptor = open(argv[1], O_RDONLY);
	if (fstat(file_descriptor, &file_info))
	{
		exit(1);
	}
	//Verification du chargement en memoire du fichier
	//void *mmap(void *addr, size_t len, int protection, int flags, int fildes, off_t off);
	else if ((data = get_header(file_info, file_descriptor)) == MAP_FAILED)
	{
		exit(1);
	}
	else 
	{	
		
		//printf("%02x\n",E.header->e_flags);
		//E.table_section = get_section_table(E.header);
		//printf("%02x\n",E.table_section->sh_offset);
		initELF(data);
		initHeader(data);
		//puts("AAAAAAAAA");
		//header(data);							
		//Unmapping du fichier en mémoire
		munmap(data, file_info.st_size);
	}
	//Fermeture du fichier
	close(file_descriptor);
	return (0);
}
*/
