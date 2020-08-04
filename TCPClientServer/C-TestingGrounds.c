
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

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Driver function
int main()
{
    char buffer[MAX];     
    char answer[MAX];
    sprintf(buffer, "Username: %s", "Nick");
    sprintf(buffer, "%s is here", buffer);
    printf("%s\n", buffer);
    strcpy(answer, buffer);
    printf("%s\n", answer);
    return 0;
}
