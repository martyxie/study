#ifndef __SWCOMMON_H__
#define __SWCOMMON_H__

#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#define false 0
#define true 1

typedef enum
{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN = 1,
	UNKNOW_ENDIAN = 2,
}endian_t;

//获取当前设备是大端还是小端模式
//返回当前的模式,大端或小端
int sw_get_endian_type(void);

uint32_t sw_swapint32_value(uint32_t value);

uint16_t sw_swapint16_value(uint16_t value);

void sw_common_init(endian_t mode);

#endif
