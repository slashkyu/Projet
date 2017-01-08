#include <stdio.h>
#include "lecture_paq.h"
//#include "messages_paq.h"
//Autres fichers .h


int main(int argc, char *argv[])
{
	char option = 'a'; 

	language = 0;
	while (1)
	{
		puts("-----------------------------------------------------");
		puts("-----------------------------------------------------");
		puts("-----------------------------------------------------");
		puts((language)?"Option e : Exit \nOption l : Changer langue a FRA \nOption a : Steps 1_2_3_4\nOption b : Header\nOption c : Section Table\nOption d : Section\nOption f : Symbol":"Option e : Sortie \nOption l : Change language to ENG \nOption a : Etapes 1_2_3_4\nOption b : Header\nOption c : Section Table\nOption d : Section\nOption f : Symbole");
		scanf(" %c",&option);
		switch (option)
		{
			case 'l' : //Language
				language = 1 - language;
				continue;
			case 'e' : //EXIT
				puts((language)?"Bye":"Au revoir");
				break;
			case 'a' : //ETAPES 1_2_3_4
			case 'b' : //ETAPE 1: HEADER
			case 'c' : //ETAPE 2: SECTION TABLE
			case 'd' : //ETAPE 3: SECTION
			case 'f' : //ETAPE 4: SYMBOLE
				if (argc < 2)
				{
					fputs((language)?"USAGE: executable <binary_file>":"USAGE: executable <fichier_binaire>",stderr);
					continue;
				}
				else
				{
					if (open(argv[1], O_RDONLY) <= 0)
					{
						fputs((language)?"Opening of the file has failed":"Ouverture du fichier impossible",stderr);
						continue;
					}
					else
					{
						process_etapes(open(argv[1], O_RDONLY),option);
						continue;
					}
				}
			//ETAPE X:
			case 'z' : 
				break;
			default :
				fputs((language)?"The character in NOT valid":"Le caractere entre n'est pas valide",stderr);
				continue;
		}
		break;
	}
	return (0);
}


