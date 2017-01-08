
global_main: global_main.o lecture_paq.o
	gcc -o global_main global_main.o lecture_paq.o

global_main.o: global_main.c
	gcc -c global_main.c

lecture_paq.o: lecture_paq.c lecture_paq.h
	gcc -c lecture_paq.c
