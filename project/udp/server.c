/*************************************************************************
  > File Name: server.c
  > Author: SongLee
 ************************************************************************/
#include"wrap.h"

/* ���հ� */
struct SendPack
{
	PackInfo head;
	char buf[BUFFER_SIZE];
} data;



int main()
{
	/* ����id */
	int send_id = 0;

	/* ����id */
	int receive_id = 0;
	
	/* �ļ���С */
	long file_len;

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	

	/* ����socket */
	int server_socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	/* ���׽ӿ� */
	Bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));


	/* ���ݴ��� */
	while(1)
	{  
		/* ����һ����ַ�����ڲ���ͻ��˵�ַ */
		struct sockaddr_in client_addr;
		socklen_t client_addr_length = sizeof(client_addr);

		/* �����ļ��� */
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		RecvFrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length);

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
