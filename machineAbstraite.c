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
        //f->entete.carInseres = 0;
        //f->entete.carSupprimes = 0;
        //f->entete.nbArticles = 0;
        //f->entete.nbBlocs = 0;
        //f->blocCourrent = 0;
        fread(&f->entete, TAILLE_ENTETE, 1, f->file);
        return 0;
    }
    else if( mode == 'n') {
        f->file = fopen("./test", "w+");
        perror("fopen");
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
    fwrite(&f->entete, TAILLE_ENTETE, 1,SEEK_SET);
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
    int cleCherche;
    int nbAccesMS = 0;
    int nonSequentiel = FAUX;

    BLOC buf = malloc(sizeof(BLOC));


    if ( Entete(f, 1) != 0) {
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

            if ( cleCherche <= buf->cleMax ) {
                LireDir(f, milieu - 1, buf);
                nbAccesMS++;

                if ( cleCherche > buf->cleMax ) { //la valeur se trouve dans le bloc mileu
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

            int j = atoi(buf->chevauch); //on commence le parcours depuis la fin du chevauchement,
            *trouv = FAUX;
            int finBloc = FAUX; //designe la fin du bloc

            char taille[4]; //variable utilise pour sauver la taille courrente
            int tailleCour; //conversion en entier de la taille
            char cleCour[5];//utilise pour sauver la cle courrente
            int cleC;       //Conversion de la cle courrente en entier

            while (!*trouv && !finBloc) {
                if ((j + TAILLE_TAILLE + TAILLE_EFF + TAILLE_CLE) <= TAILLE_BLOC - 1 ) {
                    //si l'article courrent n'est pas en chevauchement

                    memcpy(taille, &buf->Tab[j], 3); //Extraie une sous chaine allant de j a j+3 (la taille)
                    taille[3] = '\0';

                    tailleCour = atoi(taille);      //conversion de taille

                    memcpy(cleCour, &buf->Tab[j + 4], 4); //extraction de la cle
                    cleCour[4] = '\0';
                    cleC = atoi(cleCour);

                    if ( strcmp(cleCour, cle) == 0 ) { // si les cle sont identique alors
                        *trouv = VRAI;
                        *Pos = j;
                    } else {
                        if ( cleCherche > cleC || cleC == buf->cleMax) {
                            finBloc = VRAI;
                            *Pos = j; //la position ou il faut inserer
                        } else {
                            j += tailleCour;
                        }
                    }
                } else { //si l'article courrent est en chevauchement
                    *trouv = (cleCherche == buf->cleMax); //on est certain que la cle qui est en chevauchement
                    // est la derniere cle qui est aussi le max du bloc
                    // donc si on arrive au dernier element chevauché alors c'est le max
                }

            }
        }
    } else {
        *trouv = FAUX;
        *adrBloc = 0;
        *Pos = 0;
    }

    //****Affichage du nombre d'acces au disque
    printf("Il y a eu %d acces au disque.\n", nbAccesMS);
}
//-------------------------------------------------

void creationArticle(char * cle, char info[990], char taille[3], char efface, char Article[998])// Créer une chaine qui representera l'article
{
    for (int i = 0; i < 3 ; ++i) { // ecrire la taille de l'article
        Article[i]=taille[i];
    }
    Article[3]=efface; // ecrire le caractere qui mentionne si l'article est effacé ou non
    for (int i = 4; i < 8 ; ++i) { // ecrire la clé
        Article[i]=cle[i-4];
    }
    for (int i = 0; i < strlen(info) ; ++i) {// ecrire l'info
        Article[i+8]=info[i];
    }
}
//-------------------------------------------------
void decalBloc(FICHIER F,int i,int j, int * nbAccees)// décaler le bloc numero i de j positions et mettre à jour le nombre d'accees au disque
                                                    // on en aura besoin de ce module dans l'insertion
{
    BLOC buf1 = malloc(sizeof(BLOC));
    BLOC buf2 = malloc(sizeof(BLOC));

    LireDir(F,i,buf1);
    (*nbAccees)++;
    if (strlen(buf1->Tab)+j<=998){ // le cas où le nombre de caracteres dans le bloc plus le décalage est inferieur à taille bloc
        for (int k = strlen(buf1->Tab); k <1 ; --k) {
            buf1->Tab[k+j]=buf1->Tab[k];
        }
    } else{
        Aff_Entete(F,0,Entete(F,0)+1);
        if (Entete(F,0)>i){ //le cas où le bloc est au milieu
            LireDir(F,i+1,buf2);
            (*nbAccees)++;
            for (int k = 999; k <1 ; --k) {
                if (k+j>998){
                    buf2->Tab[(k+j)%999]=buf1->Tab[k];
                }else{
                    buf1->Tab[k+j]=buf1->Tab[k];
                }
            }
        }else{// le cas où le bloc est à la fin
            for (int k = 999; k <1 ; --k) {
                if (k+j>998){
                    buf2->Tab[(k+j)%999]=buf1->Tab[k];
                }else{
                    buf1->Tab[k+j]=buf1->Tab[k];
                }
            }
        }
        EcrireDir(F,i+1,buf2);
        (*nbAccees)++;
    }
    EcrireDir(F,i,buf1);
    (*nbAccees)++;
}
//-------------------------------------------------
void Insertion (FICHIER F, char *cle)
{
    int i, j, trouv, nbAccees=0;
    char Info[990];
    char Article[998];
    char taille[3];
    BLOC buf = malloc(sizeof(BLOC));

    printf("Donner l'info: ");
    scanf("%s", Info);
    itoa(strlen(Info)+3+1+4,taille,10);
    creationArticle(cle,Info,taille,'0',Article); // on crée l'article qu'on veut insérer
    Recherche(F,cle,&trouv,&i,&j);
    if (trouv){  // le cas où la clé existe deja
        printf("\n La cle existe deja!\n");
        if(j+3<999){
            LireDir(F, i, buf);
            buf->Tab[j + 3] = '0';
            EcrireDir(F, i, buf);
            nbAccees+=2;
        }else{
            LireDir(F, i + 1, buf);
            buf->Tab[(j+3)%999]='0';
            EcrireDir(F, i + 1, buf);
            nbAccees+=2;
        }
    } else{
        if (i==0 && j==0 ){// le cas de fichier vide
            strcpy(buf->Tab, Article);
            Aff_Entete(F,0,1);
            Aff_Entete(F, 1, 1);
            Aff_Entete(F, 3, atoi(taille));
            EcrireDir(F,1,buf);
            nbAccees++;
        }else{
            if (i==Entete(F,0)+1){ // le cas où il faut insérer à la fin avec creation de nouveau bloc
                strcpy(buf->Tab, Article);
                Aff_Entete(F,0,Entete(F,0)+1);
                Aff_Entete(F, 1, Entete(F,1)+1);
                Aff_Entete(F, 3, Entete(F,3)+atoi(taille));
                EcrireDir(F,i,buf);
                nbAccees++;
            }else{
                if (i==Entete(F,0) && j==Entete(F,3)%999+1){ // le cas où il faut insérer à la fin sans création de nouveau bloc
                    LireDir(F,i,buf);
                    nbAccees++;
                    Aff_Entete(F, 1, Entete(F,1)+1);
                    Aff_Entete(F, 3, Entete(F,3)+atoi(taille));
                    for (int k = 0; k < atoi(taille) ; ++k) {
                        if ((j)>999){
                            EcrireDir(F,i,buf);
                            i++;
                            Aff_Entete(F,0,Entete(F,0)+1);
                            LireDir(F,i,buf);
                            j=0;
                            nbAccees+=2;
                        }
                        buf->Tab[j]=Article[k];
                        j++;
                    }
                    EcrireDir(F,i,buf);
                    nbAccees++;
                } else{ // le cas où il faut inserer au millieu
                    Aff_Entete(F,1,Entete(F,1)+1);
                    Aff_Entete(F,3,Entete(F,3)+1);
                    for (int k = Entete(F,0); k < i;--k) {
                        decalBloc(F,k,atoi(taille),&nbAccees);
                    }
                    LireDir(F,i,buf);
                    for (int l = 0; l <atoi(taille) ; ++l) {
                        if ((j)>999){
                            EcrireDir(F,i,buf);
                            i++;
                            LireDir(F,i,buf);
                            j=0;
                            nbAccees+=2;
                        }
                        buf->Tab[j]=Article[l];
                        j++;
                    }
                    EcrireDir(F,i,buf);
                    nbAccees++;
                }
            }
        }
    }
    printf("\n Durant l'insertion, il y a eu %d accees au disque\n",nbAccees);
}
//-------------------------------------------------

void Suppression(FICHIER f,char * cle){
    int i,j,trouv,nbAccees=0;
    BLOC buf = malloc(sizeof(BLOC));
    char taille[3];

    Recherche(f,cle,&trouv,&i,&j);
    if (trouv == VRAI){ // on supprime l'info si elle existe
        LireDir(f, i, buf);
        nbAccees++;
        for (int k = 0; k < 3; ++k) {
            taille[k] = buf->Tab[j];
            if ( j == 998 ) {
                j = 0;
                EcrireDir(f, i, buf);
                i++;
                LireDir(f, i , buf);
                nbAccees+=2;
            } else{
                j++;
            }
        }
        Aff_Entete(f,2,Entete(f,2)+atoi(taille));
        buf->Tab[j]='1';
        EcrireDir(f,i,buf);
        nbAccees++;
    }
    printf("\nDurant la suppression, il y a eu %d accees au disque\n",nbAccees);
}
//------------------------------------------------

void AffichEntete(FICHIER f){
    printf("\t\n Le nombre de bloc(s) : %d"
    "\t\n Le nombre d'article(s) : %d"
    "\t\n Le nombre de caractere(s) insere(s) : %d"
    "\t\n Le nombre de caractere(s) supprime(s): %d \n",Entete(f,0),Entete(f,1),Entete(f,2),Entete(f,3));
}


