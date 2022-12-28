#include "main.h"

COULOM_CMD_TypeDef g_CoulomCmd=COULOM_IDLE;

//unsigned int crc_modbus = 0;

//uint8_t payg_enable = 1;
__IO uint8_t g_CoulomComEnable;
uint8_t coulom_cfgclear[]={0x01,0x6F,0x00,0x00,0x00,0x00,0xD5,0xC3};
uint8_t coulom_cfgdefault[]={0x01,0x6F,0x00,0x00,0x00,0xFF,0x95,0x83};
uint8_t coulom_cfgfull[]={0x01,0x6F,0x00,0x00,0x00,0x0F,0x95,0xC7};

uint8_t coulom_setFullCap48Ah[]={0x01,0x10,0x00,0x01,0x00,0x02,0x04,0x00,0x00,0xBB,0x80,0x41,0x33 };
uint8_t coulom_setFullVolt24v[]=  {0x01,0x10,0x00,0x03,0x00,0x01,0x02,0x09,0x60,0xa0,0x1b };
uint8_t coulom_setEmpty19v[]=   {0x01,0x10,0x00,0x04,0x00,0x01,0x02,0x07,0x6c,0xa5,0xc9};
uint8_t coulom_startMeter[]=   {0x01, 0x06, 0x00, 0x0C, 0x00, 0x00, 0x49, 0xC9};
uint8_t coulom_getAllReg[]=   {0x01 ,0x04 ,0x00 ,0x00 ,0x00, 0x0F ,0xB0, 0x0E};
uint8_t g_CoulomComError=0;
uint8_t g_CoulomComErrorConfirm=0;

uint16_t g_CoulomSocBack=0;

uint8_t g_Chrge_VoltOver=0;
uint16_t g_CoulomChrgeState=0;
uint16_t g_CoulomWarnState=0;

__IO uint32_t g_CoulomAccuCap=0;  //mAh
__IO uint32_t g_CoulomAccuPower=0;  // wh
__IO uint32_t g_CoulomRtPower=0;  // wh


__IO uint32_t g_CoulomAccSolarPower=0;
__IO uint32_t g_CoulomRtChrgePower=0;

__IO uint32_t g_CoulomAccRuntime=0;

__IO uint32_t g_LowBatPwroffCnt=0;
__IO uint32_t g_AcFaultCnt=0;
__IO uint8_t g_LowBatLevel1ConfirmCnt=0;
__IO uint8_t g_LowBatLevel2ConfirmCnt=0;


SYS_STATE_CODE_TypeDef  g_sysStateCode;


LOWBAT_LEVEL_TypeDef g_lowbat_protect=LOWBAT_NONE;

extern __IO  uint8_t g_Uart485Buf[UART3_RX_BUF_SIZE];

extern uint16_t  RxUart3Counter;
extern uint16_t  RxUart3ParsePos;


extern UART_HandleTypeDef huart3;

extern BQ40Z50_TypeDef g_bq40z50_state;

//extern IWDG_HandleTypeDef hiwdg;

extern __IO uint8_t  g_AcState;

extern __IO uint8_t  g_DcState;
extern __IO uint8_t  g_UsbState;


// Table of CRC values for high order byte
const uint8_t auCHCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
};

// Low-Order Byte Table
// Table of CRC values for low order byte
const uint8_t auCHCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
};

uint16_t CRC16(uint8_t* pData, uint16_t len)
{
	uint8_t uCHCRCHi = 0xFF;                    // high byte of CRC initialized
	uint8_t uCHCRCLo = 0xFF; 	                  // low byte of CRC initialized
	uint8_t uIndex; 	                          // will index into CRC lookup table
	while (len--) 	                          // pass through message buffer
	{
		uIndex = uCHCRCHi ^ *pData++;     // calculate the CRC */
		uCHCRCHi = uCHCRCLo ^ auCHCRCHi[uIndex];
		uCHCRCLo = auCHCRCLo[uIndex];
	}
	return (uCHCRCHi | uCHCRCLo<<8);
}
uint8_t CrcCheck(uint8_t *pSrc, uint16_t size)
{
	uint16_t temp = 0;
	uint16_t crc = 0;
	crc = CRC16(pSrc, size);
	temp = (pSrc[size] & 0xff) |((pSrc[size+1] & 0xff) << 8);  

	if(temp == crc)	
		return 1;
	else
		return 0;
}

