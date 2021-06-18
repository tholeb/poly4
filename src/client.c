#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "grille.h"

int main(void)
{
	// créé un socket (PF_INET = IPv4, SOCK_STREAM = TCP)
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	perror("socket");

	if (sock == -1)
	{
		return 1;
	}

	// adresse du serveur (127.0.0.1:8000)
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8000);

	// connecte le client au serveur
	int err = connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	perror("connect");

	// au cas d'échec de connexion, on ferme le socket et on sort.
	if (err)
	{
		close(sock);

		return 1;
	}

	// chaque message du serveur vers le client aura le format suivant :
	// le premier octet indique le type du message
	// le second octet est un numéro de colonne (ou rien du tout selon le type de message)
	uint8_t msg[2];

	// numéro du joueur qui joue actuellement (utile pour savoir quel pion afficher).
	int turn = 0;

	do {
		// affichage de la grille
		afficher_grille();

		// réception du message
		recv(sock, msg, 2, 0);

		switch (msg[0])
		{
			case DEMANDER_COUP:
			{
				int col = demander_coup();

				// les messages du client vers le serveur ne sont toujours
				// constitués que d'un seul octet (le numéro de colonne à jouer)
				send(sock, &col, 1, 0);
			}
			break;
			case INDIQUER_COUP:
			// ajoute le coup joué à la grille
			ajouter_pion(turn ? PION_J1 : PION_J2, msg[1]);

			// passe le tour au joueur qui ne jouait pas sur ce tour
			turn = !turn;
			break;
			case FIN_DE_PARTIE:
			switch (msg[1])
			{
				case 0: // victoire de l'opposant
				printf("Défaite\n");
				break;
				case 1: // victoire du joueur
				printf("Victoire\n");
				break;
			}
			break;
		}
	}
	while (msg[0] != FIN_DE_PARTIE);

	// ferme le socket
	close(sock);

	return 0;
}
