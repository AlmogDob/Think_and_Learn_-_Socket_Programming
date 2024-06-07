#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        error("Error opening Socket.");
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[2]);

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host.\n");
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr_list, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("connection Faild.");
    }

    while (1) {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("Error on writting.");
        }
        
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("Error on reading.");
        }
        printf("server: %s\n", buffer);

        if (!strncmp("Bye", buffer, 3)) {
            break;
        }
    }

    close(sockfd);

    return 0;
}
