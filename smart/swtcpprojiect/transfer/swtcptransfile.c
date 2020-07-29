/****
*@file swtcpsenfile.c 
*@brief 实现tcp协议传送文件
*       
*@author xiexiang
*@history 
*       2016-04-19 xiexiang created
*
***/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 1024*8
/*发送文件，socketfd = 要发送的通信描述符，filename = 文件名*/
int sw_send_file(const int socketfd, const char *filename)
{
	if( filename == NULL )
	{
		printf("invalid filename\n");
		return -1;
	}
	char sendbuf[BUF_SIZE]; 
	memset(sendbuf, 0, sizeof(sendbuf));
	long long filelen = 0;
	if( access(filename,0) == -1 )  //判断该文件是否存在
	{
		printf("No such file named %s \n",filename);
		if( send(socketfd,&filelen,sizeof(filelen),0) == 0 )
			printf("fail to sen filelen\n");
		return -1;
	}
	
	FILE *filefd = fopen(filename,"r");
	if( filefd == NULL )
	{
		printf("No such file \n");
		return -1;
	}
	//求文件的大小
	struct stat filestat;
	memset(&filestat, 0, sizeof(filestat));
	if( stat(filename,&filestat) == -1 )
	{
		printf("fail to stat\n");
		return 0;		
	}
	filelen = filestat.st_size;
	
	if( send(socketfd,&filelen,sizeof(filelen),0) <0 )
	{
		printf("fail to send filelen\n");
	}
	if( filelen == 0 )
	{
		printf("%s is NULL\n",filename);
		fclose(filefd);
		return -1;
	}
	
	int readnums = 0; //记录每次读取的字节数
	long long sumsent = 0; //记录已经发送的字节
	int sentnums = 0; //每次发送的字节大小
	fseek(filefd, 0, SEEK_SET);
	printf("filelen = %lld\n",filelen);
	while(1)
	{
		sentnums = 0;
		readnums = fread(sendbuf,sizeof(char),
				sizeof(sendbuf),filefd);
		if(readnums == 0 )
		{
			fclose(filefd);
			if(filelen == sumsent)
				break;
			else
				return -1;
		}
		
		//保证每次都把读到的数据全部发送到socket上
		for( int i = 0; i < readnums; i += sentnums )
		{

			sentnums = send(socketfd, sendbuf + i, readnums - i, 0);
			if(sentnums <= 0)
			{
				printf("\n");
				printf("disconnect\n");
				fclose(filefd);
				return -1;	
			}
		}
		sumsent += readnums;	
	       
		//printf("=====================%lld\n",sumsent);
		for(int i = 0; i<12; i++ )
			printf("\b");
		
		printf("\rsend %0.1f%%",((double) sumsent)/filelen*100);
		if(filelen == sumsent)
		{
			printf("\n");
			printf("Finish to send!\n");
			fclose(filefd);
			break;
		}
	}
	return 0;
		
}

/*发送文件，socketfd = 接收的通信描述符，filename = 文件名*/
int sw_recv_file(const int socketfd, const char *filename)
{
	if( filename == NULL )
	{
		printf("invalid filename\n");
		return -1;
	}
	char recvbuf[BUF_SIZE];
	memset(recvbuf, 0, sizeof(recvbuf));

	long long filelen = 0;
	if( recv(socketfd, &filelen, sizeof(filelen),0) <= 0 )
		printf("fail to recv filelen\n");

	if( filelen == 0 )
	{
		printf("recv file is NULL or No such file\n");
		return -1;
	}
	printf("filelen = %lld\n",filelen);

	FILE *filefd = fopen(filename, "w+");
	if( filefd == NULL )
	{
		perror("fail to creat file:");
		return -1;
	}
	int recvnums = 0; //记录每次接收的字节
	int writenums = 0; //记录每次写入的字节
	long long sumwrite = 0; //记录总共已经写了多少字节

	while(1)
	{
		recvnums = recv(socketfd,recvbuf,sizeof(recvbuf),0);
		if( recvnums<= 0 )
		{
			printf("\n");
			printf("disconnect\n");
			fclose(filefd);
			return -1;
		}
		for( int i = 0; i < recvnums; i += writenums )
		{
			writenums = fwrite(recvbuf + i, sizeof(char),
						recvnums - i, filefd);
			if( writenums < 0 )
			{
				printf("write error\n");
				return -1;
			}
		}
		sumwrite += recvnums;

		//printf("=====================%lld\n",sumwrite);
		if(filelen == sumwrite)
		{
			printf("\n");
			printf("Finish to receive!\n");
			fclose(filefd);
			break;
		}
		for( int i = 0; i<13; i++ )
			printf("\b");
		printf("\rrecved %0.1f%%",((double )sumwrite)/filelen*100);
	
	}
	
	return 0;
}
