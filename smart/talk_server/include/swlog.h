#ifndef __SWLOG_H__
#define __SWLOG_H__

typedef enum
{
	LOG_LEVEL_ALL = 0,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_ERROR,
}log_level_t;

#define SW_LOG_ALL( format, ... )  sw_log_out(LOG_LEVEL_ALL,__FILE__, __LINE__, format, ##__VA_ARGS__)
#define SW_LOG_DEBUG( format, ... )  sw_log_out(LOG_LEVEL_DEBUG,__FILE__, __LINE__, format, ##__VA_ARGS__)
#define SW_LOG_ERROR( format, ... )  sw_log_out(LOG_LEVEL_ERROR,__FILE__, __LINE__, format, ##__VA_ARGS__)

void sw_log_out( int log_level,const char *file,int line,const char* format, ... );
void sw_log_init(int log_level);

#endif
