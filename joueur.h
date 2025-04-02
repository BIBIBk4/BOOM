#ifndef JOUEUR_H
#define JOUEUR_H

// Prototypes des fonctions
void demanderEtEnvoyerPseudo(int fileId);
char *demanderReponse();
void envoyerMessage(int fileId, char *message, int type);
void demanderPret();
void afficherListeJoueurs(const char *liste);

// Gestionnaires de signaux
void pseudoInvalide(int sig);
void motValide(int sig);
void motInvalide(int sig);
void aTonTour(int sig);
void perteVie(int sig);
void perdu(int sig);
void gagne(int sig);
void pseudoValide(int sig);
void recevoirListeJoueurs(int sig);

#endif