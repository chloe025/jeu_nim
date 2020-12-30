#include <stdio.h>
#include <stdlib.h>
#include "type.h"

/*
###############################
#   on met ici les entêtes    #
###############################
*/
int LireEntier(int a, int b);
void Parametres(int* l , int* c, int* lvl, int* playeur, int* b);
int max(int a, int b);
int testban(int x, int y,int l, int c, T_Tab_Case T[], int NbanCree);
void Hasard_Ban (int nlig, int ncol, int nban, T_Tab_Case T[]);
void ZoneJeu(int nlig, int ncol, T_Tab_Case ban[], int nban, T_Case pion);
char testcase(T_Tab_Case Tab[],int nban, T_Case Pion,int x, int y);
T_Case CoupJoueur(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion);
T_Case CoupOrdi_Hasard(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion);
T_Case CoupOrdi_Gagnant(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion, int nim[VMAX][VMAX]);
void coupspossible(T_Case pion, T_Tab_Case T[], int n, int* npos, T_Tab_Case LC[] , int nlig, int ncol);
void Calcul_Nimbers(int T[][VMAX], int lig, int col);
int voisine(int x, int y, int i, int j, int T[VMAX][VMAX]);

/*
#############################
#       LES PARAMETRES      #
#############################
*/
// Lecture d'un nombre entier entre 2 valeurs données
int LireEntier(int a, int b)
{
    int val;
    do
        {
        scanf("%d", &val);
        } while (val < a || val > b);
    return val;
}
// Saisie des paramètres de début de jeu
void Parametres(int* l , int* c, int* lvl, int* playeur, int* b)
{
    printf("Entrez un nombre de lignes compris entre 5 et 30\n");
    *l = LireEntier(VMIN, VMAX);
    printf("Entrez un nombre de colonnes compris entre 5 et 30\n");
    *c = LireEntier(VMIN, VMAX);
    printf("Entrez un niveau de difficulté, 1 pour debutant, 2 pour intermediaire, 3 pour difficile ou 4 pour VIRTUOSE\n");
    *lvl = LireEntier(1,4);
    printf("Qui commence en premier ? 1 ordi, 2 joueur\n");
    *playeur =LireEntier(1,2);

    *b = rand()%max(*l,*c);
}
//Selection de la plus grande valeur
int max(int a, int b)
{
    int c;
    if (a < b)
        c = b;
    else
        c = a;
    return c;
}

/*
##########################
#    LES CASES BANNIES   #
##########################
*/

// teste des cases bannies
int testban(int x, int y,int l, int c, T_Tab_Case T[], int NbanCree)
{
    int test = 1, i=0;
    // si la case est en bas a gauche ou en haut a droite, on l'accepte et on sort du if
    if ( (x == l && y == 1) || (x == 1 && y == c))
        test=1;
    // sinon, si elle est sur les autres bords on l'a refuse
    else if ((x == 1) || (y == 1) || (x == l) || (y == c))
        {
            test = 0;
            return test;
        }

    // si la case existe déjà ou si case en diagonale
    while ((test == 1) && (i < NbanCree))
    {
        if ( (x == T[i].lig) && (y == T[i].col) )
            {
                test = 0;
                return test;
            }
        else if (((x+1) == T[i].lig) && ((y-1) == T[i].col))
        {
            test = 0;
            return test;
        }
        else if (((x-1) == T[i].lig) && ((y+1) == T[i].col))
        {
            test = 0;
            return test;
        }

        i++;
    }
    return test;
}

// Création des cases bannies
void Hasard_Ban (int nlig, int ncol, int nban, T_Tab_Case T[])
{
    int i;
    for (i=0 ; i < nban ; i++)
    {
        int resultat = 0;
        do
        {
            int coordX = rand()%nlig+1;
            int coordY = rand()%ncol+1;
            resultat = testban (coordX, coordY, nlig, ncol, T, i);
            if (resultat == 1)
            {
                T[i].lig = coordX;
                T[i].col = coordY;
            }
        }while (resultat != 1);
    }
}

