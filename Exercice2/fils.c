#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void trait_signal_sigusr1() {
  printf("\tF: Reception du signal SIGUSR1 du pere\n");
  printf("\tF: Je me termine dans 3 secondes\n");
  sleep(3);
  exit(0);
}

void trait_signal_sigusr2() {
  printf("\tF: Interception du signal SIGUSR2\n");
  printf("\tF: Envoi du signal SIGUSR1 au pere de pid %d\n", getppid());
  kill(getppid(), SIGUSR1);
}


int main() {

  signal(SIGUSR1, trait_signal_sigusr1);
  signal(SIGUSR2, trait_signal_sigusr2);

  printf("\tF: Je suis le fils\n");
  printf("\tF: Mon pid est %d\n", getpid());
  printf("\tF: Attente de reception du signal SIGUSR2: kill -SIGUSR2 %d\n", getpid());

  while (1) {
    pause();
  }
}