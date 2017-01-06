#include <stdio.h>
#include "header_paq.h"
//Autres fichers .h

int main(int argc, char *argv[])
{
	char option = 'a';
	char menu [] = "Option e : Exit \nOption a : Etapes 1_2_3\nOption b : Header\nOption c : Section Table\nOption d : Section";
	char err_option [] = "Le caractere entre n'est pas valide";
	char err_open [] = "Ouverture du fichier impossible";
	char err_args [] = "USAGE: executable <binary_file>";
	char suc_goodbye [] =  "Au revoir";
	while (1)
	{
		puts(" ");		
		puts(menu);		
		scanf(" %c",&option);
		switch (option)
		{
			case 'e' : //EXIT
				puts(suc_goodbye);
				break;
			case 'a' : //ETAPES 1_2_3	
			case 'b' : //ETAPE 1: HEADER
			case 'c' : //ETAPE 2: SECTION TABLE
			case 'd' : //ETAPE 3: SECTION
				if (argc < 2)
				{
					puts(err_args);
					continue;
				}
				else
				{
					if (open(argv[1], O_RDONLY) <= 0)
					{
						puts(err_open);
						continue;
					}
					else
					{
						process_etapes(open(argv[1], O_RDONLY),option);
						continue;
					}
				}
			//ETAPE X:
			case 'f' : 
				break;
			default :
				puts(err_option);
				continue;
		}
		break;
	}
	return (0);
}


