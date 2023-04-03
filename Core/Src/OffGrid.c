#include "main.h"

BMS_QUERY_TypeDef  g_offgridquery;
BMS_PSET_TypeDef   g_offgridPset;

uint8_t g_OffGridUserset=TRUE;

uint8_t g_OffGridReadAddr=0;
uint8_t g_OffGridRegType=OFFGRID_INPUT_REG;


extern __IO  uint8_t g_Uart485Buf[UART485_RX_BUF_SIZE];


uint16_t  RxUart1Counter;
extern uint16_t  RxUart1ParsePos;


extern UART_HandleTypeDef huart3;
extern BQ40Z50_TypeDef g_bq40z50_state;
extern __IO uint8_t g_BmsGetInfor_Enable;


void OffGridBmsInit(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);

  /* if(EEpGetJtagState()==FALSE)
   	{
	   GPIO_InitStruct.Pin = RELAY_EN_Pin;
	   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.Pull = GPIO_PULLUP;
	   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	   HAL_GPIO_Init(RELAY_EN_GPIO_Port, &GPIO_InitStruct);
	 
	   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13, GPIO_PIN_RESET);
   	}*/
  /* else
   	{
   		HAL_Delay(5000);
		EEpSetJtag(0);
		EEpUpdateEnable();
   	}*/

	HAL_GPIO_WritePin(RELAY_EN_GPIO_Port, RELAY_EN_Pin, GPIO_PIN_RESET);


}

void OffGridBmsSend(uint8_t *buffer,uint16_t size)
{
	Uart3Send(buffer,size);
}


void OffGrid_GetHoldReg(uint16_t reg,uint16_t len)
{

	uint16_t crc=0;
	
    g_offgridquery.addr=0x01;		
    g_offgridquery.func=0x03;
    g_offgridquery.regH=reg>>8;
    g_offgridquery.regL=reg;
    g_offgridquery.lenH=len>>8;
    g_offgridquery.lenL=len;


	crc=CRC16((uint8_t*)&g_offgridquery, sizeof(g_offgridquery)-2);
    g_offgridquery.crcH=crc>>8;
    g_offgridquery.crcL=crc;

	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
    huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.RxXferCount=0;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 

	OffGridBmsSend((uint8_t*)&g_offgridquery,sizeof(g_offgridquery));
	
  //  HAL_UART_Transmit(&huart1,(uint8_t*)&g_offgridquery,sizeof(g_offgridquery),200);	
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);
}

void OffGrid_GetInfo(uint16_t reg,uint16_t len)
{

	uint16_t crc=0;
	
    g_offgridquery.addr=0x01;		
    g_offgridquery.func=0x04;
    g_offgridquery.regH=reg>>8;
    g_offgridquery.regL=reg;
    g_offgridquery.lenH=len>>8;
    g_offgridquery.lenL=len;


	crc=CRC16((uint8_t*)&g_offgridquery, sizeof(g_offgridquery)-2);
    g_offgridquery.crcH=crc>>8;
    g_offgridquery.crcL=crc;

    //memset(g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
    //huart1.RxXferCount=UART485_RX_BUF_SIZE;
    //huart1.pRxBuffPtr=g_Uart485Buf; 

	//HAL_UART_Abort(&huart1);
	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
    huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.RxXferCount=0;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
	
    //HAL_UART_Transmit(&huart1,(uint8_t*)&g_offgridquery,sizeof(g_offgridquery),200);	
    OffGridBmsSend((uint8_t*)&g_offgridquery,sizeof(g_offgridquery));
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);
}


