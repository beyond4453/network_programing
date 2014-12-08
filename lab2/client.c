/**************************************************************************************************/
/* Copyright (C) SA14226214, USTC, 2014-2015                                                      */
/*                                                                                                */
/*  FILE NAME             :  client.c                                                             */
/*  PRINCIPAL AUTHOR      :  GaoZhipeng                                                           */
/*  SUBSYSTEM NAME        :  lab2                                                                 */
/*  MODULE NAME           :  client                                                               */
/*  LANGUAGE              :  C                                                                    */
/*  TARGET ENVIRONMENT    :  ANY                                                                  */
/*  DATE OF FIRST RELEASE :  2014/12/08                                                           */
/*  DESCRIPTION           :  This is a client program                                             */
/**************************************************************************************************/

/*
 *Revision log:
 *
 *Ceated by GaoZhipeng, 2014/12/08
 *     
 */
#include<stdio.h> 			
#include<stdlib.h>			
#include<sys/types.h>		
#include<sys/wait.h>		
#include<string.h>			
#include<pthread.h>
#include<unistd.h>

#include "stream.h"

#define MAXLINE 80

int main()
{
    struct stream* p_stream;
    const char* stream_name = "tcp:127.0.0.1:1234";
    uint8_t dscp = 1;
    
	int actualsend = 0;
	int actualrecv = 0;

    char buf[MAXLINE];
	char recv_buf[MAXLINE] = {0};
        if(stream_open(stream_name,&p_stream,dscp))
        {
            printf("stream open failure!\n");
        }
        else
        {
            printf("stream open sucessed!\n");

            if(stream_connect(p_stream))
            {
                printf("stream connect failure!\n");
            }
            else
            {
                printf("stream connect successed!\n");
				printf("input you message (q to exit): ");
				while(fgets(buf, MAXLINE, stdin) != NULL)
				{
					if(buf[0] == 'q' && strlen(buf) == 2)
					{
						break;
					}
					actualsend = stream_send(p_stream,buf,strlen(buf));
					if(actualsend < 0)
					{
						printf("stream send failure!\n");
					}
					else if(actualsend == 0)
					{
						printf("stream send 0 bytes!\n");
					}
					else
					{
/*此处buf中含有换行符'\n'*/
						printf("stream actual send %d bytes,Data:%s",actualsend,buf);
					}
/*接收来自server的字符串*/ 
					actualrecv = stream_recv(p_stream,recv_buf,sizeof(recv_buf));
					for(;actualrecv <=0;actualrecv = stream_recv(p_stream,recv_buf,sizeof(recv_buf)))
						;
					printf("pstream actual receive %d bytes from the server :%s\n",actualrecv,recv_buf);
					memset(recv_buf,0,strlen(recv_buf));

					printf("\ninput you message (q to exit): ");
				}
            }

        stream_close(p_stream);
        sleep(1);
    }

    return 0;
}
