#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCKET_NAME "mysocket.s"
#define BUF_SIZE 256
#define OK 0

static int sockfd;

void cleanup_socket(void)
{
    close(sockfd);
    unlink(SOCKET_NAME);
}

void sigint_handler(int signum)
{
    cleanup_socket();
    exit(OK);
}

int main(void)
{
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    struct sockaddr srvr_name;
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCKET_NAME);
    if (bind(sockfd, &srvr_name, strlen(srvr_name.sa_data) + sizeof(srvr_name.sa_family)) < 0)
    {
        perror("Failed to bind socket");
        return EXIT_FAILURE;
    }

    signal(SIGINT, sigint_handler);
    fprintf(stdout, "Server is listening.\nTo stop server press Ctrl + C.\n");

    char buf[BUF_SIZE];
    for (;;)
    {
        int bytes = recv(sockfd, buf, sizeof(buf), 0);
        if (bytes <= 0)
        {
            perror("Failed to recv");
            cleanup_socket();
            return EXIT_FAILURE;
        }

        buf[bytes] = '\0';
        fprintf(stdout, "Server read message: %s\n", buf);
    }

    fprintf(stdout, "Server stopped listening\n");
    cleanup_socket();
    fprintf(stdout, "Socket closed\n");

    return OK;
}
