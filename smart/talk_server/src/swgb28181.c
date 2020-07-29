#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<execinfo.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>

#define RTP_HDR_LEN 12
#define RTP_VERSION 2

typedef struct bits_buffer_
{ 
    int         i_size; 
    int         i_data;
    uint8_t     i_mask;
    uint8_t*    p_data;   
} bits_buffer_t;

//typedef int(*senddata_callback_t)(int skt,uint8_t* data, int lenght);

#define bits_write(buffer, count, bits)\
do  \
{\
	bits_buffer_t *p_buffer = (buffer);\
	int i_count = (count);\
	uint64_t i_bits = (bits);\
	while( i_count > 0 )\
	{\
		i_count--;\
		if( ( i_bits >> i_count )&0x01 )\
		{\
			p_buffer->p_data[p_buffer->i_data] |= p_buffer->i_mask;\
		}\
		else\
		{\
			p_buffer->p_data[p_buffer->i_data] &= ~p_buffer->i_mask;\
		}\
		p_buffer->i_mask >>= 1;         /*操作完一个字节第一位后，操作第二位*/\
		if( p_buffer->i_mask == 0 )     /*循环完一个字节的8位后，重新开始下一位*/\
		{\
			p_buffer->i_data++;\
			p_buffer->i_mask = 0x80;\
		}\
	}\
}while(0)

int sw_make_rtp_header(int ssrc, uint8_t* data, int padding, int marker_flag)
{
	bits_buffer_t  	bitsBuffer;
	if (data == NULL)
		return -1;
    //uint8_t payload = codec2payload(codec);
	bitsBuffer.i_size = RTP_HDR_LEN;
	bitsBuffer.i_data = 0;
	bitsBuffer.i_mask = 0x80;
	bitsBuffer.p_data =	(unsigned char *)(data);
	memset(bitsBuffer.p_data, 0, RTP_HDR_LEN);
	bits_write(&bitsBuffer, 2, RTP_VERSION);	/* rtp version 	*/
    bits_write(&bitsBuffer, 1, padding);				/* rtp padding 	*/
	bits_write(&bitsBuffer, 1, 0);				/* rtp extension 	*/
	bits_write(&bitsBuffer, 4, 0);				/* rtp CSRC count */
	bits_write(&bitsBuffer, 1, (marker_flag));			/* rtp marker  	*/
	bits_write(&bitsBuffer, 7, 8);			/* rtp payload type*/
	bits_write(&bitsBuffer, 16, 0);			/* rtp sequence 	 */
	bits_write(&bitsBuffer, 32, 0); 		/* rtp timestamp 	 */
	bits_write(&bitsBuffer, 32, ssrc); 		/* rtp SSRC	 	 */
	return 0;
}

