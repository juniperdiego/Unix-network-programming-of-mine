#include "com.h"

void cli(FILE *fp, int sockfd, struct sockaddr * servaddr, socklen_t servlen);

int main(int argc, char **argv)
{
    int                 sockfd;
    struct sockaddr_in  servaddr;

    if (argc != 2)
    {
        printf("usage: tcpcli <IPaddress>");
        exit(0);
    }

    //sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    //connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));     /* do it all */

    exit(0);
}

#if 0
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
        struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen);
#endif

void cli(FILE *fp, int sockfd, struct sockaddr * servaddr, socklen_t servlen)
{
    char    sendline[MAXLINE], recvline[MAXLINE];
    ssize_t n = 0;
    struct sockaddr_in replyaddr;
    int i = 0;
    char * p = NULL;

    while (fgets(sendline, MAXLINE, fp) != NULL) {

        socklen_t len =  servlen;
        sendto(sockfd, sendline, strlen(sendline), 0, servaddr, len);

        n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr*)&replyaddr, &len);

        printf("receive a replay from %s \n", inet_ntoa(replyaddr.sin_addr));
        printf("servlen = %d, len = %d\n", servlen, len);

        i = 0;
        p =  servaddr;
        while(i < len)
        {
            printf("%02x ", (*p) & 0xff);
            i++;
            p++;
        }
        printf("\n");

        i = 0;
        p = &replyaddr;
        while(i < len)
        {
            printf("%02x ", (*p) & 0xff);
            i++;
            p++;
        }
        printf("\n");
        if(len != servlen || 0 != memcmp(&replyaddr, servaddr, len))
        {
            continue;
        }

        recvline[n] = 0;
        fputs(recvline, stdout);
    }   
}
