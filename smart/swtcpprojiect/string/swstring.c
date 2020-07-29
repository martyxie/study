/****
*@file swstring.c 
*@brief 字符处理函数接口
*@author xiexiang
*@history 
*       2016-04-18 xiexiang created
*
***/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int sw_get_filename(const char *src, int size, char *dest)
{
	if( src == NULL || dest == NULL )
		return -1;
	
	char tempbuf[256];
	memset(tempbuf,0,sizeof(tempbuf));
	int length = strlen(src);
	if( length > sizeof(tempbuf)-1 )
		return -1;
	
	memcpy(tempbuf,src,length);
	//printf("tempbuf = %s\n",tempbuf);
	char *p = &tempbuf[length];
	while( *p != '/' )
		--p;
	printf("filename  = %s\n",++p);
	//printf("len = %d\n",strlen(p));
	if( (size - strlen(p)) < strlen(p) )
	{
		printf("size not ennoght\n");
		return -1;
	}
	memcpy(dest, p, strlen(p)+1);
	return 0;
}
bool sw_isspecified_filename(char *src)
{
	if( src == NULL )
		return -1;
	char tempbuf[256];
	memset(tempbuf,0,sizeof(tempbuf));
	int length = strlen(src);
	if( length >sizeof(tempbuf)-1 )
		return -1;
	else
		memcpy(tempbuf,src,length);
	
	char *p = &tempbuf[length-1];
	if( *p != '/' )
		return true;
	else
		return false;
		
}

int sw_strlcat(const char *src, int size, char *dest)
{
	if( src == NULL || dest == NULL )
		return -1;
	int length = strlen(dest)+strlen(src);
	if( length > size-1 )
		return -1;
	char *p = &dest[strlen(dest)];
	memcpy(p, src, strlen(src));
	dest[length] = '\0';
	return 0;
	
}
