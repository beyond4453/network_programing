/**************************************************************************************************/
/* Copyright (C) SA14226214, USTC, 2014-2015                                                      */
/*                                                                                                */
/*  FILE NAME             :  server.c                                                             */
/*  PRINCIPAL AUTHOR      :  GaoZhipeng                                                           */
/*  SUBSYSTEM NAME        :  lab2                                                                 */
/*  MODULE NAME           :  server                                                               */
/*  LANGUAGE              :  C                                                                    */
/*  TARGET ENVIRONMENT    :  ANY                                                                  */
/*  DATE OF FIRST RELEASE :  2014/12/08                                                           */
/*  DESCRIPTION           :  This is a server program                                             */
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

#include "stream.h"

int main()
{
    struct pstream* p_pstream; 
    struct stream* p_stream;
    const char* pstream_name = "ptcp:1234";
    char buffer[50] = {0};
    uint8_t dscp = 1;
    int actualrecv = 0;

    if(pstream_open(pstream_name,&p_pstream,dscp))
    {
        printf("pstream open failure!\n");
    }
    else
    {
       
		printf("Accepting connections ...\n");
		while(pstream_accept(p_pstream, &p_stream))
                ;
        printf("\nget one steam connect success!\n"); 

        printf("p_pstream name:%s\n", pstream_get_name(p_pstream));
        printf("p_pstream bound_port:%d\n", pstream_get_bound_port(p_pstream));
	    printf("p_stream name:%s\n", stream_get_name(p_stream));

        printf("<remote_ip,remote_port> : <%d,%d>\n", stream_get_remote_ip(p_stream),stream_get_remote_port(p_stream));
        printf("<local_ip,local_port> : <%d,%d>\n", stream_get_local_ip(p_stream),stream_get_local_port(p_stream));
		while(1)
		{
			actualrecv = stream_recv(p_stream,buffer,sizeof(buffer));
			if(actualrecv > 0) 
			{
				printf("pstream actual receive %d bytes from the client,buffer:%s\n\n",actualrecv,buffer);
				memset(buffer,0,strlen(buffer));

				stream_send (p_stream, "hi", sizeof("hi"));
			}
			else 
			{
				;
			}
		}
        stream_close(p_stream);
        pstream_close(p_pstream);
    }   

    return 0;
}
