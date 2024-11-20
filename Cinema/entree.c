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

/******************************************************************************/
/*
 * Fonctions
*/
/******************************************************************************/

bool entree_client(int *mem, int semid, int nb_places_a_acheter) {
    bool place_attribuee = false;

    /* On protège l'accès à la shm */
    P(semid);

    /* Reste-t-il des places libres ? */
    if (*mem < nb_places_a_acheter) {
        /* No more */
        printf("Pas assez de places disponibles. Il reste %d places.\n", *mem);
    } else {
        /* On écrit dans la shm */
        *mem = (*mem - nb_places_a_acheter);
        printf("Il reste %d places pour le film après un achat de %d places \n", *mem, nb_places_a_acheter);
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

    unsigned int delais = (rand() % 3) + 1;

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    int *mem;

    /*
    printf("Je suis %s, shmid=%d, semid=%d\n", argv[0], shmid, semid);
    */

    /* Attachement du segment de mémoire partagée */
    mem = attacher_segment_memoire(mem, &shmid);

    while (1) {
        attente_aleatoire(delais);
        printf("Un client se présente\n");

        // Générer un nombre de places aléatoire à acheter
        const int nb_places_a_acheter = (rand() % 7) + 1;
        while (entree_client(mem, semid, nb_places_a_acheter) == false) {
            sleep(delais);
        }
    }

    return (0);
}