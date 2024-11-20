#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

/******************************************************************************/
/*
 * Fonctions externes
*/
/******************************************************************************/
extern void attente_aleatoire();
extern int *attacher_segment_memoire();
extern int P();
extern int V();

/******************************************************************************
 *
 * Fonctions
 *
******************************************************************************/

bool entree_client(int *mem, int semid, int nb_places_a_acheter) {
    bool place_attribuee = false;

    /* On protège l'accès à la shm */
    P(semid);

    /* Reste-t-il des places libres ? */
    if (*mem < nb_places_a_acheter) {
        /* No more */
        printf("%d : Pas assez de places disponibles. Il reste %d places.\n", getpid(), *mem);

    } else {
        /* On écrit dans la shm */
        *mem = (*mem - nb_places_a_acheter);
        printf("%d : Il reste %d places pour le film après un achat de %d places \n",getpid(), *mem, nb_places_a_acheter);
        place_attribuee = true;
    }

    /* On protège l'accès à la shm */
    V(semid);

    return (place_attribuee);
}


/******************************************************************************/
/*
 * Programme principal
*/
/******************************************************************************/
int main(int argc, char *argv[]) {

    srand(time(NULL) ^ getpid());

    unsigned int delais = 3;

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    int *mem;

    /*
    printf("Je suis %s, shmid=%d, semid=%d\n", argv[0], shmid, semid);
    */

    /* Attachement du segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);

    while (*mem > 0) {
        attente_aleatoire(delais);

        // Générer un nombre de places aléatoire à acheter
        int nb_places_a_acheter = (rand() % 7) + 1;
        printf("%d : Un client se présente pour acheter %d places [%d place disponibles] \n", getpid(), nb_places_a_acheter, *mem);
        if (*mem == 0) {
            printf("%d : Il n'y a plus de places disponibles pour le film, je ferme\n", getpid());
            exit(0);
        }
        entree_client(mem, semid, nb_places_a_acheter);
    }
    printf("%d : Il n'y a plus de places disponibles pour le film, je ferme\n", getpid());
    exit(0);
}