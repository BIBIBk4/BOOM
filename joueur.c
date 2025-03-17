#include "joueur.h"
#include "variables.h"
#include <stdio.h>

void demanderEtEnvoyerPseudo(int fileId) {
    char pseudo[50];
    printf("Veuillez entrer votre pseudo : ");
    scanf("%s", pseudo);
    
<<<<<<< HEAD
    // Envoi du message formaté
    envoyerMessage(fileId, pseudo,0);
=======
    // Préparation du message au format "psd:Pseudo"
    char message[100];
    sprintf(message, "psd:%s", pseudo);
    
    // Envoi du message formaté
    envoyerMessage(fileId, message);
>>>>>>> 0ca038d (ajout types messages joueur)
}

char *demanderReponse() {
    char *reponse = malloc(50 * sizeof(char));
    printf("Entrez votre mot : ");
    scanf("%s", reponse);
    return reponse;
}

void envoyerMessage(int bol, char *reponse, int type) {
    t_message message;
    message.type = 1;
    message.corps.pid = getpid();
    message.corps.type = type;
    strcpy(message.corps.msg, reponse);

    if (msgsnd(bol, &message, sizeof(t_corps), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }
}

void demanderPret() {
    char reponse[10];
    while(strcmp(reponse, "PRET") != 0) {
        printf("\nVeuillez entrer 'PRET' pour indiquer que vous êtes prêt : ");
        scanf("%s", reponse);
    }
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), "prt",1);
}

// Gestionnaire de signal pour SIG_PSEUDOVALIDE
void pseudoValide(int sig) {
    printf("Pseudo valide !\n");
    demanderPret();
<<<<<<< HEAD
=======
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), "prt");
>>>>>>> 0ca038d (ajout types messages joueur)
}

// Gestionnaire de signal pour SIG_PSEUDOINVALIDE
void pseudoInvalide(int sig) {
    printf("Pseudo invalide !\n");
    demanderEtEnvoyerPseudo(msgget(ftok(FICHIER, 1), 0666));
}

// Gestionnaire de signal pour SIG_MOTVALIDE
void motValide(int sig) {
    printf("Mot rentré valide ! Au tour du joueur suivant\n");
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIG_MOTINVALIDE
void motInvalide(int sig) {
    printf("Mot entré invalide ! \n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), demanderReponse(),2);
    // Ajoutez ici le code de la fonction à exécuter
}

// Gestionnaire de signal pour SIG_ATONTOUR
void aTonTour(int sig) {
    printf("C'est à votre tour de jouer !\n");
    envoyerMessage(msgget(ftok("./dictionnaire.txt", 1), 0666), demanderReponse(),2);
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

    demanderEtEnvoyerPseudo(idFile);

    while (1) {
        pause();
    }    

    return 0;
}