void SysPowerOff(void)
{
	#ifdef UI1K_V13_PROJECT
 	 uint32_t tickstart = HAL_GetTick();
  	 uint32_t wait = 20000u;
	
	//if(AdcGetChrgeState()==FALSE)
	{
		HAL_GPIO_WritePin(PV_CHRG_CTRL_GPIO_Port, PV_CHRG_CTRL_Pin, GPIO_PIN_RESET);
									 
		// if(HAL_GPIO_ReadPin(PV_ON_CHRGE_IN_GPIO_Port, PV_ON_CHRGE_IN_Pin)==GPIO_PIN_SET)
		 {
			 LcdClearAll();
			 LcdUpdateAll();
			 HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET); 

			 CoulomSleep();

			 EEpProcess();

			  while(AdcGetBatVolt()<=1000U)
			 {
			 	AdcProc();
				//HAL_IWDG_Refresh(&hiwdg);

				if((HAL_GetTick() - tickstart) > wait)
					break;
			 	}
			  HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);

			 HAL_Delay(2000);
			 __disable_fault_irq(); 
			NVIC_SystemReset();
		 	}
		}
	#endif
}

uint16_t CoulomGetRtPower(void)
{
	return g_CoulomRtPower;
}

uint16_t CoulomGetRuntime(void)
{
	return g_CoulomAccRuntime;
}

uint16_t CoulomGetChrgState(void)
{
	return g_CoulomChrgeState;
}

void CoulomConfig(void)
{
	CoulomSend((uint8_t*)coulom_setFullCap48Ah,13);
	HAL_Delay(50);
	CoulomSend((uint8_t*)coulom_setFullVolt24v,11);
	HAL_Delay(50);	
	CoulomSend((uint8_t*)coulom_setEmpty19v,11);
	HAL_Delay(50);	
	CoulomSend((uint8_t*)coulom_startMeter,8);

}
void CoulomInit(void)
{
	memset((uint8_t*)g_Uart485Buf,0x00,UART3_RX_BUF_SIZE);

	huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf;
	huart3.RxXferCount=0;
	huart3.RxXferSize=UART3_RX_BUF_SIZE;
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart1Buf,UART1_RX_BUF_SIZE);

	/*CoulomSend((uint8_t*)coulom_setFullCap48Ah,13);
	HAL_Delay(50);
	CoulomSend((uint8_t*)coulom_setFullVolt24v,11);
	HAL_Delay(50);	
	CoulomSend((uint8_t*)coulom_setEmpty19v,11);*/
	HAL_Delay(50);	
	CoulomSend((uint8_t*)coulom_startMeter,8);

	CoulomComStart();

	memset(g_sysStateCode.state,0x00,4);
}
 

void CoulomSend( uint8_t * buffer,uint16_t size)
{
	//memset(g_Uart1Buf,0x00,UART1_RX_BUF_SIZE);
	//huart1.RxXferCount=UART1_RX_BUF_SIZE;
	/*HAL_UART_Abort(&huart1);
	 memset(g_Uart1Buf,0x00,UART1_RX_BUF_SIZE);
	HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart1Buf,UART1_RX_BUF_SIZE);
	HAL_Delay(10);
	HAL_UART_Transmit(&huart1,(uint8_t *)buffer,size,size*20);*/

	huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf;
	huart3.RxXferCount=0;
	huart3.RxXferSize=UART3_RX_BUF_SIZE;

	Uart3Send(buffer,size);
}

void CoulomComEnable(void)
{
	g_CoulomComEnable=TRUE;
}

void CoulomComStart(void)
{
	g_CoulomCmd=COULOM_SOC;
}


void CoulomComMerge(uint8_t reg,uint8_t size)
{
	
	MODBUS_READ_TypeDef  cl_read;
	memset((uint8_t*)&cl_read,0x00,sizeof(MODBUS_READ_TypeDef));

	cl_read.bus_addr=0x01;
	cl_read.func=0x04;
	cl_read.regL=reg;
	cl_read.numL=size;
	cl_read.crc=CRC16((uint8_t*)&cl_read, sizeof(MODBUS_READ_TypeDef)-2);

	CoulomSend(coulom_getAllReg,8/*(uint8_t*)&cl_read,sizeof(MODBUS_READ_TypeDef)*/);
}

