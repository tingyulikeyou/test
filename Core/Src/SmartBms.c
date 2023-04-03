#include "main.h"



SBMS_CMD_TypeDef g_sbms_cmd;
uint8_t g_sbms_infor[64];
uint8_t g_sbms_tx_buf[256];
uint16_t g_handCounter=0;

uint8_t g_handCmd[]={0xC5,0x6A,0x29,0x06,0x04,0xAA};

extern BQ40Z50_TypeDef g_bq40z50_state;

extern __IO  uint8_t g_Uart485Buf[UART485_RX_BUF_SIZE];

extern uint16_t  RxUart3Counter;
extern uint16_t  RxUart3ParsePos;


extern UART_HandleTypeDef huart3;
extern BQ40Z50_TypeDef g_bq40z50_state;
extern __IO uint8_t g_BmsGetInfor_Enable;

void SmartBmsInit(void)
{
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	huart3.RxXferCount=0;
	huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf;
	
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);

}

void Smart485Send(uint8_t *buffer,uint16_t size)
{
	Uart3Send(buffer,size);
}



void SbmsWakeupRelease(void)
{
	memset(g_sbms_tx_buf,0x00,256);
	//HAL_UART_Transmit(&huart1,g_sbms_tx_buf,64,1000);
	Smart485Send(g_sbms_tx_buf,64);
	HAL_Delay(10);
	//HAL_UART_Transmit(&huart1,g_handCmd,6,200);
	Smart485Send(g_sbms_tx_buf,6);
}
uint8_t SBms_GetInfo(void)
{
    uint8_t  error_cnt=0;

    g_sbms_cmd.header=0x3a;		
    g_sbms_cmd.addr=0x00;
    g_sbms_cmd.cmd=0x02;
    g_sbms_cmd.ver=0x00;
    g_sbms_cmd.len=0x0e;
    g_sbms_cmd.infor=NULL;
    g_sbms_cmd.end=0x7E;

    SBmsCmdConvert(g_sbms_cmd,g_sbms_tx_buf);

    //memset(g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
    //huart1.RxXferCount=UART485_RX_BUF_SIZE;
    //huart1.pRxBuffPtr=g_Uart485Buf; 

	//HAL_UART_Abort(&huart1);
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
    huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.RxXferCount=0;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
	
    //HAL_UART_Transmit(&huart1,g_sbms_tx_buf,strlen((char*)g_sbms_tx_buf),200);
    Smart485Send(g_sbms_tx_buf,strlen((char*)g_sbms_tx_buf));
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);
    return error_cnt;	
}

void HextoHexAscii(uint8_t hex,uint8_t*hexAscii)
{
	uint8_t temp=hex>>4;
	
	if(temp<=9)
		hexAscii[0]=temp+'0';
	else
		hexAscii[0]=temp-10+'A';

	temp=hex&0x0f;
		
	if(temp<=9)
		hexAscii[1]=temp+'0';
	else
		hexAscii[1]=temp-10+'A';
}

uint8_t HexAsciitoHex(uint8_t*hexAscii)
{
	uint8_t temp=hexAscii[0];
	uint8_t hex=0;
	
	if(temp>='0'&&temp<='9')
		hex=temp-'0';
	else
		hex=temp-'A'+0x0a;
	
	hex=hex<<4;

	temp=hexAscii[1];
		
	if(temp>='0'&&temp<='9')
		hex|=temp-'0';
	else
		hex|=temp-'A'+0x0a;

	return hex;
}


void SBmsCmdConvert(SBMS_CMD_TypeDef pack,uint8_t *cmd)
{
	uint8_t i,crc,*p;
	uint8_t len=0;

	p=cmd;
	cmd[0]= pack.header;
	crc=0;
	
	HextoHexAscii(pack.addr,&cmd[1]);	
	HextoHexAscii(pack.cmd,&cmd[3]);	
	HextoHexAscii(pack.ver,&cmd[5]);	
	HextoHexAscii(pack.len>>8,&cmd[7]);	
	HextoHexAscii(pack.len&0x00ff,&cmd[9]);	
	len=11;
	p+=11;

	if(pack.infor!=NULL)
	{
		for(i=0;i<pack.len;i++)
		{	
			HextoHexAscii(g_sbms_cmd.infor[i],&cmd[11+i*2]);
			len+=2;
			p+=2;
			}
		}

	for(i=1;i<len;i++)
		crc+=cmd[i];
	crc^=0xff;

	HextoHexAscii(crc,p);

	p+=2;
	*p=g_sbms_cmd.end;

	
}

