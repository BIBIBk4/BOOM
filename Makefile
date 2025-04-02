all: joueur serveur

joueur: joueur.c jeu.c
	gcc joueur.c jeu.c -o joueur -pthread

serveur: serveur.c jeu.c
	gcc serveur.c jeu.c -o serveur -pthread

clean:
	rm -f jeu joueur serveur