/*uint8_t  BuffCompare(uint8_t *src,uint8_t *dst,uint8_t size)
{
	uint8_t i=0;

	for(i=0;i<size;i++)
	{
		if(src[i]!=dst[i])
			break;
		}
	if(i>=size)
		return TRUE;
	else
		return FALSE;
}*/


void CoulomTxCmd(uint8_t cmd)
{
	#if 1
	CoulomComMerge(0,12);
	#else
	switch(cmd)
	{
		case COULOM_IDLE:
			break;
		case COULOM_SOC:
		case COULOM_TEMP:
		case COULOM_VOLTAGE: //V
		case COULOM_CURRENT://A
			CoulomComMerge(cmd-1,1);
			break;
		case COULOM_RT_PWR:  //KW
			CoulomComMerge(0x04,2);
			break;
		case COULOM_ACCU_ENERGE://KWH
			CoulomComMerge(0x06,2);
			break;
		case COULOM_ACCU_CAP:   //mAH
			CoulomComMerge(8,2);
			break;
		case COULOM_WARN:
			CoulomComMerge(10,1);
			break;
		case COULOM_RECHRGESTATE:
			CoulomComMerge(11,1);
			break;
		case COULOM_FULLSET:
			break;
		
		}
	#endif

	
}

uint8_t CoulomVoltOver(void)
{
	return g_Chrge_VoltOver;
}

uint8_t CoulomLowbat(void)
{
	return g_lowbat_protect;
}

