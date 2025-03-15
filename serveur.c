#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include "variables.h"



int main() {
    key_t cle = ftok("./dictionnaire.txt", 1);
    int idFile = msgget(cle, 0666 | IPC_CREAT);

    if(idFile == -1) {
        perror("Erreur lors de la récupération de la file de messages");
        return 1;
    }

    // reçois les messages et les affiches
    while(1) {
        t_message message;
        if (msgrcv(idFile, &message, sizeof(t_corps), 1, 0) == -1) {
            perror("Erreur lors de la réception du message");
            return 1;
        }
        printf("Message reçu : %s\n", message.corps.msg);

        // Envoyer un signal au processus du joueur
        if (strcmp(message.corps.msg, "SIGUSR1") == 0) {
            kill(message.corps.pid, SIGUSR1);
        } else if (strcmp(message.corps.msg, "SIGUSR2") == 0) {
            kill(message.corps.pid, SIGUSR2);
        }
    }

    return 0;
}