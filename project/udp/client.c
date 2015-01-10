/*************************************************************************
  > File Name: client.c
  > Author: SongLee
 ************************************************************************/
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

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

/* ��ͷ */
typedef struct 
{
	int id;
	int buf_size;
}PackInfo;

/* ���հ� */
struct RecvPack
{
	PackInfo head;
	char buf[BUFFER_SIZE];
} data;


int main()
{
	int id = 1;

	/* ����˵�ַ */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(SERVER_PORT);
	socklen_t server_addr_length = sizeof(server_addr);
	



	
	/* ����socket */
	int client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_socket_fd < 0)
	{
		perror("Create Socket Failed:");
		exit(1);
	}
/*
	if (fcntl(client_socket_fd, F_SETFL, fcntl(client_socket_fd, F_GETFD, 0)|O_NONBLOCK) == -1)
	{
		return -1;
	}
*/
	/* �����ļ����������� */
	char file_name[FILE_NAME_MAX_SIZE+1];
	bzero(file_name, FILE_NAME_MAX_SIZE+1);
	printf("Please input file name on server: ");
	scanf("%s", file_name);

	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));

	/* �����ļ��� */
	if(sendto(client_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&server_addr,server_addr_length) < 0)
	{
		perror("Send File Name Failed:");
		exit(1);
	}

	/* ���ļ���׼��д�� */
	bzero(file_name, FILE_NAME_MAX_SIZE+1);
	printf("Please input file name you want to save as: ");
	scanf("%s", file_name);
	FILE *fp = fopen(file_name, "w");
	if(NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write\n", file_name); 
		exit(1);
	}

	long file_len;
	if(recvfrom(client_socket_fd, &file_len, sizeof(long), 0, (struct sockaddr*)&server_addr,&server_addr_length) == -1)
	{
		perror("Receive Data Failed:");
		exit(1);
	}
	printf("File Size: %ld\n", file_len);

	/* �ӷ������������ݣ���д���ļ� */
	int len = 0;
	while(1)
	{
		PackInfo pack_info;

		if((len = recvfrom(client_socket_fd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&server_addr,&server_addr_length)) > 0)
		{
			//id����һ�������������ݰ���head.id
			if(data.head.id == id)
			{
				pack_info.id = data.head.id;
				pack_info.buf_size = data.head.buf_size;
				++id;
				/* �������ݰ�ȷ����Ϣ */
				if(sendto(client_socket_fd, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&server_addr, server_addr_length) < 0)
				{
					printf("Send confirm information failed!");
				}
				/* д���ļ� */
				if(fwrite(data.buf, sizeof(char), data.head.buf_size, fp) < data.head.buf_size)
				{
					printf("File:\t%s Write Failed\n", file_name);
					break;
				}
			}
			else if(data.head.id < id) /* ������ط��İ� */
			{
				pack_info.id = data.head.id;
				pack_info.buf_size = data.head.buf_size;
				/* �ط����ݰ�ȷ����Ϣ */
				if(sendto(client_socket_fd, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&server_addr, server_addr_length) < 0)
				{
					printf("Send confirm information failed!");
				}
			}
			else
			{

			}
		}
		else
		{
			break;
		}
	}

	printf("Receive File:\t%s From Server IP Successful!\n", file_name);
	fclose(fp);
	close(client_socket_fd);
	return 0;
}
