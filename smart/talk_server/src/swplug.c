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
#include <signal.h>

#define FILENAMEBUF_SIZE 128
#define CLIENTIP_SIZE 64

#define AUDIO_FILE "wifi_passwd_error.wav"
#define SERVER_IP "10.10.18.252"
#define SERVER_PORT 8081

#define RTP_HDR_LEN 12

static int m_interrupt = 0;

typedef struct share_info
{
	int clientfd; //socket通信描述符
	//线程间的互斥锁
	pthread_mutex_t *pshare_lock;

}share_info_t;
typedef struct rtp_head
{
    uint8_t ver;     //    :2;
    uint8_t padding; //    :1;
    uint8_t extension;//   :1;
    uint8_t csrc_count; //  :4;
    uint8_t marker; //      :1;
    uint8_t play_load; //   :7;
    uint16_t seq_num;//   :16;
    uint32_t timestamp; //   :32;
    uint32_t ssrc; //        :32;
}rtp_headr_t;


static int rtp_header_paser(rtp_headr_t *h,char *data,int len)
{
    if(h == NULL || data == NULL || len < RTP_HDR_LEN)
        return 0;
    //解析rtp header
    h->ver = (data[0]&0xc0) >> 6; //1,2
    h->padding = (data[0]&0x20) >> 5; //3
    h->extension = (data[0]&0x10) >> 4; //4
    h->csrc_count = data[0]&0x0f; //4-8
    h->marker = (data[1]&0x80) >> 7;// 9
    h->play_load = (data[1]&0x7f); //10-16
    h->seq_num = (data[2] << 8) + data[3]; //17
    h->timestamp = (data[4] <<24) +(data[5] << 16) + (data[6] << 8) +data[7];
    h->ssrc = (data[8]<<24) + (data[9] << 16) + (data[10] << 8) +data[11];
    return 0;
}
static int make_rtp_pack(uint8_t *data,int len)
{
    //2+head+playload
    len = len +12; //实际长度+RTP头
    data[0] = (len &0xff00) >> 8;
    data[1] = (len &0x00ff);
    sw_make_rtp_header(0x123,data+2,0,1);
    rtp_headr_t head ={0};
    rtp_header_paser(&head,data+2,12);
    printf("head = %d,ssrc = 0x%x\n",head.ver,head.ssrc);
    return 0;
}

static int save_data(uint8_t *data,int len)
{
    static FILE *fp = NULL;
    if(fp == NULL)
    {
        fp = fopen("./rtp_g711a.g711a","w");
    }
    if(fp)
    {
        fwrite(data,len,1,fp);
    }
    return 0;
}

//服务线程函数
void server_thread(void *arg)
{
	share_info_t *share_info = (share_info_t *)arg;
	
	int clientfd = share_info->clientfd;
	printf("fd = %d\n",clientfd);
	//解锁
	pthread_mutex_unlock(share_info->pshare_lock);
	int ret = 0;
	uint8_t commandbuf[1024]; //保存传过来的命令，要上传还是下载
	memset(commandbuf, 0, sizeof(commandbuf));
	//保存传过来的要保存文件路径或及文件名
	char filenamebuf[FILENAMEBUF_SIZE]; 
	memset(filenamebuf, 0, sizeof(filenamebuf));
	
	//接收客户端的请求命令
    memset(commandbuf,0,sizeof(commandbuf));
    printf("recv data........beigin to send data......................\n");
    ret = recv(clientfd, commandbuf, sizeof(commandbuf), 0); 
    if( ret <= 0 )
    {
        printf("fail to recv command\n");
        sw_destroy_socket(clientfd);
        pthread_exit(NULL);
    }
    FILE *fp = fopen(AUDIO_FILE,"r");
    if(fp == NULL)
    {
        printf("faile to open %s\n",AUDIO_FILE);
        goto END;
    }
    //发送数据
    int rsize = 0;
	while(m_interrupt == 0)
	{
        memset(commandbuf,0,sizeof(commandbuf));
        rsize = fread(commandbuf+14,1,360,fp);
        if(rsize <= 0)
        {
            printf("read data error \n");
            break;
        }
        //make rtp head
        make_rtp_pack(commandbuf,rsize);
        rsize += 14;
        //save_data(commandbuf,rsize);
        //send
        rsize = send(clientfd,commandbuf,rsize,0);
        if(rsize <= 0)
        {
            printf("send data finish or erro hanpen\n");
            break;
        }
        usleep(40000);
	}
    fclose(fp);
END:
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
	if( sw_bind_socket(listenfd,SERVER_IP, SERVER_PORT) == 0 )
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