uint8_t  CoulomCmdParse(uint8_t*buffer)
{
	uint16_t temp=0;
	uint8_t i=0;
	int16_t *p;
	uint32_t temp32=0;
	uint16_t soc=0;
	uint32_t hour=0;
	uint16_t current=0;
	uint32_t Power=NORMAL_CAPACITY;
	
	#ifdef UI1K_V13_PROJECT

		
			g_CoulomWarnState=(buffer[23]<< 8 | buffer[24]);
			//g_CoulomChrgeState=(buffer[25]<< 8 | buffer[26]);

			if(g_CoulomChrgeState!=(buffer[25]<< 8 | buffer[26]))
			{
				if((buffer[25]<< 8 | buffer[26])==CHRGE_STATE_CHRGE)
				{
					//EEpSetAccuCap(EEpGetAccuCap()+g_CoulomAccuCap);
					EEpSetAccuPower(EEpGetAccuPower()+g_CoulomAccuPower/**g_CoulomRtPowerCnt*//10/3600);
					g_CoulomAccuPower=0;
					}
				else 
				{
					if(g_CoulomChrgeState==CHRGE_STATE_CHRGE)
					{	EEpSetAccuSolarGen(EEpGetAccuSolarGen()+g_CoulomAccSolarPower/3600);
						g_CoulomAccSolarPower=0;
						}
					}
				}

			g_CoulomChrgeState=(buffer[25]<< 8 | buffer[26]);
	//switch(g_CoulomCmd)
	{
		//case COULOM_IDLE:
		//	break;
		//case COULOM_SOC:
			GattSetRelativeSOC( buffer[3]<< 8 | buffer[4]);
			soc=(buffer[3]<<8| buffer[4]);
			if(soc<=100)
			{
				g_bq40z50_state.Relative_State_of_Charge[0]=buffer[4];
				g_bq40z50_state.Relative_State_of_Charge[1]=buffer[3];
				}
			
			/*if(soc==0&&g_CoulomSocBack>temp)
			{
				if(g_CoulomSocBack-soc>2)  //µç³ØË¥¼õ
				{
				
					}
				}
			g_CoulomSocBack=buffer[3]<< 8 | buffer[4];*/
			//break;
		//case COULOM_TEMP:
			temp=( buffer[5]<< 8 | buffer[6]);
			p=&temp;
			*p+=9;
			
			GattSetBattTemp(25 /*buffer[5]<< 8 | buffer[6]*p*/);
			temp=250+2731;
			g_bq40z50_state.Temperature[0]=temp;//buffer[5];
			g_bq40z50_state.Temperature[1]=temp>>8;//buffer[6];
		//	break;
		//case COULOM_VOLTAGE: //V
		        
			temp=(buffer[7]<< 8 | buffer[8])*10;
			if(temp<=26000U)
			{
			        GattSetBattVolt(temp);//BatteryVoltage
			        g_bq40z50_state.Voltage[1]=temp>>8;      // mV
			        g_bq40z50_state.Voltage[0]=temp&0xff; // ex.22.9  ,2290
				}
		        
		       temp=temp/7;
		       g_bq40z50_state.CV1[1]=temp>>8;
		       g_bq40z50_state.CV2[1]=temp>>8;
		       g_bq40z50_state.CV3[1]=temp>>8;
		       g_bq40z50_state.CV4[1]=temp>>8;  

		       g_bq40z50_state.CV1[0]=temp&0x00ff;
		       g_bq40z50_state.CV2[0]=temp&0x00ff;
		       g_bq40z50_state.CV3[0]=temp&0x00ff;
		       g_bq40z50_state.CV4[0]=temp&0x00ff;  

	   
	   

		      GattSetCellVolt1(g_bq40z50_state.CV1[1] << 8 | g_bq40z50_state.CV1[0] );
		      GattSetCellVolt2(g_bq40z50_state.CV2[1] << 8 | g_bq40z50_state.CV2[0] );
		      GattSetCellVolt3(g_bq40z50_state.CV3[1] << 8 | g_bq40z50_state.CV3[0] );
		      GattSetCellVolt4(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );	
			  GattSetCellVolt5(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );
			  GattSetCellVolt6(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );
			  GattSetCellVolt7(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );
		//	break;
		//case COULOM_CURRENT://A
			temp=(buffer[9]<< 8 | buffer[10])*10; // mA
			current=temp;
		        GattSetBattCurrent(temp);//BatteryCurrent

			p=&temp;
			if(g_CoulomChrgeState!=CHRGE_STATE_CHRGE)
				*p=*p*-1;
			
		        g_bq40z50_state.Current[1]=temp>>8;   //mA
			g_bq40z50_state.Current[0]=temp&0xff;  //ex. 900ma, 90


			if(g_CoulomChrgeState==CHRGE_STATE_CHRGE)
			{
				if(temp==0)
				{
					g_bq40z50_state.Run_Time_To_Full[1]=0xff;
					g_bq40z50_state.Run_Time_To_Full[0]=0xff;
					}
				else
				{
					temp=(100-soc);
					hour=(uint32_t)temp*Power/(uint32_t)10/(uint32_t)current;

					if(hour<10)
						hour=1;
					else
						hour=hour/10;

					g_bq40z50_state.Run_Time_To_Full[1]=hour>>8;
					g_bq40z50_state.Run_Time_To_Full[0]=hour;
					}

				g_bq40z50_state.Run_Time_To_Empty[1]=0xff;
				g_bq40z50_state.Run_Time_To_Empty[0]=0xff;

				}
			else
			{
				if(temp==0)
				{
					g_bq40z50_state.Run_Time_To_Empty[1]=0xff;
					g_bq40z50_state.Run_Time_To_Empty[0]=0xff;
					}
				else
				{
					hour=(uint32_t)soc*(uint32_t)(Power/(uint32_t)10)/(uint32_t)current;

					if(hour<10)
						hour=1;
					else
						hour=hour/10;
					
					g_bq40z50_state.Run_Time_To_Empty[1]=hour>>8;
					g_bq40z50_state.Run_Time_To_Empty[0]=hour;
					}

				g_bq40z50_state.Run_Time_To_Full[1]=0xff;
				g_bq40z50_state.Run_Time_To_Full[0]=0xff;
				}

			
		//	break;
		//case COULOM_RT_PWR:  //0.0W   /10
			g_CoulomRtPower=(buffer[11]<< 24 | buffer[12]<<16|buffer[13]<< 8 | buffer[14]);

			
		
			if(g_CoulomChrgeState<=CHRGE_STATE_RECHRG)
		       	{	GattSetLoadPower((buffer[11]<< 24 | buffer[12]<<16|buffer[13]<< 8 | buffer[14])/10);  //0.00kw
		       		temp=g_CoulomRtPower/10;
		       		g_bq40z50_state.Average_Power[1]=temp>>8;
				g_bq40z50_state.Average_Power[0]=temp&0x00ff;

				//g_bq40z50_state.Charge_Power[1]=0;
				//g_bq40z50_state.Charge_Power[0]=0;
				if(AdcGetChrgeCurrent()>=500)
				{
					g_bq40z50_state.Charge_Power[1]=g_CoulomRtChrgePower>>8;
					g_bq40z50_state.Charge_Power[0]=g_CoulomRtChrgePower&0x00ff;
					}
				else
				{
					 g_bq40z50_state.Charge_Power[1]=0;
					g_bq40z50_state.Charge_Power[0]=0;
					}
				}
			else
			{	GattSetLoadPower(0);
				g_bq40z50_state.Average_Power[1]=0;
				g_bq40z50_state.Average_Power[0]=0;
				g_bq40z50_state.Charge_Power[1]=g_CoulomRtChrgePower>>8;
				g_bq40z50_state.Charge_Power[0]=g_CoulomRtChrgePower&0x00ff;
				}
		//	break;
		//case COULOM_ACCU_ENERGE:///100= 0.00KWH       *10=Wh
		  //      g_CoulomAccuPower=(buffer[15]<< 24 | buffer[16]<<16|buffer[17]<< 8 | buffer[18])*10; //wh
		//  if(g_CoulomChrgeState<=CHRGE_STATE_CHRGE)	
		  	//g_CoulomAccuPower=g_CoulomRtPower*g_CoulomRtPowerCnt/3600/2;   // 500ms one times

			if(g_CoulomChrgeState<CHRGE_STATE_CHRGE)	
			{	temp32=EEpGetAccuPower()+g_CoulomAccuPower/10/3600;
				g_bq40z50_state.Acc_Power[0]=temp32;
				g_bq40z50_state.Acc_Power[1]=temp32>>8;
				g_bq40z50_state.Acc_Power[2]=temp32>>16;
				g_bq40z50_state.Acc_Power[3]=temp32>>24;
				GattSetAccuEnergy(EEpGetAccuPower()+g_CoulomAccuPower/10/3600);

				temp32=EEpGetAccuSolarGen();
				g_bq40z50_state.Solar_Power[0]=temp32;
				g_bq40z50_state.Solar_Power[1]=temp32>>8;
				g_bq40z50_state.Solar_Power[2]=temp32>>16;
				g_bq40z50_state.Solar_Power[3]=temp32>>24;
				GattSetSolarGeneration(EEpGetAccuSolarGen());

				
				}	
			else	
			{	temp32=EEpGetAccuPower();
				g_bq40z50_state.Acc_Power[0]=temp32;
				g_bq40z50_state.Acc_Power[1]=temp32>>8;
				g_bq40z50_state.Acc_Power[2]=temp32>>16;
				g_bq40z50_state.Acc_Power[3]=temp32>>24;
				GattSetAccuEnergy(EEpGetAccuPower());

				temp32=EEpGetAccuSolarGen()+g_CoulomAccSolarPower/3600;
				g_bq40z50_state.Solar_Power[0]=temp32;
				g_bq40z50_state.Solar_Power[1]=temp32>>8;
				g_bq40z50_state.Solar_Power[2]=temp32>>16;
				g_bq40z50_state.Solar_Power[3]=temp32>>24;
				
				GattSetSolarGeneration(EEpGetAccuSolarGen()+g_CoulomAccSolarPower/3600);
				}
			//break;
		//case COULOM_ACCU_CAP:   //mAH
			//g_CoulomAccuCap=(buffer[19]<< 24 | buffer[20]<<16|buffer[21]<< 8 | buffer[22]);   //mAh
			//GattSetSolarGeneration(EEpGetAccuCap()/1000);
			g_CoulomAccuCap=(EEpGetAccuPower()+g_CoulomAccuPower/10/3600)/22.6f;  // en /22.6f;
			
			//temp32=EEpGetAccuCap()/1000;

			temp32=g_CoulomAccuCap/(NORMAL_CAPACITY/1000);

			g_bq40z50_state.Cycle_Count[1] =(temp32>>8)&0x00ff;
			g_bq40z50_state.Cycle_Count[0]=temp32&0x00ff;

			GattSetAccuCyc( g_bq40z50_state.Cycle_Count[1] << 8 | g_bq40z50_state.Cycle_Count[0] );

			temp32=NORMAL_CAPACITY*1.11628f;
			g_bq40z50_state.Full_charge_Capacity[1]=temp32>>8;
			g_bq40z50_state.Full_charge_Capacity[0]=temp32;	
			GattSetFullChrgeCap( g_bq40z50_state.Full_charge_Capacity[1]<< 8 | g_bq40z50_state.Full_charge_Capacity[0]);

			//
			temp=soc*(NORMAL_CAPACITY/100)*1.11628f;
			g_bq40z50_state.Remaning_Capacity[1] =temp>>8;
			g_bq40z50_state.Remaning_Capacity[0] =temp&0xff;
			GattSetRmCap(g_bq40z50_state.Remaning_Capacity[1] << 8 | g_bq40z50_state.Remaning_Capacity[0] );//Remaining_Capacity



		//	break;
		//case COULOM_WARN:
		//	g_CoulomCmd=COULOM_IDLE;
		//	break;
		//case COULOM_RECHRGESTATE:
		//	break;
		
		}
	#else
	
	switch(g_CoulomCmd)
	{
		case COULOM_IDLE:
			break;
		case COULOM_SOC:
			GattSetRelativeSOC( buffer[3]<< 8 | buffer[4]);
			g_bq40z50_state.Relative_State_of_Charge[0]=buffer[4];
			g_bq40z50_state.Relative_State_of_Charge[1]=buffer[3];
			break;
		case COULOM_TEMP:
			GattSetBattTemp( buffer[3]<< 8 | buffer[4]);
			g_bq40z50_state.Temperature[0]=buffer[4];
			g_bq40z50_state.Temperature[1]=buffer[3];
			break;
		case COULOM_VOLTAGE: //V
		        GattSetBattVolt((buffer[3]<< 8 | buffer[4])/100);//BatteryVoltage
		        g_bq40z50_state.Voltage[0]=buffer[4];
		        g_bq40z50_state.Voltage[1]=buffer[3];
			break;
		case COULOM_CURRENT://A
		        GattSetBattCurrent((buffer[3]<< 8 | buffer[4])/100);//BatteryCurrent
		        g_bq40z50_state.Current[0]=buffer[4];
			g_bq40z50_state.Current[1]=buffer[3];
			break;
		case COULOM_RT_PWR:  //KW
		       GattSetLoadPower((buffer[3]<< 24 | buffer[4]<<16|buffer[5]<< 8 | buffer[6])/10);
			break;
		case COULOM_ACCU_ENERGE://KWH
			GattSetAccuEnergy((buffer[3]<< 24 | buffer[4]<<16|buffer[5]<< 8 | buffer[6])/100);
			break;
		case COULOM_ACCU_CAP:   //mAH
			GattSetSolarGeneration((buffer[3]<< 24 | buffer[4]<<16|buffer[5]<< 8 | buffer[6])/1000);
			break;
		case COULOM_WARN:
			g_CoulomCmd=COULOM_IDLE;
			break;
		case COULOM_RECHRGESTATE:
			break;
		
		}

	#endif
		return 1;
}

