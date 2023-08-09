#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX_CLIENTS 6
#define BUFFER_SIZE 1024

void processClient(int clientSocket)
{
    char buffer[BUFFER_SIZE] = {0};
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) <= 0)
            break;
        if (strcmp(buffer, "exit\n") == 0)
        {
            break;
        }
        printf("Mirror Client: %s", buffer);
        send(clientSocket, buffer, strlen(buffer), 0);
    }
    close(clientSocket);
}

int main()
{
    int server_fd, client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    int clientCount = 0;
    while (clientCount < MAX_CLIENTS)
    {
        if ((client_sockets[clientCount] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        int pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            close(server_fd);
            processClient(client_sockets[clientCount]);
            exit(0);
        }
        else
        {
            clientCount++;
        }
    }

    close(server_fd);
    return 0;
}
