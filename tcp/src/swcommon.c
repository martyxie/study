/**
 * @file swcommon.c
 * @brief 
 * @author YOUR NAME (), 
 * @version 1.0
 * @history
 * 		参见 :
 * 		2012-11-21 YOUR NAME created
 */

#include "swlog.h"
#include "swcommon.h"

static int m_local_endian_mode = UNKNOW_ENDIAN; //当前设备端
static int m_other_endian_mode = UNKNOW_ENDIAN; //对端设备

int sw_get_endian_type(void)
{
	int result;
	union tmp_t
	{
		int a;
		char b;
	}tmp;
	tmp.a = 1;
	if(tmp.b == 1)
	{
		SW_LOG_ALL("this dev is LITTLE_ENDIAN\n");
		result = LITTLE_ENDIAN;
	}
	else
	{
		SW_LOG_ALL("this dev is BIG_ENDIAN\n");
		result = BIG_ENDIAN;
	}
	m_local_endian_mode = result;
	return result;
}
//获取大端形式存储的数值
uint32_t sw_swapint32_value(uint32_t value)
{
	uint32_t tmp_value = 0;
	if(m_local_endian_mode == UNKNOW_ENDIAN)
		m_local_endian_mode = sw_get_endian_type();

	if(m_local_endian_mode == m_other_endian_mode)
	{
		tmp_value = value;
		goto END;
	}
	else
	{
		tmp_value = ((value & 0x000000ff) << 24 | (value & 0x0000ff00) << 8 |
				(value & 0x00ff0000) >> 8 | (value & 0xff000000) >> 24);
	}

END:
	return tmp_value;
}

uint16_t sw_swapint16_value(uint16_t value)
{
	uint16_t tmp_value = 0;
	if(m_local_endian_mode == UNKNOW_ENDIAN)
		m_local_endian_mode = sw_get_endian_type();

	if(m_local_endian_mode == m_other_endian_mode)
	{
		tmp_value = value;
		goto END;
	}
	else
	{
		tmp_value = ((value & 0x0000ff00) << 8 | (value & 0x00ff0000) >> 8);
	}

END:
	return tmp_value;
}

void sw_common_init(endian_t mode)
{
	m_other_endian_mode = mode;
	return;
}

