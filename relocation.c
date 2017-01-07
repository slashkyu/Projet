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



void afficherReloc(Elf32_Ehdr *data){
	int i;
	int nb= 0;
	int tailleSymRela = 0;
	int tailleSymRel = 0;
	int nbSymRela = 0;
	int nbSymRel = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *sections_string = section_header_start + data->e_shstrndx;
	Elf32_Shdr *section_header;
	Elf32_Shdr *section_rela;
	Elf32_Shdr *section_rel;
	Elf32_Shdr *section_strtab;
	Elf32_Shdr *section_sym;
	char *stringsSec = (char*)((void*)data + sections_string->sh_offset);
//trouve le offset de la section SHT_RELA et la section SHT_REL
	section_rela = sectionRela(data);
	section_rel = sectionRel(data);
	section_sym = sectionSymtab(data);
	char *strSym = stringSym(data);
	
	char type[25];
	if(sectionRelE(data)){	
		Elf32_Rel *rel_Start = (Elf32_Rel*)((void*)data + section_rel->sh_offset);
		Elf32_Sym *sym_Start = (Elf32_Sym*)((void*)data + section_sym->sh_offset);
		Elf32_Rel *rel_header;
		int nb_rel = section_rel->sh_size/section_rel->sh_entsize;
		tailleSymRel = section_rel->sh_entsize;
		nbSymRel = section_rel->sh_size/tailleSymRel;


		printf("Section de relocalisation '.rel.text' à l'adresse de décalage 0x%02x contient %d entrées\n",nb_rel,section_rel->sh_offset);
		printf("Décalage   Info    Type             Sym.Value   \n");
		for(i=0;i<nbSymRel;i++){
			rel_header = rel_Start+i;
			/*
			if((ELF32_ST_TYPE((sym_Start+ELF32_R_SYM(rel_header->r_info))->st_info)) != 3){
				printf("name: %s\n",strSym+(sym_Start+i)->st_name);
			}
			*/
//Décalage	
			printf("%08x  ",(rel_header->r_offset));
//Info	
			printf("%08x ",(ELF32_R_INFO(ELF32_R_SYM(rel_header->r_info),ELF32_R_TYPE(rel_header->r_info))));
//Type	
			switch (ELF32_R_TYPE(rel_header->r_info)) {
				case 0x1c : 
					strcpy(type, "R_ARM_CALL");
					break;
				case 0x2 : 
					strcpy(type, "R_ARM_ABS32");
					break;		
			} 
			printf("%-17s ",type);
//Sym	
			printf("%08x \n",(ELF32_R_SYM(rel_header->r_info)));
//nom Sym
		}	
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
		afficherReloc((Elf32_Ehdr*)data);
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
