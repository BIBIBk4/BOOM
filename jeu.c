#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jeu.h"

// Variables globales
pthread_mutex_t mutexMots = PTHREAD_MUTEX_INITIALIZER;
pthread_t threadBombe;
int bombeTempsRestant = 10;
int joueurActuel = 0;
int joueurElimine = -1;
int nbJoueurs = 0;
char combinaisonActuelle[10];
char motsUtilises[MAX_MOTS][50];
int nbMotsUtilises = 0;

// Liste des syllabes prédéfinies
char *syllabes[MAX_SYLLABES] = {
    "ba", "be", "bi", "bo", "bu", "ca", "ce", "ci", "co", "cu",
    "da", "de", "di", "do", "du", "fa", "fe", "fi", "fo", "fu",
    "ga", "ge", "gi", "go", "gu", "la", "le", "li", "lo", "lu",
    "ma", "me", "mi", "mo", "mu", "na", "ne", "ni", "no", "nu",
    "pa", "pe", "pi", "po", "pu", "ra", "re", "ri", "ro", "ru",
    "sa", "se", "si", "so", "su", "ta", "te", "ti", "to", "tu",
    "va", "ve", "vi", "vo", "vu", "za", "ze", "zi", "zo", "zu",
    "ai", "au", "ei", "eu", "ou", "ui", "an", "en", "in", "on",
    "un", "ou", "oi", "ui", "é", "è", "ç", "ch", "gh", "qu"
};

// Générer une combinaison aléatoire
void genererCombinaison() {
    int index = rand() % MAX_SYLLABES;
    strncpy(combinaisonActuelle, syllabes[index], sizeof(combinaisonActuelle) - 1);
    combinaisonActuelle[sizeof(combinaisonActuelle) - 1] = '\0'; // Assurez-vous de la terminaison
}

// Vérifier si un mot est valide
int motEstValide(const char* mot) {
    pthread_mutex_lock(&mutexMots);

    // Vérifier si le mot contient la combinaison actuelle
    if (!strstr(mot, combinaisonActuelle)) {
        pthread_mutex_unlock(&mutexMots);
        return 0;
    }

    //Vérifier si le mot existe dans le fichier dictionnaire
    if (!checkMot(mot)) {
        pthread_mutex_unlock(&mutexMots);
        return 0;
    }

    // Vérifier si le mot a déjà été utilisé
    for (int i = 0; i < nbMotsUtilises; i++) {
        if (strcmp(motsUtilises[i], mot) == 0) {
            pthread_mutex_unlock(&mutexMots);
            return 0;
        }
    }

    pthread_mutex_unlock(&mutexMots);
    return 1;
}

// Ajouter un mot à la liste
void ajouterMot(const char* mot) {
    if (nbMotsUtilises < MAX_MOTS) {
        strncpy(motsUtilises[nbMotsUtilises++], mot, sizeof(motsUtilises[0]) - 1);
        motsUtilises[nbMotsUtilises - 1][sizeof(motsUtilises[0]) - 1] = '\0';
    } else {
        printf("Erreur : limite des mots utilisés atteinte.\n");
    }
}

int checkMot(char* saisie){

    saisie[strcspn(saisie, "\n")] = '\0';

    convertir_en_majuscules(saisie);

    if (mot_existe_dans_fichier(saisie, FICHIER_DICTIONNAIRE)) {
        return 1;
    }

    return 0;
}

// Fonction pour convertir une chaîne en majuscules
void convertir_en_majuscules(char *chaine) {
    for (int i = 0; chaine[i]; i++) {
        chaine[i] = toupper((unsigned char)chaine[i]);
    }
}

// Fonction pour vérifier si un mot existe dans un fichier
int mot_existe_dans_fichier(const char *mot, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    char ligne[TAILLE_MAX];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Enlever le '\n' à la fin de chaque mot lu
        ligne[strcspn(ligne, "\n")] = '\0';

        // Convertir la ligne lue en majuscules pour la comparaison
        convertir_en_majuscules(ligne);

        // Comparer les mots
        if (strcmp(mot, ligne) == 0) {
            fclose(fichier);
            return 1; 
        }
    }

    fclose(fichier);
    return 0;
}