/*
#############################
#       ZONE DE JEU         #
#############################
*/
// creation zone de jeu
void ZoneJeu(int nlig, int ncol, T_Tab_Case ban[], int nban, T_Case pion)
{
    int i, j;
    // pour avoir une mini-tabulation
    printf("  ");
    // on affiche le nombre de colonnes
    for (j=1 ; j <= ncol ; j++)
    {
        printf("  %.2d",j);
    }

    printf("\n");
    // on affiche la zone de jeu ainsi qu'à chaque ligne le numéro de celle-ci
    for (i=1 ; i <= nlig ; i++)
    {
        printf("%.2d",i);
        for (j=1 ; j <= ncol ; j++)
        {
            T_Case coord;
            coord.lig=i;
            coord.col=j;
            printf("| %c ", testcase(ban,nban,pion, coord.lig, coord.col));
        }
        printf("|\n");
    }
}
// Test position pour zone de jeu
char testcase(T_Tab_Case Tab[],int nban, T_Case Pion,int x, int y)
{
    int z=0, trouve=0;
    // on met la variable a _ de base, si jamais aucune des conditions est remplie
    char affiche ='_';
    while ( z < nban && trouve == 0)
    {
        if ((Tab[z].lig == x) && (Tab[z].col == y))
            {
                affiche = 'X';
            }
        z++;
    }
    if (Pion.lig == x && Pion.col == y)
        affiche = 'O';
    return affiche;
}


/*
#########################
#   GESTION DES COUPS   #
#########################
*/
// gestion coup d'un joueur
T_Case CoupJoueur(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion)
{
    int npos=0, i, choix;
    T_Tab_Case listeCoups[4];

    printf("coups possibles : ");
    coupspossible(pion, T, nban, &npos, listeCoups , nlig, ncol);

    for (i=0; i<npos; i++)
    {
        printf("numero %i en (%d,%d) ",i, listeCoups[i].lig , listeCoups[i].col);
    }
    printf("\nChoississez votre case ");
    choix = LireEntier(0,npos-1);
    // x y prennent les valeurs de la table a la choix ième case
    int x = listeCoups[choix].lig;
    int y = listeCoups[choix].col;
    // on change les coordonnées du nouveau pion, via les x,y car sinon il y a un bug
    (*nvPion).lig = x;
    (*nvPion).col = y;
    printf("Vous deplacez le pion en %d,%d\n", (*nvPion).lig, (*nvPion).col);
    return (*nvPion);
}

// gestion coup au hasard de l'ordi
T_Case CoupOrdi_Hasard(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion)
{
    // meme principe que le joueur
    int npos=0, choix;
    T_Tab_Case listeCoups[4];
    coupspossible(pion, T, nban, &npos, listeCoups , nlig, ncol);

    choix = rand()%npos;
    int x = listeCoups[choix].lig;
    int y = listeCoups[choix].col;

    (*nvPion).lig = x;
    (*nvPion).col = y;
    printf("Le PC deplace le pion en %d,%d\n", (*nvPion).lig, (*nvPion).col);
    return (*nvPion);
}

