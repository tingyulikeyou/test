#include "main.h"


PUMP_STATE_TypeDef g_PumpState;


#ifdef DC_PUMP_SUPPORT
__IO PUMP_CMD_TypeDef g_PumpCmd=PUMP_NONE;
/*const uint8_t pump_cmd_rpm[]={0x02,0xff,0x00,0x23,0x01,0x00,0x23,0x03};
const uint8_t pump_cmd_power[]={0x02,0xff,0x00,0x25,0x01,0x00,0x23,0x03};
const uint8_t pump_cmd_waterlevel[]={0x02,0xff,0x00,0x29,0x01,0x00,0x23,0x03};
const uint8_t pump_cmd_ledstatel[]={0x02,0xff,0x00,0x2b,0x01,0x00,0x23,0x03};
const uint8_t pump_cmd_ledstatel[]={0x02,0xff,0x00,0x2b,0x01,0x00,0x23,0x03};*/
const uint8_t pump_ack_header[3]={0x02,0x00,0xff};
uint8_t g_waterFull=FALSE;
uint8_t g_waterEmpty=FALSE;
uint8_t g_PumpComCounter=0;


LED_STATE_TypeDef g_PumpLedState;

uint16_t g_MotorSpeed=0;
uint16_t g_MotorCurrent=0;
uint16_t g_MotorPower=0;

#else
__IO PUMP_CMD_TypeDef g_PumpCmd=PUMP_SET_MODUBUS;
unsigned int crc_modbus = 0;
unsigned char pump_powertime[8]  =      {0x01,0x03,0xF7,0x0D,0x00,0x01,0x27,0xBD};
unsigned char pump_powertimeover[8]  =  {0x01,0x03,0xF8,0x10,0x00,0x01,0xB4,0xAF};
unsigned char pump_on[8]         =      {0x01,0x06,0x20,0x00,0x00,0x01,0x43,0xCA};
unsigned char pump_off[8]        =      {0x01,0x06,0x20,0x00,0x00,0x05,0x42,0x09};
unsigned char pump_rst[8]        =      {0x01,0x06,0x20,0x00,0x00,0x07,0xC3,0xC8};
unsigned char pump_free[8]       =      {0x01,0x06,0xf8,0x10,0x00,0x00,0xB9,0x6F};

unsigned char pump_state[8]       =     {0x01,0x03,0x10,0x01,0x00,0x07,0x51,0x08};

unsigned char pump_checkRun[8]    =     {0x01,0x03,0x30,0x00,0x00,0x01,0x8B,0x0A};

unsigned char pump_SetModbus[8]  ={0x01,0x06,0xf0,0x02,0x00,0x02,0x9a,0xcb};


const uint8_t PumpCmd_OCS[]=                             {0xC5,0x6a,0x29,0x06,0x0a,0xb5};
const uint8_t PumpCmd_OCSRcvOK[]=                        {0xc5,0x6a,0x29};

unsigned char Pump_SetPowerCmd[8];
unsigned int Pump_PowerTime = 0;
unsigned int Pump_PowerTimeOver = 0;

#endif
uint8_t payg_enable = 1;
uint8_t g_PumpComEnable;
uint8_t g_PumpUserSet=0;

/*
01 06 f8 10 00 00 B9 6F   FREE
01 06 20 00 00 01 43 CA  ON 
01 06 20 00 00 05 42 09  OFF
01 03 10 01 00 07 51 08   STATE
01 03 30 00 00 01 8B 0A   RUN CHECK
01 06 f0 02 00 02 9a cb  SET MODBUS
C5 6a 29 06 0a b5   OCS
01 03 F7 0D 00 01 27 BD  POWER TIME
01 03 F8 10 00 01 B4 AF  POWER OVER TIME
*/


extern __IO  uint8_t g_Uart485Buf[UART485_RX_BUF_SIZE];

extern uint16_t  RxUart3Counter;
extern uint16_t  RxUart3ParsePos;


extern UART_HandleTypeDef huart3;

#ifdef DC_PUMP_SUPPORT

