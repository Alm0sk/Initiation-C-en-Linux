#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shm_const.h"



extern int creer_initialiser_semaphore();
extern int * attacher_segment_memoire();
extern int P();
extern int V();
extern int creer_segment_memoire();

int main(int argc, char *argv[])
{
    pid_t pid_entree; /* no du processus du processus entree  */
    pid_t pid_sortie; /* no du processus du processus sortie */

    // Les variables qui seront utilisés pour stocker les valeurs des arguments

    int code_retour_fin_entree;
    int code_retour_fin_sortie;

    int nombre_de_guichets;
    char * nombre_de_guichets_str;

    char * titre_du_film;

    int nombre_places_cinema; /* Pour écriture dans la shm */
    char * nombre_places_cinema_str; /* Pour conversion du semid (int) en chaine */

    int shmid; /* Id du segment de mémoire partagé */
    int semid; /* Id du sémaphore */

    char shmid_str[20]; /* Pour conversion du shmid (int) en chaine */
    char semid_str[20]; /* Pour conversion du semid (int) en chaine */

    int * mem; /* Adresse du segment de mémoire partagée */

    char param_gnome_terminal[80];

    if ( argc != 4 ) {
        fprintf(stderr, "Usage : %s nombre_de_guichets titre_du_film nombre_places\n", argv[0]);
        return(3);
    }

    // Attribution des valeurs aux arguments

    nombre_places_cinema_str=argv[1];
    nombre_places_cinema=atoi(nombre_places_cinema_str);

    /* Avant de créer les fils :
    * - on crée le semaphore
    * - on crée le segment de mémoire partagé
    * - on s'attache le segment de mémoire partagé
    * - on initialise le nombre de places de cinéma libres dans la shm
    */

    /* Création sémaphore */
    creer_initialiser_semaphore(CLE_SEM, &semid);

    /* Création segment de mémoire partagé */
    creer_segment_memoire(CLE_SHM, &shmid);

    /* Attachement du segment de mémoire partagée */
    mem=attacher_segment_memoire(mem, &shmid);

    /* Pas besoin de sémaphore on est seul :-) */
    *mem=nombre_places_cinema;

    /* Conversion des shmid et semid  (int) en chaine pour appel programme externe */
    sprintf(shmid_str, "%d", shmid);
    sprintf(semid_str, "%d", semid);

    /* création du fils entree */
    pid_entree = fork();

    if (pid_entree == -1) {
        /* Erreur */
        perror("pb fork sur création entree");
        return(1);
    }

    if (pid_entree == 0) {
        execl("entree", "entree", shmid_str, semid_str, NULL);
    }

    if (pid_entree >0) {
        /* processus père */

        /* création du fils sortie */
        pid_sortie = fork();

        if (pid_sortie == -1) {
            /* Erreur */
            perror("pb fork sur création sortie");
            return(1);
        }

        if (pid_sortie == 0) {
             execl("sortie", "sortie", shmid_str, semid_str, nombre_places_cinema_str, NULL);
        }

        /* processus père */

        printf("Père, on attend 1000s \n");
        sleep(1000);

        printf("P: processus père fin\n");
        return(0);
    }
}