/*uint8_t *testcmd="\
:01825200AE000000000000006AFC100D890D230CFB0D790D4F0DB10D7C0CEC0D4A0\
D270D530D970D9E0D470D8B0DA50000000005444242414200000000000000000A000\
000000000000000000000000000000001F4CC~";*/
//SBMS_CMD_TypeDef tempack;
//uint8_t *p,crc,num=0,value=0;

void SBmsTask(void)
{

	uint16_t i=0,len=0,j=0;
	uint8_t charge_cur[2];

	if(g_BmsGetInfor_Enable)
	{	
		if(g_handCounter++>5*60)
		{
			g_handCounter=0;
			if(PaygGetPayState())
				//HAL_UART_Transmit(&huart1,g_handCmd,6,200);
				SbmsWakeupRelease();
			}
		else
		{	
			SBms_GetInfo();
			}

		g_BmsGetInfor_Enable=FALSE;
		//return ;

		}

	//if(huart1.RxState==HAL_UART_STATE_READY)
	//	HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);//SmartBmsInit();
	
	RxUart3Counter=huart3.RxXferCount;

	//RxUart1Counter=20;

	//for(i=0;i<RxUart1Counter;i++)
	{
		if((g_Uart485Buf[0]==0x3a)&&(RxUart3Counter-i>6))
		{
			SBMS_CMD_TypeDef tempack;
			uint8_t *p,crc,num=0,value=0;
			uint16_t temp16=0;
			int16_t *pint16=0,int16=0;
			uint32_t charge_power=0;
			
			crc=0;
			p=(uint8_t*)&g_Uart485Buf[0+1];  // add address

			HAL_Delay(100);
			memset((uint8_t*)&tempack,0x00,sizeof(SBMS_CMD_TypeDef));

			tempack.addr=HexAsciitoHex(p);
			tempack.cmd=HexAsciitoHex(p+2);
			tempack.ver=HexAsciitoHex(p+4);
			tempack.len=HexAsciitoHex(p+6);
			tempack.len=tempack.len<<8;
			tempack.len|=HexAsciitoHex(p+8);

			len=tempack.len;

			if(len>0&&len<UART485_RX_BUF_SIZE-8)
			{	tempack.crc=HexAsciitoHex(p+len-4);
			    tempack.end=*(p+len-2);
				}

			if(tempack.end==0x7e)
			{
				for(j=0;j<len-4;j++)
				{
					crc+=p[j];
					}

				crc^=0xff;

				if(crc==tempack.crc)
				{

					//get infor data

					if(tempack.cmd==0x82)
					{

						p=(uint8_t*)&g_Uart485Buf[i+11];  // infor addr

						//date time 
						p+=14;
						//Vbat 
						g_bq40z50_state.Voltage[1]=HexAsciitoHex(p);
						p+=2;
						g_bq40z50_state.Voltage[0]=HexAsciitoHex(p);

						GattSetBattVolt((g_bq40z50_state.Voltage[1] << 8 | g_bq40z50_state.Voltage[1])*2);//BatteryVoltage


						charge_power=(g_bq40z50_state.Voltage[1] << 8 | g_bq40z50_state.Voltage[1])*2;
						//Vcell num
						p+=2;

						num=HexAsciitoHex(p);
						p+=2;

						if(num>DIA_COUNT)
							num=DIA_COUNT;
						for(j=0;j<num;j++)
						{
						     value=HexAsciitoHex(p);

							 temp16=value<<8;

						     p+=2;
						     value=HexAsciitoHex(p);
						    #ifdef E_MOB48V_PROJECT
							 temp16|=value;
							 
							 if(j<DIA_COUNT)
							 	GattSetData(LIST_DIA,j,(uint8_t*)&temp16);
							 #endif
						     p+=2;	 
							}
					  //charge current s
      					charge_cur[1]=HexAsciitoHex(p);
      					p+=2;
      					charge_cur[0]=HexAsciitoHex(p);
      					p+=2;	
					     		 

						//recharge current
						g_bq40z50_state.Current[1]=HexAsciitoHex(p);
						  p+=2;
						g_bq40z50_state.Current[0]=HexAsciitoHex(p);
						  p+=2;	

						 if(charge_cur[1]>0||charge_cur[0]>0)
						 {	g_bq40z50_state.Current[1]=charge_cur[1];
						 	g_bq40z50_state.Current[0]=charge_cur[0];
						    temp16=(g_bq40z50_state.Current[1] << 8 | g_bq40z50_state.Current[0])*10;

							charge_power=charge_power*temp16/1000/1000;
						 	}			
						 else
						  {   temp16=(g_bq40z50_state.Current[1] << 8 | g_bq40z50_state.Current[0])*10;
						 	  temp16=~temp16;
						      temp16+=1;
							  charge_power=0;
						 	}

						 GattSetBattCurrent( temp16 );//BatteryCurrent
	
						// temp num
						num=HexAsciitoHex(p);
						 p+=2;

						g_bq40z50_state.Temperature[0]=0;
						for(j=0;j<num;j++)
						{
						   value= HexAsciitoHex(p);
						   if(g_bq40z50_state.Temperature[0]>value)
						   	g_bq40z50_state.Temperature[0]=value;
						   g_bq40z50_state.Temperature[1]=0;
						     p+=2;
							}
						//Vstate
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;
						//Cstate
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;
						//Tstate
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//Alarm
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//FETState
						value=HexAsciitoHex(p);
						p+=2;

						//NUM_VOV
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//NUM_VUV
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//NUM_VARN_VHIGH
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//NUM_VARN_VLOW
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//Blance state
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//Dchg Num
						value=HexAsciitoHex(p);
						p+=2;
						value=HexAsciitoHex(p);
						p+=2;

						//Chg Num
						value=HexAsciitoHex(p);
						//g_bq40z50_state.Relative_State_of_Charge[1]=num;
						p+=2;
						value=HexAsciitoHex(p);
						//g_bq40z50_state.Relative_State_of_Charge[0]=num;
						p+=2;

						//SOC
						value=HexAsciitoHex(p);
						p+=2;
						g_bq40z50_state.Relative_State_of_Charge[0]=value;
						g_bq40z50_state.Relative_State_of_Charge[1]=0;

						//CapNow
						value=HexAsciitoHex(p);
						g_bq40z50_state.Remaning_Capacity[1]=value;
						p+=2;
						value=HexAsciitoHex(p);
						g_bq40z50_state.Remaning_Capacity[0]=value;
						p+=2;

						//CapFull
						value=HexAsciitoHex(p);
						g_bq40z50_state.Full_charge_Capacity[1]=value;
						p+=2;
						value=HexAsciitoHex(p);
						g_bq40z50_state.Full_charge_Capacity[0]=value;
						p+=2;

						
						GattSetSysStatusCode( 0);//System_Status_Code
						GattSetRelativeSOC( g_bq40z50_state.Relative_State_of_Charge[1] << 8 | g_bq40z50_state.Relative_State_of_Charge[0]);//Relative_SOC
						GattSetRmCap((g_bq40z50_state.Remaning_Capacity[1] << 8 | g_bq40z50_state.Remaning_Capacity[0])/10 );//Remaining_Capacity
						GattSetFullChrgeCap( (g_bq40z50_state.Full_charge_Capacity[1] << 8 | g_bq40z50_state.Full_charge_Capacity[0])/10);//Full_Charge_Capacity
						
						GattSetAccuCyc( g_bq40z50_state.Cycle_Count[1] << 8 | g_bq40z50_state.Cycle_Count[0] );//Accu_Cycles
						GattSetChargePower( charge_power);//Charge_Power
						
						//GattSetAccuEnergy(g_bq40z50_state.a[1] << 8 | g_bq40z50_state.CV1[0] );//Accu_Energy_Output
						//GattSetBattVolt((g_bq40z50_state.Voltage[1] << 8 | g_bq40z50_state.Voltage[1])*2);//BatteryVoltage
						//GattSetBattCurrent( g_bq40z50_state.Current[1] << 8 | g_bq40z50_state.Current[0] );//BatteryCurrent


						//GattSetAccuEnergy(g_bq40z50_state.a[1] << 8 | g_bq40z50_state.CV1[0] );
						//GattSetCellVolt1(g_bq40z50_state.CV1[1] << 8 | g_bq40z50_state.CV1[0] );
						//GattSetCellVolt2(g_bq40z50_state.CV2[1] << 8 | g_bq40z50_state.CV2[0] );
						//GattSetCellVolt3(g_bq40z50_state.CV3[1] << 8 | g_bq40z50_state.CV3[0] );
						//GattSetCellVolt4(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );

						int16=g_bq40z50_state.Temperature[0]-40;
						
						GattSetBattTemp((uint16_t)int16);

						memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
						
						}

					
					}
				}
			


			}

			 
			}


}








