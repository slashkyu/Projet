
main: main.o interface.o lecture.o ecriture.o
	gcc -g -o main main.o interface.o lecture.o ecriture.o

main.o: main.c
	gcc -g -c main.c

interface.o: interface.c interface.h
	gcc -g -c interface.c

lecture.o: lecture.c lecture.h interface.h
	gcc -g -c lecture.c

ecriture.o: ecriture.c ecriture.h interface.h
	gcc -g -c ecriture.c
