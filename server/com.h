#ifndef __unp_h
#define __unp_h

#define SERV_PORT 9877 
#define LISTENQ     1024
#define MAXLINE 4096
#include <sys/types.h>
#include <errno.h>

#include<sys/socket.h>  
#include<errno.h>  
#include<netinet/in.h>  
#include<string.h>  
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include <netdb.h>


int atoi(const char *nptr);
ssize_t write(int fd, const void *vptr, size_t n);
ssize_t read(int fd, const void *vptr, size_t n);

pid_t fork(void);
int close(int fd);
void exit(int status);
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);


void str_echo(int sockfd);
void str_cli(FILE *fp, int sockfd);


#endif  /* __unp_h */
