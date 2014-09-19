#include "com.h"
int max(int a, int b)
{
    if(a > b)
        return a;
    else
        return b;
}

void str_cli(FILE *fp, int sockfd)
{
    char    sendline[MAXLINE], recvline[MAXLINE];
    fd_set rset;	
    FD_ZERO(&rset);
    int maxfdp;

    while(1)
    {
        memset(sendline, 0, MAXLINE);
        memset(recvline, 0, MAXLINE);

        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp = max(fileno(fp), sockfd) + 1;
        select(maxfdp, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &rset))
        {
            if (read(sockfd, recvline, MAXLINE) == 0)
            {
                printf("str_cli: server terminated prematurely");
                exit(0);
            }
            else
                fputs(recvline, stdout);
        }

        if(FD_ISSET(fileno(fp), &rset))
        {
            if(fgets(sendline, MAXLINE, fp) != NULL)
                write(sockfd, sendline, strlen(sendline));
            else
                return;
        }

    }
}
