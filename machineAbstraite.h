
#ifndef TP_FICHIERS_MACHINEABSTRAITE_H
#define TP_FICHIERS_MACHINEABSTRAITE_H
//***********Inclusion des bibliotheques standards**************
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"

#define TAILLE_BLOC 999
#define TAILLE_TAILLE 3
#define TAILLE_CLE 4
#define TAILLE_EFF 1
//Valeurs logiques:
#define VRAI 1
#define FAUX 0
//***********Declaration des types*********

typedef struct Bloc {
    //On considerera que chaque article est ordone comme ça:
    // TAILLE - EFF - CLE - INFO ...
    char Tab[TAILLE_BLOC];
}* BLOC;

typedef struct fichier  {
    struct Tentete {
        int nbBlocs;
        int nbArticles;
        int carSupprimes;
        int carInseres;
    } entete;
    int blocCourrent;
    FILE* file;
} * FICHIER;

BLOC buff , buffTmp;
#define TAILLE_ENTETE sizeof(struct Tentete)


//***********Procedures du Modele*******

int Ouvrir(FICHIER f, char* nomFicheir, char mode);
///----------------------------------------------
void Fermer(FICHIER f);
//-------------------------------------------------
void LireDir(FICHIER f, int i, BLOC buf);
//-------------------------------------------------
void EcrireDir(FICHIER f, int i, BLOC buf);
//-------------------------------------------------
int Entete(FICHIER f, int i);
//-------------------------------------------------
void LireSeq(FICHIER f, BLOC buf);
//-------------------------------------------------
void EcrireSeq(FICHIER f, BLOC buf);
//-------------------------------------------------
void Aff_Entete (FICHIER, int, int);
//--------------------------------------------


//*****************Modules***************

void Recherche(FICHIER f, char cle[4], int* trouv, int* adrBloc, int* Pos);
void Insertion (FICHIER F, char *cle);
void Suppression(FICHIER f,char * cle);
void AffichEntete(FICHIER f);
void creationArticle(char * cle, char info[990], char taille[3], char efface, char Article[998]);


#endif //TP_FICHIERS_MACHINEABSTRAITE_H