uint16_t Crc_Cal(uint16_t len, uint8_t * rxbuff)
{
   uint16_t i=0;
   uint16_t crc=0xffff;
   uint16_t src=0;
   
   while(len--)
   {
   	  src=*rxbuff;
      crc^=src;
      for(i=0;i<8;i++)
      {
        if(crc&0x01)
			crc=(crc>>1)^0xA001;
      else
 			crc=crc>>1;
      }
      rxbuff++;
	}

   return crc;
}

void PumpLedShow(LED_TypeDef led,uint8_t on )
{
	/*switch(led)
	{
		case LED_EER_I:
			HAL_GPIO_WritePin(EER_I_GPIO_Port,EER_I_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_TANK_F:
			HAL_GPIO_WritePin(TANK_F_GPIO_Port,TANK_F_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_WELL_L:
			HAL_GPIO_WritePin(WELL_L_GPIO_Port,WELL_L_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_MPPT:
			HAL_GPIO_WritePin(MPPT_GPIO_Port,MPPT_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_LOW_P:
			HAL_GPIO_WritePin(LOW_P_GPIO_Port,LOW_P_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_PUMP:
			HAL_GPIO_WritePin(PUMP_GPIO_Port,PUMP_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_SOL_AR:
			HAL_GPIO_WritePin(SOL_AR_GPIO_Port,SOL_AR_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		case LED_ALL:
			HAL_GPIO_WritePin(EER_I_GPIO_Port,EER_I_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(TANK_F_GPIO_Port,TANK_F_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(WELL_L_GPIO_Port,WELL_L_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(MPPT_GPIO_Port,MPPT_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(LOW_P_GPIO_Port,LOW_P_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(PUMP_GPIO_Port,PUMP_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			HAL_GPIO_WritePin(SOL_AR_GPIO_Port,SOL_AR_Pin,on?GPIO_PIN_RESET:GPIO_PIN_SET);
			break;
		}*/
}

void PumpCmdMerge(uint8_t cmd ) 
{
	PUMP_PACKET_TypeDef packet;
	uint8_t i=0,*p=&packet.device;

	uint16_t checksum=0;

	packet.header=0X02;
	packet.device=0xff;
	packet.target=0x00;
	packet.cmd=0x01;
	packet.end=0x03;
	
	switch(cmd)
	{
	 	case PUMP_NONE:
			break;
		case PUMP_GET_RMP:
			packet.addr=0x23;
			packet.data=0x00;
			break;
		case PUMP_GET_POWER:
			packet.addr=0x25;
			packet.data=0x00;
			break;
		case PUMP_GET_WATERLEVEL:
			packet.addr=0x29;
			packet.data=0x00;
			break;
		case PUMP_GET_LED_STATUS:
			packet.addr=0x2b;
			packet.data=0x00;
			break;
		case PUMP_SET_ON:
			packet.addr=0x28;
			packet.data=0x01;
			break;
		case PUMP_SET_OFF:
			packet.addr=0x28;
			packet.data=0x00;
			break;
		case PUMP_SET_LOCK:
			packet.addr=0x2a;
			packet.data=0x01;
			break;
		case PUMP_SET_UNLOCK:
			packet.addr=0x2a;
			packet.data=0x00;
			break;	
		}

	//for(i=0;i<5;i++)
		//checksum+=p[i];



	//packet.checksum=checksum&0x00ff;
	checksum=Crc_Cal(5, p);
	packet.checksum[1]=checksum>>8;
	packet.checksum[0]=checksum;

	if(cmd!=PUMP_NONE)
		PumpSend((const uint8_t*)&packet,sizeof(PUMP_PACKET_TypeDef)-3);
	
}
#endif

