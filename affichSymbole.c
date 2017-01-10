#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>





///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 4///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//trouve  la section .symtab (table des symboles, taille de la section, taille de chaque symboles)



//trouve la section .strtab (les nom des symbole)
Elf32_Shdr *find_section(Elf32_Ehdr* data, char *string, int type){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header;int i;
	//On récupère la section string
	Elf32_Shdr section_string = section_header_start[data->e_shstrndx];
	//On récupère la première string
	char *strings = (char*)((void*)data + section_string.sh_offset);
	for ( i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if((section_header->sh_type == type) && (strcmp(strings + section_header->sh_name, string)==0) ){
		
			//printf("strtab: %x\n",section_header->sh_offset);
			return section_header;
		}
	}
	return NULL;
}


//string symbole 
char *stringSym(Elf32_Ehdr * data){
	//Elf32_Shdr *string_sym = ;
	
	char *stringSym = (char*)((void*)data + find_section(data, ".strtab", 3)->sh_offset);
	return stringSym;
}

//string section
char *stringSec(Elf32_Ehdr * data){
	char *stringSec = (char*)((void*)data + find_section(data, ".shstrtab", 3)->sh_offset);
	return stringSec;
}

void print_sym(Elf32_Ehdr * data)
{
		int i,nb;
	//Initialisation 
	//On récupère la section symbole
	Elf32_Shdr *symbole = find_section(data, ".symtab", 2);
	//On récupère la section des noms des symboles
	Elf32_Shdr *string_Sym = find_section(data, ".strtab", 3);
	printf("Avant appel find section\n");
	Elf32_Sym *symbole_Start = (Elf32_Sym*)((void*)data + find_section(data,  ".symtab", 2)->sh_offset);
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
		print_sym((Elf32_Ehdr*)data);
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
