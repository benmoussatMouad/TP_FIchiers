
#ifndef TP_FICHIERS_MACHINEABSTRAITE_H
#define TP_FICHIERS_MACHINEABSTRAITE_H
//***********
#include "stdio.h"
#include "string.h"

#define TAILLE_BLOC 999;

//***********Declaration des types**

typedef struct Bloc {
    char* info;
    char efface;
    char cle[4];
    char chevauch;//indique la taille du chevauchement de l'article precedent dans le bloc
    //si egal à 0 alors pas de chevauchement
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


#endif //TP_FICHIERS_MACHINEABSTRAITE_H