void PumInit(void)
{
	   memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	   //HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);


	   HAL_Delay(10);

	   #ifdef DC_PUMP_SUPPORT
	   HAL_Delay(1000);
	   PumpTxCmd(PUMP_SET_LOCK);

	   if(PaygGetPayState())
	   		g_PumpUserSet=1;
	   #else
	   PumpTxCmd(PUMP_SET_MODUBUS);
	   HAL_Delay(50);
	   PumpTxCmd(PUMP_RESET);
	   HAL_Delay(50);
	   if(PaygGetPayState()/*==YES||1*/)
	   {
	   	PumpTxCmd(PUMP_ON);
	  	 HAL_Delay(50);
		 payg_enable=1;
	   }
	   #endif

	   PumpComStart();
	   
}
 

void PumpSend(const uint8_t * buffer,uint16_t size)
{
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.RxXferCount=0;
	huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf;
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);
	HAL_Delay(5);
	//HAL_UART_Transmit(&huart1,(uint8_t *)buffer,size,size*10);
	Uart3Send((uint8_t *)buffer,size);
}

void PumpComEnable(void)
{
	g_PumpComEnable=TRUE;
}

void PumpOn(uint8_t on)
{
	g_PumpUserSet=on;
}

void PumpComStart(void)
{
	#ifdef DC_PUMP_SUPPORT
	g_PumpCmd=PUMP_GET_RMP;
	#else
	g_PumpCmd=PUMP_PWR_TIME;
	#endif
}

uint8_t  BuffCompare(uint8_t *src,uint8_t *dst,uint8_t size)
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
}


void PumpTxCmd(uint8_t cmd)
{
	 #ifdef DC_PUMP_SUPPORT

	 PumpCmdMerge(cmd);

	 #else
	switch(cmd)
	{
		//idle 
		case PUMP_NONE:
			break;
		case PUMP_SET_MODUBUS:
			PumpSend(pump_SetModbus,8);
			break;
		case PUMP_PWR_TIME:
			PumpSend(pump_powertime,8);
			break;
		case PUMP_PWR_OV_TIME:
			PumpSend(pump_powertimeover,8);
			break;
		case PUMP_GET_STATUS:
			PumpSend(pump_state,8);
			break;
		case PUMP_CHECK_RUN:
			PumpSend(pump_checkRun,8);
			break;
		case PUMP_SET_PWR_CMD:
			
			  if(payg_enable == 1)
			  {
			    if(Pump_PowerTimeOver > Pump_PowerTime + 1)
			    {
			      break;
			    }
			    else
			    {
				      Pump_PowerTimeOver = Pump_PowerTime + 2;
				      Pump_SetPowerCmd[0] = 0x01;
				      Pump_SetPowerCmd[1] = 0x06;
				      Pump_SetPowerCmd[2] = 0xf8;
				      Pump_SetPowerCmd[3] = 0x10;
				      
				      Pump_SetPowerCmd[4] = Pump_PowerTimeOver >>8;
				      Pump_SetPowerCmd[5] = Pump_PowerTimeOver;
				      
				      Pump_SetPowerCmd[6] = crc_chk_value(Pump_SetPowerCmd, 6);
				      Pump_SetPowerCmd[7] = crc_chk_value(Pump_SetPowerCmd, 6) >> 8;
				      
				    //  Omnivoltaic_PumpCommand(Pump_SetPowerCmd,8,Pump_SetPowerCmd,20);
				     // Omnivoltaic_PumpCommand(pump_rst,  8, pump_rst, 20);
				//      Omnivoltaic_PumpCommand(pump_on, 8, pump_on, 20);
					PumpSend(Pump_SetPowerCmd,8);
					HAL_Delay(50);
					PumpSend(pump_rst,8);
				 
					//PumpSend(pump_rst,8);
			    }
			  }
 			 else
			  {
			    if((Pump_PowerTimeOver > Pump_PowerTime) || (Pump_PowerTimeOver == 0))
				    {
				      Pump_PowerTimeOver = Pump_PowerTime;
				      
				      Pump_SetPowerCmd[0] = 0x01;
				      Pump_SetPowerCmd[1] = 0x06;
				      Pump_SetPowerCmd[2] = 0xf8;
				      Pump_SetPowerCmd[3] = 0x10;
				      
				      Pump_SetPowerCmd[4] = Pump_PowerTimeOver >>8;
				      Pump_SetPowerCmd[5] = Pump_PowerTimeOver;
				      
				      Pump_SetPowerCmd[6] = crc_chk_value(Pump_SetPowerCmd, 6);
				      Pump_SetPowerCmd[7] = crc_chk_value(Pump_SetPowerCmd, 6) >> 8;

					PumpSend(Pump_SetPowerCmd,8);
					HAL_Delay(50);
					PumpSend(pump_rst,8);
					 
			    }
			    else
			    {
			     		break;// SYS_CHECK;
			    	}
 			 	}
				break;
			case PUMP_RESET:
				PumpSend(pump_rst,8);
				break;
			case PUMP_ON:
				PumpSend(pump_on,8);
				break;
			case PUMP_OFF:
				PumpSend(pump_off,8);
				break;
			case PUMP_FREE:
				PumpSend(pump_free,8);
				break;	
					
			}
	#endif
}


