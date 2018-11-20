
#ifndef TP_FICHIERS_MACHINEABSTRAITE_H
#define TP_FICHIERS_MACHINEABSTRAITE_H

//***********Declaration des types**

typedef struct Bloc {
    char* article;
    char efface;
    char cle[4];
    int chevauch; //indique la taille du chevauchement de l'article precedent dans le bloc
    //si egal à 0 alors pas de chevauchement
} BLOC;

typedef struct fichier {
    int nbBlocs;
    int nbArticles;
    int carSupprimes;
    int carInseres;
    BLOC* blocs;
} FICHIER;


//***********Procedures du Modele*******

void Ouvrir(FICHIER f, char* nomFicheir, char mode);
void Fermer(FICHIER f);
void LireDir(FICHIER f, int i, BLOC buf);
void EcrireDir(FICHIER f, int i, BLOC buf);
void LireSeq(FICHIER f, BLOC bud);
void EcrireSeq(FICHIER f, BLOC buf);
BLOC* AllocBloc(FICHIER f);


#endif //TP_FICHIERS_MACHINEABSTRAITE_H
