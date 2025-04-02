#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "serveur.h"

#include "jeu.h"
#include "variables.h"


int countdown = 10;
t_joueur joueurs[MAX_JOUEURS]; 
int nb_joueurs = 0;
int nb_joueurs_vivants = 0;
int tour = 0;
bool fin = false;

void ajouter_joueur(pid_t pid, const char *pseudo) {
    if (nb_joueurs < MAX_JOUEURS) {
        joueurs[nb_joueurs].pid = pid;
        strncpy(joueurs[nb_joueurs].pseudo, pseudo, sizeof(joueurs[nb_joueurs].pseudo) - 1);
        joueurs[nb_joueurs].vivant = false;
        joueurs[nb_joueurs].tour = false;
        nb_joueurs++;
        printf("Joueur ajouté : %s (PID: %d)\n", pseudo, pid);
    } else {
        printf("Nombre maximum de joueurs atteint.\n");
    }
}

void envoyer_signal(pid_t pid, int signal) {
    if (kill(pid, signal) == -1) {
        perror("Erreur lors de l'envoi du signal");
    }
}

// Vérifie si tous les joueurs sont prêts
bool tous_pret() {
    if(nb_joueurs < 2) {
        return false;
    }
    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].vivant) {
            return false;
        }
    }
    return true;
}

void envoyer_message(int pid, const char *message) {
    key_t cle = ftok("./dictionnaire.txt", 1);
    int idFile = msgget(cle, 0666 | IPC_CREAT);
    if (idFile == -1) {
        perror("Erreur lors de la récupération de la file de messages");
        return;
    }

    t_message msg;
    msg.type = pid;
    msg.corps.pid = getpid();
    msg.corps.type = 0;
    strncpy(msg.corps.msg, message, sizeof(msg.corps.msg) - 1);
    msg.corps.msg[sizeof(msg.corps.msg) - 1] = '\0';

    if (msgsnd(idFile, &msg, sizeof(msg.corps), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
    }
}

void envoyer_liste_joueurs() {
    char liste[MAX_JOUEURS * 80]; // Increase buffer size to accommodate longer player info
    liste[0] = '\0';

    for (int i = 0; i < nb_joueurs; i++) {
        char joueur_info[80]; // Increase buffer size to avoid truncation
        printf("joueur :  %s, %d, %d\n", joueurs[i].pseudo, joueurs[i].vivant, joueurs[i].tour);
        snprintf(joueur_info, sizeof(joueur_info), "%s:%d:%d\n", joueurs[i].pseudo, joueurs[i].vivant, joueurs[i].tour);
        strncat(liste, joueur_info, sizeof(liste) - strlen(liste) - 1);
    }

    for (int i = 0; i < nb_joueurs; i++) {
        printf("joueur : %d, %s\n", joueurs[i].pid, joueurs[i].pseudo);
        envoyer_signal(joueurs[i].pid, SIG_LISTEJOUEURS);
        envoyer_message(joueurs[i].pid, liste); // Assuming type 3 for the list message
    }
}

void changerTour(int tour) {
    printf("C'est au tour de %s (PID: %d)\n", joueurs[tour].pseudo, joueurs[tour].pid);
    joueurs[tour].tour = true;
    envoyer_liste_joueurs();
    sleep(2);
    envoyer_signal(joueurs[tour].pid, SIG_ATONTOUR);
    char combinaison[10];
    printf("Génération combinaison\n");
    strcpy(combinaison, genererCombinaison());
    printf("Combinaison générée : %s\n", combinaison);
    envoyer_message(joueurs[tour].pid, combinaison);
}

void *lancementPartie() {
    nb_joueurs_vivants = nb_joueurs;

    while (nb_joueurs_vivants > 1) {
        // Reset the countdown and start the next player's turn
        countdown = 10;
        changerTour(tour);

        while (countdown > 0) {
            sleep(1);
            countdown--;
            printf("Temps restant: %d secondes, au tour de : %s\n", countdown, joueurs[tour].pseudo);
        }

        joueurs[tour].vivant = false;
        printf("%s (PID: %d) est mort.\n", joueurs[tour].pseudo, joueurs[tour].pid);
        envoyer_signal(joueurs[tour].pid, SIG_PERTEVIE);
        sleep(1);
        joueurs[tour].tour = false;
        envoyer_liste_joueurs();
        sleep(1);
        nb_joueurs_vivants--;

        // Calculate the next player's turn
        do {
            tour = (tour + 1) % nb_joueurs;
        } while (!joueurs[tour].vivant);
    }

    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].vivant) {
            printf("%s (PID: %d) a gagné !\n", joueurs[i].pseudo, joueurs[i].pid);
            envoyer_signal(joueurs[i].pid, SIG_GAGNE);
        }
    }

    for (int i=0; i<nb_joueurs; i++) {
        envoyer_signal(joueurs[i].pid, SIG_FINPARTIE);
    }
    fin = true;

    exit(0);
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

    printf("Serveur démarré. En attente de joueurs...\n");

    while (!fin) {
        t_message message;

        if (msgrcv(idFile, &message, sizeof(message.corps), 1, 0) == -1) {
            perror("Erreur lors de la réception du message");
            continue;
        }

        switch (message.corps.type) {
            case 0:
                // Joueur qui se connecte
                bool joueur_existe = false;

                for (int i = 0; i < nb_joueurs; i++) {
                    if (joueurs[i].pid == message.corps.pid || strcmp(joueurs[i].pseudo, message.corps.msg) == 0) {
                        joueur_existe = true;
                        break;
                    }
                }

                if (!joueur_existe) {
                    ajouter_joueur(message.corps.pid, message.corps.msg);
                    envoyer_signal(message.corps.pid, SIG_PSEUDOVALIDE);
                } else {
                    envoyer_signal(message.corps.pid, SIG_PSEUDOINVALIDE);
                }
                break;
            case 1:
                // Joueur prêt
                for (int i = 0; i < nb_joueurs; i++) {
                    if (joueurs[i].pid == message.corps.pid) {
                        joueurs[i].vivant = true;
                        printf("%s (PID: %d) est prêt.\n", joueurs[i].pseudo, joueurs[i].pid);
                        break;
                    }
                }
                
    
                if (tous_pret()) {
                    printf("Tous les joueurs sont prêts. La partie commence !\n");
                    printf("---------------------------------------------\n");
                    // Start the countdown thread
                    countdown = 10;
                    pthread_t thread;
                    pthread_create(&thread, NULL, lancementPartie, NULL);
                    pthread_detach(thread);
                    break;
                }
                break;
            case 2:
                // Joueur envoie un mot
                if (motEstValide(message.corps.msg)) {
                    envoyer_signal(message.corps.pid, SIG_MOTVALIDE);
                    joueurs[tour].tour = false;
                    do {
                        tour = (tour + 1) % nb_joueurs;
                    } while (!joueurs[tour].vivant);
                    countdown += 4;
                    changerTour(tour);
                } else {
                    envoyer_signal(message.corps.pid, SIG_MOTINVALIDE);
                }
                break;
            default:
                printf("Type de message inconnu\n");
                break;
        
        }
    }

    msgctl(idFile, IPC_RMID, NULL);

    return 0;
}