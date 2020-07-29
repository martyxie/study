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
#include "swstransfer.h"

#define FILENAMEBUF_SIZE 128
#define CLIENTIP_SIZE 64

typedef struct share_info
{
	int clientfd; //socket通信描述符
	//线程间的互斥锁
	pthread_mutex_t *pshare_lock;

}share_info_t;

//服务线程函数
void server_thread(void *arg)
{
	share_info_t *share_info = (share_info_t *)arg;
	
	int clientfd = share_info->clientfd;
	printf("fd = %d\n",clientfd);
	//解锁
	pthread_mutex_unlock(share_info->pshare_lock);
	int ret = 0;
	char commandbuf[4]; //保存传过来的命令，要上传还是下载
	memset(commandbuf, 0, sizeof(commandbuf));
	//保存传过来的要保存文件路径或及文件名
	char filenamebuf[FILENAMEBUF_SIZE]; 
	memset(filenamebuf, 0, sizeof(filenamebuf));
	
	//接收客户端的请求命令
	ret = recv(clientfd, commandbuf, 2, 0); 
	if( ret <= 0 )
	{
		printf("fail to recv command\n");
		sw_destroy_socket(clientfd);
		pthread_exit(NULL);
	}
		
	commandbuf[ret] = '\0';
	printf("command = %c\n",commandbuf[0]); 
	
	//接收客户端的请求存放的文件路径和文件名
	ret = recv(clientfd, filenamebuf, FILENAMEBUF_SIZE - 1, 0); 
	if( ret <= 0 )
	{
		printf("fail to recv filename\n");
		sw_destroy_socket(clientfd);
		pthread_exit(NULL);
	}	
	filenamebuf[ret] = '\0';
	//回应客户端可以开始传送文件了
	if( send(clientfd, "ok",3,0) <= 0 )
	{
		printf("fail to Respond\n");
		sw_destroy_socket(clientfd);
		pthread_exit(NULL);
	}
	switch(commandbuf[0])
	{
		case 'g': printf("download %s\n",filenamebuf);
				  // 发送文件
			 if(sw_send_file(clientfd, filenamebuf) < 0)
			 {
				 printf("somthing wrong\n");
				 break;
			 }
			
			 printf("succes send\n");
			 break;
				  
		case 'p': printf("upload %s\n",filenamebuf);
			//接收文件
			if(sw_recv_file(clientfd, filenamebuf) < 0)
			{
				 printf("somthing wrong\n");
				 break;	
			}
			 //回应客户端，接收完毕，然后跳出
			if(send(clientfd,"ok",3,0) <= 0)
				 printf("fail to tell client ok\n");  
			 printf("succes recv\n");
			 break;
		default : printf("nothing to do\n");
				  
							  
	}
	
	
	printf("destroy client socket\n");
	//关闭通信socket
	sw_destroy_socket(clientfd);
	//退出线程
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	
	struct sockaddr_in clientaddr; //用来保存客户端的ip
	bzero(&clientaddr,sizeof(clientaddr));
	socklen_t addrlen = sizeof(clientaddr);
	
	int listenfd =  sw_create_socket(AF_INET, SOCK_STREAM, 0);
	if( listenfd < 0 )
	{
		printf("fail to creat socket\n");
		return -1;
	}
	printf("listenfd = %d\n",listenfd);
	if( sw_bind_socket(listenfd, NULL, 9999) == 0 )
	{
		listen(listenfd,5);
	}
	else
	{
		perror("fail to bind \n");
		sw_destroy_socket(listenfd);
		return -1;
	}
	
	pthread_mutex_t main_lock;
	pthread_mutex_init(&main_lock, NULL);
	
	share_info_t share_info;
	share_info.clientfd = -1; //新连接上来的客户端
	share_info.pshare_lock = &main_lock;
	//上锁
	pthread_mutex_lock(&main_lock);
	
	char clientip[CLIENTIP_SIZE];
	memset(clientip, 0,sizeof(clientip));
	pthread_t server_thread_tid = 0;
	while( (share_info.clientfd = accept(listenfd, 
			(struct sockaddr *)&clientaddr,&addrlen)) > 0 )
	{
		pthread_mutex_unlock(&main_lock);
		//打印客户端的ip
		if( inet_ntop(AF_INET,(void *)&clientaddr.sin_addr,
		    clientip,20) != NULL ) 
		{
			printf("%s is connect\n", clientip);
		}
		printf("begin \n");
		pthread_mutex_lock(&main_lock);
		//创建线程
		if( pthread_create( &server_thread_tid, NULL,
                          (void *)server_thread,(void *)&share_info) < 0 )
		{
			printf("fail to creat thread\n");
			//pthread_mutex_unlock(&main_lock);
			sw_destroy_socket(share_info.clientfd);
			continue;  
		}
		 
		pthread_mutex_lock(&main_lock);
		//分离线程 线程结束后自动回收资源
		if( pthread_detach(server_thread_tid) < 0 )
			printf("fail to detach thread\n");
	
	}
	pthread_mutex_destroy(&main_lock);
	sw_destroy_socket(listenfd);
	return 0;

} 
