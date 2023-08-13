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
#define MAX 65536

// PORT for MIRROR
#define PORT 3381

#define SA struct sockaddr

void processClient(int connfd)
{
    // processing file transfer
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
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

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("....Error in socket binding....\n");
        exit(1);
    }
    else
    {
        printf("....Server socket is binded....\n");
    }

    if ((listen(sockfd, 10)) != 0)
    {
        printf("....Error in listening from client side....\n");
        exit(0);
    }
    else
    {
        printf("....Mirror is listening....\n");
    }

    len = sizeof(cli);
    int cnt = 0;

    for (;;)
    {
        connfd = accept(sockfd, (SA *)&cli, &len);

        if (connfd < 0)
        {
            exit(0);
        }
        else
        {
            cnt++;
            printf("\n....Mirror server accept the client %d...\n", cnt);
        }
        printf("Client %d connected...\n\n", cnt);
        int pid = fork();
        if (pid == 0)
        {
            close(sockfd);
            processClient(connfd);
        }
    }
    close(connfd);
    return 0;
}