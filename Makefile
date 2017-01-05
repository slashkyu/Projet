
global_main: global_main.o header_paq.o
	gcc -o global_main global_main.o header_paq.o

global_main.o: global_main.c
	gcc -c global_main.c

header_paq.o: header_paq.c header_paq.h
	gcc -c header_paq.c
