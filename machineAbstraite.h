
#ifndef TP_FICHIERS_MACHINEABSTRAITE_H
#define TP_FICHIERS_MACHINEABSTRAITE_H
//***********
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


#define TAILLE_BLOC 999

//***********Declaration des types*********

typedef struct Bloc {
    char max;
    char chevauch[3]; //Indique le nombre de caractère en chevauchement dans le bloc, si egal à 0 pas de chevauchement
    char Tab[TAILLE_BLOC];
} BLOC;

typedef struct fichier {
    struct Tentete {
        int nbBlocs;
        int nbArticles;
        int carSupprimes;
        int carInseres;
    } entete;
    int blocCourrent;
    FILE* file;
} FICHIER;


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
void Inserer(FICHIER f, char cle[4]);
void Supprimer(FICHIER f, char cle[4]);

#endif //TP_FICHIERS_MACHINEABSTRAITE_H
