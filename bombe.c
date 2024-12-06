#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "jeu.h"

void* gestionBombe(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutexMots);
        if (bombeTempsRestant <= 0) {
            joueurElimine = joueurActuel;
            printf("BOUM ! Le joueur %d est éliminé.\n", joueurActuel);
            pthread_mutex_unlock(&mutexMots);
            break;
        }
        bombeTempsRestant--;
        pthread_mutex_unlock(&mutexMots);
        sleep(1);
    }
    return NULL;
}
