
#include "com.h"

void echo(int sockfd);

int
main(int argc, char **argv)
{
    int                 sockfd/*, connfd*/;
//    pid_t               childpid;
    struct sockaddr_in   servaddr;
    

    //sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    echo(sockfd);
    return 0;

#if 0    
    listen(sockfd, LISTENQ);

    for ( ; ; ) { 
        clilen = sizeof(cliaddr);
        connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);

        if ( (childpid = fork()) == 0) {    /* child process */
            close(sockfd);    /* close listening socket */
            str_echo(connfd);   /* process the request */
            exit(0);
        }   
        close(connfd);          /* parent closes connected socket */
    }   
#endif    

}

void echo(int sockfd) 
{
    struct sockaddr_in  cliaddr;
    socklen_t           len;
    ssize_t n;

#define LEN 1024
    char buf[LEN];

#if 0
    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
            struct sockaddr *src_addr, socklen_t *addrlen);
    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
            const struct sockaddr *dest_addr, socklen_t addrlen);
#endif
    for ( ; ; )
    { 
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, LEN, 0, (struct sockaddr *)&cliaddr, &len);
        sendto(sockfd, buf, n, 0, (struct sockaddr *) &cliaddr, len);

    }
}
