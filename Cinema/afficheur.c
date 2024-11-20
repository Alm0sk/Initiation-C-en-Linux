#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern int * attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************/
/*
 * Fonctions
*/
/******************************************************************************/



/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {

    unsigned int delais = 5;

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    int *mem;

    /*
    printf("Je suis %s, shmid=%d, semid=%d\n", argv[0], shmid, semid);
    */

    /* Attachement du segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);

    while (1) {
        /* On protège l'accès à la shm */
        P(semid);

        /* Affichage du nombre de places disponibles */
        printf("Afficheur %d : Il reste %d places disponibles.\n",getpid() , *mem);

        /* On libère l'accès à la shm */
        V(semid);

        /* Attente de 5 secondes */
        sleep(delais);
    }

    return (0);
}