/****
*@file swtcpclient.c 
*@brief 客户端main函数程序
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include "swsocket.h"
#include "swstransfer.h"
#include "swstring.h"

#define BUF_SIZE 128 //存放文件名的缓存大小

int main(int argc, char *argv[])
{
	if( argc < 5 )
	{
		perror("argument to few\n");
		return -1;	
	}
	int serverfd = sw_create_socket(AF_INET, SOCK_STREAM, 0);
	if( serverfd < 0 )
	{
		printf("fail to creat socket\n");
		return -1;
	}
	
	if( sw_connect_socket(serverfd, argv[1], 9999) < 0 )
	{
		printf("fail to connect\n");
		sw_destroy_socket(serverfd);
		return -1;
	}
	
	// client 192.168.1.192 -g /home/marty/hello(要下载的文件位置和文件名)  /mnt/recv/hello  下载   
	// client 192.168.1.192 -p /home/marty/(要上传的文件位置和文件名)   /mnt/recv/hello 上传
	
	char remotebuf[BUF_SIZE]; //要发送给服务器上的文件路径和文件名
	memset( remotebuf, 0, sizeof(remotebuf));
	if( strlen(argv[3]) < sizeof(remotebuf) )
		memcpy(remotebuf, argv[3], strlen(argv[3]));
	else
	{
		printf("dir to long \n");
		sw_destroy_socket(serverfd);
		return -1;
	}
	
	char localbuf[BUF_SIZE]; //本地上的文件路径和文件名
	memset( localbuf, 0, sizeof(localbuf));
	if( strlen(argv[4]) < sizeof(remotebuf) )
		memcpy(localbuf, argv[4], strlen(argv[4]));
	else
	{
		printf("localdir to long \n");
		sw_destroy_socket(serverfd);
		return -1;
	}
	
	if( (strcmp(argv[2],"-g") == 0) || (strcmp(argv[2],"g") == 0) ) //下载
	{
		//=======================================
		if(!sw_isspecified_filename(localbuf))
		{
			char tempbuf[BUF_SIZE];
			memset(tempbuf, 0, sizeof(tempbuf));
			if( sw_get_filename(remotebuf,sizeof(tempbuf),
				tempbuf)<0 )
			{
				printf("faile to get local filename\n");
				sw_destroy_socket(serverfd);
				return -1;
			}
			if( sw_strlcat(tempbuf,sizeof(localbuf), 
					localbuf)<0 )
			{
				printf("fail to strlcat filenae\n");
				sw_destroy_socket(serverfd);
				return -1;
			}
		
		}
		//sleep(1);
		printf("localbuf = %s\n",localbuf);
		
		//===================交互界面=======================
		if( access(localbuf,0) == 0)  //判断该文件是否存在
		{
			char buf[4];
			memset(buf, 0, sizeof(buf));
			while(1)
			{
				printf("File already exists,overwrite? y/n: ");
				if( fgets(buf, 3, stdin) == NULL )
					continue;
				if( strncmp(buf, "y", 1) == 0 )
					break;
				else if( strncmp(buf, "n", 1) == 0 )
				{
					sw_destroy_socket(serverfd);
					return -1;
				}
					
				else
				{
					printf("intput error\n");
					while( getchar() != '\n');
				}
					
			}
			
		}
		
		//==========================================
		//向服务器发送功能请求
		if( send(serverfd,"g",2,0) <= 0 )
		{
			printf("fail to send command\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		
		//向服务器发送要下载的文件路径及文件名
		if( send(serverfd,remotebuf,strlen(remotebuf),0)<= 0 )
		{
			printf("fail to send remotebuf\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		//==========================================
		//接收服务器回应
		if( recv(serverfd,remotebuf,3,0)<=0 )
		{
			printf("fail to recv server Respond\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		
		//接收文件
		if( sw_recv_file(serverfd,localbuf) < 0 )
		{
			//printf("fail to recv\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
	
		printf("finish to download\n");
		
	}
	
	else if( (strcmp(argv[2],"-p") == 0) || (strcmp(argv[2],"p") == 0) ) //上传
	{
		if( !sw_isspecified_filename(remotebuf) )
		{
			char tempbuf[BUF_SIZE];
			memset(tempbuf, 0, sizeof(tempbuf));
			if( sw_get_filename(localbuf,sizeof(tempbuf),
				tempbuf)<0 )
			{
				printf("faile to get local filename\n");
				sw_destroy_socket(serverfd);
				return -1;
			}
			if( sw_strlcat(tempbuf,sizeof(remotebuf),
				remotebuf)<0 )
			{
				printf("fail to strlcat filenae\n");
				sw_destroy_socket(serverfd);
				return -1;
			}
			
		}
		printf("remotebuf = %s\n",remotebuf);
		
		//向服务器发送功能请求
		if( send(serverfd,"p",2,0) <=0 )
		{
			printf("fail to send command\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		
		//向服务器发送要上传的文件路径以及文件名
		if( send(serverfd,remotebuf,strlen(remotebuf),0)<= 0 )
		{
			printf("fail to send remotebuf\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		//接收服务的回应
		if( recv(serverfd,remotebuf,3,0)<=0 )
		{
			printf("fail to recv server Respond\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
	
		//发送文件
		if( sw_send_file(serverfd, localbuf)!=0 )
		{
			printf("fail to send\n");
			sw_destroy_socket(serverfd);
			return -1;
		}
		// 等待服务器已经接收完毕回应
		if( recv(serverfd,localbuf,4,0) <= 0 )
		{
			printf("disconnect\n");
			sw_destroy_socket(serverfd);
			return -1;
		}	
		else
		printf("succes upload\n");
		
	} 
	else
		printf("command error\n");
	
	sw_destroy_socket(serverfd); //销毁通信socket
	return 0;


}
