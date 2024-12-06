#ifndef JEU_H
#define JEU_H

#include <pthread.h>

// Constantes
#define MAX_MOTS 100
#define MAX_JOUEURS 10
#define MAX_SYLLABES 500

// Variables globales
extern pthread_mutex_t mutexMots;
extern pthread_t threadBombe;
extern int bombeTempsRestant;
extern int joueurActuel;
extern int joueurElimine;
extern int nbJoueurs;
extern char combinaisonActuelle[10];
extern char motsUtilises[MAX_MOTS][50];
extern int nbMotsUtilises;

// Prototypes des fonctions
void tourDeJeu();
void genererCombinaison();
int motEstValide(const char* mot);
void ajouterMot(const char* mot);

#endif
