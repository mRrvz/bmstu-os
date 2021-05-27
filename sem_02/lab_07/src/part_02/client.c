#include "socket.h"

int main(void)
{
	const int master_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (master_sd == -1) {
		perror("Failed to create socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(SOCKET_PORT)
	};

	if (connect(master_sd, (struct sockaddr *) &addr, sizeof addr) < 0) {
		perror("Failed to connect");
		return EXIT_FAILURE;
	}

	while (1)
    {
		char msg[BUF_SIZE];
		snprintf(msg, BUF_SIZE, "My pid is %d", getpid());
		if (sendto(master_sd, msg, strlen(msg), 0, (struct sockaddr *) &addr, sizeof addr) < 0)
        {
			perror("Failed to sendto");
			return EXIT_FAILURE;
		}

		sleep(1);
	}
}