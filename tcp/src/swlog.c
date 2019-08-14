/**
 * @file sw_log.c
 * @brief 
 * @author YOUR NAME (), 
 * @version 1.0
 * @history
 * 		参见 :
 * 		2012-11-21 YOUR NAME created
 */

#include <stdio.h>
#include <stdarg.h>
#include "swlog.h"

static int m_log_level = LOG_LEVEL_ERROR;
static char *m_level_info[] = {"ALL","DEBUG","ERROR"};

void sw_log_out( int log_level,const char *file,int line,const char* format, ... )
{
	va_list args;
	char log_buf[1024*128] = {0};
	if(log_level < m_log_level || (log_level > LOG_LEVEL_ERROR || log_level < LOG_LEVEL_ALL))
		return;
	//填充日志
	va_start( args, format );
	vsnprintf(log_buf,sizeof(log_buf),format,args);
	va_end( args );
	fprintf(stdout,"[sunniwell][%s][%s,%d]%s",m_level_info[log_level],file,line,log_buf);
	return;
}

void  sw_log_init(int log_level)
{
	m_log_level = log_level;
	return;
}
