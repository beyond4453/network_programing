/*************************************************************************
  > File Name: client.c
  > Author: SongLee
 ************************************************************************/
#include"wrap.h"

/* 接收包 */
struct RecvPack
{
	PackInfo head;
	char buf[BUFFER_SIZE];
} data;


int main()
{
	int id = 1;

	/* 服务端地址 */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(SERVER_PORT);
	socklen_t server_addr_length = sizeof(server_addr);
	
	/* 创建socket */
	int client_socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	/* 输入文件名到缓冲区 */

	char file_name[FILE_NAME_MAX_SIZE+1];
	Get_FileName_On_Server(file_name);

	char buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));

	

	/* 发送文件名 */
	SendTo(client_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&server_addr,server_addr_length) ;
	
	
	/*获取文件大小*/
	long file_len;
	RecvFrom(client_socket_fd, &file_len, sizeof(long), 0, (struct sockaddr*)&server_addr,&server_addr_length);
	printf("File Size: %ld\n", file_len);

	/* 打开文件，准备写入 */
	bzero(file_name, FILE_NAME_MAX_SIZE+1);
	printf("Please input file name you want to save as: ");
	scanf("%s", file_name);
	FILE *fp = fopen(file_name, "w");
	if(NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write\n", file_name); 
		exit(1);
	}


	/* 从服务器接收数据，并写入文件 */
	int len = 0;
	long fw_size = 0;
	long fw_len = 0;
	
	dbtime_startTest ("Connect & Recv");
	PackInfo pack_info;

	while(fw_size < file_len)
	{
		
		
		if((len = recvfrom(client_socket_fd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&server_addr,&server_addr_length)) > 0 )
		{
			//id是下一个期望接收数据包的head.id
			if(data.head.id == id)
			{
				pack_info.id = data.head.id;
				pack_info.buf_size = data.head.buf_size;
				++id;
				/* 发送数据包确认信息 */
				if(sendto(client_socket_fd, (char*)&pack_info, sizeof(pack_info), 0, (struct sockaddr*)&server_addr, server_addr_length) < 0)
				{
					printf("Send confirm information failed!");
				}
				/* 写入文件 */
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
			else if(data.head.id < id) /* 如果是重发的包 */
			{
				pack_info.id = data.head.id;
				pack_info.buf_size = data.head.buf_size;
				/* 重发数据包确认信息 */
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
