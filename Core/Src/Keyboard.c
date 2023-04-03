#include "main.h" 

char cmd_allhead[3] = {0xc5,0x6a,0x29};
char cmd_ppidhead[6]={0xc5,0x6a,0x29,0x1b,0x08,0x14};
char cmd_ppidhead_new[6]={0xc5,0x6a,0x29,0x15,0x08,0x0E};
char cmd_opidhead[6]={0xc5,0x6a,0x29,0x15,0x01,0x0e};
char cmd_roothead[5]={0xc5,0x6a,0x29,0x0e,0x12};
char cmd_hashtophead[5]={0xc5,0x6a,0x29,0x0e,0x11};
char cmd_handcmd[6]={0xc5,0x6a,0x29,0x06,0x04,0xaa};
char cmd_handAckcmd[7]={0xc5,0x6a,0x29,0x07,0x04,0x01,0x24};


unsigned char ppid_ok[7]   ={0xc5,0x6a,0x29,0x07,0xf1,0x0a,0x62};
unsigned char opid_ok[7]   ={0xc5,0x6a,0x29,0x07,0xef,0x0a,0x52};
unsigned char hashtop_ok[7]={0xc5,0x6a,0x29,0x07,0xfb,0x0a,0x85};
unsigned char root_ok[7]   ={0xc5,0x6a,0x29,0x07,0xf2,0x0a,0x37};
unsigned char keyUesed[7]  ={0xC5,0x6A,0x29,0x07,0xFA,0x0B,0x1F};//密码已使用
unsigned char errkey[7]    ={0xC5,0x6A,0x29,0x07,0xFA,0xA0,0x90};//错误密码
unsigned char keyOk[7]     ={0xC5,0x6A,0x29,0x07,0xFA,0x0A,0x41};

unsigned char Answer_RPD[5]={0xC5,0x6A,0x29,0x08,0x05};//Ramaining_Days
unsigned char Answer_DHB[5]={0xC5,0x6A,0x29,0x08,0x06};//Days_hve_been_run
unsigned char Answer_PD [5]={0xC5,0x6A,0x29,0x08,0x07};//Payg_Days
unsigned char Answer_PS [5]={0xC5,0x6A,0x29,0x08,0x09};//Payg_State
unsigned char Answer_OCS[5]={0xC5,0x6A,0x29,0x08,0x0A};//Output_Control_State
unsigned char Answer_RDB[5]={0xC5,0x6A,0x29,0x08,0x13};//Run Days Backup

//test Answer
//unsigned char Answer_Ramaining_PAYG_Days[8] =  {0xC5,0x6A,0x29,0x08,0x05,0x01,0x00,0x73};
////unsigned char Answer_Days_have_been_run[8]  =  {0xC5,0x6A,0x29,0x08,0x06,0x00,0x00,0x53};
//unsigned char Answer_PAYG_Days[8]           =  {0xC5,0x6A,0x29,0x08,0x07,0x01,0x00,0x3C};
//unsigned char Answer_PAYG_State[8]          =  {0xC5,0x6A,0x29,0x08,0x09,0x00,0x00,0x0C};
//unsigned char Answer_Output_Control_State[8]=  {0xC5,0x6A,0x29,0x08,0x0A,0x01,0x00,0x2C};
unsigned char Answer_System_Status_Code[8]  =  {0xC5,0x6A,0x29,0x08,0x0B,0x00,0x00,0x43};
unsigned char Answer_System_Status_CodeE[8] =  {0xC5,0x6A,0x29,0x08,0x0B,0x04,0x00,0x78};
unsigned char Answer_System_Status_CodeE2[8] =  {0xC5,0x6A,0x29,0x08,0x0B,0x02,0x00,0xD2};
//unsigned char Answer_Run_Days_Backup[8]     =  {0xC5,0x6A,0x29,0x08,0x13,0xCE,0x04,0x25};
unsigned char Answer_RREV[8]                =  {0xC5,0x6A,0x29,0x08,0x14,0x0A,0x00,0xB1};


unsigned char Answer_Upgrade[]                =  {0x53,0x01,0x04,0x00,0x02,0xf7,0x01,0x00,0x9b,0xb8};

extern  SYS_STATE_CODE_TypeDef  g_sysStateCode;

extern uint8_t g_tokenState;
//============================
unsigned char cmd_sendbuf[64];

char Uart_Buffer[64];
char Camp_cmd[64];

char Camp_ComOK = 0;
char SaveFlash_Flag  = 0;
uint32_t temphash;//交换HashTop
uint8_t g_keyboardPwrHoldTmrLock=FALSE;

__IO uint8_t g_Uart5Buf[UART5_RX_BUF_SIZE];
uint16_t RxUart5Counter;

extern PAYG_TypeDef payg;
extern uint8_t Sys_Code;
#ifdef IAP_SUPPORT
//extern UART_HandleTypeDef huart1;
#endif
extern UART_HandleTypeDef huart5;

extern BQ40Z50_TypeDef g_bq40z50_state;

extern uint8_t g_bleComEnable;

extern uint16_t g_bleRptPause;

extern USER_SET_TypeDef g_UserSet;


#ifdef IAP_SUPPORT
const unsigned short g_crc16_tbl[] = {
0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0,
};

const uint8_t ctrl_table[]=     {0x5a,0x00,0x04,0x00,0x01,0x01,0x00,0x00,0x00,0x00};

