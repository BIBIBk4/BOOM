#include <unistd.h>
#include <stdbool.h>
#include <signal.h>


#define SIG_MOTVALIDE SIGUSR1
#define SIG_MOTINVALIDE SIGUSR2
#define SIG_PERTEVIE SIGCHLD
#define SIG_ATONTOUR SIGPWR
#define SIG_PSEUDOVALIDE SIGRTMIN
#define SIG_PSEUDOINVALIDE SIGRTMIN+1

#define FICHIER "./dictionnaire.txt"

/* Corps des messages qui transitent, utilisé par le joueur et le jeu */
typedef struct corps {
    pid_t pid; 
    char msg[256];
} t_corps;

typedef struct {
    long type;
    t_corps corps;
} t_message;

typedef struct {
    pid_t pid;
    bool estPret;
    char pseudo[50];
} t_joueur;

