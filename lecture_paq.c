#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <elf.h>
#include "lecture_paq.h"

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 1///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

static void
decode_ARM_machine_flags (unsigned e_flags, char buf[])
{
  unsigned eabi;
  int unknown = 0;

  eabi = EF_ARM_EABI_VERSION (e_flags);
  e_flags &= ~ EF_ARM_EABIMASK;

  /* Handle "generic" ARM flags.  */
  if (e_flags & EF_ARM_RELEXEC)
    {
      strcat (buf, ", relocatable executable");
      e_flags &= ~ EF_ARM_RELEXEC;
    }

  if (e_flags & EF_ARM_HASENTRY)
    {
      strcat (buf, ", has entry point");
      e_flags &= ~ EF_ARM_HASENTRY;
    }

  /* Now handle EABI specific flags.  */
  switch (eabi)
    {
    default:
      strcat (buf, ", <unrecognized EABI>");
      if (e_flags)
	unknown = 1;
      break;

    case EF_ARM_EABI_VER1:
      strcat (buf, ", Version1 EABI");
      while (e_flags)
	{
	  unsigned flag;

	  /* Process flags one bit at a time.  */
	  flag = e_flags & - e_flags;
	  e_flags &= ~ flag;

	  switch (flag)
	    {
	    case EF_ARM_SYMSARESORTED: /* Conflicts with EF_ARM_INTERWORK.  */
	      strcat (buf, ", sorted symbol tables");
	      break;

	    default:
	      unknown = 1;
	      break;
	    }
	}
      break;

    case EF_ARM_EABI_VER2:
      strcat (buf, ", Version2 EABI");
      while (e_flags)
	{
	  unsigned flag;

	  /* Process flags one bit at a time.  */
	  flag = e_flags & - e_flags;
	  e_flags &= ~ flag;

	  switch (flag)
	    {
	    case EF_ARM_SYMSARESORTED: /* Conflicts with EF_ARM_INTERWORK.  */
	      strcat (buf, ", sorted symbol tables");
	      break;

	    case EF_ARM_DYNSYMSUSESEGIDX:
	      strcat (buf, ", dynamic symbols use segment index");
	      break;

	    case EF_ARM_MAPSYMSFIRST:
	      strcat (buf, ", mapping symbols precede others");
	      break;

	    default:
	      unknown = 1;
	      break;
	    }
	}
      break;

    case EF_ARM_EABI_VER3:
      strcat (buf, ", Version3 EABI");
      break;

    case EF_ARM_EABI_VER4:
      strcat (buf, ", Version4 EABI");
      while (e_flags)
	{
	  unsigned flag;

	  /* Process flags one bit at a time.  */
	  flag = e_flags & - e_flags;
	  e_flags &= ~ flag;

	  switch (flag)
	    {
	    case EF_ARM_BE8:
	      strcat (buf, ", BE8");
	      break;

	    case EF_ARM_LE8:
	      strcat (buf, ", LE8");
	      break;

	    default:
	      unknown = 1;
	      break;
	    }
	}
      break;
	case EF_ARM_EABI_VER5:
      strcat (buf, ", Version5 EABI");
      while (e_flags)
	{
	  unsigned flag;
	  /* Process flags one bit at a time.  */
	  flag = e_flags & - e_flags;
	  e_flags &= ~ flag;
	  switch (flag)
	    {
	    case EF_ARM_BE8:
	      strcat (buf, ", BE8");
	      break;
	    case EF_ARM_LE8:
	      strcat (buf, ", LE8");
	      break;
	    case EF_ARM_ABI_FLOAT_SOFT: /* Conflicts with EF_ARM_SOFT_FLOAT.  */
	      strcat (buf, ", soft-float ABI");
	      break;
	    case EF_ARM_ABI_FLOAT_HARD: /* Conflicts with EF_ARM_VFP_FLOAT.  */
	      strcat (buf, ", hard-float ABI");
	      break;
	    default:
	      unknown = 1;
	      break;
	    }
	}
      break;

	
    case EF_ARM_EABI_UNKNOWN:
      strcat (buf, ", GNU EABI");
      while (e_flags)
	{
	  unsigned flag;

	  /* Process flags one bit at a time.  */
	  flag = e_flags & - e_flags;
	  e_flags &= ~ flag;

	  switch (flag)
	    {
	    case EF_ARM_INTERWORK:
	      strcat (buf, ", interworking enabled");
	      break;

	    case EF_ARM_APCS_26:
	      strcat (buf, ", uses APCS/26");
	      break;

	    case EF_ARM_APCS_FLOAT:
	      strcat (buf, ", uses APCS/float");
	      break;

	    case EF_ARM_PIC:
	      strcat (buf, ", position independent");
	      break;

	    case EF_ARM_ALIGN8:
	      strcat (buf, ", 8 bit structure alignment");
	      break;

	    case EF_ARM_NEW_ABI:
	      strcat (buf, ", uses new ABI");
	      break;

	    case EF_ARM_OLD_ABI:
	      strcat (buf, ", uses old ABI");
	      break;

	    case EF_ARM_SOFT_FLOAT:
	      strcat (buf, ", software FP");
	      break;

	    case EF_ARM_VFP_FLOAT:
	      strcat (buf, ", VFP");
	      break;

	    case EF_ARM_MAVERICK_FLOAT:
	      strcat (buf, ", Maverick FP");
	      break;

	    default:
	      unknown = 1;
	      break;
	    }
	}
    }

  if (unknown)
    strcat (buf,", <unknown>");
}

