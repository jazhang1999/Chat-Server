
/******************************************************************************
 * Client side implementation for the TCP server:
 * To compile: gcc TCPClient.c -o client
 * To run: ./client
 *****************************************************************************/
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>

/* Const declarations */
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void func(int sockfd, char *username) 
{ 
    fd_set readfds; 
    char buff[MAX]; 
    int n; 
    while(1) { 
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds); 
        FD_SET(STDIN_FILENO, &readfds);
        int max_sd = sockfd;
        bzero(buff, sizeof(buff)); 

        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0)
        {
            perror("Select error");
            exit(1);
        }
        
        if (FD_ISSET( STDIN_FILENO, &readfds))
        {
            n = strlen(&username); 
            strcpy(buff, &username);
            while ((buff[n++] = getchar()) != '\n'); 
            write(sockfd, buff, sizeof(buff)); 
        }
        if (FD_ISSET( sockfd, &readfds))
        {   
            read(sockfd, buff, sizeof(buff)); 
            printf("%s", buff); 
        }
    } 
} 
  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
    char username[20];

    printf("Username: ");
    scanf("%s", username);
    strcat(username, ": ");

    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    func(sockfd, username); 
  
    // close the socket 
    close(sockfd); 
} 
