#ifndef JEU_H
#define JEU_H

#include <pthread.h>
#include <ctype.h> 

#define TAILLE_MAX 100
#define MAX_MOTS 100
#define MAX_SYLLABES 87
#define FICHIER_DICTIONNAIRE "dictionnaire.txt"

extern char combinaisonActuelle[10];
extern char motsUtilises[MAX_MOTS][50];
extern int nbMotsUtilises;

char *genererCombinaison();
int motEstValide(char* mot);
void ajouterMot(const char* mot);
int checkMot(char* saisie);
void convertir_en_majuscules(char *chaine);
int mot_existe_dans_fichier(const char *mot, const char *nom_fichier);

#endif
