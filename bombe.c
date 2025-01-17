#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "jeu.h"
#include "variables.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>

#define MAX_JOUEURS 5

t_joueur joueurs[MAX_JOUEURS];
int nbJoueurs = 0;

void ajouterJoueur(pid_t pid, const char *pseudo) {
    if (nbJoueurs >= MAX_JOUEURS) {
        printf("Limite de joueurs atteinte !\n");
        return;
    }

    for (int i = 0; i < nbJoueurs; i++) {
        if (strcmp(joueurs[i].pseudo, pseudo) == 0) {
            printf("Le pseudo %s est déjà pris !\n", pseudo);
            return;
        }
    }

    joueurs[nbJoueurs].pid = pid;
    strncpy(joueurs[nbJoueurs].pseudo, pseudo, sizeof(joueurs[nbJoueurs].pseudo) - 1);
    joueurs[nbJoueurs].pseudo[sizeof(joueurs[nbJoueurs].pseudo) - 1] = '\0';
    nbJoueurs++;
    printf("Joueur ajouté : PID=%d, Pseudo=%s\n", pid, pseudo);
}

int main() {

    key_t cle = ftok("./dictionnaire.txt", 1);
    if (cle == -1) {
        perror("Erreur lors de la création de la clé");
        return 1;
    }
    int idFile = msgget(cle, 0666 | IPC_CREAT);
    if (idFile == -1) {
        perror("Erreur lors de la récupération de la file de messages");
        return 1;
    }

    t_message msg;

    printf("Boîte aux lettres créée avec l'ID : %d\n", idFile);

    while (1) {
        if (msgrcv(idFile, &msg, sizeof(t_corps), 1, 0) == -1) {
            perror("Erreur lors de la réception du message");
            continue;
        }

        ajouterJoueur(msg.corps.pid, msg.corps.msg);

        printf("Liste des joueurs actuels :\n");
        for (int i = 0; i < nbJoueurs; i++) {
            printf("PID=%d, Pseudo=%s\n", joueurs[i].pid, joueurs[i].pseudo);
        }
    }

    if (msgctl(idFile, IPC_RMID, NULL) == -1) {
        perror("Erreur lors de la suppression de la boîte aux lettres");
        return 1;
    }

    return 0;
}
