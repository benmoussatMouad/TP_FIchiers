#include "machineAbstraite.h"

#define CLEAR system("cls")

int main() {
    char nomFichier[40];
    char inputChar;
    int inpuInt;
    int erreur = 1;

    FICHIER F = malloc(sizeof(FICHIER)); //allouer un espace memoire suffisant


    do {
        printf("Entrez le nom du fichier :");
        gets(nomFichier);
        printf("Entrez le mode (n/a) : ");
        scanf("%c", &inputChar);
        if (Ouvrir(F, nomFichier, inputChar) == 0) {
            printf("Fichier ouvert avec succe");
            gets(NULL);
            erreur = 0;
        } else {
            printf("Erreur reesayez.\n\n\n");
        }

    } while (erreur);

    CLEAR;

    //***********Menu principale******************

    int quit = 0;
    while (!quit) {
        printf("Bienvenue dans le programme de gestion de ficheir\n\t\tQue voulez vous faire ?\n\n");
        printf("\t1.Rechercher une donne."
               "\n\t2.Inserer une donne."
               "\n\t3.Supprimer une donne."
               "\n\t4.Afficher l'entete de votre fichier."
               "\n\t5.Afficher le contenu d'un bloc donne en MS."
               "\n\t6.Afficher le contenu du fichier bloc par bloc."
               "\n\t7.Reorganiser le fichier manuellement.");
        printf("\n\nEntrez un choix : ");
        fflush(stdin);
        scanf("%c", &inputChar);

        //*********************************************
        char cle[5]; //Sert pour la lecture en entree
        switch (inputChar) {
            case '1': //Recherche
                CLEAR;
                puts("Entrez la cle à chercher: ");
                fflush(stdin);
                gets(cle);
                int trouv, bloc, pos;
                Recherche(F, cle, &trouv, &bloc, &pos);
                if (trouv) {
                    printf("La donne se trouve dans le bloc %d a la position %d\n.", bloc, pos);
                }
                else {
                    printf("La donne n'est pas trouve.\n");
                }
                getch();
                break;

            case '2':
                CLEAR;
                puts("Entrez la cle de la donne a inserer (4 car) : ");
                fflush(stdin);
                gets(cle);
                getch();
                break;
            case '5':
                CLEAR;
                puts("Entrez le nombre du bloc à afficher :");
                scanf("%d", &inpuInt);
                BLOC buf = malloc(sizeof(BLOC));
                LireDir(F, inpuInt, buf);
                fwrite(buf, TAILLE_BLOC, 1, stdout);
                getch();
                break;

            case '4':   //Afficher Entete
                AffichEntete(F);
                getch();
                break;
            default:
                quit = 1;
                break;
        }
    }
    return 0;
}