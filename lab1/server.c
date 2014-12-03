/**************************************************************************************************/
/* Copyright (C) SA14226214, USTC, 2014-2015                                                      */
/*                                                                                                */
/*  FILE NAME             :  server.c                                                             */
/*  PRINCIPAL AUTHOR      :  GaoZhipeng                                                           */
/*  SUBSYSTEM NAME        :  lab1                                                                 */
/*  MODULE NAME           :  server                                                               */
/*  LANGUAGE              :  C                                                                    */
/*  TARGET ENVIRONMENT    :  ANY                                                                  */
/*  DATE OF FIRST RELEASE :  2014/12/01                                                           */
/*  DESCRIPTION           :  This is a server program                                             */
/**************************************************************************************************/

/*
 *Revision log:
 *
 *Ceated by GaoZhipeng, 2014/12/01
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

int main(void) 
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char recv_buf[MAXLINE];
	char *send_buf;
	char str[INET_ADDRSTRLEN];
	int i, n;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd,20);
	printf("Accepting connections ...\n");
	
	while(1) 
	{
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

		n = read(connfd, recv_buf, MAXLINE);
		printf("received from %s at PORT %d ",inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
		printf("%.*s",n,recv_buf);
		printf("\n");

		send_buf = "hi";
		write(connfd, send_buf, sizeof(send_buf));
		close(connfd);
	}
}
