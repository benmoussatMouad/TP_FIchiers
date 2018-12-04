#include "machineAbstraite.h"

int main() {
    char nomFichier[40];
    char inputChar;
    int erreur = 1;
    FICHIER F;

    do { //TODO: erreur dans la lecture de la chaine
        printf("Entrez le nom du fichier :");
        scanf("%s", nomFichier);
        printf("Entrez le mode (n/a)");
        scanf("%c", &inputChar);
        if (Ouvrir(F, nomFichier, inputChar) == 0) {
            printf("Fichier ouvert avec succe");
            erreur = 0;
        } else
            printf("Erreur reesayez.\n\n\n");

    } while (erreur);


    return 0;
}