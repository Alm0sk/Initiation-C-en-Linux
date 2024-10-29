/*
Vous devez écrire un programme qui affiche son PID et qui attend la réception du signal
SIGUSR1. À réception de ce signal, le processus crée un fils, ce fils exécute un programme
de votre choix (eog, evince, chrome, firefox…). Le père attend la mort du fils
Amélioration : on pourrait imaginer que à chaque réception du signal SIGUSR1, le
programme se comporte encore de la même manière.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * @brief Fonction qui traite le signal SIGUSR1, et crée un fils qui Firefox en navigation privée sur la page d'accueil d'Archlinux
 */
void trait_signal_sigusr1() {
    /* Reception du signal SIGUSER1*/
    signal(SIGUSR1, trait_signal_sigusr1);
    printf("interception du signal\n");

    /* Création du fils et execution de Firefox */
    const pid_t pid = fork();

    /* si processus fils */
    if (pid == 0) {
        printf("\t pid du fils = %d, pid du parent = %d\n", getpid(), getppid());
        execl("/usr/bin/firefox", "firefox", "--private-window", "https://archlinux.org/", NULL);
    } else {
        /* si processus père */
        if (pid > 0) {
        } else {
            /* si retour <0 */
            perror("pb fork");
        }
    }
}

/**
 * @brief Fonction qui traite le signal SIGTERM de fin du processus
 */
void trait_signal_term() {
    printf("\ninterception du signal SIGTERM\n");
    printf("Fin du processus parent\n");
    exit(0);
}

int main() {
  printf("\n");
  signal(SIGUSR1, trait_signal_sigusr1);
    signal(SIGTERM, trait_signal_term);

    printf("Mon pid est %d\n", getpid());
    printf("Lancer le signal: kill -SIGUSR1 %d\n", getpid());
    printf("Terminer le programme: kill -SIGTERM %d\n\n", getpid());

    while (1) {
        pause();
    }
}
