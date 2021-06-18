#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "grille.h"
#include "utils.h"

extern char grille[NB_COL][NB_ROW];

int main(void)
{
	// créé un socket (PF_INET = IPv4, SOCK_STREAM = TCP)
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	perror("socket");

	if (sock == -1)
	{
		return 1;
	}

	int reuse = 1;

	// permet de réutiliser le port immédiatement après l'arrêt du processus (utile pour faire des tests)
	int err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	perror("setsockopt");

	if (err)
	{
		close(sock);

		return 1;
	}

	// adresses acceptées par le serveur
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8000);

	// on affecte le serveur aux adresses ci-dessus
	err = bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	perror("bind");

	if (err)
	{
		close(sock);

		return 1;
	}

	// on fait écouter le serveur sur le port affecté précédemment
	err = listen(sock, 0);

	perror("listen");

	if (err)
	{
		return 1;
	}

	int clients[2];

	// le jeu se ne jouant qu'à deux joueurs, il suffit d'accepter
	// deux connexions entrantes puis de lancer la partie
	for (int i = 0; i < 2; i++)
	{
		// accepte la connexion (bloque jusqu'à ce qu'une connexion soit effectuée)
		int client = accept(sock, NULL, NULL);

		// en cas d'échec, on ferme le premier client ainsi que le serveur et on sort
		if (client == -1)
		{
			perror("accept");

			if (i > 0)
			{
				close(clients[0]);
			}

			close(sock);

			return 0;
		}

		clients[i] = client;
	}

	srand(time(NULL));

	// choisi aléatoirement le premier joueur
	int turn = rand() % 2;

	for (;;)
	{
		uint8_t msg[2] = {DEMANDER_COUP};

		uint8_t col;

		// envoit une requête DEMANDER_COUP au joueur tant que la colonne demandée est invalide
		do {
			send(clients[turn], msg, 2, 0);

			recv(clients[turn], &col, 1, 0);
		}
		while (col > 6 || grille[col][0] != 0);

		// ajoute le pion du joueur dans la colonne désirée
		ajouter_pion(turn ? PION_J1 : PION_J2, col);

		msg[0] = INDIQUER_COUP;
		msg[1] = col;

		// valide le coup pour le joueur actuel et informe l'autre joueur du coup
		for (int i = 0; i < 2; i++)
		{
			send(clients[i], msg, 2, 0);
		}

		// si le coup résulte en un puissance 4, le serveur arrête la partie.
		if (puissance4(turn ? PION_J1 : PION_J2))
		{
			msg[0] = FIN_DE_PARTIE;

			for (int i = 0; i < 2; i++)
			{
				// pour chaque joueur, vaut 1 si le joueur en question a gagné, et 0 si il a perdu.
				msg[1] = i == turn;

				send(clients[i], msg, 2, 0);
			}

			break;
		}

		// passe le tour à l'autre joueur
		turn = !turn;
	}

	// ferme le socket de chaque client
	for (int i = 0; i < 2; i++)
	{
		close(clients[i]);
	}

	// ferme le socket du serveur
	close(sock);

	return 0;
}
