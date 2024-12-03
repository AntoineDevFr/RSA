gn.o: gn.c gn.h
	gcc -c gn.c 

main.o: main.c gn.h
	gcc -c main.c

main: gn.o main.o
	gcc gn.o main.o -g -o main

clean:
	rm -f *.o main