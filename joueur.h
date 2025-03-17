#ifndef JOUEUR_H
#define JOUEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

// Prototypes des fonctions
void demanderEtEnvoyerPseudo(int fileId);
char *demanderReponse();
<<<<<<< HEAD
void envoyerMessage(int fileId, char *message, int type);
=======
void envoyerMessage(int fileId, char *message);
>>>>>>> 0ca038d (ajout types messages joueur)
void demanderPret();

// Gestionnaires de signaux
void pseudoInvalide(int sig);
void motValide(int sig);
void motInvalide(int sig);
void aTonTour(int sig);
void perteVie(int sig);
void perdu(int sig);
void gagne(int sig);

#endif // JOUEUR_H