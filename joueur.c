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

char *demanderReponse() {
    char *reponse = malloc(50 * sizeof(char));
    printf("Entrez votre mot : ");
    scanf("%s", reponse);
    return reponse;
}

void envoyerMessage(int bol, char *reponse) {
    t_message message;
    message.type = 1;
    message.corps.pid = getpid();
    printf("pid : %d\n", message.corps.pid);
    strcpy(message.corps.msg, reponse);

    if (msgsnd(bol, &message, sizeof(t_corps), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }
}

void demanderPret() {
    printf("Appuyer sur une touche lorsque vous êtres prêt à jouer\n");
    getchar();
}

// Gestionnaire de signal pour SIGWINCH
void pseudoValide(int sig) {
    printf("Pseudo valide !\n");
    demanderPret();
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), IPC_EXCL), "pret");
    printf("En attente des autres joueurs...\n");
}

// Gestionnaire de signal pour SIGURG
void pseudoInvalide(int sig) {
    printf("Pseudo invalide !\n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), IPC_EXCL), demanderPseudo());
}

// Gestionnaire de signal pour SIGUSR1
void motValide(int sig) {
    printf("Mot rentré valide ! Au tour du joueur suivant\n");
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIGUSR2
void motInvalide(int sig) {
    printf("Mot entré invalide ! \n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), IPC_EXCL), demanderReponse());
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIGPWR
void aTonTour(int sig) {
    printf("C'est à votre tour de jouer !\n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), IPC_EXCL), demanderReponse());
}

// Gestionnaire de signal pour SIGCHLD
void perdu(int sig) {
    printf("Vous avez perdu !\n");
}

int main() {
    // Enregistrement des gestionnaires de signaux
    struct sigaction sigMotValide, sigMotinValide, sigPerdu, sigATonTour, sigPseudoValide, sigPseudoInvalide;

    sigMotValide.sa_handler = motValide;
    sigemptyset(&sigMotValide.sa_mask);
    sigMotValide.sa_flags = 0;

    sigMotinValide.sa_handler = motInvalide;
    sigemptyset(&sigMotinValide.sa_mask);
    sigMotinValide.sa_flags = 0;

    sigATonTour.sa_handler = aTonTour;
    sigemptyset(&sigATonTour.sa_mask);
    sigATonTour.sa_flags = 0;

    sigPerdu.sa_handler = perdu;
    sigemptyset(&sigPerdu.sa_mask);
    sigPerdu.sa_flags = 0;

    sigPseudoValide.sa_handler = NULL;
    sigemptyset(&sigPseudoValide.sa_mask);
    sigPseudoValide.sa_flags = 0;

    sigPseudoInvalide.sa_handler = NULL;
    sigemptyset(&sigPseudoInvalide.sa_mask);
    sigPseudoInvalide.sa_flags = 0;

    if (sigaction(SIGUSR1, &sigMotValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR1");
        exit(1);
    }

    if (sigaction(SIGUSR2, &sigMotinValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR2");
        exit(1);
    }

    if (sigaction(SIGPWR, &sigPerdu, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGCHLD");
        exit(1);
    }

    if (sigaction(SIGCHLD, &sigATonTour, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPWR");
        exit(1);
    }

    if (sigaction(SIGRTMIN, &sigPseudoValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPSEUDOVALIDE");
        exit(1);
    }

    if (sigaction(SIGRTMIN+1, &sigPseudoInvalide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPSEUDOINVALIDE");
        exit(1);
    }

    key_t cle = ftok("./dictionnaire.txt", 1);
    int idFile = msgget(cle, IPC_EXCL);

    if(idFile == -1) {
        perror("Erreur lors de la récupération de la file de messages");
        return 1;
    }

    envoyerMessage(idFile, demanderPseudo());

    while (1) {
        pause();
    }

    return 0;
}