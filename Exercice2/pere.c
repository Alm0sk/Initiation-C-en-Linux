/*
Vous devez écrire un programme qui affiche son PID et qui attend la réception du signal
SIGUSR1. À réception de ce signal, le processus crée un fils, le fils affiche son pid puis
attend la réception du signal SIGUSR2. À réception de ce signal, le fils envoie SIGUSR1 au
père qui renvoie au fils SIGUSR1. Lors de la réception de SIGUSR1, le fils affiche qu’il va se
terminer dans 3 secondes, le père attend la mort du fils et se termine.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int step = 0;
pid_t pid_fils;
/**
 * @brief Fonction qui vérifie l'existance du fichier fils.c et le compile
 */
void compile_fils() {
    if (access("fils.c", F_OK) == 0) {
        /* Le fichier fils.c existe */
        printf("P: fils.c trouvé\n");
        printf("P: compilation de fils.c...\n");

        /* Fork pour la compilation */
        const pid_t pid = fork();
        if (pid == 0) {
            /* si processus fils : compilation */
            execl("/usr/bin/make", "make", "fils", NULL);
            perror("P: Erreur lors de l'exécution de make.\n");
        } else if (pid > 0) {
            /* si processus père */
            waitpid(pid, NULL, 0);
            printf("P: Compilation terminée.\n\n");
        }
    } else {
        /* Le fichier fils.c n'existe pas */
        perror("P: Erreur : Le fichier fils.c est introuvable.\n");
    }
}

/**
 * @brief Fonction qui traite le signal SIGUSR1, et crée un fils qui affiche son pid puis attend la réception du signal du fils
 */
void trait_signal_sigusr1() {
    int status_fils;
    /* Premier signal SIGUSR1 */
    if (step == 0) {
        pid_fils = fork();

        /* Reception du 1er signal SIGUSER1 qui */
        if (pid_fils == 0) {
            /* si processus fils */
            printf("P: interception du signal SIGUSR1\n");
            printf("P: Lancement du fils...\n");
            execl("./fils", "fils", NULL);
        }
        if (pid_fils > 0) {
            /* si processus père */
            step++;
        }
        /* Deuxième signal SIGUSR1 */
    } else if (step == 1) {
        printf("P: Signal SIGUSR1 reçu du fils\n");
        printf("P: Envoi de SIGUSER1 au fils de pid %d\n", pid_fils);
        kill(pid_fils, SIGUSR1); /* Envoi du signal SIGUSR1 au fils */
        waitpid(pid_fils, &status_fils, 0);
        exit(0);
    }
}

int main() {
    printf("\n");

    signal(SIGUSR1, trait_signal_sigusr1);

    /* Vérification de l'existence du fichier fils et compilation */
    /* J'ai fais le choix de le recompiler à chaque fois pour simplifier le development */
    compile_fils();

    printf("P: Je suis le pere\n");
    printf("P: Mon pid est %d\n", getpid());
    printf("P: Attente de reception du signal SIGUSR1: kill -SIGUSR1 %d\n\n", getpid());

    while (1) {
        pause();
    }
}
