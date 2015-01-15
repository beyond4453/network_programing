#include "wrap.h"

/*获取文件大小*/
long getlength( FILE *fp )
{
	long cur_pos;
	long len;

	//取得当前文件流的读取位置
	cur_pos = ftell( fp );
	//将文件流的读取位置设为文件末尾
	fseek( fp, 0, SEEK_END );
	//获取文件末尾的读取位置,即文件大小
	len = ftell( fp );
	//将文件流的读取位置还原为原先的值
	fseek( fp, cur_pos, SEEK_SET );
	return len;
}


void perr_exit(const char *s)
{ 
	perror(s); 
	exit(1);
}

/* 创建socket */
int Socket(int domain, int type, int protocol)
{
	int socket_fd = socket(domain, type, protocol);
	if(socket_fd == -1)
	{
		perr_exit("Create Socket Failed:");
	}
	return socket_fd;
}


/* bind */
void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{ 
	if (bind(fd, sa, salen) < 0) 
	{
		perr_exit("Bind Failed:");
	}
}

/* recvfrom */
ssize_t RecvFrom(int sockfd,void* buf,int len,unsigned int flags, struct sockaddr *from, socklen_t *fromlen)
{
	if(recvfrom(sockfd, buf, len, flags, from, fromlen) == -1)
	{
		perr_exit("Receive Data Failed:");
	}
}

/* sendto */
int SendTo (int sockfd , const void* msg, int len, unsigned int flags, const struct sockaddr* to, int tolen )
{
	if(sendto(sockfd, msg, len, flags, to, tolen) < 0)
	{
		perr_exit("Send File Name Failed:");
	}
}

/* get filename on server */
void Get_FileName_On_Server(char file_name[])
{
	bzero(file_name, FILE_NAME_MAX_SIZE+1);
	printf("Please input file name on server: ");
	scanf("%s", file_name);
}