static char *
get_machine_flags (unsigned e_flags, unsigned e_machine)
{
	static char buf[1024];

	buf[0] = '\0';

	if (e_flags)
	{
		switch (e_machine)
		{
			default:
			  break;

			case EM_ARM:
			  decode_ARM_machine_flags (e_flags, buf);
			  break;
		}
	}

	return buf;
}

// Procédure d'affichage des infos du header ELF
void print_header(Elf32_Ehdr * data)
{
	int index;
	//ELF Header
	puts((language)?"ELF Header:":"En-tête ELF:");
	//Magic
	printf("%s",(language)?"  Magic:   ":"  Magique:   ");
	for (index = 0; index < EI_NIDENT; index++)
	{
		printf("%2.2x ", data->e_ident[index]);
	}
	puts(" ");
	//Class
	if (data->e_ident[4] == 1)
	{
		printf("  %s                            ELF32\n", (language)?"Class: ":"Classe:");
	}
	else if (data->e_ident[4] == 2)
	{
		printf("  %s                            ELF64\n", (language)?"Class: ":"Classe:");
	}
	else if (data->e_ident[4] == 0)
	{
		printf("  %s                            %s\n", (language)?"Class: ":"Classe:", (language)?"None":"Aucun");
	}
	//Data
	if (data->e_ident[5] == 1)
	{
		printf("  %s                          %s\n",(language)?"Data:    ":"Données:",(language)?"2's complement, little endian":"complément à 2, système à octets de poids faible d'abord (little endian)");
	}
	//////////////                              /////////
	//////////////                          /////////
	else if (data->e_ident[5] == 2)
	{
		printf("  %s                          %s\n",(language)?"Data:    ":"Données:",(language)?"big endian":"système à octets de poids fort d'abord (big endian)");
	}
	else if (data->e_ident[5] == 0)
	{
		printf("  %s                          %s\n",(language)?"Data:    ":"Données:",(language)?"None":"Aucun");
	}
	//Version
	if (data->e_ident[6] == 1)
		puts("  Version:                           1 (current)");
	else if (data->e_ident[6] == 0)
		puts("  Version:                           0");
	//OS/ABI
	printf("  OS/ABI:                            ");
	switch(data->e_ident[7]) 
	{
   		case 0  :
      			puts("UNIX - System V"); break;
   		case 1  :
      			puts("Hewlett-Packard HP-UX"); break;
   		case 2  :
      			puts("NetBSD"); break;
   		case 3  :
      			puts("Linux"); break;
   		case 6  :
      			puts("Sun Solaris"); break;
   		case 7  :
      			puts("IBM AIX"); break;
   		case 8  :
      			puts("SGI IRIX"); break;
   		case 9  :
      			puts("FreeBSD"); break;
   		case 10  :
      			puts("Compaq TRU64 UNIX"); break;
   		case 11  :
      			puts("Novell Modesto"); break;
   		case 12  :
      			puts("OpenBSD"); break;
   		case 13  :
      			puts("OpenVMS"); break;
   		case 14  :
      			puts("Hewlett-Packard NonStop Kernel"); break;
   		case 15  :
      			puts("Amiga Research OS (AROS)"); break;
   		case 16  :
      			puts("Fenix OS"); break;
   		case 17  :
      			puts("Nuxi CloudABI"); break;
   		case 18  :
      			puts("Stratus Technologies OpenVOS"); break;
   		case 64  :
      			puts("ARM EABI"); break;
   		case 83  :
      			puts("Sortix"); break;
   		case 97  :
      			puts("ARM"); break;
   		case 255  :
      			puts("Standalone"); break;
   		default  :
      			printf("%s %x\n", (language)?"Other with the code":"Autre avec le code", data->e_ident[7]); break;
	}
	//ABI Version
	printf("  %s                       %d\n", (language)?"ABI Version:":"Version ABI:", data->e_ident[8]);
	//Type
	printf("  Type:                              ");
	switch(data->e_type) 
	{
   		case ET_REL  :
      			puts((language)?"REL (Relocatable file)":"REL (Fichier de relocalisation)"); break;
   		case ET_EXEC  :
      			puts((language)?"EXEC (Executable file)":"EXEC (Executable)"); break;
  		case ET_DYN  :
      			puts((language)?"DYN (Shared object file)":"DYN (Objet partagé)"); break;
  		case ET_CORE  :
      			puts((language)?"CORE (Core file)":"CORE (Fichier Core)"); break;
		case ET_NONE  :
      			puts((language)?"None":"Aucun"); break;
	}
	//Machine
	printf("  Machine:                           ");
	switch(data->e_machine) 
	{
   		case 0  :
      			puts((language)?"None":"Aucune"); break;
   		case 1  :
      			puts("AT&T WE 32100"); break;
   		case 2  :
      			puts("SPARC"); break;
   		case 3  :
      			puts("Intel 80386 (x86)"); break;
   		case 4  :
      			puts("Motorola 68000"); break;
   		case 5  :
      			puts("Motorola 88000"); break;
   		case 6  :
      			puts("Intel MCU"); break;
   		case 7  :
      			puts("Intel i860 (Intel 80860)"); break;
   		case 8  :
      			puts("MIPS I"); break;
   		case 9  :
      			puts("IBM System/370 Processor"); break;
   		case 10  :
      			puts("MIPS RS3000 Little-endian"); break;
   		case 15  :
      			puts("Hewlett-Packard PA-RISC"); break;
   		case 17  :
      			puts("Fujitsu VPP500"); break;
   		case 18  :
      			puts("Sun SPARC 32+ (Enhanced instruction set SPARC)"); break;
   		case 19  :
      			puts("Intel i960 (Intel 80960)"); break;
   		case 0x14  :
      			puts("PowerPC"); break;
   		case EM_PPC64  :
      			puts("64-bit PowerPC"); break;
   		case EM_S390  :
      			puts("IBM System/390 Processor"); break;
   		case 23  :
      			puts("IBM SPU/SPC"); break;
   		case EM_ARM  :
      			puts("ARM 32-bit architecture (AARCH32)"); break;
		case 0x2A  :
      			puts("SuperH"); break;
		case 0x32  :
      			puts("IA-64"); break;
   		case 0x3E  :
      			puts("AMD x86-64 architecture"); break;
   		case 0xB7  :
      			puts("ARM 64-bit architecture (AARCH64)"); break;
   		default  :
      			printf("%s %d\n", (language)?"Other with the code":"Autre avec le code", data->e_machine); break;
	}
	//Version
	printf("  Version:                           0x%1x\n", data->e_version);
	//Entry point address
	printf("  %s0x%0x\n", (language)?"Entry point address:               ":"Adresse du point d'entrée:         ", data->e_entry);
	//Start of program headers
	printf("  %s%d %s\n", (language)?"Start of program headers:          ":"Début des en-têtes de programme:   ", data->e_phoff, (language)?"(bytes into file)":"(octets dans le fichier)");
	//Start of section headers
	printf("  %s%d %s\n", (language)?"Start of section headers:          ":"Début des en-têtes de section:         ", data->e_shoff, (language)?"(bytes into file)":"(octets dans le fichier)");
	//Flags
  	printf("  %s0x%0x", (language)?"Flags:                             ":"Fanions:                           ", data->e_flags);
	if (data->e_entry)
		printf(", has entry point");
	if (1)
		printf("%s",get_machine_flags (data->e_flags, data->e_machine));
	puts(" ");
	//Size of this header
  	printf("  %s%d (bytes)\n", (language)?"Size of this header:               ":"Taille de cet en-tête:             ", data->e_ehsize);
	//Size of program headers
  	printf("  %s%d (bytes)\n", (language)?"Size of program headers:           ":"Taille de l'en-tête du programme:  ", data->e_phentsize);
	//Number of program headers
	printf("  %s%d\n", (language)?"Number of program headers:         ":"Nombre d'en-tête du programme:     ", data->e_phnum);
	//Size of section headers
  	printf("  %s%d (bytes)\n", (language)?"Size of section headers:           ":"Taille des en-têtes de section:    ", data->e_shentsize);
	//Number of section headers
  	printf("  %s%d\n", (language)?"Number of section headers:         ":"Nombre d'en-têtes de section:      ", data->e_shnum);
	//Section header string table index
  	printf("  %s%d\n", (language)?"Section header string table index: ":"Table d'indexes des chaînes d'en-tête de section: ", data->e_shstrndx);

}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 2///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


