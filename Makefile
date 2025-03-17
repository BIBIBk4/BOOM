CC = gcc
CFLAGS = -Wall -pthread

all: bombe joueur serveur partie

jeu: bombe.o jeu.o partie.o
	$(CC) $(CFLAGS) -o jeu bombe.o jeu.o partie.o

bombe : bombe.c
	$(CC) $(CFLAGS) -o bombe bombe.c

jeu.o: jeu.c jeu.h
	$(CC) $(CFLAGS) -c jeu.c

joueur: joueur.o jeu.o
	$(CC) $(CFLAGS) -o joueur joueur.o jeu.o

joueur.o: joueur.c jeu.h
	$(CC) $(CFLAGS) -c joueur.c

serveur: serveur.o jeu.o
	$(CC) $(CFLAGS) -o serveur serveur.o jeu.o

serveur.o: serveur.c jeu.h partie.h
	$(CC) $(CFLAGS) -c serveur.c

partie: partie.o jeu.o
	$(CC) $(CFLAGS) -o partie partie.o jeu.o

partie.o: partie.c jeu.h
	$(CC) $(CFLAGS) -c partie.c

clean:
	rm -f *.o jeu joueur serveur partie
