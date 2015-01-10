/*************************************************************************
  > File Name: server.c
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
struct SendPack
{
	PackInfo head;
	char buf[BUFFER_SIZE];
} data;

long getlength( FILE *fp )
{
	long cur_pos;
	long len;

	//ȡ�õ�ǰ�ļ����Ķ�ȡλ��
	cur_pos = ftell( fp );
	//���ļ����Ķ�ȡλ����Ϊ�ļ�ĩβ
	fseek( fp, 0, SEEK_END );
	//��ȡ�ļ�ĩβ�Ķ�ȡλ��,���ļ���С
	len = ftell( fp );
	//���ļ����Ķ�ȡλ�û�ԭΪԭ�ȵ�ֵ
	fseek( fp, cur_pos, SEEK_SET );
	return len;
}


int main()
{
	/* ����id */
	int send_id = 0;

	/* ����id */
	int receive_id = 0;
	

	long file_len;
	/* ����UDP�׽ӿ� */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	/* ����socket */
	int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_socket_fd == -1)
	{
		perror("Create Socket Failed:");
		exit(1);
	}

	/* ���׽ӿ� */
	if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))))
	{
		perror("Server Bind Failed:");
		exit(1);
	}

	/* ���ݴ��� */
	while(1)
	{  
		/* ����һ����ַ�����ڲ���ͻ��˵�ַ */
		struct sockaddr_in client_addr;
		socklen_t client_addr_length = sizeof(client_addr);

		/* �������� */
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		if(recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length) == -1)
		{
			perror("Receive Data Failed:");
			exit(1);
		}

		/* ��buffer�п�����file_name */
		char file_name[FILE_NAME_MAX_SIZE+1];
		bzero(file_name,FILE_NAME_MAX_SIZE+1);
		strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer));
		printf("%s\n", file_name);

		/* ���ļ� */
		FILE *fp = fopen(file_name, "r");

		if(NULL == fp)
		{
			printf("File:%s Not Found.\n", file_name);
		}
		else
		{
			file_len = getlength(fp);
			printf("File Opened Succeed,File Size: %ld\n", file_len);
			
			if(sendto(server_socket_fd, &file_len, sizeof(long), 0, (struct sockaddr*)&client_addr, client_addr_length) < 0)
			{
				perror("Send File Failed:");
				break;
			}

			int len = 0;
			/* ÿ��ȡһ�����ݣ��㽫�䷢���ͻ��� */
			while(1)
			{
				PackInfo pack_info;

				if(receive_id == send_id)
				{
					++send_id;
					if((len = fread(data.buf, sizeof(char), BUFFER_SIZE, fp)) > 0)
					{
						data.head.id = send_id; /* ����id�Ž���ͷ,���ڱ��˳�� */
						data.head.buf_size = len; /* ��¼���ݳ��� */
						if(sendto(server_socket_fd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, client_addr_length) < 0)
						{
							perror("Send File Failed:");
							break;
						}
						/* ����ȷ����Ϣ */
						recvfrom(server_socket_fd, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&client_addr, &client_addr_length);
						receive_id = pack_info.id; 
					}
					else
					{
						break;
					}
				}
				else
				{
					/* ������յ�id�ͷ��͵�id����ͬ,���·��� */
					if(sendto(server_socket_fd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&client_addr, client_addr_length) < 0)
					{
						perror("Send File Failed:");
						break;
					}
					/* ����ȷ����Ϣ */
					recvfrom(server_socket_fd, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&client_addr, &client_addr_length);
					receive_id = pack_info.id; 
				}
			}
			/* �ر��ļ� */
			fclose(fp);
			printf("File:%s Transfer Successful!\n", file_name);
		}
	}
	close(server_socket_fd);
	return 0;
} 
