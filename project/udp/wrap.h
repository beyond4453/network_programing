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
#include <fcntl.h>
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


#endif 
