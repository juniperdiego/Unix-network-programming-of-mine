#include "com.h"

void str_echo(int sockfd)
{
    ssize_t     n;  
    char        buf[MAXLINE];

again:
    
    printf("wait for read data\n");
    while ( (n = read(sockfd, buf, MAXLINE)) > 0)
        write(sockfd, buf, n); 

    if (n < 0 && errno == EINTR)
    {
	   printf("read func is interrupt by a signal\n");
	    goto again;
    }
}