void OffGrid_SetReg(uint16_t reg,uint16_t state)
{
	uint16_t crc=0;
	
    g_offgridPset.addr=0x01;		
    g_offgridPset.func=0x06;
    g_offgridPset.regH=reg>>8;
    g_offgridPset.regL=reg;

	if(reg==0x0000)
	{
		if(g_OffGridUserset)
		{
			g_offgridPset.dataH=0x00;
		    g_offgridPset.dataL=0x00;
			}
		else
		{
			g_offgridPset.dataH=0x01;
		    g_offgridPset.dataL=0x00;
			}
		}
	else
	{
		g_offgridPset.dataH=state>>8;
		g_offgridPset.dataL=state;
		}

	crc=CRC16((uint8_t*)&g_offgridPset, sizeof(g_offgridPset)-2);
    g_offgridPset.crcH=crc>>8;
    g_offgridPset.crcL=crc;


	memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
	
    huart3.RxXferSize=UART485_RX_BUF_SIZE;
	huart3.RxXferCount=0;
    huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 
	
  //  HAL_UART_Transmit(&huart1,(uint8_t*)&g_offgridPset,sizeof(g_offgridPset),200);	
   OffGridBmsSend((uint8_t*)&g_offgridPset,sizeof(g_offgridPset));
}




