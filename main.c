#include "machineAbstraite.h"

#define CLEAR system("cls")

int main() {
    char nomFichier[40];
    char inputChar;
    int erreur = 1;

    FICHIER F = malloc(sizeof(FICHIER)); //allouer un espace memoire suffisant

    do {
        printf("Entrez le nom du fichier :");
        gets(nomFichier);
        printf("Entrez le mode (n/a) : ");
        scanf("%c", &inputChar);
        if (Ouvrir(F, nomFichier, 'n') == 0) {
            printf("Fichier ouvert avec succe");
            gets(NULL);
            erreur = 0;
        } else {
            printf("Erreur reesayez.\n\n\n");
        }

    } while (erreur);

    CLEAR;

    //***********Menu principale******************

    printf("Bienvenue dans le programme de gestion de ficheir\n\t\tQue voulez vous faire ?\n\n");
    printf("\t1.Rechercher une donne."
           "\n\t2.Inserer une donne."
           "\n\t3.Supprimer une donne."
           "\n\t4.Afficher l'entete de votre fichier."
           "\n\t5.Afficher le contenu d'un bloc donne en MS."
           "\n\t6.Afficher le contenu du fichier bloc par bloc."
           "\n\t7.Reorganiser le fichier manuellement.");
    printf("\n\nEntrez un choix : ");
    scanf("%c", &inputChar);

    //*********************************************
    switch (inputChar) {
        default:
            return 0;
    }
    return 0;
}