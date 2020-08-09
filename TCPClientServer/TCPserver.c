
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

#define MAX 2048
#define PORT 8080 
#define SA struct sockaddr 
#define USERS 5  

// Variable declaration. Made them global for ease of access
int master_socket, new_socket;
int client_socket[5] = {0}; 

fd_set readfds; 
char client_name[10][MAX];
char buffer[MAX];
char usr[MAX];
char pswd[MAX];
char msg[MAX];

struct sockaddr_in servaddr; 
struct userpwd{
    char *username;
    char *password;
} allUsers[] = {{"Nick", "dongGua123"}, {"Bridget", "dongGua123"}, {"Dad", "dongGua123"}, 
                {"Mom", "dongGua123"}, {"Percy", "dongGua123"}}; 
    

// Formatter for all I/O
int formatter(char io_to_format[]) {
    if (io_to_format[strlen(io_to_format) - 1] == '\n')
        io_to_format[strlen(io_to_format) - 1] = '\0';
    return 0;
}

// Prepare the server in order to recieve and handle clients
int set_up_server() {
    // Initialize the master socket 
    master_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if (master_socket == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } else
        printf("Socket successfully created..\n"); 

    // Assign server address info after cleaning out 
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
  
    // Now server is ready to listen (will accept 5 users max)
    if ((listen(master_socket, USERS)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } else
        printf("Server listening..\n"); 
    
    return sizeof(servaddr);
}

int doVerification() {
    
    int is_verified = 0;

    // Get username
    bzero(buffer, MAX);
    strcpy(buffer, "Username:");
    write(new_socket, buffer, strlen(buffer) + 1);
    bzero(buffer, MAX);  
    read(new_socket, buffer, sizeof(buffer));
    formatter(buffer);
    strcpy(usr, buffer); 

    // Get password
    bzero(buffer, MAX);
    strcpy(buffer, "Password:");
    formatter(buffer); 
    write(new_socket, buffer, strlen(buffer) + 1);
    bzero(buffer, MAX);  
    read(new_socket, buffer, sizeof(buffer));
    formatter(buffer);
    strcpy(pswd, buffer); 
   
    /* Check the register of all username/password combinations to see if one matches 
       the given credentials */
    for (int i = 0; i < USERS; i++) {
        if ((strcmp(allUsers[i].username, usr) == 0) && (strcmp(allUsers[i].password, pswd) == 0)) {
            bzero(buffer, MAX);
            strcpy(buffer, "Verification succeeded, welcome to the chat");
            write(new_socket, buffer, strlen(buffer) + 1);
            is_verified = 1;
        }
    }
   
    return is_verified;
}

// Driver function 
int main() {    
    
    int len = set_up_server();
    while(1)   
    {   
        //clear the socket set and add master socket
        FD_ZERO(&readfds);   
        FD_SET(master_socket, &readfds);   
        int max_sd = master_socket;   
             
        //add child sockets to set  
        for (int i = 0 ; i < USERS; i++)   
        {   
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
            printf("select error");   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            // Accept incoming connection 
            new_socket = accept(master_socket,(struct sockaddr *)&servaddr, (socklen_t*)&len);
            if (new_socket < 0) {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
            
            /* Call function to facilliate user signin, as well as check if 
               the given credentials are valid */ 
            if (doVerification()) { 
                //add new socket to the array of sockets, 
                for (int i = 0; i < USERS; i++) {   
                    if(client_socket[i] == 0) {   
                        client_socket[i] = new_socket;   
                        bzero(client_name[i], MAX);
                        formatter(usr);
                        strcpy(client_name[i], usr);
                        printf("Adding to list of sockets as %d\n" , i);   
                        printf("Adding to list of users as %s\n", client_name[i]);
                        break;   
                    }   
                }   
    
                // Clear out these buffers 
                bzero(usr, sizeof(usr)); 
                bzero(pswd, sizeof(pswd));

            } else {
                close(new_socket);
                printf("Invalid credentials given, connection refused\n");
            }
        }   
             
        //else its some IO operation on some other socket 
        for (int i = 0; i < USERS; i++)   
        {   
            int sd = client_socket[i];        
            if (FD_ISSET( sd, &readfds))   
            {   
                bzero(buffer, MAX);
                int valread = read(sd , buffer, sizeof(buffer));
                if (valread == 0)
                {   
                    //Somebody disconnected, get details and print  
                    getpeername(sd, (struct sockaddr*)&servaddr, (socklen_t*)&len);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(servaddr.sin_addr) , ntohs(servaddr.sin_port));   
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close(sd);   
                    client_socket[i] = 0;   
                    
                }   
                //Echo back the message that came in  
                else 
                {   
                    bzero(msg, MAX);                     
                    // copy server message in the buffer 
                    // and send that buffer to client 
                    for (int j = 0; j < USERS; j++)
                    {
                        time_t now = time(NULL);
                        struct tm *now_tm = localtime(&now);
                        int hour = now_tm->tm_hour;
                        int min = now_tm->tm_min;

                        if (j != i && client_socket[j] != 0)
                        {
                            sprintf(msg, "<%s at %02d:%02d>: %s", client_name[i], hour, min, buffer);
                            formatter(msg);
                            write(client_socket[j], msg, strlen(msg) + 1);
                            bzero(msg, MAX);
                        } else if (j == i) {
                            sprintf(msg, "<You at %d:%d>: %s", hour, min, buffer);
                            formatter(msg);
                            write(client_socket[j], msg, strlen(msg) + 1);
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
