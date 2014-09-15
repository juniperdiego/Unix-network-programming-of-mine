
#include "com.h"

int
main(int argc, char **argv)
{
    int                 listenfd, connfd;
    pid_t               childpid;
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;
    struct sigaction action, old_action;

    //action.sa_handler = sig_chld;
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);

    action.sa_flags = 0;
    action.sa_flags |= SA_RESTART;
    sigaction(SIGCHLD, &action, NULL);

    listenfd = socket(AF_INET, SOCK_STREAM, 0); 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    //signal(SIGCHLD, sig_chld); 

    for ( ; ; ) { 
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
	printf("accept suc\n");
	
	if(connfd < 0)
	{
		if(errno == EINTR)
		{
			printf("interruptted by signal\n");
			fflush(stdout);
			continue;
		}
		else
		{
			printf("error");
			abort();
		}
	}

        if ( (childpid = fork()) == 0) {    /* child process */
            close(listenfd);    /* close listening socket */
            str_echo(connfd);   /* process the request */
            exit(0);
        }   
        close(connfd);          /* parent closes connected socket */
    }   
}
