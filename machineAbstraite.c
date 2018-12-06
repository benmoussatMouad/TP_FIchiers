#include "machineAbstraite.h"

//**************** IMPLEMENTATION DES MODELES **************

int Ouvrir (FICHIER f, char* nomFichier, char mode) {
    //Retourne 0 si il n'y a pas d'erreur
    // Retourne une valeur diff de 0 sinon


    char chemin[80];
    strcpy(chemin, "./");
    strcat(chemin, nomFichier);

    if ( mode == 'a' ) {
        (*f).file = fopen(chemin, "r+");
        perror("fopen");
        f->entete.carInseres = 0;
        f->entete.carSupprimes = 0;
        f->entete.nbArticles = 0;
        f->entete.nbBlocs = 0;
        f->blocCourrent = 0;
        return 0;
    }
    else if( mode == 'n') {
        f->file = fopen("./test", "w");
        perror("fopen");
        f->entete.carInseres = 0;
        f->entete.carSupprimes = 0;
        f->entete.nbArticles = 0;
        f->entete.nbBlocs = 0;
        f->blocCourrent = 0;
        return 0;
    }
    else
        return 1;
}
//-------------------------------------------------
void Fermer (FICHIER f) {
    fclose(f->file);
}
//-------------------------------------------------
void LireDir (FICHIER f, int i, BLOC buf) {
    fseek(f->file, i* sizeof(BLOC), SEEK_SET);
    fread(&buf, TAILLE_BLOC, 1, f->file);
}
//-------------------------------------------------
void EcrireDir (FICHIER f, int i, BLOC buf) {
    fseek(f->file, i * sizeof(BLOC), SEEK_SET);
    fwrite(&buf, TAILLE_BLOC, 1, f->file);
}
//-------------------------------------------------
int Entete (FICHIER f, int i) {
    switch (i) {
        case 0:
            return f->entete.nbBlocs;
        case 1:
            return f->entete.nbArticles;
        case 2:
            return f->entete.carSupprimes;
        case 3:
            return f->entete.carInseres;
        default:
            return -1;
    }
}
//-------------------------------------------------
void LireSeq (FICHIER f, BLOC buf) {
    LireDir(f, f->blocCourrent, buf);
    f->blocCourrent++;
}
//-------------------------------------------------
void EcrireSeq (FICHIER f, BLOC buf) {
    EcrireDir(f, f->blocCourrent, buf);
    f->blocCourrent++;
}
//-------------------------------------------------
void Aff_Entete (FICHIER f, int i, int val) {
    switch (i) {
        case 0:
            f->entete.nbBlocs = val;
            break;
        case 1:
            f->entete.nbArticles = val;
            break;
        case 2:
            f->entete.carInseres = val;
            break;
        case 3:
            f->entete.carSupprimes = val;
            break;
        default:
            break;
    }
}

void Recherche (FICHIER f, char *cle, int *trouv, int *adrBloc, int *Pos) {
    int Bi, Bs, milieu;
    int blocTrouv = 0;
    BLOC buf;
    int cleCherche;
    int nbAccesMS = 0;
    int nonSequentiel = FAUX;

    if ( Entete(f, 1) == 0) {
        //*************************************
        //Recherche Dicothomique sur les blocs
        //*************************************
        cleCherche = atoi(cle); //on convertie la cle en un entier
        Bi = 1;
        Bs = Entete(f, 0);

        while (Bi <= Bs && !blocTrouv) {

            milieu = (Bi + Bs) / 2;
            LireDir(f, milieu, buf);
            nbAccesMS++;

            if ( cleCherche <= buf.cleMax ) {
                LireDir(f, milieu - 1, buf);
                nbAccesMS++;

                if ( cleCherche > buf.cleMax ) { //la valeur se trouve dans le bloc mileu
                    *adrBloc = milieu;
                    blocTrouv = 1;
                } else {
                    Bs = milieu - 1;
                }
            } else {
                Bi = milieu + 1;
                if ( Bi > Entete(f, 0)) { //la valeurs cherche est plus grande que toutes les valeurs du fichiers

                    nonSequentiel = VRAI; //pas la peine de faire une recherche sequentielle dans le bloc
                    *trouv = FAUX;

                    *Pos = Entete(f, 2) % 999; //On calcul la position du dernier caractere dans le dernier bloc

                    if ( *Pos == 0 ) { //si le dernier bloc est plein alors
                        *adrBloc = Entete(f, 0) + 1; //pour faciliter l'insertion aucas ou la valeur depasse la plus grande valeur
                    } else
                        *adrBloc = Entete(f, 0); //sinon on retourne la position ou il faut inserer
                }
            }
        }

        //*********************************
        //Recherche SEQUENTIEL dans le bloc trouvé
        //*********************************
        if (!nonSequentiel) {
            LireDir(f, *adrBloc, buf);
            nbAccesMS++;

            int j = atoi(buf.chevauch); //on commence le parcours depuis la fin du chevauchement,
            *trouv = FAUX;
            int finBloc = FAUX; //designe la fin du bloc

            char taille[4]; //variable utilise pour sauver la taille courrente
            int tailleCour; //conversion en entier de la taille
            char cleCour[5];//utilise pour sauver la cle courrente
            int cleC;       //Conversion de la cle courrente en entier

            while (!*trouv && !finBloc) {
                if ((j + TAILLE_TAILLE + TAILLE_EFF + TAILLE_CLE) <= TAILLE_BLOC - 1 ) {
                    //si l'article courrent n'est pas en chevauchement

                    memcpy(taille, &buf.Tab[j], 3); //Extraie une sous chaine allant de j a j+3 (la taille)
                    taille[3] = '\0';

                    tailleCour = atoi(taille);      //conversion de taille

                    memcpy(cleCour, &buf.Tab[j + 4], 4); //extraction de la cle
                    cleCour[4] = '\0';
                    cleC = atoi(cleCour);

                    if ( strcmp(cleCour, cle) == 0 ) { // si les cle sont identique alors
                        *trouv = VRAI;
                        *Pos = j;
                    } else {
                        if ( cleCherche > cleC || cleC == buf.cleMax) {
                            finBloc = VRAI;
                            *Pos = j; //la position ou il faut inserer
                        } else {
                            j += tailleCour;
                        }
                    }
                } else { //si l'article courrent est en chevauchement
                    *trouv = (cleCherche == buf.cleMax); //on est certain que la cle qui est en chevauchement
                    // est la derniere cle qui est aussi le max du bloc
                    // donc si on arrive au dernier element chevauché alors c'est le max
                }

            }
        } else {
            *trouv = FAUX;
            *adrBloc = 0;
            *Pos = 0;
        }
    }

    //****Affichage du nombre d'acces au disque
    printf("Il y a eu %d acces au disque.\n", nbAccesMS);
}
//-------------------------------------------------