uint16_t crc16_ccitt(uint16_t init,  uint8_t *buffer, uint16_t length)
{
/*	uint8_t i;
	uint16_t crc = init;
	for (i = 0; i < length; i++) 
	{
		crc = g_crc16_tbl[((crc >> 8) ^ *buffer++) & 0xff] ^ (crc << 8);
	}*/

    uint16_t crc16 = init;
    uint16_t crc_h8, crc_l8;
    
    while( length-- ) {

        crc_h8 = (crc16 >> 8);
        crc_l8 = (crc16 << 8);
        crc16 = crc_l8 ^ g_crc16_tbl[crc_h8 ^ *buffer];
        buffer++;
    }


return crc16;//(crc16>>8|crc16<<8);
}

uint16_t hi_crc16(uint8_t *buffer, uint16_t length)
{
	return crc16_ccitt(0x0000, buffer, length);
}


void IapAck(uint8_t sub ,uint16_t data)
{
	uint16_t chksum;
	IAP_PACKET_Def  packet;
	
	memcpy((uint8_t*)&packet,ctrl_table,10);

	packet.sub=sub;

	packet.opt=data;//<<8;
	chksum=hi_crc16(&packet.cmd, 4);
	packet.chksum=chksum;
	
	HAL_Delay(5);
	//HAL_UART_Transmit(&huart3,(uint8_t*)&packet,sizeof(IAP_PACKET_Def),1000);
	Uart5Send((uint8_t*)&packet,sizeof(IAP_PACKET_Def));
}

void IapAckUart1(uint8_t sub ,uint16_t data)
{
	uint16_t chksum;
	IAP_PACKET_Def  packet;
	
	memcpy((uint8_t*)&packet,ctrl_table,10);

	packet.sub=sub;

	packet.opt=data;//<<8;
	chksum=hi_crc16(&packet.cmd, 4);
	packet.chksum=chksum;
	
	HAL_Delay(5);
	//HAL_UART_Transmit(&huart1,(uint8_t*)&packet,sizeof(IAP_PACKET_Def),1000);
	Uart5Send((uint8_t*)&packet,sizeof(IAP_PACKET_Def));
}


#endif


void debug_printf(uint8_t*tstr,uint8_t*str,uint16_t value)
{
#ifdef DUBUG_LOG
	uint8_t printstr[128];
	memset(printstr,0x00,128);
	sprintf(printstr,"%s: %s%d \r\n",tstr,str,value);
	
	//HAL_UART_Transmit(&huart3,printstr,strlen(printstr),1000);
	Uart5Send(printstr,strlen(printstr));
#endif	
}
void KeyBoardInit(void)
{
	//  HAL_UART_Receive_IT(&huart1,g_Uart1Buf,UART3_RX_BUF_SIZE);
	  //HAL_UART_Receive_IT(&huart3,(uint8_t*)g_Uart3Buf,UART3_RX_BUF_SIZE);

	huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf;
	huart5.RxXferCount=0;
	huart5.RxXferSize=UART5_RX_BUF_SIZE;
	  
}

void Printf_Usart_num(unsigned char *str, int num)
{
      //HAL_UART_Transmit_IT(&huart3,str,num);
	  Uart5Send(str,num);
}

void KeyBoardHand(void)
{
	// HAL_UART_Transmit_IT(&huart3,(uint8_t *)cmd_handcmd,6);
	Uart5Send((uint8_t*)cmd_handcmd,6);
}

#ifdef IAP_SUPPORT
void IapDetect(uint8_t* buffer,uint16_t* size)
{
	uint16_t i;

	
	for(i=0;i<*size;i++)
	{
		if(buffer[i]==0x5a&&buffer[i+1]==0x01)
		{
			if(i+10<=*size)
			{
				uint16_t chksum=0;
				IAP_PACKET_Def	ack_packet;
				
				chksum=hi_crc16((uint8_t*)&buffer[i+4], 4);
		
				memcpy((uint8_t *)&ack_packet,(uint8_t *)&buffer[i],sizeof(IAP_PACKET_Def));
				
				if(chksum==ack_packet.chksum)
				{
					memset((uint8_t*)buffer,0x00,*size);
					//RxUart5Counter=0;
					*size=0;
		
					switch(ack_packet.sub)
					{
						case 0xf7:
							IapAck(0xf7,0x0301);
							//HAL_FLASH_Unlock();
							//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
							FlashPageErase(ApplicationAddress+PAGE_SIZE*127-0X2000);
		
							//HAL_FLASH_Lock();
								
							__disable_fault_irq(); 
							NVIC_SystemReset();
							break;
						case 0xf8 :
							IapAck(0xf8,0x0301);
							break;
						}
						
					break;
					}
				}
			}
		}

}

#endif