void OffGridBmsTask(void)
{
	#ifdef BMS_OFFGRID_SUPPROT
	uint16_t i=0,len=0,j=0;
	static uint8_t offgridset=TRUE;
	uint16_t temp16;
	uint16_t temp32;
	uint8_t *p_reg=NULL;


	if(g_BmsGetInfor_Enable)
	{	
		if(offgridset!=g_OffGridUserset)
		{	
			OffGrid_SetReg(0x00,0);
			offgridset=g_OffGridUserset;
			}

		if(PaygGetPayState()||PaygGetFreeState())
		{
			g_OffGridUserset=1;

			//if(EEpGetJtagState()==FALSE)
				HAL_GPIO_WritePin(RELAY_EN_GPIO_Port,RELAY_EN_Pin, GPIO_PIN_SET);
			}
		else
		{
			g_OffGridUserset=0;
			//if(EEpGetJtagState()==FALSE)
				HAL_GPIO_WritePin(RELAY_EN_GPIO_Port,RELAY_EN_Pin, GPIO_PIN_RESET);
			}

		if(g_OffGridRegType==OFFGRID_INPUT_REG)
		{
			if(g_OffGridReadAddr==80)
				OffGrid_GetInfo(g_OffGridReadAddr,27);
			else
				OffGrid_GetInfo(g_OffGridReadAddr,40);

			g_OffGridReadAddr+=40;

			if(g_OffGridReadAddr>=107)
				g_OffGridReadAddr=0;
			}
		else
		{
			OffGrid_GetHoldReg(g_OffGridReadAddr,40);

			g_OffGridReadAddr+=40;

			if(g_OffGridReadAddr>=80)
			{	g_OffGridReadAddr=0;
				}
			}
		

		g_BmsGetInfor_Enable=FALSE;

		}

	//if(huart1.RxState==HAL_UART_STATE_READY)
	//	HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart485Buf,UART485_RX_BUF_SIZE);
	
	RxUart1Counter=huart3.RxXferCount;



	for(i=0;i<RxUart1Counter;i++)
	{
		if((g_Uart485Buf[i]==0x01)&&(g_Uart485Buf[i+1]==0x03)&&(g_Uart485Buf[i+2]==0x50))
		{
			p_reg=(uint8_t*)&g_Uart485Buf[3];

			HAL_Delay(100);

			if(g_OffGridReadAddr==40)
			{

			temp16=(p_reg[0]<<8| p_reg[1]);  //0
			GattSetData( LIST_CMD, CMD_AOCT, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]); //1
			GattSetData( LIST_CMD, CMD_AOSS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //2
			GattSetData( LIST_CMD, CMD_BCSS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //3
			GattSetData( LIST_CMD, CMD_UOST, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //4
			GattSetData( LIST_CMD, CMD_UOET, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //5
			GattSetData( LIST_CMD, CMD_UCST, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //6
			GattSetData( LIST_CMD, CMD_UCET, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //7
			GattSetData( LIST_CMD, CMD_PIMD, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //8
			GattSetData( LIST_CMD, CMD_AIMD, (uint8_t*)&temp16);

			p_reg+=2;//9
			p_reg+=2;//10
			p_reg+=2;//11
			p_reg+=2;//12
			p_reg+=2;//13
			p_reg+=2;//14

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //15
			GattSetData( LIST_CMD, CMD_LNGS, (uint8_t*)&temp16);

			p_reg+=2; //16
			p_reg+=2; //17

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //18
			GattSetData( LIST_CMD, CMD_AOVS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //19
			GattSetData( LIST_CMD, CMD_AOFS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //20
			GattSetData( LIST_CMD, CMD_OLRS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //21
			GattSetData( LIST_CMD, CMD_OTRS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //22
			GattSetData( LIST_CMD, CMD_BZON, (uint8_t*)&temp16);
			
			p_reg+=2;//23
			p_reg+=2;//24
			p_reg+=2;//25
			p_reg+=2;//26
			p_reg+=2;//27
			p_reg+=2;//28
			p_reg+=2;//29
			
			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //30
			GattSetData( LIST_CMD, CMD_CASS, (uint8_t*)&temp16);
			p_reg+=2;//31
			p_reg+=2;//32
			p_reg+=2;//33

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //34
			GattSetData( LIST_CMD, CMD_MXCC, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //35
			GattSetData( LIST_CMD, CMD_BBCV, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //36
			GattSetData( LIST_CMD, CMD_BFCV, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //37
			GattSetData( LIST_CMD, CMD_LBUS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //38
			GattSetData( LIST_CMD, CMD_ACCS, (uint8_t*)&temp16);

			memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			}
			else
			{
			

			//p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //40
			GattSetData( LIST_CMD, CMD_AGMD, (uint8_t*)&temp16);
			p_reg+=2;//41
			p_reg+=2;//42
			p_reg+=2;//43
			p_reg+=2;//44

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //45
			GattSetData( LIST_CMD, CMD_STYS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //46
			GattSetData( LIST_CMD, CMD_STMS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //47
			GattSetData( LIST_CMD, CMD_STDS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //48
			GattSetData( LIST_CMD, CMD_STHS, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //49
			GattSetData( LIST_CMD, CMD_STMM, (uint8_t*)&temp16);

			p_reg+=2;
			temp16=(p_reg[0]<<8| p_reg[1]);  //50
			GattSetData( LIST_CMD, CMD_STSS, (uint8_t*)&temp16);

			g_OffGridRegType=OFFGRID_INPUT_REG;
			g_OffGridReadAddr=0;

			memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			}
			}
		
		if((g_Uart485Buf[i]==0x01)&&(g_Uart485Buf[i+1]==0x04)&&(g_Uart485Buf[i+2]==0x50||g_Uart485Buf[i+2]==0x36))
		{
			p_reg=(uint8_t*)&g_Uart485Buf[3];

			HAL_Delay(100);

			if(g_OffGridReadAddr==40)
			{
				temp16=(p_reg[0]<<8| p_reg[1]);
				//On/Off
				p_reg+=2;
				
				//01	Vpv1//	
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mv
				GattSetData( LIST_DTA, DTA_PV1V, (uint8_t*)&temp16);
				p_reg+=2;
				//02	Vpv2//	
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mv
				GattSetData( LIST_DTA, DTA_PV2V, (uint8_t*)&temp16);
				p_reg+=2;
				//03	Ppv1 H//
				temp32=(p_reg[0]<<8| p_reg[1])*100;
				temp32=temp32<<16;
				p_reg+=2;
				//04	Ppv1 L//	
				temp32|=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DTA, DTA_PV1P, (uint8_t*)&temp32);
				
				p_reg+=2;
				//05	Ppv2 H//	
				temp32=(p_reg[0]<<8| p_reg[1])*100;
				temp32=temp32<<16;
				
				//p_reg+=2;
				//06	Ppv2 L//
				p_reg+=2;
				temp32|=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DTA, DTA_PV2P, (uint8_t*)&temp32);
				p_reg+=2;
				//07	Buck1Curr// 	
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mA
				GattSetData( LIST_DTA, DTA_CG1C, (uint8_t*)&temp16);
				p_reg+=2;
				//08	Buck2Curr// 
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mA
				GattSetData( LIST_DTA, DTA_CG2C, (uint8_t*)&temp16);
				p_reg+=2;
				//09	OP_Watt H// 
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				//temp32=temp32<<16;
				
				p_reg+=2;
				//10	OP_Watt L// 
				//temp32|=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DTA, DTA_AOAP, (uint8_t*)&temp32);
				p_reg+=2;
				//11	OP_VA H //	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				
				p_reg+=2;
				//12	OP_VA L //	
				GattSetData( LIST_DTA, DTA_AOVA, (uint8_t*)&temp32);
				p_reg+=2;
				//13	ACChr_Watt H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//14	ACChr_Watt L//	
				GattSetData( LIST_DTA, DTA_ACAP, (uint8_t*)&temp32);
				p_reg+=2;;
				//15	ACChr_VA H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//16	ACChr_VA L//
				GattSetData( LIST_DTA, DTA_ACVA, (uint8_t*)&temp32);
				p_reg+=2;
				//17	Bat Volt//	
				temp16=(p_reg[0]<<8| p_reg[1]);//**100;  //mV
				temp16*=10u;
				GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				//GattSetData( LIST_DTA, DTA_BPKV, (uint8_t*)&temp16);

				g_bq40z50_state.Voltage[0]=temp16;
				g_bq40z50_state.Voltage[1]=temp16>>8;
				p_reg+=2;
				//18	BatterySOC//
				temp16=(p_reg[0]<<8| p_reg[1]);  //mV
				GattSetData( LIST_DTA, DTA_RSOC, (uint8_t*)&temp16);
				p_reg+=2;
				//19	Bus Volt//	
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mV
				//GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				GattSetData( LIST_DTA, DTA_BPKV, (uint8_t*)&temp16);
				GattSetData( LIST_DIA, DIA_BUSV, (uint8_t*)&temp16);
				p_reg+=2;
				//20	Grid Volt// 
				temp16=(p_reg[0]<<8| p_reg[1])*100;  //mV    ?
				GattSetData( LIST_DTA, DTA_ACOV, (uint8_t*)&temp16);
				GattSetData( LIST_DIA, DIA_ACIV, (uint8_t*)&temp16);
				p_reg+=2;
				//21	Line Freq// 
				temp16=(p_reg[0]<<8| p_reg[1])/100;
				GattSetData( LIST_DTA, DTA_ACOF, (uint8_t*)&temp16);
				GattSetData( LIST_DIA, DIA_ACIF, (uint8_t*)&temp16);
				p_reg+=2;
				//22	OutputVolt//
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DTA, DTA_DCOV, (uint8_t*)&temp16);
				p_reg+=2;
				//23	OutputFreq//  0.01hz
				//GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				p_reg+=2;
				//24	Ouput DCV// 
				//GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				p_reg+=2;
				//25	InvTemp //	
				temp16=(p_reg[0]<<8| p_reg[1])/10;
				GattSetData( LIST_DIA, DIA_IVTT, (uint8_t*)&temp16);
				//GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				p_reg+=2;
				//26	DcDc Temp// 
				temp16=(p_reg[0]<<8| p_reg[1])/10;
				GattSetData( LIST_DIA, DIA_CVTT, (uint8_t*)&temp16);
				//GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp16);
				p_reg+=2;
				//27	LoadPercent //	
				temp16=(p_reg[0]<<8| p_reg[1])/10;
				GattSetData( LIST_DTA, DTA_LDPP, (uint8_t*)&temp16);
				p_reg+=2;
				//28	Bat_s_Volt//	
				temp16=(p_reg[0]<<8| p_reg[1])*10;
				GattSetData( LIST_DIA, DIA_BPTV, (uint8_t*)&temp16);
				p_reg+=2;
				//29	Bat_Volt_DSP//	
				temp16=(p_reg[0]<<8| p_reg[1])*10;
				GattSetData( LIST_DIA, DIA_BBSV, (uint8_t*)&temp16);
				p_reg+=2;
				//30	Time total H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])/2;
				p_reg+=2;
				//31	Time total L//	
				GattSetData( LIST_DIA, DIA_WKTM, (uint8_t*)&temp32);
				p_reg+=2;
				//32	Buck1_NTC// 
				temp16=(p_reg[0]<<8| p_reg[1])*10;
				GattSetData( LIST_DIA, DIA_BK1T, (uint8_t*)&temp16);
				p_reg+=2;
				//33	Buck2_NTC// 
				temp16=(p_reg[0]<<8| p_reg[1])*10;
				GattSetData( LIST_DIA, DIA_BK2T, (uint8_t*)&temp16);
				p_reg+=2;
				//34	OP_Curr //	
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DTA, DTA_OUTC, (uint8_t*)&temp16);
				p_reg+=2;
				//35	Inv_Curr//	
				temp16=(p_reg[0]<<8| p_reg[1])*10;
				GattSetData( LIST_DIA, DIA_INTC, (uint8_t*)&temp16);
				p_reg+=2;
				//36	AC_InWatt H //
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//37	AC_InWatt L //	
				
				//GattSetData( LIST_DTA, DTA_AIPP, (uint8_t*)&temp32);
				GattSetData( LIST_DIA, DIA_AIAP, (uint8_t*)&temp32);
				p_reg+=2;
				//38	AC_InVA H// 		
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//39	AC_InVA L// 
				GattSetData( LIST_DTA, DTA_AIPP, (uint8_t*)&temp32);
				GattSetData( LIST_DIA, DIA_AIVA, (uint8_t*)&temp32);
				p_reg+=2;
				memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			}
			else if(g_OffGridReadAddr==80)
			{
				//40	Fault bit// 
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DTA, DTA_FLTB, (uint8_t*)&temp16);
				p_reg+=2;
				//41	Warning bit //	
				
				temp32=(p_reg[0]<<8| p_reg[1]| p_reg[2]<<24| p_reg[3]<<16);
				p_reg+=2;
				//42	Warning bit high	
				GattSetData( LIST_DTA, DTA_WRNB, (uint8_t*)&temp32);
				GattSetData( LIST_DIA, DIA_WRNV, (uint8_t*)&temp32);
				p_reg+=2;
				//43	warning value// 	
				p_reg+=2;
				//44	DTC //		
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_DVTC, (uint8_t*)&temp16);
				p_reg+=2;
				//45	Check Step//	
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_DVCS, (uint8_t*)&temp16);
				p_reg+=2;
				//46	Production Line Mode
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_PRLM, (uint8_t*)&temp16);
				p_reg+=2;
				//47	ConstantPowerOKFlag 	
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_CPOK, (uint8_t*)&temp16);
				p_reg+=2;
				//48	Epv1_today H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//49	Epv1_today L//	
				GattSetData( LIST_DIA, DIA_P1DE, (uint8_t*)&temp32);
				p_reg+=2;
				//50	Epv1_total H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//51	Epv1_total L//
				GattSetData( LIST_DIA, DIA_P1TE, (uint8_t*)&temp32);
				p_reg+=2;
				//52	Epv2_today H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//53	Epv2_today L//
				GattSetData( LIST_DIA, DIA_P2DE, (uint8_t*)&temp32);
				p_reg+=2;
				//54	Epv2_total H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//55	Epv2_total L//	
				GattSetData( LIST_DIA, DIA_P2TE, (uint8_t*)&temp32);
				p_reg+=2;
				//56	Eac_chrToday H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//57	Eac_chrToday L//
				GattSetData( LIST_DTA, DTA_ACDE, (uint8_t*)&temp32);
				p_reg+=2;
				//58	Eac_chrTotal H//
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//59	Eac_chrTotal L//
				GattSetData( LIST_DTA, DTA_ACDE, (uint8_t*)&temp32);
				p_reg+=2;
				//60	Ebat_dischrToday H	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//61	Ebat_dischrToday L	
				GattSetData( LIST_DTA, DTA_BDDE, (uint8_t*)&temp32);
				p_reg+=2;
				//62	Ebat_dischrTotal H	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//63	Ebat_dischrTotal L
				GattSetData( LIST_DTA, DTA_BDTE, (uint8_t*)&temp32);
				p_reg+=2;
				//64	Eac_dischrToday H	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//65	Eac_dischrToday L
				GattSetData( LIST_DTA, DTA_AODE, (uint8_t*)&temp32);
				p_reg+=2;
				//66	Eac_dischrTotal H
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//67	Eac_dischrTotal L
				GattSetData( LIST_DTA, DTA_AOTE, (uint8_t*)&temp32);
				p_reg+=2;
				//68	ACChrCurr// 	
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DIA, DIA_ABCC, (uint8_t*)&temp16);
				p_reg+=2;
				//69	AC_DisChrWatt H 
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//70	AC_DisChrWatt L 
				GattSetData( LIST_DIA, DIA_ACDP, (uint8_t*)&temp32);
				p_reg+=2;
				//71	AC_DisChrVA H// 	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//72	AC_DisChrVA L// 	
				GattSetData( LIST_DIA, DIA_ADVA, (uint8_t*)&temp32);
				p_reg+=2;
				//73	Bat_DisChrWatt H	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//74	Bat_DisChrWatt L	
				GattSetData( LIST_DIA, DIA_BDAP, (uint8_t*)&temp32);
				p_reg+=2;
				//75	Bat_DisChrVA H//
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				p_reg+=2;
				//76	Bat_DisChrVA L//	
				GattSetData( LIST_DIA, DIA_BDVA, (uint8_t*)&temp32);
				p_reg+=2;
				//77	Bat_Watt H//	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3])*100;
				
				p_reg+=2;
				//78	Bat_Watt L//	
				GattSetData( LIST_DTA, DTA_BTNP, (uint8_t*)&temp32);
				p_reg+=2;
				//79	uwSlaveExistCnt
				p_reg+=2;
				memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
			}
			else
			{
				//80	BatOverCharge//
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DTA, DTA_BOCF, (uint8_t*)&temp16);
				p_reg+=2;
				//81	MpptFanSpeed//	
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_MCFS, (uint8_t*)&temp16);
				p_reg+=2;
				//82	InvFanSpeed //	
				temp16=(p_reg[0]<<8| p_reg[1]);
				GattSetData( LIST_DIA, DIA_IVFS, (uint8_t*)&temp16);
				p_reg+=2;
				//83	TotalChgCur //	
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DIA, DIA_CCRT, (uint8_t*)&temp16);
				p_reg+=2;
				//84	TotalDisChgCur//
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DIA, DIA_DCCT, (uint8_t*)&temp16);
				p_reg+=2;
				//85	Eop_dischrToday_H
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3]);
				p_reg+=2;
				//86	Eop_dischrToday_L
				GattSetData( LIST_DIA, DIA_DCDE, (uint8_t*)&temp32);
				p_reg+=2;
				//87	Eop_dischrTotal_H	
				temp32=(p_reg[0]<<24| p_reg[1]<<16| p_reg[2]<<8| p_reg[3]);
				p_reg+=2;
				//88	Eop_dischrTotal_L		
				GattSetData( LIST_DIA, DIA_DCTE, (uint8_t*)&temp32);
				p_reg+=2;
				//////								  
				//90	ParaChgCurr //	
				temp16=(p_reg[0]<<8| p_reg[1])*100;
				GattSetData( LIST_DIA, DIA_PCCR, (uint8_t*)&temp16);
				//////								  
				//102 Var1 Value//
				p_reg+=2;
				//103 Var2 Value//	
				p_reg+=2;
				//104 Var1 address//
				p_reg+=2;
				//105 Var2 address//
				p_reg+=2;
				//106 Var1 Setting//	
				p_reg+=2;
				//107 Chip Select		
				p_reg+=2;

				g_OffGridRegType=OFFGRID_HOLD_REG;

				g_OffGridReadAddr=0;
				memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
				}
			
			}
			 
			}

	#endif
}








