#include "com.h"

#define MAXN    16384       /* max # bytes to request from server */

int tcp_connect(const char *host, const char *serv)
{
    int             sockfd, n;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
    {
        printf("tcp_connect error for %s, %s: %s\n",
                host, serv, gai_strerror(n));
        exit(0);
    }
    ressave = res;

    do {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0)
            continue;   /* ignore this one */

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
            break;      /* success */

        close(sockfd);  /* ignore this one */
    } while ( (res = res->ai_next) != NULL);

    if (res == NULL)    /* errno set from final connect() */
    {
        printf("tcp_connect error for %s, %s", host, serv);
        exit(0);
    }
    printf("tcp_connect suc for %s, %s\n", host, serv);

    freeaddrinfo(ressave);

    return(sockfd);
}
/* end tcp_connect */


/* Read "n" bytes from a descriptor. */
ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char    *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;      /* and call read() again */
            else
                return(-1);
        } else if (nread == 0)
            break;              /* EOF */

        nleft -= nread;
        ptr   += nread;
    }           
    return(n - nleft);      /* return >= 0 */
}           
/* end readn */

int main(int argc, char **argv)
{
    int     i, j, fd, nchildren, nloops, nbytes;
    pid_t   pid;
    ssize_t n;
    char    request[MAXLINE], reply[MAXN];

    if (argc != 6)
    {
        printf("usage: client <hostname or IPaddr> <port> <#children> "
                "<#loops/child> <#bytes/request>\n");
        exit(0);
    }

    nchildren = atoi(argv[3]);
    nloops = atoi(argv[4]);
    nbytes = atoi(argv[5]);
    snprintf(request, sizeof(request), "%d\n", nbytes); /* newline at end */

    for (i = 0; i < nchildren; i++) {
        if ( (pid = fork()) == 0) {     /* child */
            for (j = 0; j < nloops; j++) {
                fd = tcp_connect(argv[1], argv[2]);

                write(fd, request, strlen(request));
                printf("request %d bytes\n", nbytes);

                if ( (n = readn(fd, reply, nbytes)) != nbytes)
                {
                    printf("server returned %d bytes", n);
                    exit(0);
                }

                close(fd);      /* TIME_WAIT on client, not server */
            }
            printf("child %d done\n", i);
            exit(0);
        }
        /* parent loops around to fork() again */
    }

    while (wait(NULL) > 0)  /* now parent waits for all children */
        ;
    if (errno != ECHILD)
        printf("wait error");

    exit(0);
}

#if 0
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
#endif
