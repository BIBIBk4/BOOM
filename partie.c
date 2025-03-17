#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jeu.h"
#include "partie.h"

void jeu() {
    // Initialisation
    srand(time(NULL));
    printf("Bienvenue dans le jeu Tik Tak Boum dans ton Ben !\n\n");

    // Générer une combinaison aléatoire
    genererCombinaison();
    printf("La combinaison actuelle est : %s\n", combinaisonActuelle);

    char mot[50];
    while (1) {
        printf("\nEntrez un mot contenant la combinaison \"%s\" (ou 'exit' pour quitter) : ", combinaisonActuelle);
        scanf("%49s", mot);

        // Quitter le jeu si l'utilisateur entre 'exit'
        if (strcmp(mot, "exit") == 0) {
            printf("Merci d'avoir joué !\n");
            break;
        }

        // Vérifier si le mot est valide
        if (motEstValide(mot)) {
            printf("Mot accepté !\n");
            ajouterMot(mot);
        } else {
            printf("Mot invalide ou déjà utilisé. Essayez encore.\n");
        }

        // Afficher les mots utilisés
        printf("\nMots utilisés jusqu'à présent :\n");
        for (int i = 0; i < nbMotsUtilises; i++) {
            printf("%s\n", motsUtilises[i]);
        }
    }
}
