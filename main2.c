#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>




void afficherSym(Elf32_Ehdr * data)
{
	int i;
	int nb= 0;
	int tailleSym = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *sections_string = section_header_start + data->e_shstrndx;
	Elf32_Shdr *section_header;
	Elf32_Shdr *section_Sym;
	Elf32_Shdr *string_Sym;
	char *stringsSec = (char*)((void*)data + sections_string->sh_offset);
	char nom1[] = ".symtab";
	char nom2[] = ".strtab";
	
//trouve le offset de la section .symtab et la section .strtab
	for (i = 0; i < data->e_shnum; i++){
		section_header = section_header_start+i;
		if(strcmp((stringsSec + section_header->sh_name),nom1)==0){
			section_Sym = section_header;
			nb = section_header->sh_size/section_header->sh_entsize;
			tailleSym = section_header->sh_entsize; 
		}
		if(strcmp((stringsSec + section_header->sh_name),nom2)==0){
			string_Sym = section_header;
			//break;
		}
	}


//afficher les noms des symboles
	Elf32_Sym *sym_Start = (Elf32_Sym*)((void*)data + section_Sym->sh_offset);
	Elf32_Sym *section_header2;
	char *stringsSym = (char*)((void*)data + string_Sym->sh_offset);
	char err_bind[] = "Impossible de lire lien du symbole\n";
	char bind[12], type[12];

	printf("Table de symboles << .symtab >> contient %d entrées:\n",nb);
	printf("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
	for (i = 0; i < nb; i++)
    {
		section_header2 = sym_Start+i;
//Num						
		printf("%6d: ",i);
//Value		
		printf("%d0000000",sym_Start->st_value);
//Tail		
		printf("%6d ",section_header2->st_size);
//Type	
		switch (ELF32_ST_TYPE(section_header2->st_info)) {
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
		switch (ELF32_ST_BIND(section_header2->st_info)){
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
		if(section_header2->st_other == 0){
			printf("DEFAULT  ");
		}
//Ndx
		if(section_header2->st_shndx == 0){
			printf("UND ");
		}
		else if(section_header2->st_shndx == 0xfff1){
			printf("ABS ");
		}
		else{
			printf("%3x ",section_header2->st_shndx);
		}
//Nom
		printf("%-10s\n",stringsSym + section_header2->st_name);
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
		  afficherSym((Elf32_Ehdr*)data);
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
