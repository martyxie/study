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
	
	
	int listenfd =  sw_create_socket(AF_INET, SOCK_STREAM, 0);
	if( listenfd < 0 )
	{
		printf("fail to creat socket\n");
		return -1;
	}
	printf("listenfd = %d\n",listenfd);
	if( sw_connect_socket(listenfd,"10.10.1.17", 8081) == 0 )
	{
		printf("connect success\n");
	}
	else
	{
		perror("fail to bind \n");
		sw_destroy_socket(listenfd);
		return -1;
	}
	
	int ret = 0;
	int count = 0;
	char buf[125*1024] = {0};
	while(1)
	{
		count ++;
		memset(buf,count,sizeof(buf));
		ret = send(listenfd,buf,125*1024 -1,0);
		sleep(1);
		printf("send data ret = %d\n",ret);
	}
	sw_destroy_socket(listenfd);
	return 0;

} 
