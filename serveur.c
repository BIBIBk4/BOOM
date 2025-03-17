#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "variables.h"

#define MAX_JOUEURS 10

t_joueur joueurs[MAX_JOUEURS]; 
int nb_joueurs = 0;         

void ajouter_joueur(pid_t pid, const char *pseudo) {
    if (nb_joueurs < MAX_JOUEURS) {
        joueurs[nb_joueurs].pid = pid;
        strncpy(joueurs[nb_joueurs].pseudo, pseudo, sizeof(joueurs[nb_joueurs].pseudo) - 1);
        joueurs[nb_joueurs].estPret = false;
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

void envoyer_message_a_tous(const char *message) {
    for (int i = 0; i < nb_joueurs; i++) {
        printf("Envoi du message à %s (PID: %d) : %s\n", joueurs[i].pseudo, joueurs[i].pid, message);
        envoyer_signal(joueurs[i].pid, SIG_PSEUDOVALIDE);
    }
}

bool tous_pret() {
    if (nb_joueurs < 2) {
        return false;
    }
    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].estPret) {
            return false;
        }
    }
    return true;
}

void envoyer_tableau_joueurs(int idFile) {
    for (int i = 0; i < nb_joueurs; i++) {
        t_message_joueurs msg;
        msg.type = joueurs[i].pid;
        msg.nb_joueurs = nb_joueurs;
        memcpy(msg.joueurs, joueurs, sizeof(joueurs));

        if (msgsnd(idFile, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("Erreur lors de l'envoi du tableau des joueurs");
        } else {
            printf("Tableau envoyé à %s (PID: %d)\n", joueurs[i].pseudo, joueurs[i].pid);
        }
    }
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

    bool partieCommence = false;

    printf("Serveur démarré. En attente de joueurs...\n");

    while (1) {
        t_message message;

        if (msgrcv(idFile, &message, sizeof(message.corps), 1, 0) == -1) {
            perror("Erreur lors de la réception du message");
            continue;
        }

        switch (message.corps.type) {
            case 0: {
                if (partieCommence) {
                    printf("Connexion refusée : la partie a déjà commencé. Joueur (PID: %d) tué.\n", message.corps.pid);
                    kill(message.corps.pid, SIGKILL);
                    break;
                }
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
            }

            case 1: {
                for (int i = 0; i < nb_joueurs; i++) {
                    if (joueurs[i].pid == message.corps.pid) {
                        joueurs[i].estPret = true;
                        joueurs[i].enVie = true;
                        printf("%s (PID: %d) est prêt.\n", joueurs[i].pseudo, joueurs[i].pid);
                        break;
                    }
                }

                if (tous_pret()) {
                    printf("Tous les joueurs sont prêts. La partie commence !\n");
                    printf("---------------------------------------------\n");
                    partieCommence = true;
                    envoyer_tableau_joueurs(idFile);
                } else {
                    if (nb_joueurs >= 2) {
                        for (int i = 0; i < nb_joueurs; i++) {
                            if (!joueurs[i].estPret) {
                                envoyer_signal(joueurs[i].pid, SIG_PSEUDOVALIDE); 
                            }
                        }
                    }
                }
                break;
            }

            case 2:
                printf("Message reçu de %d : %s, type : %d\n", message.corps.pid, message.corps.msg, message.corps.type);
                break;

            default:
                printf("Type de message inconnu\n");
                break;
        }
    }

    msgctl(idFile, IPC_RMID, NULL);
    return 0;
}
