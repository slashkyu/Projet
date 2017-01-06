#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>

/*
char* typeRelo(Elf32_rel rel_header){
	char type[30];
	switch (ELF32_R_TYPE(rel_header->r_info))) {
		case 0 : 
			strcpy(type, "R_ARM_NONE");
			break;
		case 1 : 
			strcpy(type, "R_ARM_PC24");
			break;	
		case 2 : 
			strcpy(type, "R_ARM_ABS32");
			break;	
		case 3 : 
			strcpy(type, "R_ARM_REL32");
			break;	
		case 4 : 
			strcpy(type, "R_ARM_LDR_PC_G0");
			break;	
		case 5 : 
			strcpy(type, "R_ARM_ABS16");
			break;	
		case 6 : 
			strcpy(type, "R_ARM_ABS12");
			break;		
		case 8 : 
			strcpy(type, "R_ARM_ABS_8");
			break;	
		case 9 : 
			strcpy(type, "R_ARM_SBREL32");
			break;		
		case 12 : 
			strcpy(type, "R_ARM_BREL_ADJ");
			break;	
		case 13 : 
			strcpy(type, "R_ARM_TLS_DESC");
			break;	
		case 14 : 
			strcpy(type, "R_ARM_THM_SWI8");
			break;
		case 15 : 
			strcpy(type, "R_ARM_XPC25");
			break;	
		case 16 : 
			strcpy(type, "R_ARM_THM_XPC22");
			break;	
		case 17 : 
			strcpy(type, "R_ARM_TLS_DTPMOD32");
			break;	
		case 18 : 
			strcpy(type, "R_ARM_TLS_DTPOFF32");
			break;	
		case 19 : 
			strcpy(type, "R_ARM_TLS_TPOFF32");
			break;	
		case 20 : 
			strcpy(type, "R_ARM_COPY");
			break;	
		case 31 : 
			strcpy(type, "R_ARM_BASE_ABS");
			break;	
		case 32 : 
			strcpy(type, "R_ARM_ALU_PCREL_7_0");
			break;	
		case 33 : 
			strcpy(type, "R_ARM_ALU_PCREL_15_8");
			break;	
		case 34 : 
			strcpy(type, "R_ARM_ALU_PCREL_23_15");
			break;
		case 35 : 
			strcpy(type, "R_ARM_LDR_SBREL_11_0_NC");
			break;	
		case 36 : 
			strcpy(type, "R_ARM_ALU_SBREL_19_12_NC");
			break;	
		case 37 : 
			strcpy(type, "R_ARM_ALU_SBREL_27_20_CK");
			break;	
		case 38 : 
			strcpy(type, "R_ARM_TARGET1");
			break;	
		case 39 : 
			strcpy(type, "R_ARM_SBREL31");
			break;	
		case 40 : 
			strcpy(type, "R_ARM_V4BX");
			break;	
		case 41 : 
			strcpy(type, "R_ARM_TARGET2");
			break;	
		case 42 : 
			strcpy(type, "R_ARM_PREL31");
			break;	
		case 43 : 
			strcpy(type, "R_ARM_MOVW_ABS_NC");
			break;
		case 44 : 
			strcpy(type, "R_ARM_MOVT_ABS");
			break;	
		case 45 : 
			strcpy(type, "R_ARM_MOVW_PREL_NC");
			break;	
		case 46 : 
			strcpy(type, "R_ARM_MOVT_PREL");
			break;	
		case 55 : 
			strcpy(type, "R_ARM_ABS32_NOI");
			break;	
		case 56 : 
			strcpy(type, "R_ARM_REL32_NOI");
			break;	
		case 57 : 
			strcpy(type, "R_ARM_ALU_PC_G0_NC ");
			break;	
		case 58 : 
			strcpy(type, "R_ARM_ALU_PC_G0 ");
			break;
		case 59 : 
			strcpy(type, "R_ARM_ALU_PC_G1_NC");
			break;
		case 60 : 
			strcpy(type, "R_ARM_ALU_PC_G1");
			break;	
		case 61 : 
			strcpy(type, "R_ARM_ALU_PC_G2");
			break;	
		case 62 : 
			strcpy(type, "R_ARM_LDR_PC_G1");
			break;	
		case 63 : 
			strcpy(type, "R_ARM_LDR_PC_G2");
			break;
		case 64 : 
			strcpy(type, "R_ARM_LDRS_PC_G0");
			break;	
		case 65 : 
			strcpy(type, "R_ARM_LDRS_PC_G1");
			break;	
		case 66 : 
			strcpy(type, "R_ARM_LDRS_PC_G2");
			break;
		case 67 : 
			strcpy(type, "R_ARM_LDC_PC_G0");
			break;	
		case 68 : 
			strcpy(type, "R_ARM_LDC_PC_G1");
			break;	
		case 69 : 
			strcpy(type, "R_ARM_LDC_PC_G2");
			break;	
		case 70 : 
			strcpy(type, "R_ARM_ALU_SB_G0_NC");
			break;
		case 71 :
			strcpy(type, "R_ARM_ALU_SB_G0");
			break;
		case 72 :
			strcpy(type, "R_ARM_ALU_SB_G1_NC");
			break;
		case 73 :
			strcpy(type, "R_ARM_ALU_SB_G1");
			break;
		case 74 :
			strcpy(type, "R_ARM_ALU_SB_G2");
			break;
		case 75 :
			strcpy(type, "R_ARM_LDR_SB_G0");
			break;
		case 76 :
			strcpy(type, "R_ARM_LDR_SB_G1");
			break;
		case 77 :
			strcpy(type, "R_ARM_LDR_SB_G2");
			break;
		case 78 :
			strcpy(type, "R_ARM_LDRS_SB_G0");
			break;
		case 79 :
			strcpy(type, "R_ARM_LDRS_SB_G1");
			break;
		case 80 :
			strcpy(type, "R_ARM_LDRS_SB_G2");
			break;
		case 81 :
			strcpy(type, "R_ARM_LDC_SB_G0");
			break;
		case 82 :
			strcpy(type, "R_ARM_LDC_SB_G1");
			break;
		case 83 :
			strcpy(type, "R_ARM_LDC_SB_G2");
			break;
		case 84 :
			strcpy(type, "R_ARM_MOVW_BREL_NC");
			break;
		case 85 :
			strcpy(type, "R_ARM_MOVT_BREL");
			break;
		case 86 :
			strcpy(type, "R_ARM_MOVW_BREL");
			break;
		case 90 :
			strcpy(type, "R_ARM_TLS_GOTDESC");
			break;
		case 91 :
			strcpy(type, "R_ARM_TLS_CALL");
			break;
		case 92 :
			strcpy(type, "R_ARM_TLS_DESCSEQ");
			break;
		case 94 :
			strcpy(type, "R_ARM_PLT32_ABS");
			break;
		case 95 :
			strcpy(type, "R_ARM_GOT_ABS");
			break;
		case 96 :
			strcpy(type, "R_ARM_GOT_PREL");
			break;
		case 97 :
			strcpy(type, "R_ARM_GOT_BREL12");
			break;
		case 98 :
			strcpy(type, "R_ARM_GOTOFF12");
			break;
		case 99 :
			strcpy(type, "R_ARM_GOTRELAX");
			break;
		case 100 :
			strcpy(type, "R_ARM_GNU_VTENTRY");
			break;
		case 101 :
			strcpy(type, "R_ARM_GNU_VTINHERIT");
			break;
		case 104 :
			strcpy(type, "R_ARM_TLS_GD32");
			break;
		case 105 :
			strcpy(type, "R_ARM_TLS_LDM32");
			break;
		case 106 :
			strcpy(type, "R_ARM_TLS_LD032");
			break;
		case 107 :
			strcpy(type, "R_ARM_TLS_IE32");
			break;
		case 108 :
			strcpy(type, "R_ARM_TLS_LE32");
			break;
		case 109 :
			strcpy(type, "R_ARM_TLS_LD012");
			break;
		case 110 :
			strcpy(type, "R_ARM_TLS_LE12");
			break;
		case 111 :
			strcpy(type, "R_ARM_TLS_IE12GP");
			break;
	}
	return type;
}
*/

