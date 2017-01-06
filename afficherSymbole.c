#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>



//trouve  la section .symtab (table des symboles, taille de la section, taille de chaque symboles)
Elf32_Shdr *sectionSymtab(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 2){
			//printf("%x\n",section_header->sh_offset);
			return section_header;
		}
	}
	
}

//pourquoi il marche pas????
/*
int sectionSymtab2(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 2){
			printf("offset: %x\n",section_header->sh_offset);
			Elf32_Shdr *symtab = (Elf32_Shdr*)((void*)data + (int)section_header->sh_offset);
			printf("size: %x\n",symtab->sh_offset);
			return (int)(section_header->sh_offset);
		}
	}
	
}
*/
//trouve la section .shstrtab (les nom des sections)
Elf32_Shdr *sectionShstrtab(Elf32_Ehdr* data){
	int i;
	int nb= 0;
	int tailleSym = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header = section_header_start + data->e_shstrndx;
	//printf("%x\n",section_header->sh_offset);
	return section_header;
}


//trouve la section .strtab (les nom des symbole)
Elf32_Shdr *sectionStrtab(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	Elf32_Shdr *shstrtab = sectionShstrtab(data);
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if((section_header->sh_type == 3)&&(section_header->sh_offset != shstrtab->sh_offset)){
			//printf("strtab: %x\n",section_header->sh_offset);
			return section_header;
		}
	}
	return section_header;
}




//trouve la section .rel
Elf32_Shdr *sectionRel(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 9){
			return section_header;
		}
	}
	return section_header;
}

//section.rel exsite
int sectionRelE(Elf32_Ehdr* data){
	int e = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 9){
			e = 1;		
		}
	}
	return e;
}


//trouve la section .rela
Elf32_Shdr *sectionRela(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 4){
			return section_header;
			break;
		}
	}

}


//section.rela exsite
int sectionRelaE(Elf32_Ehdr* data){
	int e = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;
	for (int i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(section_header->sh_type == 4){
			e = 1;		
		}
	}
	return e;
}

//string symbole 
char *stringSym(Elf32_Ehdr * data){
	char *stringSym = (char*)((void*)data + sectionStrtab(data)->sh_offset);
	return stringSym;
}


//string section
char *stringSec(Elf32_Ehdr * data){
	char *stringSec = (char*)((void*)data + sectionShstrtab(data)->sh_offset);
	return stringSec;
}


void afficherSym(Elf32_Ehdr * data)
{
	int i,nb;
	
	Elf32_Shdr *symbole = sectionSymtab(data);
	Elf32_Shdr *string_Sym = sectionStrtab(data);
	Elf32_Sym *symbole_Start = (Elf32_Sym*)((void*)data + sectionSymtab(data)->sh_offset);
	Elf32_Sym *section_header;
	char *strSym = stringSym(data);
	char err_bind[] = "Impossible de lire lien du symbole\n";
	char bind[12], type[12];
	
	nb = symbole->sh_size/symbole->sh_entsize;

	printf("Table de symboles << .symtab >> contient %d entrées:\n",nb);
	printf("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
	for (i = 0; i < nb; i++)
    {
		section_header = symbole_Start+i;
//Num						
		printf("%6d: ",i);
//Value		
		printf("%08x",section_header->st_value);
//Tail		
		printf("%6d ",section_header->st_size);
//Type	
		switch (ELF32_ST_TYPE(section_header->st_info)) {
			case 0 : 
				strcpy(type, "NOTYPE");
				break;
			case 1 : 
				strcpy(type, "OBJCET");
				break;	
			case 2 : 
				strcpy(type, "FUNC");
				break;	
			case 3 : 
				strcpy(type, "SECTION");
				break;	
			case 4 : 
				strcpy(type, "FILE");
				break;	
			case 13 : 
				strcpy(type, "LOPROC");
				break;	
			case 14 : 
				strcpy(type, "HIPROC");
				break;	
		} 
		printf("%-7s ",type);
//Lien
		switch (ELF32_ST_BIND(section_header->st_info)){
			case 0 :
				strcpy(bind, "LOCAL");
				break;
			case 1 :
				strcpy(bind, "GLOBAL");
				break;
			case 2 :
				strcpy(bind, "WEAK");
				break;
			case 13 :
				strcpy(bind, "LOPROC");
				break;
			case 15 :
				strcpy(bind, "HIPROC");
				break;	
			default :
				printf("%s\n", err_bind);
				break;		
		}	
		printf("%-6s ",bind);
//Vis
		if(section_header->st_other == 0){
			printf("DEFAULT  ");
		}
//Ndx
		if(section_header->st_shndx == 0){
			printf("UND ");
		}
		else if(section_header->st_shndx == 0xfff1){
			printf("ABS ");
		}
		else{
			printf("%3x ",section_header->st_shndx);
		}
//Nom
		printf("%-10s\n",strSym + section_header->st_name);
	}



}

int main(int argc, char *argv[])
{
  int fd;
  void *data;
  struct stat file_infos;

  if (argc == 2)
    {
      if ((fd = open(argv[1], O_RDONLY)) > 0)
	{
	  if (fstat(fd, &file_infos))
	    fprintf(stderr, "Recuperation des informations du fichier impossible\n");
	  else if ((data = mmap(0, file_infos.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	    fprintf(stderr, "Chargement du fichier en memoire impossible\n");
	  else
	    {
		//afficherSym1((Elf32_Ehdr*)data);
		afficherSym((Elf32_Ehdr*)data);
	/*	stringSym((Elf32_Ehdr*)data);
		sectionSymtab((Elf32_Ehdr*)data);
		sectionStrtab((Elf32_Ehdr*)data);
		sectionRel((Elf32_Ehdr*)data);
		sectionRelE((Elf32_Ehdr*)data);
		sectionRela((Elf32_Ehdr*)data);
		sectionRelaE((Elf32_Ehdr*)data);
		sectionShstrtab((Elf32_Ehdr*)data);
		sectionStrtab((Elf32_Ehdr*)data);
	*/	
		munmap(data, file_infos.st_size);	      
	    }
	  close(fd);
	}
      else
	fprintf(stderr, "Ouverture du fichier impossible\n");
    }
  else
    fprintf(stderr, "USAGE: %s <binary_file>\n", argv[0]);
  return (0);
}
