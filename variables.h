#include <unistd.h>

/* Corps des messages qui transitent, utilisé par le joueur et le jeu */
typedef struct corps {
    pid_t pid; /* pid de l'émetteur du message */
    char msg[256];
} t_corps;

typedef struct {
    long type;
    t_corps corps;
} t_message;