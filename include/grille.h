#ifndef GRILLE_H
#define GRILLE_H

#define DEMANDER_COUP 0
#define INDIQUER_COUP 1
#define FIN_DE_PARTIE 2

#define NB_COL 7
#define NB_ROW 6

#define PION_J1 'X'
#define PION_J2 'O'

void ajouter_pion(char pion, int col);
void afficher_grille();
int demander_coup();
int puissance4(char pion);

#endif /* GRILLE_H */
