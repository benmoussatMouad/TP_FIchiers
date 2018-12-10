#include "machineAbstraite.h"

#define CLEAR //system("cls");

int main() {
    char nomFichier[40];
    char inputChar;
    int inpuInt;
    int erreur = 1;

    FICHIER F = malloc(sizeof(FICHIER)); //allouer un espace memoire suffisant
    Intermediaire=malloc(sizeof(FICHIER));
    do {
        printf("Entrez le nom du fichier :");
        gets(nomFichier);
        fflush(stdin);
        printf("Entrez le mode (n/a) : ");
        scanf("%c", &inputChar);
        if (Ouvrir(F, nomFichier, inputChar) == 0) {
            printf("Fichier ouvert avec succees");
            getch();
            erreur = 0;
        } else {
            printf("Erreur reesayez.\n\n\n");
            getch();
        }

    } while (erreur);
    Ouvrir(Intermediaire,".\\INTERMEDIAIRE",'n');
    CLEAR;

    //***********Menu principale******************
    char OUT[1000];
    char cle[5]; //Sert pour la lecture en entree
    int quit = 0;
    int trouv, bloc, pos;

    while (!quit) {
        printf("\nBienvenue dans le programme de gestion de fichier\n\t\tQue voulez vous faire ?\n\n");
        printf("\t1.Rechercher une donne."
               "\n\t2.Inserer une donne."
               "\n\t3.Supprimer une donne."
               "\n\t4.Afficher l'entete de votre fichier."
               "\n\t5.Afficher le contenu d'un bloc donne en MS."
               "\n\t6.Afficher le contenu du fichier bloc par bloc."
               "\n\t7.Reorganiser le fichier manuellement."
               "\n\tAutres. Fermer et sauvegarder.");
        printf("\n\nEntrez un choix : ");
        scanf(" %d", &inpuInt);

        //*********************************************
        switch (inpuInt) {
            case 1: //Recherche
                CLEAR;
                puts("Entrez la cle à chercher: ");
                fflush(stdin);
                scanf(" %s", cle);
                Recherche(F, cle, &trouv, &bloc, &pos);
                if (trouv) {
                    printf("\nLa donne se trouve dans le bloc %d a la position %d\n.", bloc, pos);
                }
                else {
                    printf("\nLa donne n'est pas trouve.\n");
                }
                getch();
                break;
            case 2:
                CLEAR;
                puts("\nEntrez la cle de la donne a inserer (4 car) : ");
                fflush(stdin);
                scanf(" %s", cle);
                Insertion(F, cle);
                if (Entete(F,3)<=2*Entete(F,2)){
                    Reorganisation(F);
                }
                getch();
                break;
            case 3:
                CLEAR;
                puts("\nEntrez la cle à supprimer : ");
                fflush(stdin);
                scanf("%s", cle);
                Suppression(F, cle);
                getch();
                break;
            case 5:
                CLEAR;
                puts("\nEntrez le nombre du bloc à afficher :");
                fflush(stdin);
                scanf("%d", &inpuInt);
                if (inpuInt<=Entete(F,0)){
                LireDir(F, inpuInt, buff);
                for (int i = 0; i < 999; ++i) {
                    OUT[i] = buff->Tab[i];
                }
                OUT[999] = '\0';
                printf("\nLe bloc %d:",inpuInt);
                puts(OUT);}else printf("\nle bloc demandé n'existe pas.");
                getch();
                break;

            case 4:   //Afficher Entete
                AffichEntete(F);
                getch();
                break;
            case 6:
                for (int j = 1; j <Entete(F,0)+1 ; ++j) {
                    LireDir(F, j, buff);
                    for (int k = 0; k < 999; ++k) {
                        OUT[k] = buff->Tab[k];
                    }
                    OUT[999] = '\0';
                    printf(" \n %d\\BLOC %d : %s",j,j,OUT);
                    //puts(OUT);
                }
                getch();
                break;
            case 7:
                Reorganisation(F);
                getch();
                break;
            default:
                Fermer(F);
                Fermer(Intermediaire);
                quit = 1;
                break;
        }
    }
    return 0;
}