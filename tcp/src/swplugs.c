/****
*@file swtcpserver.c 
*@brief 服务器main函数程序
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#include<stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "swsocket.h"
#include "base64.h"
#include "swjson.h"


int main(int argc, char *argv[])
{
	
	struct sockaddr_in clientaddr; //用来保存客户端的ip
	bzero(&clientaddr,sizeof(clientaddr));
	socklen_t addrlen = sizeof(clientaddr);

	int clientfd = 0;
	char clientip[128*1024] = {0};
	
	int listenfd =  sw_create_socket(AF_INET, SOCK_STREAM, 0);
	if( listenfd < 0 )
	{
		printf("fail to creat socket\n");
		return -1;
	}
	printf("listenfd = %d\n",listenfd);
	if( sw_bind_socket(listenfd,"10.10.1.17", 8081) == 0 )
	{
		listen(listenfd,5);
	}
	else
	{
		perror("fail to bind \n");
		sw_destroy_socket(listenfd);
		return -1;
	}
	
	int ret = 0;
	while( (clientfd = accept(listenfd, 
			(struct sockaddr *)&clientaddr,&addrlen)) > 0 )
	{
		//打印客户端的ip
		if( inet_ntop(AF_INET,(void *)&clientaddr.sin_addr,
		    clientip,20) != NULL ) 
		{
			printf("%s is connect\n", clientip);
		}
		while(1) //接收数据
		{
			memset(clientip,0,sizeof(clientip));
			ret = recv(clientfd,clientip,sizeof(clientip),0);
			printf("data ret = %d\n",ret);
		
		
		}
	
	}
	sw_destroy_socket(listenfd);
	return 0;

} 
