/****
*@file swstransfer.h 
*@brief 传送文件函数接口
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#ifndef __SWSTRANSFER_H__
#define __SWSTRANSFER_H__

/****
* @brief 把filename文件发送到sockefd中
*
*@param[in]socketfd 要发送的socketfd 
*@param[in]filename 要发送的文件名
*
*@return 0，成功; -1, 失败
***/
int sw_send_file(const int socketfd, const char *filename);

/****
* @brief 读取socketfd的内容，并保存到filename文件中
*
*@param[in]socketfd 要从读取的socketfd 
*@param[in]filename 要保存的文件名
*
*@return 0，成功; -1, 失败
***/
int sw_recv_file(const int socketfd, const char *filename);

#endif