unsigned int PumpParseModbus(unsigned char *buf)
{
  unsigned int crc = 0,temp = 0;

/*  if(buf[0] == 0x01)
  {
  	crc=buf[3+buf[3]+1] << 8 | buf[3+buf[3]];

	if(crc_chk_value(buf, buf[2]+3) == crc)
		return TRUE;
	else
		return FALSE;
	
  	}*/

  if(buf[0] == 0x01)
  {
    crc = buf[6] << 8 | buf[5];
    if(crc_chk_value(buf, 5) == crc)
    {
      temp = buf[3] << 8 | buf[4];
      return temp;
    }
    else
    {
      return 0xffff;
    }
  }
  else
  {
    return 0xffff;
  }
}

unsigned int PumpParseModbusState(unsigned char *buf)
{
  unsigned int crc = 0,temp = 0;

  if(buf[0] == 0x01)
  {
    crc = buf[18] << 8 | buf[17];
    if(crc_chk_value(buf, 17) == crc)
    {
      temp = buf[3] << 8 | buf[4];
      g_PumpState.mFrq = buf[3] << 8 | buf[4];
      g_PumpState.busV = buf[5] << 8 | buf[6];
      g_PumpState.outV = buf[7] << 8 | buf[8];
      g_PumpState.outC = buf[9] << 8 | buf[10];
      g_PumpState.outP = buf[11] << 8 | buf[12];
      g_PumpState.mTrq = buf[13] << 8 | buf[14];
      g_PumpState.mSpd = buf[14] << 8 | buf[16];

	#ifdef PUMP_PROJECT
        GattSetPumpMfrqSet( g_PumpState.mFrq );
        GattSetPumpOutputPwr(  g_PumpState.outP);
        GattSetPumpOutputVolt( g_PumpState.outV);
        GattSetPumpInputVolt( g_PumpState.busV);
        GattSetPumpOutC( g_PumpState.outC);
        GattSetPumpMTorque (  g_PumpState.mTrq);
        GattSetPumpMSpeed(  g_PumpState.mSpd );
        //GattSetPumpMTemp( mtmp);
        #endif
	 
      return temp;
    }
    else
    {
      return 0xffff;
    }
  }
  else
  {
    return 0xffff;
  }
}
uint8_t  PumpCmdParse(uint8_t*buffer)
{
	uint16_t temp=0;
	uint8_t i=0;

	#ifdef DC_PUMP_SUPPORT
	switch(buffer[3])
	{
		case 0x23:   // rpm
			if(buffer[4])
			{
				GattSetPumpRunState( buffer[6]);
				GattSetPumpMSpeed( 0);
				GattSetPumpOutC(0);

				g_MotorSpeed=0;
				}
			else
			{	 GattSetPumpMSpeed( buffer[6] <<8|buffer[5]);
				 GattSetPumpOutC( buffer[8] <<8|buffer[7]);
			
				 g_MotorSpeed=(buffer[6] <<8|buffer[5]);

				 g_MotorCurrent=( buffer[8] <<8|buffer[7]);
				}

			g_PumpCmd=PUMP_GET_POWER;
			break;
		case 0x25:   //power
			/*if(buffer[4]<=1)
				GattSetPumpMTemp(buffer[6]);
			else
				GattSetPumpMTemp(buffer[6] <<8|buffer[5]);*/

			g_MotorPower=(buffer[5] <<8|buffer[4]);

			if(PaygGetFreeState())
				g_PumpCmd=PUMP_SET_UNLOCK;//PUMP_GET_WATERLEVEL;
			else
				g_PumpCmd=PUMP_SET_LOCK;//PUMP_GET_WATERLEVEL;
			//g_PumpCmd=PUMP_GET_WATERLEVEL; 
			break;
		case 0x28:  //on /off
			if(buffer[5])
				GattSetPumpRunState(TRUE);
			else
				GattSetPumpRunState(FALSE);
			break;	
		case 0x29:  //water level
			if(buffer[5])
				g_waterEmpty=TRUE;
			else
				g_waterEmpty=FALSE;

			if(buffer[6])
				g_waterFull=TRUE;
			else
				g_waterFull=FALSE;
			g_PumpCmd=PUMP_GET_LED_STATUS;	
			break;
		case 0x2a:  //hand
			g_PumpCmd=PUMP_GET_LED_STATUS;
			break;	
		case 0x2b:  //led
			g_PumpLedState.ledstate=buffer[5];
			g_PumpCmd=PUMP_NONE;	
		
			temp=g_PumpLedState.ledbitstate.tank_state;
			GattSetData(LIST_DTA,DTA_TNKF,(uint8_t*)&temp);
			temp=g_PumpLedState.ledbitstate.well_state;
			GattSetData(LIST_DTA,DTA_WLLE,(uint8_t*)&temp);
			
			//PumpTxCmd(PUMP_SET_UNLOCK);
			HAL_Delay(500);
			if(g_PumpUserSet&&PaygGetPayState())
				PumpTxCmd(PUMP_SET_ON);
			else
				PumpTxCmd(PUMP_SET_OFF);

			HAL_Delay(300);
			//PumpTxCmd(PUMP_SET_LOCK);
			PumpComStart();
			break;
		}

	#else
	
	switch(g_PumpCmd)
	{
		case PUMP_NONE:
			break;
		case PUMP_SET_MODUBUS:
			if(BuffCompare(buffer,pump_SetModbus,8))
			{	
				g_PumpCmd++;
				}
			else
				temp=0xffff;
			break;
		case PUMP_PWR_TIME:
			Pump_PowerTime=PumpParseModbus(buffer);
			temp=Pump_PowerTime;
			if(temp!=0xffff)
				g_PumpCmd++;
			break;
		case PUMP_PWR_OV_TIME:
			Pump_PowerTimeOver=PumpParseModbus(buffer);
			temp=Pump_PowerTimeOver;
			if(temp!=0xffff)
				g_PumpCmd++;
			break;
		case PUMP_GET_STATUS:
			temp=PumpParseModbusState(buffer);
			if(temp!=0xffff)
				g_PumpCmd++;
			break;
		case PUMP_CHECK_RUN:
			g_PumpState.runS =PumpParseModbus(buffer);
			temp=g_PumpState.runS;
			if(temp!=0xffff)
			{	g_PumpCmd++;
				#ifdef PUMP_PROJECT
				GattSetPumpRunState( g_PumpState.runS);
				#endif
				}
			break;
		case PUMP_SET_PWR_CMD:
			if(BuffCompare(buffer,Pump_SetPowerCmd,8))
				g_PumpCmd++;
			else
				temp=0xffff;
			break;
		case PUMP_RESET:
			if(BuffCompare(buffer,pump_rst,8))
				g_PumpCmd=PUMP_PWR_TIME;
			else
				temp=0xffff;
			break;
		case PUMP_ON:
			if(BuffCompare(buffer,pump_on,8))
				g_PumpCmd=PUMP_NONE;
			else
				temp=0xffff;
			break;	
		case PUMP_OFF:
			if(BuffCompare(buffer,pump_off,8))
				g_PumpCmd=PUMP_NONE;
			else
				temp=0xffff;
			break;	
		case PUMP_FREE:
			if(BuffCompare(buffer,pump_off,8))
				g_PumpCmd=PUMP_NONE;
			else
				temp=0xffff;
			break;	
					
		}

	#endif

	if(temp==0xffff)
		return 0;
	else
		return 1;
}
	


