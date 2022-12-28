#include "main.h"


uint8_t g_run_protect=0;

uint8_t g_jbsbms_tx_buf[16];

uint8_t g_JbsBmsCmd=JBS_CMD_INFOR;

uint8_t g_jbdCmd_Factorymode[]={0xDD,0x5A,0x00,0x02,0x56,0x78,0xFF,0x30,0x77};
uint8_t g_jbdCmd_FETCtrlTime[]={0xDD,0x5A,0x30,0x02,0x02,0x58,0xFF,0x74,0x77};
uint8_t g_jbdCmd_FETCtrl5STime[]={0xDD,0x5A,0x30,0x02,0x00,0x05,0xFF,0xC9,0x77};
uint8_t g_jbdCmd_FETCtrl20MinTime[]={0xDD,0x5A,0x30,0x02,0x04,0xB0,0xFF,0x1A,0x77};
uint8_t g_jbdCmd_FETCtrlFreeTime[]={0xDD,0x5A,0x30,0x02,0x00,0x00,0xFF,0xCE,0x77};
uint8_t g_jbdCmd_FETCtrlLongTime[]={0xDD,0x5A,0x30,0x02,0xFF,0xEF,0xFD,0xE0,0x77};





uint8_t g_jbdCmd_ExitFactory[]={0xDD,0x5A,0x01,0x02,0x28,0x28,0xFF,0xAD,0x77};
uint8_t g_jbdCmd_Exit0000Factory[]={0xDD,0x5A,0x01,0x02,0x00,0x00,0xFF,0xFD,0x77};

uint8_t g_charge_counter=0;
uint16_t g_jbd_watchdog_timer=0;
uint16_t g_jbd_epromWrite_timer=0;

uint16_t g_jbd_ComErr_Cnt=0;

//DD5A30020005FFC977   5S

extern BQ40Z50_TypeDef g_bq40z50_state;

extern __IO  uint8_t g_Uart485Buf[UART485_RX_BUF_SIZE];

extern uint16_t  RxUart3Counter;
extern uint16_t  RxUart3ParsePos;


extern UART_HandleTypeDef huart3;
extern BQ40Z50_TypeDef g_bq40z50_state;
extern __IO uint8_t g_BmsGetInfor_Enable;
extern SYS_STATE_CODE_TypeDef  g_sysStateCode;

void Jbd485Send(uint8_t *buffer,uint16_t size)
{
		Uart3Send(buffer,size);
}

void JbsBmsInit(void)
{
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);

	//if(huart1.RxState==HAL_UART_STATE_READY)
	//	HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);

	
	g_JbsBmsCmd=JBS_CMD_INFOR;

	//if(PaygGetFreeState())
	{
	//	HAL_UART_Transmit(&huart1,g_jbdCmd_ExitFactory,9,200);	
		}
	//else
	{
		//HAL_UART_Transmit(&huart1,g_jbdCmd_Factorymode,9,200);	

		//HAL_Delay(200);
		
		if(PaygGetPayState()||PaygGetFreeState())
		{	//HAL_UART_Transmit(&huart1,g_jbdCmd_Factorymode,9,200);
			Jbd485Send(g_jbdCmd_Factorymode,9);
			HAL_Delay(100);

			if(PaygGetFreeState())
				Jbd485Send(g_jbdCmd_FETCtrlFreeTime,9);
				//HAL_UART_Transmit(&huart1,/*g_jbdCmd_FETCtrlTime*/g_jbdCmd_FETCtrlFreeTime,9,200);	
			else
				//HAL_UART_Transmit(&huart1,/*g_jbdCmd_FETCtrlTime*/g_jbdCmd_FETCtrlLongTime,9,200);
				Jbd485Send(g_jbdCmd_FETCtrlLongTime,9);
			HAL_Delay(100);
			//HAL_UART_Transmit(&huart1,g_jbdCmd_ExitFactory,9,200);
			Jbd485Send(g_jbdCmd_ExitFactory,9);
			HAL_Delay(100);

			g_run_protect=TRUE;
			}
		/*else
		{	HAL_UART_Transmit(&huart1,g_jbdCmd_FETCtrl5STime,9,200);
			}*/
		}

}


uint16_t JbsBmsChecksum(uint8_t *cmd,uint8_t size)
{
	uint8_t i=0;
	uint16_t checksum=0;

	for(i=0;i<size;i++)
		checksum+=cmd[i];

	checksum=(~checksum)+1;

	return checksum ;
}