static const char *get_section_type_name (unsigned int sh_type)
{
  switch (sh_type)
    {
    case SHT_NULL:			return "NULL";
    case SHT_PROGBITS:		return "PROGBITS";
    case SHT_SYMTAB:		return "SYMTAB";
    case SHT_STRTAB:		return "STRTAB";
	case SHT_RELA:			return "RELA";
	case SHT_HASH:			return "HASH";
    case SHT_DYNAMIC:		return "DYNAMIC";
    case SHT_NOTE:			return "NOTE";
    case SHT_NOBITS:		return "NOBITS";
	case SHT_REL:			return "REL";
	case SHT_SHLIB:			return "SHLIB";
    case SHT_DYNSYM:		return "DYNSYM";
    case SHT_INIT_ARRAY:	return "INIT_ARRAY";
    case SHT_FINI_ARRAY:	return "FINI_ARRAY";
    case SHT_PREINIT_ARRAY:	return "PREINIT_ARRAY";
    case SHT_GROUP:			return "GROUP";
    case SHT_SYMTAB_SHNDX:	return "SYMTAB SECTION INDICIES";
    case SHT_GNU_verdef:	return "VERDEF";
    case SHT_GNU_verneed:	return "VERNEED";
    case SHT_GNU_versym:	return "VERSYM";
    case 0x6ffffff0:		return "VERSYM";
    case 0x6ffffffc:		return "VERDEF";
    case 0x70000003:		return "ARM_ATTRIBUTES";
    case 0x7ffffffd:		return "AUXILIARY";
    case 0x7fffffff:		return "FILTER";
    case SHT_GNU_LIBLIST:	return "GNU_LIBLIST";
	default: 				
		if(sh_type >= 0x70000000 && sh_type <= 0x7fffffff)
			return "PROCSPECSEM";
		else if (sh_type >= 0x80000000 && sh_type <= 0xffffffff)
			return "APPINDEX";
		else
			return "OTHER";
    }
}