// gestion coup gagnant d'un ordi
T_Case CoupOrdi_Gagnant(T_Case pion, T_Tab_Case T[], int nban, int nlig, int ncol, T_Case* nvPion, int nim[VMAX][VMAX])
{
    int npos=0, i,j=0, choix, x,y;
    T_Tab_Case listeCoups[4];
    T_Tab_Case LCGagnant[4];
    LCGagnant[0].lig = -1; LCGagnant[0].col = -1;
    coupspossible(pion, T, nban, &npos, listeCoups , nlig, ncol);
    // une fois la liste des coups possibles obtenus, on cherche ceux qui sont gagnants
    for (i=0; i<npos; i++)
    {

        int coordX = listeCoups[i].lig;
        int coordY = listeCoups[i].col;
        if (nim[coordX][coordY]==0)
        {
            LCGagnant[j].lig = coordX;
            LCGagnant[j].col = coordY;
            printf("position coup gagnant : %d,%d\n", coordX,coordY);
            j++;
        }

    }
    // si la table de coups gagnants est vide
    if (LCGagnant[0].lig == -1 || LCGagnant[0].col == -1)
    {
        choix = rand()%npos;
        printf("Pas coup gagnant possible\n");
        x = listeCoups[choix].lig;
        y = listeCoups[choix].col;
    }
    else
    {
        choix = rand()%j;
        printf("coup gagnant possible\n");
        x = LCGagnant[choix].lig;
        y = LCGagnant[choix].col;
    }

    (*nvPion).lig = x;
    (*nvPion).col = y;
    printf("Le PC deplace le pion en %d,%d\n", (*nvPion).lig, (*nvPion).col);
    return (*nvPion);
}
// liste des coups possibles
void coupspossible(T_Case pion, T_Tab_Case T[], int n, int* npos, T_Tab_Case LC[] , int nlig, int ncol)
{
    int i=0, deplacdroit1 = 1, deplacdroit2 = 1, deplacbas1 = 1, deplacbas2 = 1;

    while ((deplacdroit1 == 1) && (i < n))
    {
        // si il y a une case bannie a droite, ou que la case a droite est supérieure au nombre de colonnes prédef
        if ((pion.col+1 > ncol) || (pion.lig == T[i].lig && pion.col+1 == T[i].col))
            deplacdroit1 = 0;
        i++;
    }
    i=0;
    while ((deplacdroit2 == 1) && (i < n))
    {
        if ((deplacdroit1==0) || (pion.col+2 > ncol) || ((pion.lig == T[i].lig && pion.col+2 == T[i].col)))
        {
            deplacdroit2 = 0;
        }
        i++;
    }
    i=0;
    while ((deplacbas1 == 1) && (i < n))
    {
        if ((pion.lig+1 > nlig) || (pion.lig+1 == T[i].lig && pion.col == T[i].col))
        {
            deplacbas1= 0;
        }
        i++;
    }
    i=0;
    while ((deplacbas2 == 1) && (i < n))
    {
        if ((deplacbas1 == 0) ||  (pion.lig+2 > nlig) || ((pion.lig+2 == T[i].lig && pion.col == T[i].col)))
        {
            deplacbas2 = 0;
        }
        i++;
    }
    // si il y a un déplacement possible, on mets les coordonnées dans la table et on incrémente
        if (deplacdroit1 == 1)
    {
        LC[*npos].lig = pion.lig;
        LC[*npos].col = pion.col+1;
        *npos= *npos+1;
    }
    if (deplacdroit2 == 1)
    {
        LC[*npos].lig = pion.lig;
        LC[*npos].col = pion.col+2;
        *npos= *npos+1;
    }

    if (deplacbas1 == 1)
    {
        LC[*npos].lig = (pion.lig+1);
        LC[*npos].col = pion.col;
        *npos= *npos+1;
    }
    if (deplacbas2 == 1)
    {
        LC[*npos].lig = pion.lig+2;
        LC[*npos].col = pion.col;
        *npos= *npos+1;
    }
}



/*
#################################
#   LA DIFFICULTE ET VOISINS    #
#################################
*/
// creation table nimbers
void Calcul_Nimbers(int T[VMAX][VMAX], int lig, int col)
{
    int i, j;
    int x=lig, y=col;
    for (i=y ; i >= 1 ; i--)
    {
        for (j=x ; j >= 1 ; j--)
        {
            int vois = voisine(x,y, i, j, T);
            T[j][i]= vois;
        }
    }
}

// détection des voisines
int voisine(int x, int y, int i, int j, int T[VMAX][VMAX])
{
    int z, vois=0;
    for (z=1; z<=2 ; z++)
    {
        if (i==y && j==x)
        {
            return vois;
        }
        // si on est inferieur au nombre de lignes
        if ((i+z) <= y)
        {
            if (T[j][i+z]==0)
            {
                return vois=1;
            }
        }
        // si on est inferieur au nombre de colonnes
        if ((j+z) <= x)
        {
            if (T[j+z][i]==0)
            {
                return vois=1;
            }
        }
    }
    return vois;
}
