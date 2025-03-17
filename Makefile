CC = gcc
CFLAGS = -Wall -pthread

all: bombe joueur serveur

jeu: bombe.c jeu.c partie.c
	$(CC) $(CFLAGS) -o jeu bombe.c jeu.c partie.c

bombe: bombe.c
	$(CC) $(CFLAGS) -o bombe bombe.c

joueur: joueur.c jeu.c
	$(CC) $(CFLAGS) -o joueur joueur.c jeu.c

<<<<<<< HEAD
serveur: serveur.c jeu.c partie.c
	$(CC) $(CFLAGS) -o serveur serveur.c jeu.c partie.c
=======
serveur: serveur.c jeu.c
	$(CC) $(CFLAGS) -o serveur serveur.c jeu.c

serveur.o: serveur.c jeu.h partie.h
	$(CC) $(CFLAGS) -c serveur.c
>>>>>>> e3cfb820064d5026f7bf68898f29b0ce536dc6df

clean:
	rm -f jeu joueur serveur partie
