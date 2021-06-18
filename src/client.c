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
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	perror("socket");

	if (sock == -1)
	{
		return 1;
	}

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8000);

	int err = connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	perror("connect");

	if (err)
	{
		close(sock);

		return 1;
	}

	uint8_t msg[2];

	int turn = 0;

	do {
		afficher_grille();

		recv(sock, msg, 2, 0);

		switch (msg[0])
		{
			case DEMANDER_COUP:
			{
				int col = demander_coup();

				send(sock, &col, 1, 0);
			}
			break;
			case INDIQUER_COUP:
			ajouter_pion(turn ? PION_J1 : PION_J2, msg[1]);

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

	close(sock);

	return 0;
}
