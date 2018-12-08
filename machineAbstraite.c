#include "machineAbstraite.h"

//**************** IMPLEMENTATION DES MODELES **************

int Ouvrir (FICHIER f, char* nomFichier, char mode) {
    //Retourne 0 si il n'y a pas d'erreur
    // Retourne une valeur diff de 0 sinon


    char chemin[80];
    strcpy(chemin, ".\\");
    strcat(chemin, nomFichier);

    if ( mode == 'a' ) {
        (*f).file = fopen(chemin, "r+");
        if ( f->file == NULL) {
            perror("fopen");
        }
        //f->entete.carInseres = 0;
        //f->entete.carSupprimes = 0;
        //f->entete.nbArticles = 0;
        //f->entete.nbBlocs = 0;
        //f->blocCourrent = 0;
        fread(&f->entete, TAILLE_ENTETE, 1, f->file);
        return 0;
    }
    else if( mode == 'n') {
        f->file = fopen(chemin, "w+");
        if ( f->file == NULL) {
            perror("fopen");
        }
        f->entete.carInseres = 0;
        f->entete.carSupprimes = 0;
        f->entete.nbArticles = 0;
        f->entete.nbBlocs = 0;
        f->blocCourrent = 0;
        fwrite(&f->entete, TAILLE_ENTETE, 1, f->file);
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
    fseek(f->file, i* sizeof(BLOC) + TAILLE_ENTETE, SEEK_SET);
    fread(buf, TAILLE_BLOC, 1, f->file);
    f->blocCourrent = i;
}
//-------------------------------------------------
void EcrireDir (FICHIER f, int i, BLOC buf) {
    fseek(f->file, i * sizeof(BLOC) + TAILLE_ENTETE, SEEK_SET);
    fwrite(buf, TAILLE_BLOC, 1, f->file);
    f->blocCourrent = i;
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
    if ( f->blocCourrent != 0) {
        LireDir(f, f->blocCourrent, buf);
    } else {
        LireDir(f, 1, buf);
        f->blocCourrent = 1;
    }
    f->blocCourrent++;
}
//-------------------------------------------------
void EcrireSeq (FICHIER f, BLOC buf) {
    if ( f->blocCourrent != 0) {
        EcrireDir(f, f->blocCourrent, buf);
    } else {
        EcrireDir(f, 1, buf);
        f->blocCourrent = 1;
    }
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
//-------------------------------------------------
void Recherche (FICHIER f, char *cleCherche, int *trouv, int *adrBloc, int *Pos) {

    int cleChercheCv = atoi(cleCherche);
    int nbAccesMS = 0;
    int parcour;

    char cleCour[5]; //pour lire la cle courrente
    int cleCv;       //Cle courrente convertie en entier
    char tailleCour[4];//Pour lire la taille de l'article courrent
    int tailleCv;       //La taille courrente convertie en entier
    
    BLOC buf = malloc(sizeof(BLOC));

    if ( Entete(f, 0) != 0 ) {
        LireSeq(f, buf);
        nbAccesMS++;
        parcour = TAILLE_ENTETE;

        int finRech = FAUX;
        *trouv = FAUX;
        while (!*trouv && !finRech) {

            //****Lecture de la taille et la cle courrente******
            int repere; //parcour de la cle ou la taille en chevauchement

            if ( repere + TAILLE_TAILLE > TAILLE_BLOC-1 ) { //si la taille est chevauche
                repere = parcour; //repere pointe le debut de la taille
                for (int i = 0; i < 3; ++i) {
                    if ( repere <= TAILLE_BLOC-1 )
                        tailleCour[i] = buf->Tab[repere];
                    else {
                        LireSeq(f, buf);
                        nbAccesMS++;
                        repere = 0;
                        tailleCour[i] = buf->Tab[repere];
                    }
                    repere++;
                }
                tailleCour[4] = '\0';
                //Si la taille est chevauche la cle est surement chevauche donc on lit directement
                memcpy(cleCour, &buf->Tab[parcour + TAILLE_TAILLE + TAILLE_EFF], 4);
                cleCour[4] = '\0';

            } else { // la taille n'est pas chevauche
                memcpy(tailleCour, &buf->Tab[parcour], 3); //on lit directement la taille
                tailleCour[4] = '\0';

                repere = parcour + TAILLE_TAILLE + TAILLE_EFF; //repere pointe le debut de la cle
                if ( repere + TAILLE_CLE > TAILLE_BLOC-1 ) {//Si la cle est chevauche

                    for (int i = 0; i < 4; ++i) { //on lit caractere par caractere, on saute au prochain bloc si il ya
                        // chevauchement
                        if ( repere <= TAILLE_BLOC-1 )
                            cleCour[i] = buf->Tab[repere];
                        else {
                            LireSeq(f, buf);
                            nbAccesMS++;
                            repere = 0;
                            cleCour[i] = buf->Tab[repere];
                        }
                        repere++;
                    }
                } else {
                    memcpy(cleCour, &buf->Tab[parcour + TAILLE_TAILLE + TAILLE_EFF], 4);
                    cleCour[4] = '\0';
                }
            }

            cleCv = atoi(cleCour);
            tailleCv = atoi(tailleCour);

            if ( cleCv > cleChercheCv ) { //la cle n'existe pas
                finRech = VRAI;
                *adrBloc = f->blocCourrent;
                *Pos = parcour;
            } else {
                if ( cleCv == cleChercheCv ) { //la cle est trouve
                    *trouv = VRAI;
                    *adrBloc = f->blocCourrent;
                    *Pos = parcour;
                } else { // ou bien on passe a la suivante, traitant les cas du chevauchement
                    parcour += tailleCv;
                    if ( parcour > TAILLE_BLOC-1 ) {
                        LireSeq(f, buf);
                        nbAccesMS;
                        parcour %= TAILLE_BLOC;
                    }
                }
            }
        } //FTQ

    } else {
        *trouv = VRAI;
        *adrBloc = 0;
        *Pos = 0;
    }

    //****Affichage du nombre d'acces au disque
    printf("La recherche a pris %d acces au disque.\n", nbAccesMS);
}
//-------------------------------------------------

void creationArticle(char * cle, char info[990], char taille[3], char efface, char Article[998])
{
    for (int i = 0; i < 3 ; ++i) {
        Article[i]=taille[i];
    }
    Article[3]=efface;
    for (int i = 4; i < 8 ; ++i) {
        Article[i]=cle[i-4];
    }
    for (int i = 0; i < strlen(info) ; ++i) {
        Article[i+8]=info[i];
    }
}
//-------------------------------------------------

//-------------------------------------------------

//------------------------------------------------

void AffichEntete(FICHIER f){
    printf("\t\n Le nombre de bloc(s) : %d"
    "\t\n Le nombre d'article(s) : %d"
    "\t\n Le nombre de caractere(s) insere(s) : %d"
    "\t\n Le nombre de caractere(s) supprime(s): %d \n",Entete(f,0),Entete(f,1),Entete(f,2),Entete(f,3));
}