void PKeybordProc(void)  //
{
	uint16_t i,len=0,value_len=0;
	#if 1 //def UI1K_V13_PROJECT
	uint8_t *p=NULL;
	#endif
	

	//if(huart3.RxState==HAL_UART_STATE_READY)
	//	KeyBoardInit();


	RxUart5Counter=huart5.RxXferCount;

	#ifdef GPS_SUPPORT
	//if(g_bleRptPause==0)
	//	GpsUart3Switch();
	#endif

    #ifdef BLE_ENABLE
	//if(GpsOccupyUart3()==FALSE)
		// BleDataCheck();
    #endif



	for(i=0;i<RxUart5Counter;i++)
	{
		#ifdef UI1K_V13_PROJECT
		p=(uint8_t*)strstr((uint8_t*)g_Uart5Buf,"AT+COULOMCFG=1\r\n");
		if(p!=NULL)
			CoulomConfig();
		#endif
		
		//if(huart3.RxState==HAL_UART_STATE_READY)
		//	KeyBoardInit();
		#ifdef BLE_ENABLE
		//if(GpsOccupyUart3()==FALSE)
		//	BleCmdProc((uint8_t*)&g_Uart3Buf[i]);	
		#endif 

		#ifdef GPS_SUPPORT
		//if(g_bleRptPause==0)
		//{	GpsProc((uint8_t*)&g_Uart3Buf[i]);
		//	GpsUart3Switch();
		//	}
		#endif
		#ifdef IAP_SUPPORT
		if(g_Uart5Buf[i]==0x5a&&g_Uart5Buf[i+1]==0x01)
		{
			if(i+10<=RxUart5Counter)
			{
				uint16_t chksum=0;
				IAP_PACKET_Def  ack_packet;
				
				chksum=hi_crc16((uint8_t*)&g_Uart5Buf[i+4], 4);

				memcpy((uint8_t *)&ack_packet,(uint8_t *)&g_Uart5Buf[i],sizeof(IAP_PACKET_Def));
				
				if(chksum==ack_packet.chksum)
				{
					memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
					RxUart5Counter=0;

					switch(ack_packet.sub)
					{
						case 0xf7:
							IapAck(0xf7,0x0301);
							//HAL_FLASH_Unlock();
							//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
							FlashPageErase(ApplicationAddress+PAGE_SIZE*127-0X2000);

							//HAL_FLASH_Lock();
								
							__disable_fault_irq(); 
				 			NVIC_SystemReset();
							break;
						case 0xf8 :
							IapAck(0xf8,0x0301);
							break;
						}
						
					break;
					}
				}
			}
		#endif

		p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+APN="); //"apn,username,passowrd"
		if(p!=NULL)
		{
			uint8_t tempBuff[128];
			HAL_Delay(20);
			
			p=p+7;
			value_len=AtCmdGetValueLen((char*)p,',');

			if(value_len>=APN_LEN)
				value_len=APN_LEN;
			memset(g_UserSet.NetInfor.apn,0x00,APN_LEN);
			if(value_len)
				memcpy(g_UserSet.NetInfor.apn,p,value_len);

			p=p+value_len+1;
			value_len=AtCmdGetValueLen((char*)p,',');

			if(value_len>=APN_USENAME_LEN)
				value_len=APN_USENAME_LEN;
			memset(g_UserSet.NetInfor.apn_usename,0x00,APN_USENAME_LEN);
			if(value_len)
				memcpy(g_UserSet.NetInfor.apn_usename,p,value_len);

			p=p+value_len+1;
			value_len=AtCmdGetValueLen((char*)p,'\r');

			if(value_len>=APN_PASSWORD_LEN)
				value_len=APN_PASSWORD_LEN;
			memset(g_UserSet.NetInfor.apn_password,0x00,APN_PASSWORD_LEN);

			if(value_len)
				memcpy(g_UserSet.NetInfor.apn_password,p,value_len);

		
			memset(tempBuff,0x00,128);

			sprintf((char*)tempBuff,"\"OK \r\n +APN= %s,%s,%s",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,g_UserSet.NetInfor.apn_password);

			//HAL_UART_Transmit(&huart5,tempBuff,strlen((char*)tempBuff),1000);
			Uart5Send(tempBuff,strlen((char*)tempBuff));

			memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
		   	huart5.RxXferCount=0;
			huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 

			EEpUpdateEnable();

			}
		p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+APN?"); //"apn,username,passowrd"
		if(p!=NULL)
		{
			uint8_t tempBuff[128];

			HAL_Delay(20);
			
			memset(tempBuff,0x00,128);

			sprintf((char*)tempBuff,"\"OK \r\n +APN= %s,%s,%s",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,g_UserSet.NetInfor.apn_password);

			//HAL_UART_Transmit(&huart5,tempBuff,strlen((char*)tempBuff),1000);

			Uart5Send(tempBuff,strlen((char*)tempBuff));

			memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
		   	huart5.RxXferCount=0;
			huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 
			}

			p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+BROKER=");  //"ip ,port,username,password"
			if(p!=NULL)
			{
				uint8_t tempBuff[128];

				HAL_Delay(20);
				p=p+10;
				value_len=AtCmdGetValueLen((char*)p,',');

				if(value_len>=MQTT_BROKER_LEN)
					value_len=MQTT_BROKER_LEN;
				memset(g_UserSet.NetInfor.mqtt_broker,0x00,MQTT_BROKER_LEN);
				memcpy(g_UserSet.NetInfor.mqtt_broker,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,',');

				if(value_len>=MQTT_PORT_LEN)
					value_len=MQTT_PORT_LEN;
				memset(g_UserSet.NetInfor.mqtt_port,0x00,MQTT_PORT_LEN);
				memcpy(g_UserSet.NetInfor.mqtt_port,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,',');

				if(value_len>=MQTT_USENAME_LEN)
					value_len=MQTT_USENAME_LEN;
				memset(g_UserSet.NetInfor.mqtt_usename,0x00,MQTT_USENAME_LEN);
				memcpy(g_UserSet.NetInfor.mqtt_usename,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,'\r');

				if(value_len>=MQTT_PASSWORD_LEN)
					value_len=MQTT_PASSWORD_LEN;
				memset(g_UserSet.NetInfor.mqtt_password,0x00,MQTT_PASSWORD_LEN);
				memcpy(g_UserSet.NetInfor.mqtt_password,p,value_len);


				memset(tempBuff,0x00,128);

				sprintf((char*)tempBuff,"+BROKER=%s,%s,%s,%s",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);

				Uart5Send(tempBuff,strlen((char*)tempBuff));

				memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
			   	huart5.RxXferCount=0;
				huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 
				EEpUpdateEnable();
				
				}

			p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+BROKER?");  //"ip ,port,username,password"
			if(p!=NULL)
			{
				uint8_t tempBuff[128];
				
				memset(tempBuff,0x00,128);

				sprintf((char*)tempBuff,"+BROKER=%s,%s,%s,%s\r\n",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);
				
				Uart5Send(tempBuff,strlen((char*)tempBuff));

				memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
			   	huart5.RxXferCount=0;
				huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 

				}

			p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+FBROKER=");  //"ip ,port,username,password"
			if(p!=NULL)
			{
				uint8_t tempBuff[128];
				p=p+11;
				HAL_Delay(20);
				value_len=AtCmdGetValueLen((char*)p,',');

				

				if(value_len>=MQTT_BROKER_LEN)
					value_len=MQTT_BROKER_LEN;
				memset(g_UserSet.NetInforFactory.mqtt_broker,0x00,MQTT_BROKER_LEN);
				memcpy(g_UserSet.NetInforFactory.mqtt_broker,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,',');

				if(value_len>=MQTT_PORT_LEN)
					value_len=MQTT_PORT_LEN;
				memset(g_UserSet.NetInforFactory.mqtt_port,0x00,MQTT_PORT_LEN);
				memcpy(g_UserSet.NetInforFactory.mqtt_port,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,',');

				if(value_len>=MQTT_USENAME_LEN)
					value_len=MQTT_USENAME_LEN;
				memset(g_UserSet.NetInforFactory.mqtt_usename,0x00,MQTT_USENAME_LEN);
				memcpy(g_UserSet.NetInforFactory.mqtt_usename,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen((char*)p,'\r');

				if(value_len>=MQTT_PASSWORD_LEN)
					value_len=MQTT_PASSWORD_LEN;
				memset(g_UserSet.NetInforFactory.mqtt_password,0x00,MQTT_PASSWORD_LEN);
				memcpy(g_UserSet.NetInforFactory.mqtt_password,p,value_len);


				memset(tempBuff,0x00,128);

				sprintf((char*)tempBuff,"+FBROKER=%s,%s,%s,%s",g_UserSet.NetInforFactory.mqtt_broker,g_UserSet.NetInforFactory.mqtt_port,g_UserSet.NetInforFactory.mqtt_usename,g_UserSet.NetInforFactory.mqtt_password);

				Uart5Send(tempBuff,strlen((char*)tempBuff));

				memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
			   	huart5.RxXferCount=0;
				huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 
				EEpUpdateEnable();
				
				}

			p=(uint8_t*)strstr((char*)g_Uart5Buf,"AT+FBROKER?");  //"ip ,port,username,password"
			if(p!=NULL)
			{
				uint8_t tempBuff[128];
				
				memset(tempBuff,0x00,128);

				sprintf((char*)tempBuff,"+FBROKER=%s,%s,%s,%s\r\n",g_UserSet.NetInforFactory.mqtt_broker,g_UserSet.NetInforFactory.mqtt_port,g_UserSet.NetInforFactory.mqtt_usename,g_UserSet.NetInforFactory.mqtt_password);
				
				Uart5Send(tempBuff,strlen((char*)tempBuff));

				memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
			   	huart5.RxXferCount=0;
				huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 

				//EEpUpdateEnable();
				}
		
		
		if((g_Uart5Buf[i]==0xc5&&g_Uart5Buf[i+1]==0x6a)&&g_Uart5Buf[i+2]==0x29)
		{

			len=g_Uart5Buf[i+3];

			if(g_keyboardPwrHoldTmrLock==FALSE)
				TimerKbPwrHoldSet(5*60*1000);
			g_keyboardPwrHoldTmrLock=TRUE;

			HAL_Delay(15);

			if(RxUart5Counter-i>=len&&RxUart5Counter-i>=4)
			{

			  if(g_Uart5Buf[i+len - 1] == CRC8((unsigned char *)&g_Uart5Buf[i], len - 1))
			  {
			//          Camp_ComOK = 1;
	           memcpy(Camp_cmd, (uint8_t*)&g_Uart5Buf[i], len);
	           memcpy(Uart_Buffer, (uint8_t*)&g_Uart5Buf[i], len);
	           Serial_Cmd(g_Uart5Buf[i+4]);

			   Process_cmd_all(Camp_ComOK);
			   }

			  g_bleComEnable=FALSE;

			  g_bleRptPause=2000u;// 5 min 30000u/150u;

			  memset((uint8_t*)g_Uart5Buf,0x00,UART5_RX_BUF_SIZE);
			  huart5.RxXferCount=0;
			  huart5.pRxBuffPtr=(uint8_t*)g_Uart5Buf; 

			}

			 
			}


		
		}

	
}


void Serial_Cmd(char cmd)
{
  switch (cmd)
  {
  case 0x00://单体电压 温度
    {Camp_ComOK = 0xff;};break;  
  case 0x01:
    {Camp_ComOK = 0x01;};break;  
  case 0x05:
    {Camp_ComOK = 0x05;};break;  
  case 0x06:
    {Camp_ComOK = 0x06;};break; 
  case 0x07:
    {Camp_ComOK = 0x07;};break;     
  case 0x08:
    {Camp_ComOK = 0x08;};break;
  case 0x09:
    {Camp_ComOK = 0x09;};break;   
  case 0x0a:
    {Camp_ComOK = 0x0a;};break;
  case 0x0b:
    {Camp_ComOK = 0x0b;};break;
  case 0x0c:
    {Camp_ComOK = 0x0c;};break; 
  case 0x0d:
    {Camp_ComOK = 0x0d;};break; 
  case 0x0E:
    {Camp_ComOK = 0x0E;};break;
  case 0x0F:
    {Camp_ComOK = 0x0F;};break; 
  case 0x10:
    {Camp_ComOK = 0x10;};break;
  case 0x11:
    {Camp_ComOK = 0x11;};break;
  case 0x12:
    {Camp_ComOK = 0x12;};break;
  case 0x13:
    {Camp_ComOK = 0x13;};break;     
  case 0x14:
    {Camp_ComOK = 0x14;};break;    
  case 0x17:
    {Camp_ComOK = 0x17;};break;  
  case 0x18:
    {Camp_ComOK = 0x18;};break;
  case 0x7F:
    {Camp_ComOK = 0x7F;};break;
  case 0xef:
    {Camp_ComOK = 0xef;};break;
  case 0xf1:
    {Camp_ComOK = 0xf1;};break; 
  case 0xf2:
    {Camp_ComOK = 0xf2;};break;
  case 0xf5:
    {Camp_ComOK = 0xf5;};break;  
  case 0xfa:
    {Camp_ComOK = 0xfa;};break;    
  case 0xfb:
    {Camp_ComOK = 0xfb;};break;    
  default:(Camp_ComOK = 0);break;
  }
}


void Send_RechargeOK(void)
{
	     Printf_Usart_num(keyOk , 7);	
}

void Process_cmd(char cmd)
{

	switch (cmd)
	{
	case 0x01://Read_OEMID
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_opidhead, 6);
		memcpy(cmd_sendbuf + 6, payg.oem_id, 14);
		cmd_sendbuf[20] = CRC8(cmd_sendbuf, 20);
		Printf_Usart_num(cmd_sendbuf, 21);
	};break;
	//case 0x04:
	//	TimerKbInsertSet(5*1000);
	//	break;
	case 0x05://Read_Remaining_PAYG_Days
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_RPD, 5);
		cmd_sendbuf[5] = payg.remaing_days & 0xFF;
		cmd_sendbuf[6] = (payg.remaing_days >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x06://Read_Days have been run
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_DHB, 5);
		cmd_sendbuf[5] = payg.days_has_been_runing & 0xFF;
		cmd_sendbuf[6] = (payg.days_has_been_runing >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break; 
	case 0x07://Read_PAYG_Days 
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_PD, 5);
		cmd_sendbuf[5] = payg.recharge_days & 0xFF;
		cmd_sendbuf[6] = (payg.recharge_days >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;     
	case 0x08://Read_PPID
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_ppidhead, 6);
		memcpy(cmd_sendbuf + 6, payg.payg_id, 20);
		cmd_sendbuf[26] = CRC8(cmd_sendbuf, 26);
		Printf_Usart_num(cmd_sendbuf, 27);
	};break;
	case 0x09://Read_PAYG_State
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_PS, 5);
		if(payg.free == YES)
			cmd_sendbuf[5] = 0x01;
		else        
			cmd_sendbuf[5] = 0x00;
		cmd_sendbuf[6] = 0x00;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x0a://Read_Output_Control_State
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_OCS, 5);
		if((payg.free == YES) || (payg.remaing_days > 0))
			cmd_sendbuf[5] = 0x01;
		else                
			cmd_sendbuf[5] = 0x00;

		cmd_sendbuf[6] = 0x00;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break; 
	case 0x0b://Read_System_Status_Code
	{
		if(/*Sys_Code == 0xE1*/  g_sysStateCode.error.rtc_error)//DS1302掉电
		{
			Printf_Usart_num(Answer_System_Status_CodeE, 8);
		}
		else if(Sys_Code == 0xE2)//晶体不工作
		{
			Printf_Usart_num(Answer_System_Status_CodeE2, 8);
		}
		else
			Printf_Usart_num(Answer_System_Status_Code, 8);
	};break;      
	case 0x11://Read_HashTop
	{
		memset(cmd_sendbuf, 0, 32);      
		memcpy(cmd_sendbuf, cmd_hashtophead, 5);

		memcpy(cmd_sendbuf + 5, payg.hast_otp + 1, 4);//这里指针+1 根据指针类型 uint32相当于4字节 不是1字节
		memcpy(cmd_sendbuf + 9, payg.hast_otp, 4);
		cmd_sendbuf[13] = CRC8(cmd_sendbuf, 13);
		Printf_Usart_num(cmd_sendbuf, 14);
	};break;
	case 0x12://Read_root
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_roothead, 5);
		memcpy(cmd_sendbuf + 5, payg.hast_root, 8);
		cmd_sendbuf[13] = CRC8(cmd_sendbuf, 13);
		Printf_Usart_num(cmd_sendbuf, 14);
	};break;
	case 0x13://Read_Run_Days_Backup
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_RDB, 5);
		cmd_sendbuf[5] = payg.run_days_backup & 0xFF;
		cmd_sendbuf[6] = (payg.run_days_backup >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;    
	case 0x14://Read_REV
	{
		

		Answer_RREV[5]=KEYB_VERSION;

		Answer_RREV[7] = CRC8(Answer_RREV, 7);

		Printf_Usart_num(Answer_RREV, 8);

	};break;
	case 0xe1://RTC_Err
	{
		//Printf_Usart("Rtc_Err!!!!!!!!!!!!\r\n");
	};break;
	case 0xef://Write_OEMID
	{
		memset(payg.oem_id,0x00, 20);
		memcpy(payg.oem_id, Camp_cmd + 6, 14);
		//SaveFlash_Flag = 1;
		EEpUpdateEnable();
		Printf_Usart_num(opid_ok, 7);
	};break;
	case 0xf1://Write_PPID
	{
		memset(payg.payg_id,0x00, 20);
		memcpy(payg.payg_id, Camp_cmd + 6, 20); 
		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();
		Printf_Usart_num(ppid_ok, 7);      
	};break;
	case 0xf2://Write_HashRoot
	{   
		Printf_Usart_num(root_ok, 7);
	};break; 
	case 0xfa://充值
	{
		memset(cmd_sendbuf, 0, 32);
		//倒序 按照正确命令排序
		cmd_sendbuf[3] = Camp_cmd[12];
		cmd_sendbuf[2] = Camp_cmd[11];
		cmd_sendbuf[1] = Camp_cmd[10];
		cmd_sendbuf[0] = Camp_cmd[9];
		cmd_sendbuf[7] = Camp_cmd[8];
		cmd_sendbuf[6] = Camp_cmd[7];
		cmd_sendbuf[5] = Camp_cmd[6];
		cmd_sendbuf[4] = Camp_cmd[5];

		memcpy(payg.hast_input, cmd_sendbuf, 8);
		//      Oves_SaveConfig(Pay_Info.PPID, 0x08006000, 52);
		PaygOvesInput();

		#ifdef PUMP_PROJECT
		 if(payg.remaing_days>0)
		 {
		 	#ifdef DC_PUMP_SUPPORT
			PumpTxCmd(PUMP_SET_ON);
			#else
		 	PumpTxCmd(PUMP_FREE);
			HAL_Delay(50);
		 	PumpTxCmd(PUMP_RESET);
			HAL_Delay(50);
			PumpTxCmd(PUMP_ON);
			#endif
		 	}
		 #endif

		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();

	};break; 
	case 0xfb://write_HashTop
	{ 
		memset(payg.hast_otp,0x00, 8);
		memcpy(payg.hast_otp, Camp_cmd + 5, 8); 
		temphash = payg.hast_otp[0];
		payg.hast_otp[0] =  payg.hast_otp[1];
		payg.hast_otp[1] = temphash;
		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();
		Printf_Usart_num(hashtop_ok, 7);
	};break;    
	default:
		break;//(Printf_Usart("Command_err!\r\n"));break;
	}
	PaygOvesTask();
	Camp_ComOK = 0;
	}

