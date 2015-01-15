/*************************************************************************
  > File Name: client.c
  > Author: SongLee
 ************************************************************************/
#include"wrap.h"

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
	int client_socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	/* �����ļ����������� */

	char file_name[FILE_NAME_MAX_SIZE+1];
	Get_FileName_On_Server(file_name);

	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));

	

	/* �����ļ��� */
	SendTo(client_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&server_addr,server_addr_length) ;
	
	
	/*��ȡ�ļ���С*/
	long file_len;
	RecvFrom(client_socket_fd, &file_len, sizeof(long), 0, (struct sockaddr*)&server_addr,&server_addr_length);
	printf("File Size: %ld\n", file_len);

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


	/* �ӷ������������ݣ���д���ļ� */
	int len = 0;
	long fw_size = 0;
	long fw_len = 0;
	
	dbtime_startTest ("Connect & Recv");
	PackInfo pack_info;

	while(fw_size < file_len)
	{
		
		
		if((len = recvfrom(client_socket_fd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&server_addr,&server_addr_length)) > 0 )
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
				if((fw_len = (fwrite(data.buf, sizeof(char), data.head.buf_size, fp))) < data.head.buf_size)
				{
					printf("File:\t%s Write Failed\n", file_name);
					break;
				}

				else {
					fw_size += fw_len;
					printf("%ld\n", fw_size);
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
	
	dbtime_endAndShow ();

	printf("Receive File:\t%s From Server IP Successful!\n", file_name);
	fclose(fp);
	close(client_socket_fd);
	return 0;
}
