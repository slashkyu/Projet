#include <stdio.h>
#include "header_paq.h"
//Autres fichers .h

int main(int argc, char *argv[])
{
	char option;
	char menu [] = "Option e : Exit \nOption h : Header";
	char err_option [] = "Le caractere entre n'est pas valide";
	char err_open [] = "Ouverture du fichier impossible";
	char err_args [] = "USAGE: executable <binary_file>";
	char suc_goodbye [] =  "Au revoir";
	while (1)
	{
		puts(menu);		
		scanf(" %c",&option);
		switch (option)
		{
			case 'e' :
				puts(suc_goodbye);
				break;
			case 'h' : //ETAPE 1: HEADER
				//Verification nb arguments
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
						process_header(open(argv[1], O_RDONLY));
						continue;
					}
				}
			//ETAPE X:
			case 'c' : 
				break;
			case 'd' : 
				break;
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


