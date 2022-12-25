
#ifndef __GPS_H__

#define __GPS_H__

#define UART4_RX_BUF_SIZE 1024

void GpsInit(void);
void GpsProc(uint8_t* gpsbuff);
uint8_t GpsOccupyUart3(void);
void GpsUart3Switch(void);

uint16_t  GetAsciiValueLen(uint8_t *str,uint8_t end_tag);
uint8_t*mStrStr(uint8_t *src,uint8_t *dst,uint16_t size);

#endif