void Process_cmd_all(char cmd)
{
  
  switch (cmd)
{
	case 0x01://Read_OEMID
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_opidhead, 6);
		memcpy(cmd_sendbuf + 6, payg.oem_id, 14);
		cmd_sendbuf[20] = CRC8(cmd_sendbuf, 20);
		Printf_Usart_num(cmd_sendbuf, 21);
	};break;
	case 0x05://Read_Remaining_PAYG_Days
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_RPD, 5);
		cmd_sendbuf[5] = payg.remaing_days & 0xFF;
		cmd_sendbuf[6] = (payg.remaing_days >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x06://Read_Days have been run
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_DHB, 5);
		cmd_sendbuf[5] = payg.days_has_been_runing & 0xFF;
		cmd_sendbuf[6] = (payg.days_has_been_runing >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break; 
	case 0x07://Read_PAYG_Days 
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_PD, 5);
		cmd_sendbuf[5] = payg.recharge_days & 0xFF;
		cmd_sendbuf[6] = (payg.recharge_days >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;     
	case 0x08://Read_PPID
	{
		if(Uart_Buffer[5] == 0x14)
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_ppidhead, 6);
			memcpy(cmd_sendbuf + 6, payg.payg_id, 20);
			cmd_sendbuf[26] = CRC8(cmd_sendbuf, 26);
			Printf_Usart_num(cmd_sendbuf, 27);
		}
		else
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_ppidhead_new, 6);
			memcpy(cmd_sendbuf + 6, payg.payg_id, 0x0e);
			cmd_sendbuf[20] = CRC8(cmd_sendbuf, 20);
			Printf_Usart_num(cmd_sendbuf, 21);
		}
	};break;
	case 0x09://Read_PAYG_State
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_PS, 5);
		if(payg.free == YES)
			cmd_sendbuf[5] = 0x01;
		else                
			cmd_sendbuf[5] = 0x00;
		cmd_sendbuf[6] = 0x00;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x0a://Read_Output_Control_State
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_OCS, 5);
		if((payg.free == YES) || (payg.remaing_days > 0))
			cmd_sendbuf[5] = 0x01;
		else                
			cmd_sendbuf[5] = 0x00;
		cmd_sendbuf[6] = 0x00;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break; 
	case 0x0b://Read_System_Status_Code
	{
		#if 1
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x0b;
		cmd_sendbuf[5] = g_sysStateCode.state[0];
		cmd_sendbuf[6] = g_sysStateCode.state[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
		#else
		if(/*Sys_Code == 0xE1*/  g_sysStateCode.error.rtc_error)//DS1302掉电
		{
			Printf_Usart_num(Answer_System_Status_CodeE, 8);
		}
		else if(Sys_Code == 0xE2)//晶体不工作
		{
			Printf_Usart_num(Answer_System_Status_CodeE2, 8);
		}
		else
			Printf_Usart_num(Answer_System_Status_Code, 8);
		#endif
	};break;
	case 0x0c://Relative_SOC
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x0c;
		cmd_sendbuf[5] = g_bq40z50_state.Relative_State_of_Charge[0];
		cmd_sendbuf[6] = g_bq40z50_state.Relative_State_of_Charge[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x0d://Remaning_Capacity
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x0d;
		cmd_sendbuf[5] = g_bq40z50_state.Remaning_Capacity[0];
		cmd_sendbuf[6] = g_bq40z50_state.Remaning_Capacity[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x0e://Full_Charge_Capacity
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x0e;
		cmd_sendbuf[5] = g_bq40z50_state.Full_charge_Capacity[0];
		cmd_sendbuf[6] = g_bq40z50_state.Full_charge_Capacity[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x0f://---Accu_Energy_Output老协议
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x0f;
		cmd_sendbuf[5] = g_bq40z50_state.Average_Power[0];
		cmd_sendbuf[6] = g_bq40z50_state.Average_Power[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x10://Accu_Cycles
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x08;
		cmd_sendbuf[4] = 0x10;
		cmd_sendbuf[5] = g_bq40z50_state.Cycle_Count[0];
		cmd_sendbuf[6] = g_bq40z50_state.Cycle_Count[1];
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;
	case 0x11://Read_HashTop
	{
		memset(cmd_sendbuf, 0, 32);      
		memcpy(cmd_sendbuf, cmd_hashtophead, 5);

		memcpy(cmd_sendbuf + 5, payg.hast_otp + 1, 4);//这里指针+1 根据指针类型 uint32相当于4字节 不是1字节
		memcpy(cmd_sendbuf + 9, payg.hast_otp, 4);
		cmd_sendbuf[13] = CRC8(cmd_sendbuf, 13);
		Printf_Usart_num(cmd_sendbuf, 14);
	};break;
	case 0x12://Read_root
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_roothead, 5);
		memcpy(cmd_sendbuf + 5, payg.hast_root, 8);
		cmd_sendbuf[13] = CRC8(cmd_sendbuf, 13);
		Printf_Usart_num(cmd_sendbuf, 14);
	};break;
	case 0x13://Read_Run_Days_Backup
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, Answer_RDB, 5);
		cmd_sendbuf[5] = payg.run_days_backup & 0xFF;
		cmd_sendbuf[6] = (payg.run_days_backup >> 8) & 0xFF;
		cmd_sendbuf[7] = CRC8(cmd_sendbuf, 7);
		Printf_Usart_num(cmd_sendbuf, 8);
	};break;    
	case 0x14://Read_REV
	{
		//Printf_Usart_num(Answer_RREV, 8);
		Answer_RREV[5]=KEYB_VERSION;

		Answer_RREV[7] = CRC8(Answer_RREV, 7);

		Printf_Usart_num(Answer_RREV, 8);
	};break;
	case 0x17://Load_Power  acc
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x0C;
		cmd_sendbuf[4] = 0x17;
		cmd_sendbuf[5] = g_bq40z50_state.Acc_Power[0];
		cmd_sendbuf[6] = g_bq40z50_state.Acc_Power[1];
		cmd_sendbuf[7] = g_bq40z50_state.Acc_Power[2];
		cmd_sendbuf[8] = g_bq40z50_state.Acc_Power[3];
		//      cmd_sendbuf[7] = g_bq40z50_state.Solar_Generation[0];
		//      cmd_sendbuf[8] = g_bq40z50_state.Solar_Generation[1];
		 cmd_sendbuf[9] = g_bq40z50_state.Average_Power[0];
		 cmd_sendbuf[10] = g_bq40z50_state.Average_Power[1];
		/*cmd_sendbuf[7] = 0;
		cmd_sendbuf[8] = 0;
		cmd_sendbuf[9] = 0;
		cmd_sendbuf[10] = 0;*/
		cmd_sendbuf[11] = CRC8(cmd_sendbuf, 11);
		Printf_Usart_num(cmd_sendbuf, 12);
	};break;
	case 0x18://Charge_Power  solar
	{
		memset(cmd_sendbuf, 0, 32);
		memcpy(cmd_sendbuf, cmd_allhead, 3);
		cmd_sendbuf[3] = 0x0C;
		cmd_sendbuf[4] = 0x18;
		cmd_sendbuf[5] = g_bq40z50_state.Solar_Power[0];
		cmd_sendbuf[6] = g_bq40z50_state.Solar_Power[1];
		cmd_sendbuf[7] = g_bq40z50_state.Solar_Power[2];
		cmd_sendbuf[8] = g_bq40z50_state.Solar_Power[3];
		//      cmd_sendbuf[7] = g_bq40z50_state.Solar_Generation[0];
		//      cmd_sendbuf[8] = g_bq40z50_state.Solar_Generation[1];
		 cmd_sendbuf[9] = g_bq40z50_state.Charge_Power[0];
		 cmd_sendbuf[10] = g_bq40z50_state.Charge_Power[1];

		/*cmd_sendbuf[7] = 0;
		cmd_sendbuf[8] = 0;
		cmd_sendbuf[9] = 0;
		cmd_sendbuf[10] = 0;*/
		cmd_sendbuf[11] = CRC8(cmd_sendbuf, 11);
		Printf_Usart_num(cmd_sendbuf, 12);
	};break;
	case 0x7f://Charge_Power
	{
		if(Uart_Buffer[5] == sizeof(BQ40Z50_TypeDef))//如果接受协议用户数据区域等于接受缓冲区定义
		{
			memcpy(&g_bq40z50_state, Uart_Buffer+6, sizeof(BQ40Z50_TypeDef));
		}
	};break;
	case 0xe1://RTC_Err
	{
		;//Printf_Usart("Rtc_Err!!!!!!!!!!!!\r\n");
	};break;
	case 0xef://Write_OEMID
	{
		memset(payg.oem_id,0x00, 20);
		memcpy(payg.oem_id, Camp_cmd + 6, 14);
		//SaveFlash_Flag = 1;
		EEpUpdateEnable();
		Printf_Usart_num(opid_ok, 7);
	};break;
	case 0xf1://Write_PPID
	{
		memset(payg.payg_id,0x00, 20);
		memcpy(payg.payg_id, Camp_cmd + 6, 20); 
		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();
		Printf_Usart_num(ppid_ok, 7);      
	};break;
	case 0xf2://Write_HashRoot
	{   
		Printf_Usart_num(root_ok, 7);
	};break; 
	case 0xf5://Write_HashRoot
	{   
		if(/*(Sys_Code == 0xE1) || (Sys_Code == 0xE3)*/  g_sysStateCode.error.rtc_error)
		{
			payg.free = NO;
			payg.lock = NO;
			payg.remaing_days = 0;
			payg.days_has_been_runing = 0;
			InitDs1302();//已运行天数清0
			payg.time_cnt = 0;
			payg.recharge_days = 0;

			Sys_Code = 0;//clean Err
		        g_sysStateCode.error.rtc_error=0;

			payg.day_cem = 0;//运行天数备份清零
		}
		else
		{
			//Printf_Usart("No error!\r\n");
		}
	};break; 
	case 0xfa://充值
	{
		memset(cmd_sendbuf, 0, 32);
		//倒序 按照正确命令排序
		cmd_sendbuf[3] = Camp_cmd[12];
		cmd_sendbuf[2] = Camp_cmd[11];
		cmd_sendbuf[1] = Camp_cmd[10];
		cmd_sendbuf[0] = Camp_cmd[9];
		cmd_sendbuf[7] = Camp_cmd[8];
		cmd_sendbuf[6] = Camp_cmd[7];
		cmd_sendbuf[5] = Camp_cmd[6];
		cmd_sendbuf[4] = Camp_cmd[5];

		memcpy(payg.hast_input, cmd_sendbuf, 8);
		//      Oves_SaveConfig(Pay_Info.PPID, 0x08006000, 52);
		PaygOvesInput();

		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();

		#ifdef DC_PUMP_SUPPORT
		if(/*PaygGetPayState()&&*/g_tokenState==TOKEN_OK)
			PumpOn(TRUE);
	    #endif

	};break; 
	case 0xfb://write_HashTop
	{ 
		memset(payg.hast_otp,0x00, 8);
		memcpy(payg.hast_otp, Camp_cmd + 5, 8); 
		temphash = payg.hast_otp[0];
		payg.hast_otp[0] =  payg.hast_otp[1];
		payg.hast_otp[1] = temphash;
		//SaveFlash_Flag = 1;//保存到FLASH
		EEpUpdateEnable();
		Printf_Usart_num(hashtop_ok, 7);
	};break; 
	case 0xff://bat
	{
		if(Uart_Buffer[5] == 0x3F)//CV1
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x3F;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.CV1[0];
			cmd_sendbuf[8] = g_bq40z50_state.CV1[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x3E)//CV2
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x3e;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.CV2[0];
			cmd_sendbuf[8] = g_bq40z50_state.CV2[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x3D)//CV3
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x3D;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.CV3[0];
			cmd_sendbuf[8] = g_bq40z50_state.CV3[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x3C)//CV4
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x3C;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.CV4[0];
			cmd_sendbuf[8] = g_bq40z50_state.CV4[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x0A)//BatteryCurrent
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x0A;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.Current[0];
			cmd_sendbuf[8] = g_bq40z50_state.Current[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x09)//BatteryVoltage
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x09;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.Voltage[0];
			cmd_sendbuf[8] = g_bq40z50_state.Voltage[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
		if(Uart_Buffer[5] == 0x08)//BatteryTemperature
		{
			memset(cmd_sendbuf, 0, 32);
			memcpy(cmd_sendbuf, cmd_allhead, 3);
			cmd_sendbuf[3] = 0x0a;
			cmd_sendbuf[4] = 0x00;
			cmd_sendbuf[5] = 0x08;
			cmd_sendbuf[6] = 0x02;
			cmd_sendbuf[7] = g_bq40z50_state.Temperature[0];
			cmd_sendbuf[8] = g_bq40z50_state.Temperature[1];
			cmd_sendbuf[9] = CRC8(cmd_sendbuf, 9);
			Printf_Usart_num(cmd_sendbuf, 10);
		}
	};break;
	default:
		//(Printf_Usart("Command_err!\r\n"));
	break;
	}
  
  PaygOvesTask();
  Camp_ComOK = 0;
}






