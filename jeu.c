#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jeu.h"
/*
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

// Gérer les tours de jeu
void tourDeJeu() {
    while (joueurElimine == -1) {
        pthread_mutex_lock(&mutexMots);
        printf("\nJoueur %d, c'est votre tour ! Temps restant : %d secondes.\n", joueurActuel, bombeTempsRestant);
        printf("Combinaison actuelle : '%s'\n", combinaisonActuelle);

        char mot[50];
        printf("Entrez un mot contenant la combinaison : ");
        scanf("%s", mot);

        if (motEstValide(mot)) {
            printf("Mot accepté !\n");
            ajouterMot(mot);
            joueurActuel = (joueurActuel + 1) % nbJoueurs;
            bombeTempsRestant = 10;
            genererCombinaison();
        } else {
            printf("Mot invalide ou déjà utilisé !\n");
            bombeTempsRestant -= 2;
        }

        pthread_mutex_unlock(&mutexMots);
        sleep(1);
    }
}

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
        motsUtilises[nbMotsUtilises - 1][sizeof(motsUtilises[0]) - 1] = '\0'; // Assurez-vous de la terminaison
    } else {
        printf("Erreur : limite des mots utilisés atteinte.\n");
    }
}*/