#include "grille.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Matrice de 2d pour le stockage des pions
char grille[NB_COL][NB_ROW];

// Prototypes
void ajouter_pion(char pion, int col);
int demander_coup();
void afficher_grille();
int puissance4(char pion);

/**
	Ajoute un pion à la grille.
	@param pion Le pion a ajouter à la grille.
	@param col La colonne à laquelle insérer le pion.
*/
void ajouter_pion(char pion, int col) {
    int ligne = 0;
    while (ligne < NB_ROW-1 && grille[col][ligne+1] == 0)
    {
        ligne++;
    }
    grille[col][ligne] = pion;
}

/**
	Demande sur stdin quel coup l'utilisateur souhaite jouer.

	@return La colonne à jouer, telle que 0 <= colonne < NB_COL
*/
int demander_coup() {
    int col = 0;
    do {
        afficher_grille();

        printf("Colonne: ");

        // équilvalent à stdin
        col = getchar();
    } while(col < '0' || col > '0' + NB_COL-1); // range pour le placement du pion

    return col-'0';
}

/**
	Cherche un alignement de 4 pions identiques au pion passés en paramètre.

	@param pion Le pion pour lequel chercher un alignement.

	@return 1 si un alignement de 4 pions a été trouvé, 0 sinon.
*/
int puissance4(char pion) {
    int counter = 0;

    // Colonne par colonne
    for (int col = 0; col < NB_COL; col++)
    {
        for (int ligne = 0; ligne < NB_ROW; ligne++)
        {
            if (grille[col][ligne] == pion)
            {
                counter++;
                if (counter == 4) return 1;
            } else {
                counter = 0;
            }
        }
        counter = 0;
    }

    counter = 0;

    // Ligne par ligne
    for (int ligne = 0; ligne < NB_ROW; ligne++)
    {
        for (int col = 0; col < NB_COL; col++)
        {
            if (grille[col][ligne] == pion)
            {
                counter++;
                if (counter == 4) return 1;
            } else {
                counter = 0;
            }
        }
        counter = 0;
    }

    // Diagonal
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            int count1 = 0;
            int count2 = 0;

            for (int j = 0; j < 4; j++)
            {
                if (grille[x+j][y+j] == pion)
                {
                    count1++;
                    if (count1 == 4) return 1;
                } else {
                    count1 = 0;
                }

                if (grille[NB_ROW - x - j][y+j] == pion)
                {
                    count2++;
                    if (count2 == 4) return 1;
                } else {
                    count2 = 0;
                }

            }
        }
    }

    return 0;
}

/**
	Affiche la grille sur stdout.
*/
void afficher_grille() {
    if (system("clear")) return;

    // Ligne d'en tête
    printf("+");
    for (int col = 0; col < NB_COL; col++) printf("---+");
    printf("\n");

    // Numéros des colonnes
    for (int col = 0; col < NB_COL; col++) printf("  %d ", col);
    printf("\n");

    // génère la première ligne
    printf("+");
    for (int col = 0; col < NB_COL; col++) printf("---+");


    //  Génère les lignes et colonnes avec les pions
    printf("\n");
    for (int lgn = 0; lgn < NB_ROW; ++lgn)
    {
        printf("|");

        for (int col = 0; col < NB_ROW+1; ++col) {
            char c = grille[col][lgn];

            switch (c)
            {
                // Switch pour la couleur des pions
                case 'X':
                    printf(" \033[0;31m%c\033[0m |", grille[col][lgn]);
                    break;
                
                case 'O':
                    printf(" \033[0;33m%c\033[0m |", grille[col][lgn]);
                    break;

                default:
                    printf("   |");
                    break;
            }

        }

        // Dernière ligne de l'affichage
        printf("\n+");
        for (int col = 1; col <= NB_COL; ++col) printf("---+");

        printf("\n");
    }
   printf("\n");
}
