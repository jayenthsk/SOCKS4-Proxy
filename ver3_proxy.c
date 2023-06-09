#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validation.h"
#define PORT 8080

int main()
{

    int server_fd, new_socket, valread, opt = 1;
    struct sockaddr_in server;
    int addrlen = sizeof(server);
    char buffer[1024] = {0};
    char reply[] = "90";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Error Binding socket");
        exit(1);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Error in listening");
        exit(1);
    }
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server, (socklen_t *)&addrlen)) < 0)
        {
            perror("Error in accept");
            exit(1);
        }

        // Validation - param buffer return 0/1 and set buffer
        // pass request in this format eg : 4 1 8080 ip userid 0(just zero)
        // sending status code and lintening for next connection

        valread = read(new_socket, buffer, sizeof(buffer));
        ip = server.sin_addr.s_addr;
        int validate_flag = validate(buffer);

        if (!validate_flag)
        {
            send(new_socket, buffer, sizeof(buffer), 0);
            close(new_socket);
            continue;
        }

        // Tunneling

        int client_fd;
        struct sockaddr_in client;

        if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("\n Error in secondary dovket creation");
            close(new_socket);
            break;
        }

        client.sin_family = AF_INET;
        client.sin_port = htons(atoi(port));
        client.sin_addr.s_addr = inet_addr(dest_ip);

        int sock;
        socklen_t len = sizeof(client);

        if((sock = connect(client_fd, (struct sockaddr*)&client, len)) < 0){
            perror("\n Error in connect");
            // Try to send success msg here.
            close(new_socket);
            break;
        }
        send(new_socket, buffer, sizeof(buffer), 0);
        
            bzero(buffer,1024);
            valread = read(new_socket, buffer, sizeof(buffer));


            // Forwarding to dest host
            send(client_fd, buffer, strlen(buffer), 0);
            printf("request sent");
            bzero(buffer, 1024);
            char response[10000] = {0};
            valread = read(client_fd, response, sizeof(response));

            send(new_socket, response, sizeof(response), 0);
            
    }
    // If keyboard interrupt shutdown socket

    shutdown(server_fd, SHUT_RDWR);
    return 0;
}