#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void checkMot() {
    char saisie[TAILLE_MAX];

    printf("Entrez un mot : ");
    if (fgets(saisie, sizeof(saisie), stdin) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture.\n");
        return 1;
    }

    // Enlever le caractère '\n' à la fin si présent
    saisie[strcspn(saisie, "\n")] = '\0';

    // Convertir en majuscules
    convertir_en_majuscules(saisie);

    if (mot_existe_dans_fichier(saisie, FICHIER_DICTIONNAIRE)) {
        printf("Ce mot existe dans le dictionnaire.\n");
    } else {
        printf("Ce mot n'existe pas dans le dictionnaire.\n");
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
            return 1; // Trouvé
        }
    }

    fclose(fichier);
    return 0; // Non trouvé
}