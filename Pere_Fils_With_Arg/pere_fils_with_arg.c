#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/* Objectif: Générer un nombre de processus fils mis en paramètre */

int main(int argc, char *argv[]) {

  	printf("\n");

    /* Récupération de l'argument du script pour avoir le nombre de fork souhaités */
    const int nb_fork = atoi(argv[1]);

    if (nb_fork < 1) {
        printf("Le nombre de fork doit être supérieur à 0\n");
        return 1;
    }

    printf("Nombre de fork souhaité: %d\n", nb_fork);

    /* Récupération du pid du processus père */
    const pid_t pid = getpid();
    printf("P: processus père\n");
    printf("P: pid = %d\n\n", pid);

    /* Création de 10 processus fils */
    for (int i = 0; i < nb_fork; i++) {
        /* création du fils */
        pid_t pid_fils = fork();

        if (pid_fils == 0) {
            /* si processus fils */
            printf("F-%d: pid = %d, ppid = %d\n", i + 1, getpid(), getppid());
            return 0;
        }
    }

    /* Attendre que tous les processus fils se terminent */
    for (int i = 0; i < 10; i++) {
        wait(NULL);
    }
    return 0;
}