#ifndef GRILLE_H
#define GRILLE_H

// taille de la grille
#define NB_COL 7
#define NB_ROW 6

// utilisé pour l'affichage des pions
#define PION_J1 'X'
#define PION_J2 'O'

/**
	Ajoute un pion à la grille.
	@param pion Le pion a ajouter à la grille.
	@param col La colonne à laquelle insérer le pion.
*/
void ajouter_pion(char pion, int col);

/**
	Affiche la grille sur stdout.
*/
void afficher_grille();
/**
	Demande sur stdin quel coup l'utilisateur souhaite jouer.

	@return La colonne à jouer, telle que 0 <= colonne < NB_COL
*/
int demander_coup();

/**
	Cherche un alignement de 4 pions identiques au pion passés en paramètre.

	@param pion Le pion pour lequel chercher un alignement.

	@return 1 si un alignement de 4 pions a été trouvé, 0 sinon.
*/
int puissance4(char pion);

#endif /* GRILLE_H */
