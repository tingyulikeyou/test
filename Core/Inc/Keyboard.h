#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#define KEYB_VERSION  33

#define UART5_RX_BUF_SIZE  512

#ifdef IAP_SUPPORT

typedef struct
{
	uint8_t header;
	uint8_t ctrl;  // PANEL:0X00  BASE:0X01
	uint16_t lenght;
	uint8_t cmd;  //CTRL 01,CTRL ACK 02  ,QUERY 03,QUERY ACK 04
	uint8_t sub;
	uint16_t opt;
	uint16_t chksum;
}IAP_PACKET_Def;


void IapAckUart1(uint8_t sub ,uint16_t data);

#endif

#define UART3_RX_BUF_SIZE 1024

void KeyBoardInit(void);
void PKeybordProc(void) ;
void Serial_Cmd(char cmd);
void Process_cmd_all(char cmd);
void Printf_Usart_num(unsigned char *str, int num);
void debug_printf(uint8_t*tstr,uint8_t*str,uint16_t value);
void Send_RechargeOK(void);
uint16_t hi_crc16(uint8_t *buffer, uint16_t length);


#endif

