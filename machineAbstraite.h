
#ifndef TP_FICHIERS_MACHINEABSTRAITE_H
#define TP_FICHIERS_MACHINEABSTRAITE_H
//***********
#include "stdio.h"
#include "string.h"

#define TAILLE_BLOC 999;

//***********Declaration des types*********

typedef struct Bloc {
    char taille[3];
    char efface;
    char cle[4];
    char info[991]; //taille du bloc moins les premiers champs
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

void Recherche(int* trouv,int* adrBloc, int* adrPos);

#endif //TP_FICHIERS_MACHINEABSTRAITE_H