uint16_t crc_chk_value(unsigned char *data_value,unsigned char length)
{
  unsigned int crc_value = 0xffff;
  int i;
  while(length --)
  {
    crc_value ^= *data_value++;
    for( i = 0; i < 8; i++)
    {
      if(crc_value & 0x0001)
      {
        crc_value = (crc_value >> 1) ^ 0xa001;
      }
      else
      {
        crc_value = crc_value >> 1;
      }
    }
  }
  return crc_value;
}

void PumpProc(void)
{
	uint16_t i,len=0;
	
	RxUart3Counter=huart3.RxXferCount;



	#ifdef DC_PUMP_SUPPORT

	if(huart1.RxState==HAL_UART_STATE_READY)
		PumInit();

	for(i=0;i<RxUart3Counter;i++)
	{	
		if(BuffCompare(&g_Uart485Buf[i],(uint8_t*)pump_ack_header,3))
		{

			HAL_Delay(10);
			
			PumpCmdParse(&g_Uart485Buf[i]);

			g_PumpComCounter=0;
		
			memset(g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			huart3.RxXferCount=0;
			huart3.RxXferSize=UART485_RX_BUF_SIZE;
			huart3.pRxBuffPtr=g_Uart485Buf; 
			}
		}


	PumpLedShow(LED_EER_I,g_PumpLedState.ledbitstate.fault_state);
	PumpLedShow(LED_TANK_F,g_PumpLedState.ledbitstate.tank_state);
	PumpLedShow(LED_WELL_L,g_PumpLedState.ledbitstate.well_state);
	PumpLedShow(LED_MPPT,g_PumpLedState.ledbitstate.MPPT_state);
	PumpLedShow(LED_LOW_P,g_PumpLedState.ledbitstate.lowpress_state);
	PumpLedShow(LED_PUMP,g_PumpLedState.ledbitstate.pump_run_state);
	PumpLedShow(LED_SOL_AR,g_PumpLedState.ledbitstate.dc_state);

	#else

	//for(i=0;i<RxUart1Counter;i++)
	{
		if(RxUart3Counter>6)
		{
			HAL_Delay(10);
			PumpCmdParse((uint8_t*)g_Uart485Buf);
		
			memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			huart3.RxXferCount=0;
			huart3.RxXferSize=UART485_RX_BUF_SIZE;
			huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
			
			}
		}
	#endif

       #ifdef PUMP_TEST
	/* GattSetPumpMfrqSet( 50);
        GattSetPumpOutputPwr( 1);
        GattSetPumpOutputVolt( 380);
        GattSetPumpInputVolt(400);
        GattSetPumpOutC( 2);
        GattSetPumpMTorque ( 1000);
        GattSetPumpMSpeed( 3189 );
        GattSetOutputCtrlState( 1);
	GattSetSysStatusCode(1);
	GattSetPumpMTemp( 45);
	GattSetPumpRunState(3120);*/
	#endif
	

	 if(PaygGetPayState())
	  {
	   	if(payg_enable)
	   	{	
	   		#ifdef DC_PUMP_SUPPORT
			#else
	   		PumpTxCmd(PUMP_RESET);
			#endif
	  	 	HAL_Delay(50);
			 payg_enable=FALSE;
	   		}
	   }
	
       if(g_PumpComEnable)
       	{
       		
			PumpTxCmd(g_PumpCmd);
			g_PumpComEnable=FALSE;

			#ifdef DC_PUMP_SUPPORT
			if(g_PumpComCounter<20)
				g_PumpComCounter++;
			else
			{	g_PumpLedState.ledstate=0x00;
				g_MotorSpeed=0;
			    g_MotorPower=0;
				g_MotorCurrent=0;
				}
			#endif
       	}
}