void JbsMosCtrl(uint8_t on)
{
	uint16_t chksum=0;

	memset(g_jbsbms_tx_buf,0X00,16);
	
	g_jbsbms_tx_buf[0]=0xdd;
	g_jbsbms_tx_buf[1]=0x5a;
    g_jbsbms_tx_buf[2]=0xe1;
	g_jbsbms_tx_buf[3]=2;
	g_jbsbms_tx_buf[4]=00;

	if(on)
		g_jbsbms_tx_buf[5]=0x00;  //out
	else	
		g_jbsbms_tx_buf[5]=0x02;  //out

	chksum=JbsBmsChecksum(g_jbsbms_tx_buf,2);


	g_jbsbms_tx_buf[6]=chksum>>8;  //checksun high byte
	g_jbsbms_tx_buf[7]=chksum;  //checksun low byte


	g_jbsbms_tx_buf[8]=0x77; 

	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	huart3.RxXferSize=UART485_RX_BUF_SIZE;
    huart3.RxXferCount=0;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
	
    //HAL_UART_Transmit(&huart1,g_jbsbms_tx_buf,9,200);	
    Jbd485Send(g_jbsbms_tx_buf,9);
}


void JbsBms_GetInfo(uint8_t cmd)
{
    uint8_t  error_cnt=0;
	uint16_t chksum=0;

	memset(g_jbsbms_tx_buf,0X00,16);


	g_jbsbms_tx_buf[0]=0xdd;
	g_jbsbms_tx_buf[1]=0xa5;
    g_jbsbms_tx_buf[2]=cmd;
	g_jbsbms_tx_buf[3]=0;

	chksum=JbsBmsChecksum(&g_jbsbms_tx_buf[2],2);
	
	g_jbsbms_tx_buf[4]=chksum>>8;  //checksun high byte
	g_jbsbms_tx_buf[5]=chksum;  //checksun low byte

	g_jbsbms_tx_buf[6]=0x77;

	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
    huart3.RxXferCount=0;
	huart3.RxXferSize=UART485_RX_BUF_SIZE;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 

	g_jbd_watchdog_timer++;

	if(g_jbd_watchdog_timer<300u)
	{	
		if(cmd>=JBS_CMD_FACTOR)  //小于15分钟，不写BMS watchdog
		{	
			g_JbsBmsCmd=JBS_CMD_INFOR;
			return ;
			}
		}

	 if(cmd==JBS_CMD_LED)
    {	memset(g_jbsbms_tx_buf,0X00,16);
    	sprintf((char*)g_jbsbms_tx_buf,"$001,%d#",GattGetRelativeSOC());
		//HAL_UART_Transmit(&huart1,g_jbsbms_tx_buf,strlen((char*)g_jbsbms_tx_buf),200);	
		Jbd485Send(g_jbsbms_tx_buf,strlen((char*)g_jbsbms_tx_buf));
		return ;
		}

	if(cmd>=JBS_CMD_FACTOR) //15 min
	{
		if(PaygGetPayState()||PaygGetFreeState()||g_run_protect)
		{	
			if(cmd==JBS_CMD_FACTOR)
				//HAL_UART_Transmit(&huart1,g_jbdCmd_Factorymode,9,200);
				Jbd485Send(g_jbdCmd_Factorymode,9);
			if(cmd==JBS_CMD_FET)
			{	
				if(PaygGetFreeState())
					//HAL_UART_Transmit(&huart1,g_jbdCmd_FETCtrlFreeTime,9,200);	
					Jbd485Send(g_jbdCmd_FETCtrlFreeTime,9);
				else	
					//HAL_UART_Transmit(&huart1,g_jbdCmd_FETCtrlLongTime,9,200);	
					Jbd485Send(g_jbdCmd_FETCtrlLongTime,9);
				}
			
			if(cmd==JBS_CMD_EXIT)
			{	
				g_jbd_epromWrite_timer++;
				
				if(g_jbd_epromWrite_timer<50)
					//HAL_UART_Transmit(&huart1,g_jbdCmd_Exit0000Factory,9,200);
					Jbd485Send(g_jbdCmd_Exit0000Factory,9);
				else
					//HAL_UART_Transmit(&huart1,g_jbdCmd_ExitFactory,9,200);
					Jbd485Send(g_jbdCmd_ExitFactory,9);
				
				g_jbd_epromWrite_timer=g_jbd_epromWrite_timer%50;
				
				g_jbd_watchdog_timer=0;
				}
			
			}
 
		return ;
		}
	
	{
		// HAL_UART_Transmit(&huart1,g_jbsbms_tx_buf,7,200);	
		Jbd485Send(g_jbsbms_tx_buf,7);
		}
	 
}

//uint8_t g_testbuff[]={0xDD,0x04,0x00,0x1E,0x0F,0x66,0x0F,0x63,0x0F,0x63,0x0F,0x64,0x0F,0x3E,0x0F,0x63,0x0F,0x37,0x0F,0x5B,0x0F,0x65,0x0F,0x3B,0x0F,0x63,0x0F,0x63,0x0F,0x3C,0x0F,0x66,0x0F,0x3D,0xF9,0xF9,0x77};


