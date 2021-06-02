#include "socket.h"

#define MAX_CLIENTS_COUNT 10

static int master_sd;
static int clients[MAX_CLIENTS_COUNT];

int cleanup()
{
	close(master_sd);
	exit(EXIT_FAILURE);
}

void sigint_handler(int signum)
{
    cleanup();
    exit(OK);
}

void handle_connection(void)
{
	const int sd = accept(master_sd, NULL, NULL);
	if (sd == -1) {
		cleanup();
	}

	for (int i = 0; i < MAX_CLIENTS_COUNT; ++i)
    {
		if (!clients[i])
        {
			clients[i] = sd;
			fprintf(stdout, "New connection.\n");
			return;
		}
	}

	fprintf(stderr, "Reached MAX_CLIENTS_COUNT (%d)\n", MAX_CLIENTS_COUNT);
    cleanup();
}

void handle_client(int i)
{
	char msg[BUF_SIZE];
	const ssize_t bytes = recv(clients[i], &msg, BUF_SIZE, 0);

	if (!bytes)
    {
		close(clients[i]);
		clients[i] = 0;
		return;
	}

	msg[bytes] = '\0';
    fprintf(stdout, "Get message from client: %s\n", msg);
}

int main(void)
{
	if ((master_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		perror("Failed to create socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(SOCKET_PORT)
	};

	if (bind(master_sd, (struct sockaddr *) &addr, sizeof addr) < 0)
    {
		cleanup();
	}

	if (listen(master_sd, MAX_CLIENTS_COUNT) < 0)
    {
		cleanup();
	}

    signal(SIGINT, sigint_handler);
    fprintf(stdout, "Server is listening.\nTo stop server press Ctrl + C.\n");

	while (1)
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(master_sd, &readfds);

		int max_sd = master_sd;

		for (int i = 0; i < MAX_CLIENTS_COUNT; ++i)
        {
			if (clients[i] > 0)
            {
				FD_SET(clients[i], &readfds);
			}

			if (clients[i] > max_sd)
            {
				max_sd = clients[i];
			}
		}

		if (pselect(max_sd + 1, &readfds, NULL, NULL, NULL, NULL) < 0)
        {
			cleanup();
            perror("Failed to select");
		}

		if (FD_ISSET(master_sd, &readfds))
        {
			handle_connection();
		}

		for (int i = 0; i < MAX_CLIENTS_COUNT; ++i)
        {
			if (clients[i] && FD_ISSET(clients[i], &readfds))
            {
				handle_client(i);
			}
		}
	}
}