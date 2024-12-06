#ifndef CHECKDICO_H
#define CHECKDICO_H

#define TAILLE_MAX 100
#define FICHIER_DICTIONNAIRE "dictionnaire.txt"

void checkMot();
void convertir_en_majuscules(char *chaine);
int mot_existe_dans_fichier(const char *mot, const char *nom_fichier);


#endif
