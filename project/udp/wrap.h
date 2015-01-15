#ifndef WRAP_H 
#define WRAP_H

#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>
#include "dbtime.h"


#define SERVER_PORT 8000
#define BUFFER_SIZE 50240
#define FILE_NAME_MAX_SIZE 512

/*°üÍ·*/
typedef struct 
{
	int id;
	int buf_size;
}PackInfo;

long getlength( FILE *fp );
int Socket(int domain, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Get_FileName_On_Server(char buffer[]);
ssize_t RecvFrom(int sockfd,void* buf, int len, unsigned int flags, struct sockaddr *from, socklen_t *fromlen);
int SendTo (int sockfd , const void* msg, int len, unsigned int flags, const struct sockaddr* to, int tolen );


#endif 
