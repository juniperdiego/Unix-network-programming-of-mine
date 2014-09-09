#include "com.h"

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2)
    {
        printf("usage: tcpcli <IPaddress>");
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    str_cli(stdin, sockfd);     /* do it all */

    exit(0);
}