void CoulomSleep(void)
{
	EEpSetAccuPower(EEpGetAccuPower()+g_CoulomAccuPower/**g_CoulomRtPowerCnt*//10/3600);
	g_CoulomAccuPower=0;
	EEpSetAccuSolarGen(EEpGetAccuSolarGen()+g_CoulomAccSolarPower/3600);
	g_CoulomAccSolarPower=0;

	EEpSetAccuRuntime(EEpGetAccuRuntime()+g_CoulomAccRuntime);
	g_CoulomAccRuntime=0;

	EEpProcess();
}
void CoulomProc(void)
{
	uint16_t i,len=0;
	uint16_t SysVolt=0;


	 #ifdef UI1K_V13_PROJECT
	
	RxUart3Counter=huart3.RxXferCount;

	for(i=0;i<RxUart3Counter;i++)
	{
		if((CrcCheck(&g_Uart485Buf[i],g_Uart485Buf[i+2]+3))&&g_Uart485Buf[i]==1&&RxUart1Counter>=g_Uart485Buf[i+2]+3+2)
		{
			HAL_Delay(10);
			CoulomCmdParse(&g_Uart485Buf[i]);

			g_CoulomComError=0;
			g_CoulomComErrorConfirm=0;

			//if(GetSysErrorCode()==0XE1)
			//	SetSysErrorCode(0X00);

			g_sysStateCode.error.coulom_com=0;
		
			memset(g_Uart485Buf,0x00,UART3_RX_BUF_SIZE);
			huart1.RxXferCount=0;
			huart1.RxXferSize=UART3_RX_BUF_SIZE;
			huart1.pRxBuffPtr=g_Uart485Buf; 
			//HAL_UART_Abort(&g_Uart1DiBuf);
			// memset(g_UartDiBuf,0x00,UART_DI_RX_BUF_SIZE);
	  		// HAL_UART_Receive_IT(&g_Uart1DiBuf,(uint8_t*)g_UartDiBuf,UART_DI_RX_BUF_SIZE);

			
			}
		}

	
	if(g_CoulomComErrorConfirm>=3/*GetSysErrorCode()==0XE1*/)
      	         SysVolt=AdcGetBmsVolt();
	else
	{	SysVolt=GattGetBattVolt();//coulom
	
		if(SysVolt==0)
			SysVolt=AdcGetBmsVolt();
		}
	
	if(GetTimerCoulomFullEvent()||SysVolt>=25300U||AdcGetBmsVolt()>=26000u)
	{
		//CoulomSend(coulom_cfgfull,8);
		if(GetTimerCoulomFullEvent())
			TimerEventClear(TIMER_CL_FULL);
		g_Chrge_VoltOver=TRUE;
		g_sysStateCode.error.volt_over=1;
		}

		
	// high volt protect
	if(SysVolt>=24850U)  //24.85v  keep 30 sec
	{	
		if(GetTimerCoulomFullState()==FALSE)
		{
			TimerCoulomFullSet(30000U);
			}
		}
	else
	{	TimerCoulomStop();
		}

	
	
	if(SysVolt<23800U&&AdcGetBmsVolt()<26000u)  
	{	g_Chrge_VoltOver=FALSE;
		g_sysStateCode.error.volt_over=0;
		}
	

	// low battery protect
	if(SysVolt<19000u+200u)
	{	
		if(g_LowBatLevel2ConfirmCnt>=5)
		{
			g_lowbat_protect=LOWBAT_LEVEL2;
		 	g_sysStateCode.error.volt_low=1;
			g_DcState=FALSE;
			g_UsbState=FALSE;
			debug_printf("coulom","low bat leve 1,volt=",SysVolt);
			}
		}
	else if(SysVolt>=19000u+200u&&SysVolt<20000u+200u)
	{	
		
		if(g_LowBatLevel1ConfirmCnt>=5)
		{
			if(g_lowbat_protect==LOWBAT_NONE)
			{	g_lowbat_protect=LOWBAT_LEVEL1;
			 	debug_printf("coulom","low bat leve 2,volt=",SysVolt);
				}

			g_UsbState=FALSE;
			}
		g_LowBatLevel2ConfirmCnt=0;
		}
	else
	{
		g_LowBatLevel1ConfirmCnt=0;
		g_LowBatLevel2ConfirmCnt=0;
		
		if(g_lowbat_protect!=LOWBAT_NONE)
		{
			if(GattGetRelativeSOC()>=5)
			{	g_lowbat_protect=LOWBAT_NONE;
				g_sysStateCode.error.volt_low=0;
				 debug_printf("coulom","soc >%5 recover,volt=",SysVolt);
				}
			}
		}
	
       if(g_CoulomComEnable)  // 1 sec event
       	{

		if(g_LowBatLevel1ConfirmCnt<10)
			g_LowBatLevel1ConfirmCnt++;
		
		if(g_LowBatLevel2ConfirmCnt<10)
			g_LowBatLevel2ConfirmCnt++;
		
		if(g_AcState)
		{
			if(HAL_GPIO_ReadPin(AC_FAULT_GPIO_Port, AC_FAULT_Pin)==GPIO_PIN_SET)
			{	
				if(g_AcFaultCnt<10)
				{
					g_AcFaultCnt++;
					}
				else
				{
					g_sysStateCode.error.ac_fault=1;
					g_AcState=FALSE;
					 debug_printf("coulom","ac fault confirm,volt=",SysVolt);
					}
				}
			else
			{
				g_AcFaultCnt=0;
				g_sysStateCode.error.ac_fault=0;
				}
			}
		else
		{
			g_AcFaultCnt=0;
			}

		
       		if(SysVolt<=17000u)
       		{	
       			g_LowBatPwroffCnt++;

			if(g_LowBatPwroffCnt>=60)
			{
				debug_printf("coulom","low battery power off,volt=",SysVolt);
				/*HAL_GPIO_WritePin(PV_CHRG_CTRL_GPIO_Port, PV_CHRG_CTRL_Pin, GPIO_PIN_RESET);
							 
				// if(HAL_GPIO_ReadPin(PV_ON_CHRGE_IN_GPIO_Port, PV_ON_CHRGE_IN_Pin)==GPIO_PIN_SET)
				 {
					 LcdClearAll();
					 LcdUpdateAll();
					 HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_RESET);
					 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET); 

					 CoulomSleep();

					  while(AdcGetBatVolt()==0)
					 {
					 	AdcProc();
					 	}
					  HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);

					 HAL_Delay(2000);
				 	}*/
				 	SysPowerOff();
				}
       			}
		else
		{	g_LowBatPwroffCnt==0;
			}
       		//runtime counter
       		g_CoulomAccRuntime++;
			
       		// realtime charge power =  BMS volt*bms current   W/sec
			
		

		//charge mode accu solar power	
		if(g_CoulomChrgeState==CHRGE_STATE_CHRGE||AdcGetChrgeCurrent()>500u)
		{	
			uint32_t current=AdcGetChrgeCurrent();
			uint32_t volt=AdcGetBmsVolt();
			g_CoulomRtChrgePower=current*volt/1000/1000;

			if(g_CoulomRtChrgePower<=3)
				g_CoulomRtChrgePower=0;

			g_CoulomAccSolarPower+=g_CoulomRtChrgePower;
			
			//charge power 
			g_bq40z50_state.Charge_Power[1]=g_CoulomRtChrgePower>>8;
			g_bq40z50_state.Charge_Power[0]=g_CoulomRtChrgePower&0x00ff;

			
			}
		/*else
		{
			}*/
		
       		//power accu W/sec
       		if(g_CoulomChrgeState<=CHRGE_STATE_RECHRG)
		{	
			g_CoulomAccuPower+=	g_CoulomRtPower;
       			}
			
       		if(g_CoulomComError<5)
       			g_CoulomComError++;
		else
		{	
			if(g_CoulomComError==5)
			{
				if(g_CoulomComErrorConfirm<3)
				{	
					g_CoulomComErrorConfirm++;
					if(g_CoulomComErrorConfirm==3/*&&GetSysErrorCode()==0*/)
						g_sysStateCode.error.coulom_com=1;///SetSysErrorCode(0xe1);
					}
				
				g_CoulomComError=0;
				
				//HAL_UART_Abort(&huart1);
				 memset(g_Uart485Buf,0x00,UART3_RX_BUF_SIZE);
		  		 //HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart1Buf,UART1_RX_BUF_SIZE);
		  		 huart3.RxXferCount=0;
				 huart3.RxXferSize=UART3_RX_BUF_SIZE;
			}
			}

		// coulom no ack 
		if(g_CoulomComErrorConfirm>=3)
		{
			
			uint16_t soc=0,tempvolt=AdcGetBmsVolt();
			uint32_t temp32;
			float volt=(float)(tempvolt/1000.0f);


			GattSetBattVolt(AdcGetBmsVolt());//BatteryVoltage
			
		        g_bq40z50_state.Voltage[1]=tempvolt>>8;      // mV
		        g_bq40z50_state.Voltage[0]=tempvolt&0xff; // ex.22.9  ,2290
		        
			//soc=(7*volt*volt/3-86*volt+791.7f);///100;
			soc=(3.8757f*volt*volt-153.0242f*volt+1509.3376f);

			if(tempvolt>24850)
				soc=100;
			if(tempvolt<=19200)
				soc=0;

			if(soc>100)
				soc=100;
				
			GattSetRelativeSOC( soc);
			g_bq40z50_state.Relative_State_of_Charge[0]=soc;
			g_bq40z50_state.Relative_State_of_Charge[1]=soc>>8;

			temp32=NORMAL_CAPACITY*1.11628f;
			g_bq40z50_state.Full_charge_Capacity[1]=temp32>>8;
			g_bq40z50_state.Full_charge_Capacity[0]=temp32;	
			GattSetFullChrgeCap( g_bq40z50_state.Full_charge_Capacity[1]<< 8 | g_bq40z50_state.Full_charge_Capacity[0]);

			g_bq40z50_state.Remaning_Capacity[1] =temp32>>8;
			g_bq40z50_state.Remaning_Capacity[0] =temp32&0xff;
			GattSetRmCap(g_bq40z50_state.Remaning_Capacity[1] << 8 | g_bq40z50_state.Remaning_Capacity[0] );//Remaining_Capacity
			
			}
		#if 0
		if(g_CoulomCmd<COULOM_RECHRGESTATE)
			g_CoulomCmd++;
		else
			g_CoulomCmd=COULOM_SOC;
		#endif
		
		CoulomTxCmd(g_CoulomCmd);
		g_CoulomComEnable=FALSE;
       	}

	  #endif
}


