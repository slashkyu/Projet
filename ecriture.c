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
//#include "lecture_paq.h" 
#include "ecriture.h"



//char *tab_type_section[14] = {"NULL", "PROGBITS", "SYMTAB" , "STRTAB", "RELA", "HASH", "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "PROCSPECSEM", "APPINDEX"};

char * tab_classe[3]={"Aucun","ELF32","ELF64",};

char * tab_type_sym[7]={"NOTYPE", "OBJCET", "FUNC","SECTION","FILE","LOPROC","HIPROC"};

char * tab_lien_sym[5]={"LOCAL","GLOBAL","WEAK","LOPROC","HIPROC"};

char *tab_donnees[3]={"Aucun","complément à 2, système à octets de poids faible d'abord (little endian)","système à octets de poids fort d'abord (big endian)"};

char * tab_header_type[5]= {"NONE (None)","REL (Fichier de Relocalisation)","EXEC (Fichier Exécutable)","DYN (Objet Partagé)","CORE (Fichier Core)"};



static const char * get_osabi_name (unsigned int osabi)
{
  static char buff[32];

  switch (osabi)
    {
    case ELFOSABI_NONE:		return "UNIX - System V";
    case ELFOSABI_HPUX:		return "UNIX - HP-UX";
    case ELFOSABI_NETBSD:	return "UNIX - NetBSD";
    case ELFOSABI_LINUX:	return "UNIX - Linux";
    case ELFOSABI_SOLARIS:	return "UNIX - Solaris";
    case ELFOSABI_AIX:		return "UNIX - AIX";
    case ELFOSABI_IRIX:		return "UNIX - IRIX";
    case ELFOSABI_FREEBSD:	return "UNIX - FreeBSD";
    case ELFOSABI_TRU64:	return "UNIX - TRU64";
    case ELFOSABI_MODESTO:	return "Novell - Modesto";
    case ELFOSABI_OPENBSD:	return "UNIX - OpenBSD";
    case 13:			return "VMS - OpenVMS";
    case 14:			return "HP - Non-Stop Kernel";
    case 15:			return "Amiga Research OS";
    case ELFOSABI_STANDALONE:	return "Standalone App";
    case ELFOSABI_ARM:		return "ARM";
    default:
      snprintf (buff, sizeof (buff), "<autre: %x>", osabi);
      return buff;
    }
}

static char *get_machine_name (unsigned e_machine)
{
  static char buff[64]; /* XXX */

  switch (e_machine)
    {
    case EM_NONE:		return "None";
    case EM_ARM:		return "ARM 32-bit architecture (AARCH32)";
    case EM_M32:		return "AT&T WE 32100";
    case EM_SPARC:		return "Sparc";
    case EM_386:		return "Intel 80386 (x86)";
    case EM_68K:		return "MC68000";
    case EM_88K:		return "MC88000";
    case EM_860:		return "Intel 80860";
    case EM_MIPS:		return "MIPS R3000";
    case EM_S370:		return "IBM System/370";
    case EM_MIPS_RS3_LE:	return "MIPS R4000 big-endian";
    case EM_PARISC:		return "HPPA";
    case EM_SPARC32PLUS:	return "Sparc v8+" ;
    case EM_960:		return "Intel 90860";
    case EM_PPC:		return "PowerPC";
    case EM_PPC64:		return "PowerPC64";
    case EM_V800:		return "NEC V800";
    case EM_FR20:		return "Fujitsu FR20";
    case EM_SH:			return "Renesas / SuperH SH";
    case EM_SPARCV9:		return "Sparc v9";
    case EM_TRICORE:		return "Siemens Tricore";
    case EM_ARC:		return "ARC";
    case EM_IA_64:		return "Intel IA-64";
    case EM_MIPS_X:		return "Stanford MIPS-X";
    case EM_COLDFIRE:		return "Motorola Coldfire";
    case EM_68HC12:		return "Motorola M68HC12";
    case EM_ALPHA:		return "Alpha";
    case EM_M32R:		return "Renesas M32R (formerly Mitsubishi M32r)";
    case EM_FR30:		return "Fujitsu FR30";
    case EM_MMA:		return "Fujitsu Multimedia Accelerator";
    case EM_SVX:		return "Silicon Graphics SVx";
    case EM_PRISM:		return "Vitesse Prism";
    case EM_X86_64:		return "Advanced Micro Devices X86-64";
    case EM_S390:		return "IBM S/390";
    case 0xB7:			return "ARM 64-bit architecture (AARCH64)";
    default:
      snprintf (buff, sizeof (buff), "<autre>: %x", e_machine);
      return buff;
    }
}
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

static char *get_machine_flags (unsigned e_flags, unsigned e_machine)
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

static const char *get_flag(int flags){
	//char* str_flag;
	char str_flags[4] = "   "; char *str_flag = str_flags;
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
		return str_flags;
		//return str_flag;
}

