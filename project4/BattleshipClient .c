#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define sockport 80
int main()
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(sockport);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int connectionStat = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    if (connectionStat == -1)
    {
        printf("Error connecting to remote socket.\n");
    }

    char servResponse[256];
    recv(sockfd, &servResponse, sizeof(servResponse), 0);

    printf("The server sent the data: %s\n", servResponse);

    close(sockfd);

    return 0;

}