void JbsBmsTask(void)
{

	uint16_t i=0,len=0,j=0;

	if(g_BmsGetInfor_Enable)
	{	
		JbsBms_GetInfo(g_JbsBmsCmd++);

		if(g_JbsBmsCmd>JBS_CMD_EXIT)
			g_JbsBmsCmd=JBS_CMD_INFOR;

		g_BmsGetInfor_Enable=FALSE;

		if(g_jbd_ComErr_Cnt<100)
			g_jbd_ComErr_Cnt++;

		if(g_jbd_ComErr_Cnt>20)
		{
			g_sysStateCode.error.coulom_com=1;
			}
		}

	//if(huart1.RxState==HAL_UART_STATE_READY)
	//	HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);//SmartBmsInit();
	
	RxUart3Counter=huart3.RxXferCount;


	if((PaygGetPayState()||PaygGetFreeState())&&g_run_protect==FALSE)
	{
		g_run_protect=TRUE;
		}

	//RxUart1Counter=20;
	  
	//memcpy(g_Uart485Buf,g_testbuff,37);
	//for(i=0;i<RxUart1Counter;i++)
	//{
		if((g_Uart485Buf[0]==0xdd)&&(g_Uart485Buf[1]==0x03||g_Uart485Buf[1]==0x04||g_Uart485Buf[1]==0x05))
		{
			uint8_t *p,crc,num=0,value=0,ntc=0,cmd=0;
			uint16_t temp16=0;
			int16_t *ptemp16;
			uint32_t charge_power=0;
			
			crc=0;
			p=(uint8_t*)&g_Uart485Buf[0+1];  // add address

			HAL_Delay(100);

			len=g_Uart485Buf[3];
			cmd=g_Uart485Buf[1];

	      if(g_Uart485Buf[6+len]==0x77)
		  {
		  		g_jbd_ComErr_Cnt=0;
				
				switch(cmd)
				{
					case 0x03:
						p=(uint8_t*)&g_Uart485Buf[4];

						//volt
						temp16=*p*256+*(p+1);
					    GattSetBattVolt(temp16*10);//BatteryVoltage 10mV

						charge_power=temp16*10;

						p+=2;
					    // current
					    temp16=*p*256+*(p+1);
						if(temp16>=0x8000)
						{
							ptemp16=(int16_t*)&temp16;
							*ptemp16=(*ptemp16)*10;

							GattSetBattCurrent(temp16);
							charge_power=0;
							}
						else	
					   	{	GattSetBattCurrent(temp16*10);//BatteryCurrent  1mA
					   		charge_power=charge_power*temp16*10/1000/1000;
							}

						if(PaygGetPayState()==FALSE&&PaygGetFreeState()==FALSE)
						{
							if(temp16<0x8000&&temp16>=0x0010)
							{	
								if(g_charge_counter<100)
								{	
									g_charge_counter++;

									if(g_charge_counter>5)
										g_run_protect=FALSE;
									}
								}
							else
							{
								g_charge_counter=0;
								}
							}
						else
						{
							g_charge_counter=0;
							}

						p+=2;
						//remain cap
						temp16=*p*256+*(p+1);
						GattSetRmCap(temp16/100);//Remaining_Capacity

						p+=2;

						//full cap
						temp16=*p*256+*(p+1);
						GattSetFullChrgeCap(temp16/100);//Full_Charge_Capacity

						p+=2;
						//cycle times
						temp16=*p*256+*(p+1);
						GattSetAccuCyc(temp16);//Accu_Cycles

						p+=2;
						//date 

						p+=2;
						//balance 

						p+=2;
						//bal high

						p+=2;
						//protect 

						p+=2;
						//version

						p++;
						//rsoc

						temp16=(uint16_t)*p;//*p*256+*(p+1);

						GattSetRelativeSOC(temp16);//Relative_SOC

						p++;
						//FET STATE

						if(*p==0)
						   GattSetSysStatusCode( 0);//System_Status_Code
						else
						   GattSetSysStatusCode( 1);//System_Status_Code	

						p++;

						//cell number

						p++;
						//ntc number;
						ntc=*p;

						p++;
						//ntc 
						p+=2*ntc;

						//normal cap

						
						GattSetChargePower( charge_power);//Charge_Power

						GattSetBattTemp(g_bq40z50_state.Temperature[0]);
						
							
						break;
					case 0x04:

						p=(uint8_t*)&g_Uart485Buf[4];

						for(j=0;j<len/2;j++)
						{
							temp16=p[0];
							temp16=(temp16<<8)+p[1];

							p+=2;
							
							 if(j<DIA_COUNT)
							 	GattSetData(LIST_DIA,j,(uint8_t*)&temp16);
							}
						break;
					case 0x05:
						break;
					}

				

				memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
			    huart3.RxXferCount=0;
				huart3.RxXferSize=UART485_RX_BUF_SIZE;
			    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
			}

		}
	//}


}