void afficherReloc(Elf32_Ehdr * data)
{
	int i;
	int nb= 0;
	int tailleSymRela = 0;
	int tailleSymRel = 0;
	int nbSymRela = 0;
	int nbSymRel = 0;
	int relaexiste = 0;
	int relexiste = 0;
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *sections_string = section_header_start + data->e_shstrndx;
	Elf32_Shdr *section_header;
	Elf32_Shdr *section_rela;
	Elf32_Shdr *section_rel;
	Elf32_Shdr *section_strtab;
	Elf32_Shdr *section_sym;
	char *stringsSec = (char*)((void*)data + sections_string->sh_offset);
	
	
//trouve le offset de la section SHT_RELA et la section SHT_REL
	for (i = 0; i < data->e_shnum; i++){
//rela
		section_header = section_header_start+i;
		if(section_header->sh_type == 4){
			section_rela = section_header;
			relaexiste = 1;
			tailleSymRela = section_rela->sh_entsize;
			nbSymRela = section_rela->sh_size/tailleSymRela;
			printf("%d: %02x\n",nbSymRela,section_rela->sh_offset);
		}
//rel
		if(section_header->sh_type == 9){
			section_rel = section_header;
			relexiste = 1;
			tailleSymRel = section_rel->sh_entsize;
			nbSymRel = section_rel->sh_size/tailleSymRel;
			printf("%d: %02x\n",nbSymRel,section_rel->sh_offset);
		}
//strtab
		if(section_header->sh_type == 3){
			section_strtab = section_header;
		}
		if(section_header->sh_type == 2){
			section_sym = section_header;
			nb = section_header->sh_size/section_header->sh_entsize;
		}
	}
	printf("rela: %d\n",nb);
	printf("rel: %d\n",relexiste);
	char *strtab = (char*)((void*)data + section_strtab->sh_offset);

	

	//Elf32_Shdr *string_Sym;

	
	Elf32_Sym *sym_Start = (Elf32_Sym*)((void*)data + section_sym->sh_offset);
	Elf32_Sym *section_header2;

	char *stringsSym = (char*)((void*)data + section_strtab->sh_offset);


/*

	if(relaexiste = 1){
		Elf32_Rela *rel_Start = (Elf32_Rela*)((void*)data + section_rela->sh_offset);
		Elf32_Rela *rela_header;
		for(i=0;i<nbSymRela;i++){
			
		}	
	}
*/
	if(relexiste = 1){
		
		Elf32_Rel *rel_Start = (Elf32_Rel*)((void*)data + section_rel->sh_offset);
		Elf32_Rel *rel_header;
		for(i=0;i<nbSymRel;i++){
			rel_header = rel_Start+i;
//Décalage
			
			printf("a:%08x ",(rel_header->r_offset));
//Info
			
			printf("b:%08x ",(ELF32_R_INFO(ELF32_R_SYM(rel_header->r_info),ELF32_R_TYPE(rel_header->r_info))));
//Type
				
			printf("c:%-x ",(ELF32_R_TYPE(rel_header->r_info)));


//Sym	
			printf("d:%08x \n",(ELF32_R_SYM(rel_header->r_info)));
//nom Sym

			for(i=0;i<nb;i++){
				printf("e:");
				if(nb == ELF32_R_SYM(rel_header->r_info)){
					printf("f:");
					section_header2 = sym_Start+i;
					if((stringsSym + section_header2->st_name)!= 0){
						printf("%-10s\n",stringsSym + section_header2->st_name);
					}	
				}
			}
		
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
