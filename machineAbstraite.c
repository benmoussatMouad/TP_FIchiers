#include "machineAbstraite.h"

//**************** IMPLEMENTATION DES MODELES **************

int Ouvrir (FICHIER f, char* nomFichier, char mode) {
    //Retourne 0 si il n'y a pas d'erreur
    // Retourne une valeur diff de 0 sinon

    if ( mode == 'a' ) {
        f.file = fopen(nomFichier, "r+");
        f.entete.carInseres = 0;
        f.entete.carSupprimes = 0;
        f.entete.nbArticles = 0;
        f.entete.nbBlocs = 0;
        f.blocCourrent = 0;
        return 0;
    }
    else if( mode == 'n') {
        f.file = fopen(nomFichier, "w");
        f.entete.carInseres = 0;
        f.entete.carSupprimes = 0;
        f.entete.nbArticles = 0;
        f.entete.nbBlocs = 0;
        f.blocCourrent = 0;
        return 0;
    }
    else
        return 1;
}
//-------------------------------------------------
void Fermer (FICHIER f) {
    fclose(f.file);
}
//-------------------------------------------------
void LireDir (FICHIER f, int i, BLOC buf) {
    fseek(f.file, i* sizeof(BLOC), SEEK_SET);
    fread(&buf,999, 1, f.file);
}
//-------------------------------------------------
void EcrireDir (FICHIER f, int i, BLOC buf) {
    fseek(f.file, i * sizeof(BLOC), SEEK_SET);
    fwrite(&buf, 999, 1, f.file);
}
//-------------------------------------------------
int Entete (FICHIER f, int i) {
    switch (i) {
        case 0:
            return f.entete.nbBlocs;
        case 1:
            return f.entete.nbArticles;
        case 2:
            return f.entete.carSupprimes;
        case 3:
            return f.entete.carInseres;
        default:
            return -1;
    }
}
//-------------------------------------------------
void LireSeq (FICHIER f, BLOC buf) {
    LireDir(f, f.blocCourrent, buf);
    f.blocCourrent++;
}
//-------------------------------------------------
void EcrireSeq (FICHIER f, BLOC buf) {
    EcrireDir(f, f.blocCourrent, buf);
    f.blocCourrent++;
}
//-------------------------------------------------
void Aff_Entete (FICHIER f, int i, int val) {
    switch (i) {
        case 0:
            f.entete.nbBlocs = val;
            break;
        case 1:
            f.entete.nbArticles = val;
            break;
        case 2:
            f.entete.carInseres = val;
            break;
        case 3:
            f.entete.carSupprimes = val;
            break;
        default:
            break;
    }
}
//-------------------------------------------------



