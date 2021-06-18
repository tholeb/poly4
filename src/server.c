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
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	perror("socket");

	if (sock == -1)
	{
		return 1;
	}

	int reuse = 1;

	int err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

	perror("setsockopt");

	if (err)
	{
		close(sock);

		return 1;
	}

	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8000);

	err = bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	perror("bind");

	if (err)
	{
		close(sock);

		return 1;
	}

	err = listen(sock, 0);

	perror("listen");

	if (err)
	{
		return 1;
	}

	int clients[2];

	for (int i = 0; i < 2; i++)
	{
		int client = accept(sock, NULL, NULL);

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

	int turn = rand() % 2;

	for (;;)
	{
		uint8_t msg[2] = {DEMANDER_COUP};

		uint8_t col;

		do {
			send(clients[turn], msg, 2, 0);

			recv(clients[turn], &col, 1, 0);
		}
		while (col > 6 || grille[col][0] != 0);

		ajouter_pion(turn ? PION_J1 : PION_J2, col);

		msg[0] = INDIQUER_COUP;
		msg[1] = col;

		for (int i = 0; i < 2; i++)
		{
			send(clients[i], msg, 2, 0);
		}

		if (puissance4(turn ? PION_J1 : PION_J2))
		{
			msg[0] = FIN_DE_PARTIE;

			for (int i = 0; i < 2; i++)
			{
				msg[1] = i == turn;

				send(clients[i], msg, 2, 0);
			}

			break;
		}

		turn = !turn;
	}

	for (int i = 0; i < 2; i++)
	{
		close(clients[i]);
	}

	close(sock);

	return 0;
}
