
/****
*@file swstring.h 
*@brief 字符串操作接口
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/

#ifndef __SWSTING_H__
#define __SWSTING_H__
#include <stdbool.h>
/****
* @brief 在src中获取文件名，并保存到dest中
*
*@param[in]src 文件名的指针（包括路径） 
*@param[in]size dest 缓存区的大小
*@param[out]dest 要存放文件名的缓存区 
*@return 0，成功; -1, 失败
***/

int sw_get_filename(const char *src, int size, char *dest);
/****
* @brief 判断src中是否包含了文件名，默认/结尾的为目录
*
*@param[in]src 文件名的指针（包括路径） 
*
*@return true，有文件名; false, 没有文件名
***/

bool sw_isspecified_filename(const char *src);

/****
* @brief 在src中的字符串接到dest后面
*
*@param[in]src 要拼接的字符串
*@param[in]size dest 缓存区的大小
*@param[out]dest 被接的字符串指针 
*@return 0，成功; -1, 失败
***/
int sw_strlcat(const char *src,int size, char *dest);


#endif