void print_section_table(Elf32_Ehdr * elf_header)
{
	// on recupere un pointeur sur le premier header de section, avec le offset correspondant
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)elf_header + elf_header->e_shoff);
	// on retrouve la string table avec le nom des sections
	Elf32_Shdr sections_string = section_header_start[elf_header->e_shstrndx];
	// on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
	char *strings = (char*)((void*)elf_header + sections_string.sh_offset);
	// pour stoquer chaque section
	Elf32_Shdr section_header; unsigned int i;

	printf("There are %d section headers, starting at offset 0x%02x:\n\n",elf_header->e_shnum, elf_header->e_shoff);
	if (elf_header->e_shnum > 1)
		puts ((language)?"Section Headers:":"Table des en-têtes de sections :");
	else
		puts ((language)?"Section Header:":"Table des en-têtes de sections :");
	puts("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al");
	//Affichage du tableau
	for (i = 0; i < elf_header->e_shnum; i++)
	{
		// on recupere le header de section courrant
		section_header = section_header_start[i];
		//Nr
		printf("  [%2u]",i);
		//Name
		printf(" %-17.17s", strings + section_header.sh_name);
		//Type
		//printf(" %-15.15s");//printf(" %-15.15x",section_header.sh_type);
		printf(" %-15.15s",get_section_type_name(section_header.sh_type));
		//Addr
		printf(" %08x", section_header.sh_addr);
		//Off
		printf(" %6.6lx", section_header.sh_offset);
		//Size
		printf(" %6.6lx", section_header.sh_size);
		//ES
		printf(" %2.2lx", section_header.sh_entsize);
		//Flg
 		char str_flags[4] = "   "; char *str_flag = str_flags;
		if (section_header.sh_flags & (1u << 0)){	//écriture
			strcpy(str_flag,"W");str_flag++;
		}
		if (section_header.sh_flags & (1u << 1)){	//allocation
			strcpy(str_flag,"A");str_flag++;
		}
		if (section_header.sh_flags & (1u << 2)){	//exécution
			strcpy(str_flag,"X");str_flag++;
		}
		if (section_header.sh_flags & (1u << 4)){	//fusion
			strcpy(str_flag,"M");str_flag++;
		}
		if (section_header.sh_flags & (1u << 5)){	//chaînes
			strcpy(str_flag,"S");str_flag++;
		}
		if (section_header.sh_flags & (1u << 6)){	//info
			strcpy(str_flag,"I");str_flag++;
		}
		if (section_header.sh_flags & (1u << 7)){	//ordre des liens
			strcpy(str_flag,"L");str_flag++;
		}
		if (section_header.sh_flags & (1u << 8)){	//os nonconforming
			strcpy(str_flag,"O");str_flag++;
		}
		if (section_header.sh_flags & (1u << 9)){	//groupe
			strcpy(str_flag,"G");str_flag++;
		}
		if (section_header.sh_flags & (1u << 10)){	//TLS
			strcpy(str_flag,"T");str_flag++;
		}
		printf(" %3s",str_flags);
		//Lk
		printf(" %2d", section_header.sh_link);
		//Inf
		printf(" %3d", section_header.sh_info);
		//Al
		printf(" %2d", section_header.sh_addralign);

		puts(" ");
	}
    puts("Key to Flags:\nW (write), A (alloc), X (execute), M (merge), S (strings)");
	puts("I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)");
	puts("O (extra OS processing required) o (OS specific), p (processor specific)");
}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 3///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void print_section(Elf32_Ehdr * data)
{
	int i, section_int = -1;
	char option;
	char section_string [17];
	//Lecture de nom de section
	while (1)
	{
		puts((language)?"Section names :":"Noms de sections :");
		puts("1 .text | 2 .rel.text | 3 .data | 4 .bss | 5 .comment | \n6 .ARM.attributes | 7 .shstrtab | 8 .symtab | 9 .strtab");
		puts((language)?"Enter section number :":"Entrez numero de section :");
		scanf(" %c",&option);
		switch (option)
		{
			case '1' :
				strcpy(section_string,".text");
				break;
			case '2' :
				strcpy(section_string,".rel.text");
				break;
			case '3' :
				strcpy(section_string,".data");
				break;
			case '4' :
				strcpy(section_string,".bss");
				break;
			case '5' :
				strcpy(section_string,".comment");
				break;
			case '6' :
				strcpy(section_string,".ARM.attributes");
				break;
			case '7' :
				strcpy(section_string,".shstrtab");
				break;
			case '8' : 
				strcpy(section_string,".symtab");
				break;
			case '9' : 
				strcpy(section_string,".strtab");
				break;
			default :
				fputs(((language)?"The character in NOT valid":"Le caractere entre n'est pas valide"),stderr);
				continue;
		}
		break;
	}

	// on recupere un pointeur sur le premier header de section, avec le offset correspondant
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);

	// on retrouve la string table avec le nom des sections ..
	Elf32_Shdr sections_string = section_header_start[data->e_shstrndx];

	// on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
	char *strings = (char*)((void*)data + sections_string.sh_offset);

	// pour stoquer chaque section
	Elf32_Shdr section_header;

	//lecture de la section choisie
  
	if(section_string[0] == '0'){
		section_int=0;
		section_header = section_header_start[section_int];
	}  
	else if(atoi(section_string) < data->e_shnum && atoi(section_string) > 0)
	{	//si la section est choisie par son numéro
		section_int = atoi(section_string);
		section_header = section_header_start[section_int];
	}
	else
	{	//si la section est choisie par son nom
		i=1;
		section_header = section_header_start[i];
		while (strstr(strings + section_header.sh_name, section_string) == NULL && i < data->e_shnum)
		{
			i++;
			section_header = section_header_start[i];
		}  	
		if (i < data ->e_shnum)
			section_int = i;
	}

	if (section_int == -1)//erreur d'argument
	{
		fprintf(stderr, "%s  %s\n", ((language)?"In this file there is no section":"Dans ce fichier il n'y a pas de section"),section_string);
	}
	else
	{
		if (section_header.sh_size == 0) //La section a une taille vide
		{
			fprintf(stderr, "Section \" %s \" %s", strings + section_header.sh_name,((language)?"has nothing":"n'a pas de données à être videngé"));
		}
		else //hexdump de la section
		{
			unsigned char *content = (unsigned char*)((void*)data + section_header.sh_offset);
			i=0;
			printf((language)?"Hexdump of section ":"Vidange hexadécimale de la section ");
			printf("\" %s \":\n  Ox%08x",strings + section_header.sh_name,i);
			while(i < section_header.sh_size) //boucle de récupération du contenu
			{
				if (i % 4 == 0)	//Formatage
					printf(" ");
				printf("%02x", *(content+i));
				i++;
				if (i % 16 == 0 && i < section_header.sh_size)	//Formatage
					printf("\n  Ox%08x",i);
			}
			puts(" ");			
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 4///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//trouve  la section .symtab (table des symboles, taille de la section, taille de chaque symboles)
Elf32_Shdr *sectionSymtab(Elf32_Ehdr* data){
	Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)data + data->e_shoff);
	Elf32_Shdr *section_header; int i;
	for (i = 0; i < data->e_shnum; i++){
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
	Elf32_Shdr *section_header; int i;
	Elf32_Shdr *shstrtab = sectionShstrtab(data);
	for (i = 0; i < data->e_shnum; i++){
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
	Elf32_Shdr *section_header; int i;
	for (i = 0; i < data->e_shnum; i++){
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
	Elf32_Shdr *section_header; int i;
	for (i = 0; i < data->e_shnum; i++){
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
	Elf32_Shdr *section_header; int i;
	for (i = 0; i < data->e_shnum; i++){
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
	Elf32_Shdr *section_header; int i;
	for (i = 0; i < data->e_shnum; i++){
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

void print_sym(Elf32_Ehdr * data)
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
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////ETAPE 5///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


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






///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////ETAPES 1 2 3 4/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int check_format(Elf32_Ehdr * data)
{
	// Verification fichier ELF
	// Vérification des 4 premiers octets du nombre magique ("7f 45 4c 46")
	if (data->e_ident[EI_MAG0] != ELFMAG0 || data->e_ident[EI_MAG1] != ELFMAG1 ||
	data->e_ident[EI_MAG2] != ELFMAG2 || data->e_ident[EI_MAG3] != ELFMAG3)
	{
		fputs((language)?"Invalid ELF file":"Fichier ELF invalide",stderr);
		return (0);
	}
	// Vérification de l'architecture 32bits
	if (data->e_ident[EI_CLASS] != 1)
	{
		fputs((language)?"Unsupported ELF File Class (32 expected)":"Classe de ELF invalide (32 attendu)",stderr);
		return (0);
	}
	// Vérification d'endianisme
	if(data->e_ident[EI_DATA] != ELFDATA2LSB) {
		fputs((language)?"Unsupported ELF File byte order (little endian expected)":"Ordre d'octets de ELF invalide (petit-boutiste attendu)",stderr);
		return (0);
	}
	if(data->e_machine != 0x28) {
		fputs((language)?"Unsupported ELF File target":"Nom de machine-cible de ELF invalide",stderr);
		return (0);
	}
	if(data->e_ident[EI_VERSION] != EV_CURRENT) {
		fputs((language)?"Unsupported ELF File version (1 expected)":"Version de fichier ELF invalide (1 attendu)\n",stderr);
		return (0);
	}
	if(data->e_type != ET_REL && data->e_type != ET_EXEC) {
		fputs((language)?"Unsupported ELF File type (relocation or execution expexted)":"Type de fichier ELF invalide (relocation ou executable attendu)",stderr);
		return (0);
	}
	return (1);
}



//ETAPES 1_2_3_4_5 MAIN
void process_etapes(int file_descriptor, char sous_option)
{

	Elf32_Ehdr *data;
	struct stat file_info;

	//Verification de récupartion des infos du fichier
	//int fstat(int fd, struct stat *buf);
	//printf("%d\n",fstat(file_descriptor, &file_info));
	if (fstat(file_descriptor, &file_info) == -1)
	{
		fputs((language)?"File information was NOT read":"Recuperation des informations du fichier impossible",stderr);
	}
	//Verification du chargement en memoire du fichier
	//void *mmap(void *addr, size_t len, int protection, int flags, int fildes, off_t off);
	else if ((data = mmap(0, file_info.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0)) == MAP_FAILED)
	{
		fputs((language)?"Impossible to store file into memory":"Chargement du fichier en memoire impossible",stderr);
	}
	else if (check_format(data))
	{							
		switch (sous_option)
		{
			case 'a' : //all
			case 'b' : //header
				print_header(data);
				if (sous_option!='a')
					break;
			case 'c' : //section table
				print_section_table(data);
				if (sous_option!='a')
					break;
			case 'd' : //section
				print_section(data);
				if (sous_option!='a')
					break;
			case 'f' : //symbol
				print_sym(data);
				if (sous_option!='a')
					break;
			case 'g' : //symbol
				afficherReloc(data);
				if (sous_option!='a')
					break;
		}
		//Unmapping du fichier en mémoire
		munmap(data, file_info.st_size);
	}
	//Fermeture du fichier
	close(file_descriptor);

}


