#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdbool.h>
#include <sys/types.h>

// Constantes
#define MAX_JOUEURS 10

// fonctions
void ajouter_joueur(pid_t pid, const char *pseudo);
void envoyer_signal(pid_t pid, int signal);
void envoyer_message(pid_t pid, const char *message);
void envoyer_liste_joueurs();
void changerTour(int tour);
bool tous_pret();
void *lancementPartie();

#endif