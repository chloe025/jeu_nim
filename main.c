#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "type.h"

int main()
{
    // création des variables pour les paramètres
    int nlig, ncol, niveau, next, nban;
    T_Tab_Case ban[VMAX];
    int nim[VMAX][VMAX];
    T_Case pion;
    T_Case puit;
    pion.lig=1;
    pion.col=1;


     // on initialise les paramètres de la partie
    srand((unsigned int)time(NULL));
    printf("Nouvelle Partie!\n");

    // on envoie les adresses des variables dans la fonction -> modifier les valeurs
    Parametres(&nlig, &ncol, &niveau, &next, &nban);

    // on initialise le pion et le puit une fois toute les valeurs récupérées
    puit.lig=nlig;
    puit.col=ncol;

    // création des cases bannies
    int i=0;
    printf("Nombre de cases bannies : %d\n",nban);
    Hasard_Ban (nlig, ncol, nban, ban);
    for (i=0 ; i<nban ; i++)
        printf("(%d,%d)\t",ban[i].lig, ban[i].col);

    // tableau du nimber
    printf("\n\n");
    Calcul_Nimbers(nim, nlig, ncol);

    // initialisation difficulté
    float proba;
    switch(niveau)
    {
    case 1 :
        {
            proba = 0.;
            break;
        }
    case 2 :
        {
            proba = (1/3.)*100;
            break;
        }
    case 3 :
        {
            proba = (2/3.)*100;
            break;
        }
    case 4 :
        {
            proba = 1.*100;
            break;
        }
    }

    // création de la zone de jeu de départ
    printf("\nCreation zone de jeu\n");

    //  lancement du jeu
    int fin = 0;
    T_Case plot;
    while (fin == 0)
    {
        if (next%2 == 0)
        {
            printf("au joueur de jouer\n");
            ZoneJeu(nlig,ncol, ban, nban, pion);
            CoupJoueur(pion, ban, nban, nlig, ncol, &plot);
            pion.lig = plot.lig;
            pion.col = plot.col;
        }
        else
        {
            printf("au PC de jouer\n");
            ZoneJeu(nlig,ncol, ban, nban, pion);
            // si la valeur obtenue est inférieur à la proba d'un coup gagnant, c'est un coup gagnant
            if (rand()%101 <= proba) CoupOrdi_Gagnant(pion, ban, nban, nlig, ncol, &plot, nim);
            else CoupOrdi_Hasard(pion, ban, nban, nlig, ncol, &plot);
            pion.lig = plot.lig;
            pion.col = plot.col;
        }
        if (pion.lig == nlig && pion.col == ncol)
            {
                fin=1;
                ZoneJeu(nlig,ncol, ban, nban, pion);
                printf("fin !\n");
                if (next%2 == 0)
                    printf("Vous avez vaincu votre adversaire !");
                else
                    printf("L'IA a été plus forte que vous, etre humain...");

            }
        printf("\n");
        next++;
    }

    return 0;
}
