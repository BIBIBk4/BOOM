#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "jeu.h"
#include "bombe.h"

int main() {
    srand(time(NULL));

    printf("Entrez le nombre de joueurs (2 à %d) : ", MAX_JOUEURS);
    scanf("%d", &nbJoueurs);
    if (nbJoueurs < 2 || nbJoueurs > MAX_JOUEURS) {
        printf("Nombre de joueurs invalide !\n");
        return 1;
    }

    genererCombinaison();
    pthread_create(&threadBombe, NULL, gestionBombe, NULL);
    tourDeJeu();
    pthread_join(threadBombe, NULL);

    printf("Fin de la partie. Le joueur %d a perdu !\n", joueurElimine);
    return 0;
}
