#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

// MAX LIMIT
#define MAX 65536

// 1st Port (SERVER)
#define PORT1 3380

// 2nd Port (MIRROR)
#define PORT2 3381

// Socket Address
#define SA struct sockaddr

int main()
{
    int _socketFD, connFD, n;
    struct sockaddr_in servaddr, cli;
    char buff[MAX];

    _socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if (_socketFD < 0)
    {
        printf("....Error in socket creation....\n");
        exit(1);
    }
    else
    {
        printf("....Client socket is created....\n");
    }

    bzero(&servaddr, sizeof(servaddr));
    bzero(buff, sizeof(buff));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int counterfd = open("totalClients.txt", O_RDWR);
    char buff1[10];

    read(counterfd, buff1, 1);
    int b = buff1[0] - 48;
    // int b = 0;
    printf("Server: %d | %d\n", b, buff1[0]);

    // Logic for load balancing
    if (b <= 5)
    {
        servaddr.sin_port = htons(PORT1);
    }
    else if (b > 5 && b <= 10)
    {
        servaddr.sin_port = htons(PORT2);
    }
    else
    {
        int m = b % 2;
        if (m == 0)
        {
            servaddr.sin_port = htons(PORT2);
        }
        else
        {
            servaddr.sin_port = htons(PORT1);
        }
    }

    // Trying to establish connections
    if (connect(_socketFD, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("....Error in connecting with server....\n");
        exit(1);
    }
    else
    {
        // updating the number of active files in totalClients.txt

        printf("....Connected to the server....\n");
        buff1[0] = b + 49;
        lseek(counterfd, 0, SEEK_SET);
        write(counterfd, buff1, 1);
    }

    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("\nEnter the command: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
        {
            write(_socketFD, buff, sizeof(buff));
        }

        if ((strncmp(buff, "quit", 4)) == 0)
        {
            close(_socketFD);
            printf("....Disconnected from Server side....\n");
            break;
        }
        bzero(buff, sizeof(buff));
        read(_socketFD, buff, sizeof(buff));
        printf("\nOutput of given Command recieved from server:\n %s", buff);
        bzero(buff, sizeof(buff));
    }
    return 0;
}