#include "machineAbstraite.h"

//**************** IMPLEMENTATION DES MODELES **************

int Ouvrir (FICHIER f, char* nomFichier, char mode) {
    //Retourne 0 si il n'y a pas d'erreur
    // Retourne une valeur diff de 0 sinon


    char chemin[80];
    strcpy(chemin, ".\\");
    strcat(chemin, nomFichier);
    buff= malloc(sizeof(BLOC));
    buffTmp= malloc(sizeof(BLOC));

    if ( mode == 'a' ) {
        (*f).file = fopen(chemin, "rb+");
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
        f->file = fopen(chemin, "wb+");
        if ( f->file == NULL) {
            perror("fopen");
        }
        f->entete.carInseres = 0;
        f->entete.carSupprimes = 0;
        f->entete.nbArticles = 0;
        f->entete.nbBlocs = 0;
        f->blocCourrent = 0;
        fwrite(&f->entete, 1, TAILLE_ENTETE, f->file);
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
void LireDir (FICHIER f, int i, BLOC buff) {
    fseek(f->file, (i-1)* TAILLE_BLOC + TAILLE_ENTETE, SEEK_SET);
    fread(&buff->Tab,1,TAILLE_BLOC, f->file);
    f->blocCourrent = i;
}
//-------------------------------------------------
void EcrireDir (FICHIER f, int i, BLOC buff) {
    fseek(f->file, (i-1) * TAILLE_BLOC+ TAILLE_ENTETE, SEEK_SET);
    fwrite(&buff->Tab, 1, TAILLE_BLOC, f->file);
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
void LireSeq (FICHIER f, BLOC buff) {
    if ( f->blocCourrent != 0) {
        LireDir(f, f->blocCourrent, buff);
    } else {
        LireDir(f, 1, buff);
        f->blocCourrent = 1;
    }
    f->blocCourrent++;
}
//-------------------------------------------------
void EcrireSeq (FICHIER f, BLOC buff) {
    if ( f->blocCourrent != 0) {
        EcrireDir(f, f->blocCourrent, buff);
    } else {
        EcrireDir(f, 1, buff);
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
            f->entete.carSupprimes = val;
            break;
        case 3:
            f->entete.carInseres = val;
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

    if ( Entete(f, 0) != 0 ) {
        f->blocCourrent = 0;
        LireSeq(f, buff);
        nbAccesMS++;

        parcour = 0;

        int finRech = FAUX;
        *trouv = FAUX;

        while (!*trouv && !finRech) {
            printf("%d",Entete(f,3));
            if (parcour != (Entete(f, 3) % 999) || (f->blocCourrent-1 ) != Entete(f, 0)) { //Si on arrive a la fin du fichier
//            //****Lecture de la taille et la cle courrente******
//            int repere; //parcour de la cle ou la taille en chevauchement
//
//            if ( parcour + TAILLE_TAILLE-1 > TAILLE_BLOC-1 ) { //si la taille est chevauche
//                puts("Taille Chevauche");
//                getch();
//                repere = parcour; //repere pointe le debut de la taille
//                for (int i = 0; i < 3; ++i) {
//                    if ( repere <= TAILLE_BLOC-1 )
//                        tailleCour[i] = buff->Tab[repere];
//                    else {
//                        LireSeq(f, buff);
//                        nbAccesMS++;
//                        repere = 0;
//                        tailleCour[i] = buff->Tab[repere];
//                    }
//                    repere++;
//                }
//                tailleCour[4] = '\0';
//                //Si la taille est chevauche la cle est surement chevauche donc on lit directement
//                memcpy(cleCour, &buff->Tab[parcour + TAILLE_TAILLE + TAILLE_EFF], 4);
//                cleCour[4] = '\0';
//
//            } else { // la taille n'est pas chevauche
//                puts("TAILE TAILE");
//                memcpy(tailleCour, &buff->Tab[parcour], 3); //on lit directement la taille
//                tailleCour[4] = '\0';
//
//
//                repere = parcour + TAILLE_TAILLE + TAILLE_EFF; //repere pointe le debut de la cle
//                if ( repere + TAILLE_CLE > TAILLE_BLOC-1 ) {//Si la cle est chevauche
//
//                    for (int i = 0; i < 4; ++i) { //on lit caractere par caractere, on saute au prochain bloc si il ya
//                        // chevauchement
//                        if ( repere <= TAILLE_BLOC-1 )
//                            cleCour[i] = buff->Tab[repere];
//                        else {
//                            LireSeq(f, buff);
//                            nbAccesMS++;
//                            repere = 0;
//                            cleCour[i] = buff->Tab[repere];
//                        }
//                        repere++;
//                    }
//                } else {
//                    puts("CLE NON CHEVA");
//                    memcpy(cleCour, &buff->Tab[parcour + TAILLE_TAILLE + TAILLE_EFF], 4);
//                    cleCour[4] = '\0';
//                }
//            }
                strncpy(tailleCour, &buff->Tab[parcour], 3);
                tailleCour[4] = '\0';
                strncpy(cleCour, &buff->Tab[parcour + TAILLE_TAILLE + TAILLE_EFF], 4);
                cleCour[5] = '\0';
                cleCv = atoi(cleCour);
                tailleCv = atoi(tailleCour);

                if ( cleCv > cleChercheCv ) { //la cle n'existe pas
                    finRech = VRAI;
                    *adrBloc = f->blocCourrent - 1;
                    *Pos = parcour;
                } else {
                    if ( cleCv == cleChercheCv ) { //la cle est trouve
                        *trouv = VRAI;
                        *adrBloc = f->blocCourrent - 1;
                        *Pos = parcour;
                    } else { // ou bien on passe a la suivante, traitant les cas du chevauchement
                        parcour += tailleCv;
                        if ( parcour > TAILLE_BLOC-1 ) {
                            LireSeq(f, buff);
                            nbAccesMS++;
                            parcour %= TAILLE_BLOC;
                        }
                    }
                }
            } else {
                *trouv = FAUX;
                finRech=VRAI;
                *adrBloc = Entete(f, 0);
                *Pos = parcour;
            }
        } //FTQ

    } else {
        *trouv = FAUX;
        *adrBloc = 0;
        *Pos = 0;
    }

    //****Affichage du nombre d'acces au disque
    printf("La recherche a pris %d acces au disque.\n", nbAccesMS);
}
//-------------------------------------------------

void creationArticle(char * cle, char info[990], char taille[4], char efface, char Article[998])// Créer une chaine qui representera l'article
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
    Article[strlen(info)+8]='\0';
}
//-------------------------------------------------
void decalBloc(FICHIER F,int i,int j, int * nbAccees)// décaler le bloc numero i de j positions et mettre à jour le nombre d'accees au disque
                                                    // on en aura besoin de ce module dans l'insertion
{
    //BLOC buff = malloc(sizeof(BLOC));
    //BLOC buffTmp = malloc(sizeof(BLOC));
    LireDir(F,i,buff);
    (*nbAccees)++;
    if (Entete(F,0)==i){
    if (Entete(F,3)%999-1+j <=TAILLE_BLOC-1 ){ // le cas où le nombre de caracteres dans le bloc plus le décalage est inferieur à taille bloc, cas du denier bloc
        for (int k = Entete(F,3)%999-1; k > -1 ; --k) {
            buff->Tab[k+j]=buff->Tab[k];
        }
    }else{// le cas où le bloc est à la fin
        for (int k = Entete(F,3)%999-1; k > -1 ; --k) {
            if (k+j>998){
                buffTmp->Tab[(k+j)%999]=buff->Tab[k];
            }else{
                buff->Tab[k+j]=buff->Tab[k];
            }
        }
    }
    EcrireDir(F,i+1,buffTmp);
    (*nbAccees)++;}
    else{
        if (Entete(F,0)>i){ //le cas où le bloc est au milieu
            LireDir(F,i+1,buffTmp);
            (*nbAccees)++;
            for (int k = 998; k > -1 ; --k) {
                if (k+j>998){
                    buffTmp->Tab[(k+j)%999]=buff->Tab[k];
                }else{
                    buff->Tab[k+j]=buff->Tab[k];
                }
            }
            EcrireDir(F,i+1,buffTmp);
            (*nbAccees)++;
        }
    }
    EcrireDir(F,i,buff);
    (*nbAccees)++;
}
//-------------------------------------------------
void Insertion (FICHIER F, char *cle)
{
    int i, j, trouv, nbAccees=0;
    char Info[990];
    char Article[998];
    char taille[4];

    printf("Donner l'info: ");
    fflush(stdin);
    gets(Info);
    itoa(strlen(Info)+3+1+4,taille,10);
    puts(taille);
    if ( strlen(Info) + TAILLE_CLE + TAILLE_TAILLE + TAILLE_EFF < 10) {
        taille[2] = taille[0];
        taille[1] = taille[0] = '0';
    } else if ( strlen(Info) + TAILLE_CLE + TAILLE_TAILLE + TAILLE_EFF < 100 ) {
        taille[2] = taille[1];
        taille[1] = taille[0];
        taille[0] = '0';
    }
    taille[3]='\0';
    creationArticle(cle,Info,taille,'0',Article); // on crée l'article qu'on veut insérer
    Recherche(F,cle,&trouv,&i,&j);
    if (trouv){  // le cas où la clé existe deja
        printf("\nLa cle existe deja!\n");
        if(j+3<999){
            LireDir(F, i, buff);
            buff->Tab[j + 3] = '0';
            EcrireDir(F, i, buff);
            nbAccees+=2;
        }else{
            LireDir(F, i + 1, buff);
            buff->Tab[(j+3)%999]='0';
            EcrireDir(F, i + 1, buff);
            nbAccees+=2;
        }
    } else{
        if (i==0 && j==0 ){// le cas de fichier vide
            for (int k = 0; k < atoi(taille) ; ++k) {
                buff->Tab[k]=Article[k];
            }
            Aff_Entete(F,0,1);
            Aff_Entete(F, 1, 1);
            Aff_Entete(F, 3, atoi(taille));
            EcrireDir(F,1,buff);
            nbAccees++;
        }else{
            if (i==Entete(F,0)+1){ // le cas où il faut insérer à la fin avec creation de nouveau bloc
                for (int k = 0; k < atoi(taille) ; ++k) {
                    buff->Tab[k] = Article[k];
                }
                Aff_Entete(F,0,Entete(F,0)+1);
                Aff_Entete(F, 1, Entete(F,1)+1);
                Aff_Entete(F, 3, Entete(F,3)+atoi(taille));
                EcrireDir(F,i,buff);
                nbAccees++;
            }else{
                if (i==Entete(F,0) && j==Entete(F,3)%999){ // le cas où il faut insérer à la fin sans création de nouveau bloc
                    LireDir(F,i,buff);
                    puts(Article);
                    nbAccees++;
                    Aff_Entete(F, 1, Entete(F,1)+1);
                    Aff_Entete(F, 3, Entete(F,3)+atoi(taille));
                    for (int W = 0; W < atoi(taille) ; ++W) {
                        if ((j)>998){
                            EcrireDir(F,i,buff);
                            i++;
                            Aff_Entete(F,0,Entete(F,0)+1);
                            j=0;
                            nbAccees+=2;
                        }
                        buff->Tab[j]=Article[W];
                        j++;
                    }
                    EcrireDir(F,i,buff);
                    nbAccees++;
                } else{ // le cas où il faut inserer au millieu
                    Aff_Entete(F,1,Entete(F,1)+1);
                    Aff_Entete(F,3,Entete(F,3)+atoi(taille));
                    for (int k = Entete(F,0); k > i-1;--k) {
                        decalBloc(F,k,atoi(taille),&nbAccees);
                    }
                    LireDir(F,i,buff);
                    for (int v = 0; v <atoi(taille) ; ++v) {
                        if ((j)>998){
                            EcrireDir(F,i,buff);
                            i++;
                            LireDir(F,i,buff);
                            j=0;
                            nbAccees+=2;
                        }
                        buff->Tab[j]=Article[v];
                        j++;
                    }
                    EcrireDir(F,i,buff);
                    nbAccees++;
                }
            }
        }
    }
    printf("\nDurant l'insertion, il y a eu %d accees au disque\n",nbAccees);
}

//-------------------------------------------------

void Suppression(FICHIER f,char * cle){
    int i,j,trouv,nbAccees=0;
    char taille[3];

    Recherche(f,cle,&trouv,&i,&j);
    if (trouv == VRAI){ // on supprime l'info si elle existe
        LireDir(f, i, buff);
        nbAccees++;
        for (int k = 0; k < 3; ++k) {
            taille[k] = buff->Tab[j];
            if ( j == 998 ) {
                j = 0;
                EcrireDir(f, i, buff);
                i++;
                LireDir(f, i , buff);
                nbAccees+=2;
            } else{
                j++;
            }
        }
        Aff_Entete(f,2,Entete(f,2)+atoi(taille));
        buff->Tab[j]='1';
        EcrireDir(f,i,buff);
        nbAccees++;
    }
    printf("\nDurant la suppression, il y a eu %d accees au disque\n",nbAccees);
}
//------------------------------------------------

void AffichEntete(FICHIER f){
    printf("\t\n Le nombre de bloc(s) : %d"
    "\t\n Le nombre d'article(s) : %d"
    "\t\n Le nombre de caractere(s) insere(s) : %d"
    "\t\n Le nombre de caractere(s) supprime(s): %d \n",Entete(f,0),Entete(f,1),Entete(f,3),Entete(f,2));
}


//----------------------------------


void Reorganisation(FICHIER F){


    int nbBloc=0, nbArticle=0,CarInserer=0;
    int j=0,i=1,k=0,l=1,parcour=0;
    char efface;
    char taille[4];
    char info[990];
    char Article[998];
    char cle[5];
    taille[4]='\0';



    if(Entete(F,0)!=0){
        LireDir(F,1,buff);
        while (parcour != Entete(F,3)%999 || i != Entete(F,0)){
            j=parcour;
            for (int m = 0; m <3; ++m) {
                if (j>998){
                    i++;
                    LireDir(F,i,buff);
                    j=0;
                }
                taille[m]=buff->Tab[j];
                j++;
            }
            taille[3]='\0';
            if (j>998){
                i++;
                LireDir(F,i,buff);
                j=0;
                efface=buff->Tab[j];
            } else {efface=buff->Tab[j];}
            j++;
            for (int n = 0; n <4; ++n) {
                if (j>998){
                    i++;
                    LireDir(F,i,buff);
                    j=0;
                }
                cle[n]=buff->Tab[j];
                j++;
            }
            cle[4]='\0';
            for (int n = 0; n < atoi(taille)-8; ++n) {
                if (j>998){
                    i++;
                    LireDir(F,i,buff);
                    j=0;
                }
                info[n]=buff->Tab[j];
                j++;
            }
            info[990]='\0';
            creationArticle(cle,info,taille,efface,Article);
            if (efface=='1'){
                parcour+=atoi(taille);
            }else{
                if (Entete(Intermediaire,0)==0){
                    Aff_Entete(Intermediaire,0,1);
                }
                for (int m = 0; m < atoi(taille); ++m) {
                    if (k>998){
                        EcrireDir(Intermediaire,l,buffTmp);
                        Aff_Entete(Intermediaire,0,Entete(Intermediaire,0)+1);
                        l++;
                        k=0;
                    }
                    buffTmp->Tab[k]=Article[m];
                    k++;
                }
                Aff_Entete(Intermediaire,1,Entete(Intermediaire,1)+1);
                Aff_Entete(Intermediaire,3,Entete(Intermediaire,3)+atoi(taille));
                parcour+=atoi(taille);
            }
        }
        EcrireDir(Intermediaire,l,buffTmp);
        Aff_Entete(F,0,Entete(Intermediaire,0));
        Aff_Entete(F,1,Entete(Intermediaire,1));
        Aff_Entete(F,3,Entete(Intermediaire,3));
        Aff_Entete(F,2,Entete(Intermediaire,2));
        for (int i1 = 1; i1 <Entete(F,0)+1 ; ++i1) {
            LireDir(Intermediaire,i1,buff);
            EcrireDir(F,i1,buff);
        }
    }
    }





