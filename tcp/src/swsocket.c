/****
*@file swsocket.c 
*@brief 关于socket的接口函数
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
/* 创建socket, af=AF_INET,..., type=SOCK_STREAM, ... , protocol=IPPROTO_TCP,... */
int sw_create_socket( int af, int type, int protocol )
{
	
	return socket( af, type, protocol );
	
}
/* 销毁socket */
void sw_destroy_socket(int socketfd)
{
	if( socketfd > 2 )
		close(socketfd);
	
}
/* 绑定接收地址和端口, sever使用bind()与本机绑定*/
/*ipadress == NULL 则绑定INADDR_ANY*/
int sw_bind_socket(int socketfd, char *ipadress, int port )
{
	
	struct sockaddr_in local_addr;
	bzero(&local_addr,sizeof(local_addr));
	socklen_t addrlen = sizeof(local_addr);
	
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port);
	int  bReuseaddr = 1;
	int ret = setsockopt(socketfd ,SOL_SOCKET ,SO_REUSEADDR,
			      (const int*)&bReuseaddr,sizeof(int));
	//printf("ret = %d\n",ret);
	if( ret < 0 )
		printf("fail to setsockopt\n");
	
	
	if(ipadress == NULL )
	{
		
		local_addr.sin_addr.s_addr = INADDR_ANY;
		return bind(socketfd,(struct sockaddr *)&local_addr,addrlen);
		
	}
	else	
	{
		local_addr.sin_addr.s_addr = inet_addr(ipadress);
		return bind(socketfd,(struct sockaddr *)&local_addr,addrlen);	
	}
}
/* 连接远端 */	
int sw_connect_socket(int socketfd, char *ipadress, int port )
{
	struct sockaddr_in sever_addr;
	bzero(&sever_addr,sizeof(sever_addr));
	socklen_t addrlen = sizeof(sever_addr);
	
	sever_addr.sin_family = AF_INET;
	sever_addr.sin_port = htons(port);
	sever_addr.sin_addr.s_addr = inet_addr(ipadress);
	/* 开始连接 */
	return connect(socketfd,(struct sockaddr *)&sever_addr,addrlen);
	
}

