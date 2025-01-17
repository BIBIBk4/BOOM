CC = gcc
CFLAGS = -Wall -pthread

all: jeu joueur serveur

jeu: bombe.o jeu.o main.o
	$(CC) $(CFLAGS) -o jeu bombe.o jeu.o main.o

bombe.o: bombe.c jeu.h
	$(CC) $(CFLAGS) -c bombe.c

jeu.o: jeu.c jeu.h
	$(CC) $(CFLAGS) -c jeu.c

main.o: main.c jeu.h
	$(CC) $(CFLAGS) -c main.c

joueur: joueur.o jeu.o
	$(CC) $(CFLAGS) -o joueur joueur.o jeu.o

joueur.o: joueur.c
	$(CC) $(CFLAGS) -c joueur.c

serveur: serveur.o
	$(CC) $(CFLAGS) -o serveur serveur.o

serveur.o: serveur.c
	$(CC) $(CFLAGS) -c serveur.c

clean:
	rm -f *.o jeu joueur serveur