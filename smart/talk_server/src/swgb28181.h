#ifndef __SWGB28181_H__
#define __SWGB28181_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef HANDLE
#define HANDLE void*
#endif
//RTP 包的大小,一般是 188 *7，mtu = 1500,c此处跟中维的保持一致
#define RTP_MAX_PACKET_BUFF 1395

int sw_make_rtp_header(int ssrc, uint8_t* data, int padding, int marker_flag);

#ifdef __cplusplus
}
#endif
#endif

