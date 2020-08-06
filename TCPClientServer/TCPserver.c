
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/types.h>  
#include <sys/time.h> 
#include <time.h>

#define MAX 80
#define PORT 8080 
#define SA struct sockaddr 
  
// Driver function 
int main() 
{ 
    int master_socket, new_socket;
    int client_socket[5] = {0}, max_clients = 5; 
    char client_name[5][MAX];
    int is_verified = 0;
    struct sockaddr_in servaddr; 
    fd_set readfds; 
    char buffer[MAX];
    char usr[MAX];
    char pswd[MAX];
    char msg[MAX];

    struct userpwd{
        char *username;
        char *password;
        int available;
    } allUsers[] = {{"Nick", "dongGua123", 1}, {"Bridget", "dongGua123", 1}, 
                    {"Dad", "dongGua123", 1}, {"Mom", "dongGua123", 1}, {"Percy", "dongGua123", 1}};
    
    // socket create and verification - server creating a server socket
    master_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (master_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } else
        printf("Socket successfully created..\n"); 

    /* Assign server address info after cleaning out */ 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(master_socket, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    } else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(master_socket, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } else
        printf("Server listening..\n"); 

    int len = sizeof(servaddr); 

    while(1) {   
        //clear the socket set and add master socket
        FD_ZERO(&readfds);   
        FD_SET(master_socket, &readfds);   
        int max_sd = master_socket;   
             
        //add child sockets to set  
        for (int i = 0 ; i < max_clients ; i++) {   
            //socket descriptor  
            int sd = client_socket[i];    
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd, &readfds);   
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        struct timeval timeout = {1, 0};
        int activity = select(max_sd + 1 , &readfds, NULL , NULL , &timeout);    
        if ((activity < 0) && (errno!=EINTR))   
            printf("Select error");   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            new_socket = accept(master_socket,(struct sockaddr *)&servaddr, (socklen_t*)&len);
            if (new_socket < 0) {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            //inform user of socket number - used in send and receive commands  
            printf("New connection , socket fd is %d , ip is : %s , port : %d, asking for verification now \n"
            ,new_socket , inet_ntoa(servaddr.sin_addr) , ntohs(servaddr.sin_port));   
            
            // Get username
            bzero(buffer, MAX);
            strcpy(buffer, "<Server> Enter Username:");
            write(new_socket, buffer, strlen(buffer));
            bzero(buffer, MAX);  
            read(new_socket, buffer, sizeof(buffer));
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';
            strcpy(usr, buffer); 

            // Get password
            bzero(buffer, MAX);
            strcpy(buffer, "<Server> Enter Password:");
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';
            write(new_socket, buffer, strlen(buffer));
            bzero(buffer, MAX);  
            read(new_socket, buffer, sizeof(buffer));
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';
            strcpy(pswd, buffer); 

            for (int i = 0; i < 2; i++) {
               if ((strcmp(allUsers[i].username, usr) == 0) && (strcmp(allUsers[i].password, pswd) == 0)) {
                    bzero(buffer, MAX);
                    if (allUsers[i].available) {
                        strcpy(buffer, "Verification succeeded, welcome to the chat");
                        write(new_socket, buffer, strlen(buffer));
                        is_verified = 1;    
                        allUsers[i].available = 0;
                    } else {
                        strcpy(buffer, "Sorry, this account is already in use");
                        write(new_socket, buffer, strlen(buffer));
                    } 
               }
            }
             
            if (is_verified) {
                //add new socket to array of sockets  
                for (int i = 0; i < max_clients; i++) {
                    //if position is empty  
                    if(client_socket[i] == 0) {
                        client_socket[i] = new_socket;   
                        bzero(client_name[i], MAX);
                        if (usr[strlen(usr) - 1] == '\n')
                            usr[strlen(usr) - 1] = '\0';
                        strcpy(client_name[i], usr);
                        printf("Adding to list of sockets as %d\n" , i);   
                        printf("Adding to list of users as %s\n", client_name[i]);
                        break;   
                    }   
                }   
            } else {
                bzero(buffer, MAX);
                strcpy(buffer, "Credentials either incorrect or already in use");
                if (buffer[strlen(buffer) - 1] == '\n')
                    buffer[strlen(buffer) - 1] = '\0';
                write(new_socket, buffer, strlen(buffer));
                close(new_socket);
                printf("Invalid credentials given, connection refused\n");
            }
            bzero(usr, MAX);
            bzero(pswd, MAX);
            is_verified = 0;
        }   
             
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_clients; i++) {
            int sd = client_socket[i];        
            if (FD_ISSET( sd, &readfds)) {
                bzero(buffer, MAX);
                int valread = read(sd , buffer, sizeof(buffer));
                /* If there is nothing from the client, they exited. If there is,
                   send it to all other active users */
                if (valread == 0) {
                    //Somebody disconnected, get details and print  
                    getpeername(sd, (struct sockaddr*)&servaddr, (socklen_t*)&len);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(servaddr.sin_addr) , ntohs(servaddr.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close(sd);   
                    client_socket[i] = 0;   
                } else {
                    bzero(msg, MAX);                     
                    // copy server message in the buffer 
                    // and send that buffer to client 
                    for (int j = 0; j < max_clients; j++) {
                        if (j != i && client_socket[j] != 0) {
                            // Get Timestamp
                            time_t now = time(NULL);
                            struct tm *now_tm = localtime(&now);
                            int hour = now_tm->tm_hour;
                            int min = now_tm->tm_min;

                            sprintf(msg, "<%s at %d:%d>: %s", client_name[i], hour, min, buffer);
                            if (msg[strlen(msg) - 1] == '\n')
                                msg[strlen(msg) - 1] = '\0';
                            write(client_socket[j], msg, strlen(msg));
                            bzero(msg, MAX);
                        }
                    }
                    bzero(buffer, MAX);     
                } 
                
            }   
        }   
    }   
    return 0;
} 
