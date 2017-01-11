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
#include "lecture_paq.h" 
#include "ecriture.h"



//char *tab_type_section[14] = {"NULL", "PROGBITS", "SYMTAB" , "STRTAB", "RELA", "HASH", "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "PROCSPECSEM", "APPINDEX"};

char * tab_classe[2]={"ELF32","ELF64"};

char * tab_type_sym[7]={"NOTYPE", "OBJCET", "FUNC","SECTION","FILE","LOPROC","HIPROC"};

char * tab_lien_sym[5]={"LOCAL","GLOBAL","WEAK","LOPROC","HIPROC"};

char *tab_donnees[2]={"little_endian","big_endian"};

char * tab_header_type[4]= {"Fichier repositionnable","Fichier exécutable","Objet partagé","Fichier core"};



char *get_flag(int flags){
	char* str_flag;
	if (flags & (1u << 0)){	//écriture
			strcpy(str_flag,"W");str_flag++;
		}
		if (flags & (1u << 1)){	//allocation
			strcpy(str_flag,"A");str_flag++;
		}
		if (flags & (1u << 2)){	//exécution
			strcpy(str_flag,"X");str_flag++;
		}
		if (flags & (1u << 4)){	//fusion
			strcpy(str_flag,"M");str_flag++;
		}
		if (flags & (1u << 5)){	//chaînes
			strcpy(str_flag,"S");str_flag++;
		}
		if (flags & (1u << 6)){	//info
			strcpy(str_flag,"I");str_flag++;
		}
		if (flags & (1u << 7)){	//ordre des liens
			strcpy(str_flag,"L");str_flag++;
		}
		if (flags & (1u << 8)){	//os nonconforming
			strcpy(str_flag,"O");str_flag++;
		}
		if (flags & (1u << 9)){	//groupe
			strcpy(str_flag,"G");str_flag++;
		}
		if (flags & (1u << 10)){	//TLS
			strcpy(str_flag,"T");str_flag++;
		}
}

void afficherHeader(Elf32 *e){

	int i;
	//Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
	/* //Nb Magique	
	printf("e:  ");
	for(i=0;i<EI_NIDENT;i++){
		printf("%02x ",(e->header)->e_ident[i]);
	}	puts(" ");*/
	//header->e_ident = h->e_ident;
	printf("Classe :%s \n ", tab_classe[(e->header)->e_ident[4]-1]);
	printf("Données : %s\n", tab_donnees[(e->header)->e_ident[5]-1]);	
	printf("  Version: %x\n", (e->header)->e_ident[6]);
	printf("  OS/ABI:%d\n", (e->header)->e_ident[7]);
	printf("  Version ABI:%d\n", (e->header)->e_ident[8]);
	printf("Type : %s \n",tab_header_type[(e->header)->e_type]);
	printf("Code Machine : %02x \n",(e->header)->e_machine);
	printf("Version %02x \n",(e->header)->e_version);
	printf("Adresse du point d'entrée%02x \n",(e->header)->e_entry);
	printf("Début des en-têtes de programmes%02x \n",(e->header)->e_phoff);
	printf("Débuts des en-têtes de sections %02x \n",(e->header)->e_shoff);
	printf("Fanions : %02x \n",(e->header)->e_flags);
	printf("Taille en-tête%02x \n",(e->header)->e_ehsize);
	printf("Taille en-tête programme%02x \n",(e->header)->e_phentsize);
	printf("Nombre d'en-tête de programme%02x \n",(e->header)->e_phnum);
	printf("Taille en-tête de section%02x \n",(e->header)->e_shentsize);
	printf("Nombre de section : %02x \n",(e->header)->e_shnum);
	printf("Table d'indexes des chaines d'en-têtes de section : %02x \n\n",(e->header)->e_shstrndx);
}
 
void afficherSectionTable(Elf32 *e){
	int i;
	printf("Il y a %d en-têtes de sections \n", e->nb_Section);
	
	printf("[Nr] Nom               Type            Adr      Décala.Taille ES Fan LN Inf Al\n");
	for(i=0;i<e->nb_Section;i++){
		printf("[%02d] ", i);
		printf("                 "); //(e->table_section + i)->sh_name

     	printf(" %-15.15s",get_section_type_name ((e->table_section + i)->sh_type));			
		
		//Addr
		printf(" %08x", (e->table_section + i)->sh_addr);
		//Off
		printf(" %6.6lx",(e->table_section + i)->sh_offset);
		//Size
		printf(" %6.6lx", (e->table_section + i)->sh_size);
		//ES
		printf(" %2.2lx", (e->table_section + i)->sh_entsize);
		//printf("T%02x \n",(e->table_section + i)->sh_entsize);
		printf(" %3s",get_flag((e->table_section + i)->sh_flags));
		//Lk
		printf(" %2d", (e->table_section + i)->sh_link);
		//Inf
		printf(" %3d", (e->table_section + i)->sh_info);
		//Al
		printf(" %2d", (e->table_section + i)->sh_addralign);
		printf("\n");
	}	
}




void afficherSectionSymbole(Elf32 *e){
	int i, type, bind;
	printf("Table de symboles << .symtab >> contient %d entrées:\n",e->nb_Symbole);
	printf("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");
	for (i = 0; i < e->nb_Symbole; i++){
		printf("%6d: ",i);

		//affichage de la value
		printf("%08x ",(e->table_symbole + i)->st_value);

		//affichage de la taille
		printf("%6d ",(e->table_symbole + i)->st_size);

		//Type
		type = ELF32_ST_TYPE((e->table_symbole + i)->st_info);
		if(type == 13) { 
			printf("%-7s ", tab_type_sym[5]);
		}	
		else if(type == 14 ){ 
			printf("%-7s ", tab_type_sym[6]);
		}
		else {
			printf("%-7s ", tab_type_sym[type]);
		}

		//affichage du lien
		bind = ELF32_ST_BIND((e->table_symbole + i)->st_info);
		if( type == 13) { 
			printf("%-7s", tab_type_sym[3]);
		}	
		else if(type == 15 ){ 
			printf("%-7s", tab_type_sym[4]);
		}
		else {
			printf("%-7s", tab_lien_sym[bind]);
		}


		if((e->table_symbole + i)->st_other == 0){
			printf(" DEFAULT");
		}

		if((e->table_symbole + i)->st_shndx == 0){
			printf(" UND ");
		}
		else if((e->table_symbole + i)->st_shndx == 0xfff1){
			printf(" ABS ");
		}
		else{
			printf("%3x ",(e->table_symbole + i)->st_shndx);
		}
		printf("\n");
		//printf("%08x ",(e->table_symbole + i)->st_name);

	}
}
