/****
*@file swsocket.h 
*@brief 套接字相关的函数接口
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/


#ifndef __SWSOCKET_H__
#define __SWSOCKET_H__
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

/****
* @brief 创建一个套接字
*
*@param[in]af]套接字的类型 AF_INET ==iPv4
*@param[in]type类型 SOCK_STREAM/SOCK_DGRAM
*@param[in]protocol  一般为0 
*@return 一个文件描述符，成功; -1, 失败
***/
int sw_create_socket( int af, int type, int protocol );

/****
* @brief 绑定一个套接字
*
*@socketfd[in] 需要绑定的套接字描述符
*@ipadress[in] 要绑定的ip地址，比如“192.168.1.109”
*@port[in] 要绑定的端口号
*@return 0，成功; -1, 失败
***/
int sw_bind_socket( int socketfd, char *ipadress, int port );
/****
* @brief 连接到一个套接字
*
*@socketfd[in] 需要连接的套接字描述符
*@ipadress[in] 要连接的ip地址，比如“192.168.1.109”
*@port[in] 要连接的端口号
*@return 0，成功; -1, 失败
***/
int sw_connect_socket( int socketfd, char *ipadress, int port );
/****
* @brief 销毁一个套接字
*
*@socketfd[in] 要销毁的套接字描述符
*
***/
void sw_destroy_socket(int socketfd);

#endif