void afficherHeader(Elf32 *e){

	int index;
	//ELF Header
	puts("En-tête ELF:");
	//Magic
	printf("%s","  Magique:   ");
	for (index = 0; index < EI_NIDENT; index++)
	{
		printf("%2.2x ", (e->header)->e_ident[index]);
	}
	puts(" ");
	//Class
	printf("  Classe:                            %s\n", tab_classe[(e->header)->e_ident[4]]);
	//Data
	printf("  Données:                          %s\n", tab_donnees[(e->header)->e_ident[5]]);
	//Version
	printf("  Version:                           %x%s\n", (e->header)->e_ident[6],((e->header)->e_ident[6])?" (current)":"");
	//OS/ABI
	printf("  OS/ABI:                            %s\n", get_osabi_name((e->header)->e_ident[7]));
	//ABI Version
	printf("  Version ABI:                       %d\n", (e->header)->e_ident[8]);
	//Type
	printf("  Type:                              %s\n",tab_header_type[(e->header)->e_type]);
	//Machine
	printf("  Machine:                           %s\n",get_machine_name((e->header)->e_machine));
	//Version
	printf("  Version:                           0x%1x\n", (e->header)->e_version);
	//Entry point address
	printf("  Adresse du point d'entrée:         0x%0x \n",(e->header)->e_entry);
	//Start of program headers
	printf("  Début des en-têtes de programme:   %d (octets dans le fichier)\n",(e->header)->e_phoff);
	//Start of section headers
	printf("  Début des en-têtes de section:          %d (octets dans le fichier)\n",(e->header)->e_shoff);
	//Flags
	printf("  Fanions:                           0x%0x",(e->header)->e_flags);
	printf("%s",((e->header)->e_entry)?", has entry point":"");
	printf("%s\n",get_machine_flags ((e->header)->e_flags, (e->header)->e_machine));
	//Size of this header
	printf("  Taille de cet en-tête:             %d (bytes)\n",(e->header)->e_ehsize);
	//Size of program headers
	printf("  Taille de l'en-tête du programme:  %d (bytes)\n",(e->header)->e_phentsize);
	//Number of program headers
	printf("  Nombre d'en-tête du programme:     %d\n",(e->header)->e_phnum);
	//Size of section headers
	printf("  Taille des en-têtes de section:    %d (bytes)\n",(e->header)->e_shentsize);
	//Number of section headers
	printf("  Nombre d'en-têtes de section:      %d\n",(e->header)->e_shnum);
	//Section header string table index
	printf("  Table d'indexes des chaînes d'en-tête de section: %d\n",(e->header)->e_shstrndx);
}
 
void afficherSectionTable(Elf32 *e){
	
	//Elf32_Ehdr * elf_header; elf_header= e->header;
	// on recupere un pointeur sur le premier header de section, avec le offset correspondant
	Elf32_Shdr *section_header_start=e->table_section;//get_section_table(e->header);
	//Elf32_Shdr *section_header_start = (Elf32_Shdr*)((void*)elf_header + elf_header->e_shoff);
	// on retrouve la string table avec le nom des sections
	//Elf32_Shdr sections_string = section_header_start[(e->header)->e_shstrndx];
	// on recupere un pointeur sur le debut de la section et donc sur la premiere chaine
	//char *strings = string_section(e->header);
	//char *strings = (char*)((void*)e->header + sections_string.sh_offset);
	// pour stoquer chaque section
	//char *strings = e->string_table_section + section_header.sh_name)


	Elf32_Shdr section_header; 

	unsigned int i;	


	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x:\n", e->nb_Section, (e->header)->e_shoff);
	
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
	//Affichage du tableau
	for (i = 0; i < e->nb_Section; i++)
	{
		// on recupere le header de section courrant
		section_header = section_header_start[i];
		//Nr
		printf("  [%2u]",i);
		//Name
		//printf(" %-17.17s"," ");
		printf(" %-17.17s", e->string_table_section + section_header.sh_name);
		//printf(" %-17.17s", strings + section_header.sh_name);
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
		//printf(" %3s",get_flag(section_header.sh_flags));
		//Lk
		printf(" %2d", section_header.sh_link);
		//Inf
		printf(" %3d", section_header.sh_info);
		//Al
		printf(" %2d", section_header.sh_addralign);

		puts(" ");
	}
    puts("Clé des fanions:\nW (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)");
	puts("I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu), x (inconnu)");
	puts("O (traiterment additionnel requis pour l'OS) o (spécifique à l'OS), p (spécifique au processeur)");	

}


 void afficherSectionSymbole(Elf32 *e){
	int i, type, bind;
	printf("Table de symboles « .symtab » contient %d entrées:\n",e->nb_Symbole);
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
		printf("%09x ",(e->string_table_symbole)[(e->table_symbole + i)->st_name]);

	}
} 

