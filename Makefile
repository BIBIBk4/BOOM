CC = gcc
CFLAGS = -Wall -pthread

all: bombe joueur serveur

jeu: bombe.c jeu.c partie.c
	$(CC) $(CFLAGS) -o jeu bombe.c jeu.c partie.c

bombe: bombe.c
	$(CC) $(CFLAGS) -o bombe bombe.c

joueur: joueur.c jeu.c
	$(CC) $(CFLAGS) -o joueur joueur.c jeu.c

serveur: serveur.c jeu.c partie.c
	$(CC) $(CFLAGS) -o serveur serveur.c jeu.c partie.c

clean:
	rm -f jeu joueur serveur partie
