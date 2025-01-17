#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include "variables.h"

char *demanderPseudo() {
    char *pseudo = malloc(50 * sizeof(char));
    printf("Entrez votre pseudo : ");
    scanf("%s", pseudo);
    return pseudo;
}

void envoyerPresence(int bol) {
    t_message message;
    message.type = 1;
    message.corps.pid = getpid();
    strcpy(message.corps.msg, demanderPseudo());

    if (msgsnd(bol, &message, sizeof(t_corps), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }
}

// Gestionnaire de signal pour SIGUSR1
void handle_sigusr1(int sig) {
    printf("Reçu SIGUSR1 ! Exécution de la fonction correspondante.\n");
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIGUSR2
void handle_sigusr2(int sig) {
    printf("Reçu SIGUSR2 ! Exécution de la fonction correspondante.\n");
    // Ajoutez ici le code de la fonction à exécuter
}

int main() {
    // Enregistrement des gestionnaires de signaux
    struct sigaction sa1, sa2;

    sa1.sa_handler = handle_sigusr1;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;

    sa2.sa_handler = handle_sigusr2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR1");
        exit(1);
    }

    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR2");
        exit(1);
    }

    key_t cle = ftok("./dictionnaire.txt", 1);
    int idFile = msgget(cle, IPC_EXCL);

    if(idFile == -1) {
        perror("Erreur lors de la récupération de la file de messages");
        return 1;
    }

    envoyerPresence(idFile);

    // Boucle infinie pour attendre les signaux
    while (1) {
        pause(); // Attendre un signal
    }

    return 0;
}