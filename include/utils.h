#ifndef UTILS_H
#define UTILS_H

/*
Chaque message serveur -> client est composé de deux octets :
- Le premier a l'une des valeurs suivantes ci-dessous.
- Le second est un argument.
*/

// type de la requête faite par le serveur
#define DEMANDER_COUP 0 // le serveur demande au joueur la colonne qu'il souhaite jouer (argument inutilisé)
#define INDIQUER_COUP 1 // le serveur valide le coup du joueur et informe l'autre joueur du coup joué (l'argument est le numéro de colonne)
#define FIN_DE_PARTIE 2 // le serveur arrête la partie (pour chaque client, l'argument est 1 en cas de victoire et 0 en cas de défaite) // TODO : ajouter égalité

#endif /* UTILS_H */
