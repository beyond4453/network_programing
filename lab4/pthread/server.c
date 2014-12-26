/**************************************************************************************************/
/* Copyright (C) SA14226214, USTC, 2014-2015                                                      */
/*                                                                                                */
/*  FILE NAME             :  server.c                                                             */
/*  PRINCIPAL AUTHOR      :  GaoZhipeng                                                           */
/*  SUBSYSTEM NAME        :  lab1                                                                 */
/*  MODULE NAME           :  server                                                               */
/*  LANGUAGE              :  C                                                                    */
/*  TARGET ENVIRONMENT    :  ANY                                                                  */
/*  DATE OF FIRST RELEASE :  2014/12/22                                                           */
/*  DESCRIPTION           :  This is a server program                                             */
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
#include<pthread.h>

#define MAXLINE 80
#define SERV_PORT 8000

int listenfd, connfd;
struct sockaddr_in servaddr, cliaddr;
char str[INET_ADDRSTRLEN];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


void Client_Thread()
{
	char recv_buf[MAXLINE];
	char *send_buf = "hi";
	int i, n;
	int index = 0;
//为线程加锁，使其不可重入
	pthread_mutex_lock(&mutex);
	while(1)
	{
//读入的内容存入recv_buf，返回读到的字节数
		n = read(connfd, recv_buf, MAXLINE);
		if(n == 0)
		{
			printf("the client has been closed , please restart again\n");
			break;
		}
		printf("received from %s at PORT %d ",(char *)inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
//print the recv_buf on the terminal
		write(STDOUT_FILENO, recv_buf, n);
		index++;
		printf(" %d\n", index);
		write(connfd, send_buf, strlen(send_buf));
	}
	close(connfd);
	pthread_mutex_unlock(&mutex);
	exit(0);
}


int main(void) 
{
	socklen_t cliaddr_len;
	pthread_t ntid;

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
		pthread_create(&ntid, NULL, (void *)Client_Thread, NULL);
		ntid++;
	}
}
