#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

// Maximum number of limit
#define MAX 65536

// Server port decided
#define PORT 3380
#define SA struct sockaddr

void processClient(int connfd)
{
    // processing file transfer
    char buff[MAX];
    for (;;)
    {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        if ((strncmp(buff, "quit", 4)) == 0)
        {
            printf("\n....Disconnected from client side....\n");
            break;
        }
        printf("\n\nRecieved Command: %s", buff);
    }
}

int main()
{
    system("echo 1 > totalClients.txt");

    int _socketFD, connfd, len;
    struct sockaddr_in servaddr, cli;
    _socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFD < 0)
    {
        printf("....Error in socket creation....\n");
        exit(1);
    }
    else
    {
        printf("....Server socket is created....\n");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Trying to bind server connection
    if ((bind(_socketFD, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("....Error in socket binding....\n");
        exit(1);
    }
    else
    {
        printf("....Server socket is binded....\n");
    }

    if ((listen(_socketFD, 10)) != 0)
    {
        printf("....Error in listening from client side....\n");
        exit(0);
    }
    else
    {
        printf("....Server is listening....\n");
    }

    len = sizeof(cli);

    int _counter = 0;

    for (;;)
    {
        connfd = accept(_socketFD, (SA *)&cli, &len);

        if (connfd < 0)
        {
            exit(0);
        }
        else
        {
            _counter++;
            printf("\n....server accept the client %d...\n", _counter);
        }
        printf("Client %d connected...\n", _counter);
        int pid = fork();
        if (pid == 0)
        {
            close(_socketFD);
            processClient(connfd);
        }
    }
    close(connfd);
    return 0;
}