#include "grille.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char grille[NB_COL][NB_ROW];

void ajouter_pion(char pion, int col);
int demander_coup();
void afficher_grille();
int puissance4(char pion);

void ajouter_pion(char pion, int col) {
    int ligne = 0;
    while (ligne < NB_ROW-1 && grille[col][ligne+1] == 0)
    {
        ligne++;
    }
    grille[col][ligne] = pion;
}

int demander_coup() {
    int col = 0;
    do {
        afficher_grille();

        printf("Colonne: ");

        col = getchar();
    } while(col < '0' || col > '0' + NB_COL-1);

    return col-'0';
}

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

void afficher_grille() {
    if (system("clear")) return;

    // Ligne d'en tête
    printf("+");
    for (int col = 0; col < NB_COL; col++) printf("---+");
    printf("\n");

    // Nombres
    for (int col = 0; col < NB_COL; col++) printf("  %d ", col);
    printf("\n");

    // génère la première ligne
    printf("+");
    for (int col = 0; col < NB_COL; col++) printf("---+");


    //  Génère les lignes et colonnes
    printf("\n");
    for (int lgn = 0; lgn < NB_ROW; ++lgn)
    {
        printf("|");

        for (int col = 0; col < NB_ROW+1; ++col) {
            char c = grille[col][lgn];

            switch (c)
            {
                case PION_J1:
                case PION_J2:
                printf(" %c |", c);
                break;
                default:
                printf("   |");
                break;
            }

        }

        printf("\n+");

        for (int col = 1; col <= NB_COL; ++col) printf("---+");

        printf("\n");
    }
   printf("\n");
}
