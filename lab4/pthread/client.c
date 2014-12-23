/**************************************************************************************************/
/* Copyright (C) SA14226214, USTC, 2014-2015                                                      */
/*                                                                                                */
/*  FILE NAME             :  client.c                                                             */
/*  PRINCIPAL AUTHOR      :  GaoZhipeng                                                           */
/*  SUBSYSTEM NAME        :  lab1                                                                 */
/*  MODULE NAME           :  client                                                               */
/*  LANGUAGE              :  C                                                                    */
/*  TARGET ENVIRONMENT    :  ANY                                                                  */
/*  DATE OF FIRST RELEASE :  2014/12/22                                                           */
/*  DESCRIPTION           :  This is a client program                                             */
/**************************************************************************************************/

/*
 *Revision log:
 *
 *Ceated by GaoZhipeng, 2014/12/22
 *     
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char *argv[]) 
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
	int index = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));


	//从标准输入设备中读取字符串写入buf
	while(1) {
		write(sockfd, "hello", sizeof("hello"));
		n = read(sockfd, buf, MAXLINE);
		if(n == 0)
        {
            printf("the connect side has been closed. please run it again\n");
            continue;
        }
		write(STDOUT_FILENO, "Response from server : ", sizeof("Response from server : "));
		write(STDOUT_FILENO, buf, n);
		index++;
		printf(" %d \n", index);
	}
	close(sockfd);
	return 0;
}
