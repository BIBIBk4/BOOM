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
    printf("Appuyez sur une touche lorsque vous êtes prêt\n");
    getchar();
}

// Gestionnaire de signal pour SIG_PSEUDOVALIDE
void pseudoValide(int sig) {
    printf("Pseudo valide !\n");
    printf("Tapez PRET pour commencer : ");
    char input[10];
    scanf("%s", input);
    if (strcmp(input, "PRET") == 0) {
        envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), "PRET");
    }
}

// Gestionnaire de signal pour SIG_PSEUDOINVALIDE
void pseudoInvalide(int sig) {
    printf("Pseudo invalide !\n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), demanderPseudo());
}

// Gestionnaire de signal pour SIG_MOTVALIDE
void motValide(int sig) {
    printf("Mot rentré valide ! Au tour du joueur suivant\n");
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIG_MOTINVALIDE
void motInvalide(int sig) {
    printf("Mot entré invalide ! \n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), demanderReponse());
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIG_ATONTOUR
void aTonTour(int sig) {
    printf("C'est à votre tour de jouer !\n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), demanderReponse());
}

// Gestionnaire de signal pour SIG_PERTEVIE
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

    sigPseudoValide.sa_handler = pseudoValide;
    sigemptyset(&sigPseudoValide.sa_mask);
    sigPseudoValide.sa_flags = 0;

    sigPseudoInvalide.sa_handler = pseudoInvalide;
    sigemptyset(&sigPseudoInvalide.sa_mask);
    sigPseudoInvalide.sa_flags = 0;

    if (sigaction(SIG_MOTVALIDE, &sigMotValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR1");
        exit(1);
    }

    if (sigaction(SIG_MOTINVALIDE, &sigMotinValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGUSR2");
        exit(1);
    }

    if (sigaction(SIG_PERTEVIE, &sigPerdu, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGCHLD");
        exit(1);
    }

    if (sigaction(SIG_ATONTOUR, &sigATonTour, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPWR");
        exit(1);
    }

    if (sigaction(SIG_PSEUDOVALIDE, &sigPseudoValide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPSEUDOVALIDE");
        exit(1);
    }

    if (sigaction(SIG_PSEUDOINVALIDE, &sigPseudoInvalide, NULL) == -1) {
        perror("Erreur lors de l'enregistrement du gestionnaire de SIGPSEUDOINVALIDE");
        exit(1);
    }

    key_t cle = ftok(FICHIER, 1);
    int idFile = msgget(cle, 0666);

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