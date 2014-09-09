#include "com.h"

void str_echo(int sockfd)
{
    ssize_t     n;  
    char        buf[MAXLINE];

again:
    while ( (n = read(sockfd, buf, MAXLINE)) > 0)
        write(sockfd, buf, n); 

    if (n < 0 && errno == EINTR)
        goto again;
}
