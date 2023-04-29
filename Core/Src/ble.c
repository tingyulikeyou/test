#include "main.h"


__IO uint8_t g_blebuffer[BLE_BUFFER_SIZE];

uint8_t g_bleCmd_State=BLE_COM_IDLE;
uint8_t g_bleComEnable=FALSE;
uint8_t g_bleAck_State=BLE_COM_IDLE;
uint8_t g_bleReport_Counter=0;

uint8_t g_bletkre=0;
uint16_t g_bleRptPause=0;

__IO uint8_t g_Uart1Buf[UART1_RX_BUF_SIZE];


extern UART_HandleTypeDef huart1;
extern uint8_t g_GattMem[MEM_GATT_SIZE];
extern uint8_t g_tokenState;

extern USER_SET_TypeDef g_UserSet;

void BleComEnable(void)
{
	g_bleComEnable=TRUE;

}
void BleUartInit(void)
{
    huart1.RxXferCount=0;
	huart1.RxXferSize=UART1_RX_BUF_SIZE;
    huart1.pRxBuffPtr=(uint8_t*)g_Uart1Buf; 

}


void BleSend(uint8_t *data ,uint8_t size)
{	
	//HAL_UART_Transmit(&huart1,data,size,200);
	Uart1Send(data,size);
}


void BlePacktSend(uint8_t cmd,uint8_t *data,uint16_t lenght)
{
	BLE_PACKET_TypeDef  g_BleTxPackt;
	uint8_t i;
	
	g_BleTxPackt.header=0xAA;
	g_BleTxPackt.lenght=lenght+5;

	g_BleTxPackt.com=cmd;
	g_BleTxPackt.datalen=lenght;

	 memset((void *)g_blebuffer,0x00,BLE_BUFFER_SIZE);

	 memcpy((void *)g_blebuffer,(uint8_t*)&g_BleTxPackt.header,4);
	 
	if(data!=NULL&&lenght<=BLE_BUFFER_SIZE-4&&lenght)
	{	
		 memcpy((void *)(g_blebuffer+4),data,lenght);
		}
	
	g_BleTxPackt.checksum=CRC16((uint8_t*)(g_blebuffer+1),g_BleTxPackt.lenght-2);
	
	g_BleTxPackt.end=0x5a5b;

	memcpy((uint8_t*)&g_blebuffer[g_BleTxPackt.lenght-1],(uint8_t*)&g_BleTxPackt.checksum,4);
	

    for(i=0;i<g_BleTxPackt.lenght+3;)
    {
    	if(i+8<g_BleTxPackt.lenght+3)
    	{	
    		BleSend((uint8_t*)&g_blebuffer[i],8);
    		}
		else
		{
			BleSend((uint8_t*)&g_blebuffer[i],g_BleTxPackt.lenght+3-i);
			}
		
		HAL_Delay(2);
		i+=8;
    	}


	//BleSend((uint8_t*)&g_BleTxPackt.checksum,2);
	//BleSend((uint8_t*)&g_BleTxPackt.end,2);


}

uint8_t  BlePacktParse(uint8_t *buf,uint8_t *data,uint8_t*len)
{

	BLE_PACKET_TypeDef * RxPackt;
//	uint8_t *pdata=NULL;
	uint16_t *p_end=NULL;
	uint16_t *p_checksun=NULL;
	//uint16_t *p_datalen=NULL;
	
	//data=NULL;
	

	RxPackt=(BLE_PACKET_TypeDef*)buf;

	
	if(RxPackt->header==0xaa&&RxPackt->datalen+5==RxPackt->lenght)
	{
		p_end=(uint16_t*)&buf[RxPackt->lenght+1];
	    p_checksun=(uint16_t*)&buf[RxPackt->lenght-1];
		
		if((*p_checksun)==CRC16((uint8_t*)&RxPackt->lenght,RxPackt->lenght-2)&&(*p_end==0x5a5b))
		{

				if(RxPackt->datalen)
				//	data=(uint8_t*)&RxPackt->datalen+1;
					memcpy(data,&RxPackt->datalen+1,RxPackt->datalen);

				*len=RxPackt->datalen;
				#ifdef BLE_MASTER_ENABLE
				g_bleAck_State=RxPackt->com;
				#else
				g_bleCmd_State=RxPackt->com;
				#endif

				return TRUE;
			}
		}

	return  FALSE;
}


void BleDataCheck(void)
{
	if(g_bleComEnable)
	{
		#ifdef BLE_MASTER_ENABLE
		/*if(g_bleCmd_State<BLE_COM_IDLE)
		{
			
			//for(i=0;i<32;i++)
			//	buffer[i]=i;
			BlePacktSend(g_bleCmd_State,NULL,0);
			//g_bleCmd_State++;
			}
		else
			g_bleCmd_State=0;*/
		
		#else

		//HAL_GPIO_TogglePin(GSM_EN_GPIO_Port, GSM_EN_Pin);

		if(g_bleRptPause)
			g_bleRptPause--;

		if(g_bleRptPause==0)
			BleDataReprot(g_bleReport_Counter++);

		#ifdef P10KW_PROJECT
		if(g_bleReport_Counter>BLE_DIA_PCCR)
			g_bleReport_Counter=BLE_ATT_OPID;
		#else
			#ifdef BMS_JBD_SUPPROT
			if(g_bleReport_Counter>BLE_DIA_CV20)
				g_bleReport_Counter=BLE_ATT_OPID;
			#else
			if(g_bleReport_Counter>BLE_DIA_CV16)
				g_bleReport_Counter=BLE_ATT_OPID;	
			#endif
		#endif
		#endif
		
		g_bleComEnable=FALSE;
		}


}

void BleDataReprot(uint8_t cmd )
{
	uint8_t buffer[32];
	uint8_t size=0;
	uint8_t ack=FALSE;

	memset(buffer,0x00,32);
	#ifdef BLE_ENABLE
	
	switch(cmd)
	{
		case BLE_ATT_OPID:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OPID],MEM_SIZE_OPID);
			size=sizeof(buffer);
			break;
		case BLE_ATT_PPID:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PPID],MEM_SIZE_PPID);
			size=sizeof(buffer);
			break;
		case BLE_ATT_FLID:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_FLID],MEM_SIZE_FLID);
			size=sizeof(buffer);
			break;
		case BLE_ATT_FRMV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_FRMV],MEM_SIZE_FRMV);
			size=sizeof(buffer);
			break;
		case BLE_CMD_PUBK: 
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PUBK],MEM_SIZE_PUBK);
			size=sizeof(buffer);
			break;
		case BLE_CMD_GSTW:	
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_GSTW],MEM_SIZE_GSTW);
			size=MEM_SIZE_GSTW;
			break;
		case BLE_CMD_GCTW:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_GCTW],MEM_SIZE_GCTW);
			size=MEM_SIZE_GCTW;
			break;
		case BLE_CMD_NAPN:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_NAPN],MEM_SIZE_NAPN);
			size=sizeof(buffer);
			break;
		case BLE_CMD_SWCH:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_GCTW],MEM_SIZE_GCTW);
			size=MEM_SIZE_SWCH;
			break;
		case BLE_CMD_READ:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_READ],MEM_SIZE_READ);
			size=MEM_SIZE_READ;
			break;
		case BLE_CMD_RPTM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RPTM],MEM_SIZE_RPTM);
			size=MEM_SIZE_RPTM;
			break;
		case BLE_CMD_HBFQ:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_HBFQ],MEM_SIZE_HBFQ);
			size=MEM_SIZE_HBFQ;
			break;	
		#ifdef P10KW_PROJECT	
		case BLE_CMD_AOCT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOCT],MEM_SIZE_AOCT);
			size=MEM_SIZE_AOCT;
			break;
		case BLE_CMD_AOSS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOSS],MEM_SIZE_AOSS);
			size=MEM_SIZE_AOSS;
			break;
		case BLE_CMD_BCSS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BCSS],MEM_SIZE_BCSS);
			size=MEM_SIZE_BCSS;
			break;
		case BLE_CMD_UOST:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_UOST],MEM_SIZE_UOST);
			size=MEM_SIZE_UOST;
			break;
		case BLE_CMD_UOET:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_UOET],MEM_SIZE_UOET);
			size=MEM_SIZE_UOET;
			break;
		case BLE_CMD_UCST:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_UCST],MEM_SIZE_UCST);
			size=MEM_SIZE_UCST;
			break;
		case BLE_CMD_UCET:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_UCET],MEM_SIZE_UCET);
			size=MEM_SIZE_UCET;
			break;
		case BLE_CMD_PIMD:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PIMD],MEM_SIZE_PIMD);
			size=MEM_SIZE_PIMD;
			break;
		case BLE_CMD_AIMD:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AIMD],MEM_SIZE_AIMD);
			size=MEM_SIZE_AIMD;
			break;
		case BLE_CMD_LNGS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_LNGS],MEM_SIZE_LNGS);
			size=MEM_SIZE_LNGS;
			break;
		case BLE_CMD_AOVS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOVS],MEM_SIZE_AOVS);
			size=MEM_SIZE_AOVS;
			break;
		case BLE_CMD_AOFS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOFS],MEM_SIZE_AOFS);
			size=MEM_SIZE_AOFS;
			break;
		case BLE_CMD_OLRS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OLRS],MEM_SIZE_OLRS);
			size=MEM_SIZE_OLRS;
			break;
		case BLE_CMD_OTRS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OTRS],MEM_SIZE_OTRS);
			size=MEM_SIZE_OTRS;
			break;
		case BLE_CMD_BZON:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BZON],MEM_SIZE_BZON);
			size=MEM_SIZE_BZON;
			break;
		case BLE_CMD_CASS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CASS],MEM_SIZE_CASS);
			size=MEM_SIZE_CASS;
			break;
		case BLE_CMD_MXCC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_MXCC],MEM_SIZE_MXCC);
			size=MEM_SIZE_MXCC;
			break;
		case BLE_CMD_BBCV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BBCV],MEM_SIZE_BBCV);
			size=MEM_SIZE_BBCV;
			break;
		case BLE_CMD_BFCV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BFCV],MEM_SIZE_BFCV);
			size=MEM_SIZE_BFCV;
			break;
		case BLE_CMD_LBUS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_LBUS],MEM_SIZE_LBUS);
			size=MEM_SIZE_LBUS;
			break;
		case BLE_CMD_ACCS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACCS],MEM_SIZE_ACCS);
			size=MEM_SIZE_ACCS;
			break;
		case BLE_CMD_AGMD:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AGMD],MEM_SIZE_AGMD);
			size=MEM_SIZE_AGMD;
			break;
		case BLE_CMD_STYS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STYS],MEM_SIZE_STYS);
			size=MEM_SIZE_STYS;
			break;
		case BLE_CMD_STMS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STMS],MEM_SIZE_STMS);
			size=MEM_SIZE_STMS;
			break;
		case BLE_CMD_STDS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STDS],MEM_SIZE_STDS);
			size=MEM_SIZE_STDS;
			break;
		case BLE_CMD_STHS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STHS],MEM_SIZE_STHS);
			size=MEM_SIZE_STHS;
			break;
		case BLE_CMD_STMM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STMM],MEM_SIZE_STMM);
			size=MEM_SIZE_STMM;
			break;
		case BLE_CMD_STSS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_STSS],MEM_SIZE_STSS);
			size=MEM_SIZE_STSS;
			break;
		#endif	
		case BLE_STS_SSTC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SSTC],MEM_SIZE_SSTC);
			size=MEM_SIZE_SSTC;
			break;
		case BLE_STS_CRTM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CRTM],MEM_SIZE_CRTM);
			size=sizeof(buffer);
			break;
		case BLE_STS_UDTM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_UDTM],MEM_SIZE_UDTM);
			size=sizeof(buffer);
			break;
		case BLE_STS_TIME:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_TIME],MEM_SIZE_TIME);
			size=sizeof(buffer);
			break;
		case BLE_STS_PLAT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PLAT],MEM_SIZE_PLAT);
			size=sizeof(buffer);
			break;
		case BLE_STS_PLON:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PLON],MEM_SIZE_PLON);
			size=sizeof(buffer);
			break;
		case BLE_STS_GPSS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_GPSS],MEM_SIZE_GPSS);
			size=MEM_SIZE_GPSS;
			break;
		case BLE_STS_GPFC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_GPFC],MEM_SIZE_GPFC);
			size=MEM_SIZE_GPFC;
			break;
		case BLE_STS_RCRD:	
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RCRD],MEM_SIZE_RCRD);
			size=MEM_SIZE_RCRD;
			break;
		case BLE_STS_TRHD:	
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_TRHD],MEM_SIZE_TRHD);
			size=MEM_SIZE_TRHD;
			break;
		case BLE_STS_TPGD:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_TPGD],MEM_SIZE_TPGD);
			size=MEM_SIZE_TPGD;
			break;
		case BLE_STS_PGST:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PGST],MEM_SIZE_PGST);
			size=MEM_SIZE_PGST;
			break;
		case BLE_STS_TKRE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_TKRE],MEM_SIZE_TKRE);
			size=MEM_SIZE_TKRE;
			break;	
		case BLE_STS_OCST:	
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OCST],MEM_SIZE_OCST);
			size=MEM_SIZE_OCST;
			break;
		case BLE_DTA_BATP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BATP],MEM_SIZE_BATP);
			size=MEM_SIZE_BATP;
			break;

		#ifdef P10KW_PROJECT
		/*case BLE_DTA_BATP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BATP],MEM_SIZE_BATP);
			size=MEM_SIZE_BATP;
			break;*/
		case BLE_DTA_INPP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_INPP],MEM_SIZE_INPP);
			size=MEM_SIZE_INPP;
			break;
		case BLE_DTA_OUTP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OUTP],MEM_SIZE_OUTP);
			size=MEM_SIZE_OUTP;
			break;
		case BLE_DTA_AENG:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AENG],MEM_SIZE_AENG);
			size=MEM_SIZE_AENG;
			break;
		case BLE_DTA_PCKV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKV],MEM_SIZE_PCKV);
			size=MEM_SIZE_PCKV;
			break;
		case BLE_DTA_PCKC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKC],MEM_SIZE_PCKC);
			size=MEM_SIZE_PCKC;
			break;
		case BLE_DTA_RSOC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RSOC],MEM_SIZE_RSOC);
			size=MEM_SIZE_RSOC;
			break;
		case BLE_DTA_RCAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RCAP],MEM_SIZE_RCAP);
			size=MEM_SIZE_RCAP;
			break;
		case BLE_DTA_FCCP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_FCCP],MEM_SIZE_FCCP);
			size=MEM_SIZE_FCCP;
			break;
		case BLE_DTA_PCKT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKT],MEM_SIZE_PCKT);
			size=MEM_SIZE_PCKT;
			break;
		case BLE_DTA_ACYC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACYC],MEM_SIZE_ACYC);
			size=MEM_SIZE_ACYC;
			break;
		case BLE_DTA_PV1V:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PV1V],MEM_SIZE_PV1V);
			size=MEM_SIZE_PV1V;
			break;
		case BLE_DTA_PV2V:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PV2V],MEM_SIZE_PV2V);
			size=MEM_SIZE_PV2V;
			break;
		case BLE_DTA_PV1P:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PV1P],MEM_SIZE_PV1P);
			size=MEM_SIZE_PV1P;
			break;
		case BLE_DTA_PV2P:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PV2P],MEM_SIZE_PV2P);
			size=MEM_SIZE_PV2P;
			break;
		case BLE_DTA_CG1C:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CG1C],MEM_SIZE_CG1C);
			size=MEM_SIZE_CG1C;
			break;
		case BLE_DTA_CG2C:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CG2C],MEM_SIZE_CG2C);
			size=MEM_SIZE_CG2C;
			break;
		case BLE_DTA_AOAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOAP],MEM_SIZE_AOAP);
			size=MEM_SIZE_AOAP;
			break;
		case BLE_DTA_AOVA:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOVA],MEM_SIZE_AOVA);
			size=MEM_SIZE_AOVA;
			break;
		case BLE_DTA_ACAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACAP],MEM_SIZE_ACAP);
			size=MEM_SIZE_ACAP;
			break;
		case BLE_DTA_ACVA:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACVA],MEM_SIZE_ACVA);
			size=MEM_SIZE_ACVA;
			break;
		case BLE_DTA_BPKV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BPKV],MEM_SIZE_BPKV);
			size=MEM_SIZE_BPKV;
			break;
		case BLE_DTA_ACOV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACOV],MEM_SIZE_ACOV);
			size=MEM_SIZE_ACOV;
			break;
		case BLE_DTA_ACOF:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACOF],MEM_SIZE_ACOF);
			size=MEM_SIZE_ACOF;
			break;
		case BLE_DTA_DCOV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DCOV],MEM_SIZE_DCOV);
			size=MEM_SIZE_DCOV;
			break;
		case BLE_DTA_LDPP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_LDPP],MEM_SIZE_LDPP);
			size=MEM_SIZE_LDPP;
			break;
		case BLE_DTA_OUTC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OUTC],MEM_SIZE_OUTC);
			size=MEM_SIZE_OUTC;
			break;
		case BLE_DTA_AIPP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AIPP],MEM_SIZE_AIPP);
			size=MEM_SIZE_AIPP;
			break;
		case BLE_DTA_FLTB:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_FLTB],MEM_SIZE_FLTB);
			size=MEM_SIZE_FLTB;
			break;
		case BLE_DTA_WRNB:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_WRNB],MEM_SIZE_WRNB);
			size=MEM_SIZE_WRNB;
			break;
		case BLE_DTA_ACDE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACDE],MEM_SIZE_ACDE);
			size=MEM_SIZE_ACDE;
			break;
		case BLE_DTA_ACTE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACTE],MEM_SIZE_ACTE);
			size=MEM_SIZE_ACTE;
			break;
		case BLE_DTA_BDDE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BDDE],MEM_SIZE_BDDE);
			size=MEM_SIZE_BDDE;
			break;
		case BLE_DTA_BDTE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BDTE],MEM_SIZE_BDTE);
			size=MEM_SIZE_BDTE;
			break;
		case BLE_DTA_AODE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AODE],MEM_SIZE_AODE);
			size=MEM_SIZE_AODE;
			break;
		case BLE_DTA_AOTE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AOTE],MEM_SIZE_AOTE);
			size=MEM_SIZE_AOTE;
			break;
		case BLE_DTA_BTNP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BTNP],MEM_SIZE_BTNP);
			size=MEM_SIZE_BTNP;
			break;
		case BLE_DTA_BOCF:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BOCF],MEM_SIZE_BOCF);
			size=MEM_SIZE_BOCF;
			break;
		//DIA

		case BLE_DIA_BUSV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BUSV],MEM_SIZE_BUSV);
			size=MEM_SIZE_BUSV;
			break;
		case BLE_DIA_ACIV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACIV],MEM_SIZE_ACIV);
			size=MEM_SIZE_ACIV;
			break;
		case BLE_DIA_ACIF:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACIF],MEM_SIZE_ACIF);
			size=MEM_SIZE_ACIF;
			break;
		case BLE_DIA_IVTT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_IVTT],MEM_SIZE_IVTT);
			size=MEM_SIZE_IVTT;
			break;
		case BLE_DIA_CVTT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CVTT],MEM_SIZE_IVTT);
			size=MEM_SIZE_CVTT;
			break;
		case BLE_DIA_BPTV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BPTV],MEM_SIZE_BPTV);
			size=MEM_SIZE_BPTV;
			break;
		case BLE_DIA_BBSV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BBSV],MEM_SIZE_BBSV);
			size=MEM_SIZE_BBSV;
			break;
		case BLE_DIA_WKTM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_WKTM],MEM_SIZE_WKTM);
			size=MEM_SIZE_WKTM;
			break;
		case BLE_DIA_BK1T:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BK1T],MEM_SIZE_BK1T);
			size=MEM_SIZE_BK1T;
			break;
		case BLE_DIA_BK2T:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BK2T],MEM_SIZE_BK2T);
			size=MEM_SIZE_BK2T;
			break;
		case BLE_DIA_INTC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_INTC],MEM_SIZE_INTC);
			size=MEM_SIZE_INTC;
			break;
		case BLE_DIA_AIAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AIAP],MEM_SIZE_AIAP);
			size=MEM_SIZE_AIAP;
			break;
		case BLE_DIA_AIVA:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AIVA],MEM_SIZE_AIVA);
			size=MEM_SIZE_AIVA;
			break;
		case BLE_DIA_WRNV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_WRNV],MEM_SIZE_WRNV);
			size=MEM_SIZE_WRNV;
			break;
		case BLE_DIA_DVTC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DVTC],MEM_SIZE_DVTC);
			size=MEM_SIZE_DVTC;
			break;
		case BLE_DIA_DVCS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DVCS],MEM_SIZE_DVCS);
			size=MEM_SIZE_DVCS;
			break;
		case BLE_DIA_PRLM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PRLM],MEM_SIZE_PRLM);
			size=MEM_SIZE_PRLM;
			break;
		case BLE_DIA_CPOK:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CPOK],MEM_SIZE_CPOK);
			size=MEM_SIZE_CPOK;
			break;
		case BLE_DIA_P1DE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_P1DE],MEM_SIZE_P1DE);
			size=MEM_SIZE_P1DE;
			break;
		case BLE_DIA_P1TE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_P1TE],MEM_SIZE_P1TE);
			size=MEM_SIZE_P1TE;
			break;
		case BLE_DIA_P2DE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_P2DE],MEM_SIZE_P2DE);
			size=MEM_SIZE_P2DE;
			break;
		case BLE_DIA_P2TE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_P2TE],MEM_SIZE_P2TE);
			size=MEM_SIZE_P2TE;
			break;
		case BLE_DIA_ABCC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ABCC],MEM_SIZE_ABCC);
			size=MEM_SIZE_ABCC;
			break;
		case BLE_DIA_ACDP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACDP],MEM_SIZE_ACDP);
			size=MEM_SIZE_ACDP;
			break;
		case BLE_DIA_ADVA:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ADVA],MEM_SIZE_ADVA);
			size=MEM_SIZE_ADVA;
			break;
		case BLE_DIA_BDAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BDAP],MEM_SIZE_BDAP);
			size=MEM_SIZE_BDAP;
			break;
		case BLE_DIA_BDVA:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_BDVA],MEM_SIZE_BDVA);
			size=MEM_SIZE_BDVA;
			break;
		case BLE_DIA_MCFS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_MCFS],MEM_SIZE_MCFS);
			size=MEM_SIZE_MCFS;
			break;
		case BLE_DIA_IVFS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_IVFS],MEM_SIZE_IVFS);
			size=MEM_SIZE_IVFS;
			break;
		case BLE_DIA_CCRT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CCRT],MEM_SIZE_CCRT);
			size=MEM_SIZE_CCRT;
			break;
		case BLE_DIA_DCCT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DCCT],MEM_SIZE_DCCT);
			size=MEM_SIZE_DCCT;
			break;
		case BLE_DIA_DCDE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DCDE],MEM_SIZE_DCDE);
			size=MEM_SIZE_DCDE;
			break;
		case BLE_DIA_DCTE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_DCTE],MEM_SIZE_DCTE);
			size=MEM_SIZE_DCTE;
			break;
		case BLE_DIA_PCCR:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCCR],MEM_SIZE_PCCR);
			size=MEM_SIZE_PCCR;
			break;
		#else
		case BLE_DTA_INPP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_INPP],MEM_SIZE_INPP);
			size=MEM_SIZE_INPP;
			break;
		case BLE_DTA_OUTP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_OUTP],MEM_SIZE_OUTP);
			size=MEM_SIZE_OUTP;
			break;
		case BLE_DTA_AENG:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_AENG],MEM_SIZE_AENG);
			size=MEM_SIZE_AENG;
			break;
		case BLE_DTA_PCKV:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKV],MEM_SIZE_PCKV);
			size=MEM_SIZE_PCKV;
			break;
		case BLE_DTA_PCKC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKC],MEM_SIZE_PCKC);
			size=MEM_SIZE_PCKC;
			break;
		case BLE_DTA_RSOC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RSOC],MEM_SIZE_RSOC);
			size=MEM_SIZE_RSOC;
			break;
		case BLE_DTA_RCAP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_RCAP],MEM_SIZE_RCAP);
			size=MEM_SIZE_RCAP;
			break;
		case BLE_DTA_FCCP:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_FCCP],MEM_SIZE_FCCP);
			size=MEM_SIZE_FCCP;
			break;
		case BLE_DTA_PCKT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_PCKT],MEM_SIZE_PCKT);
			size=MEM_SIZE_PCKT;
			break;
		case BLE_DTA_SLON:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SLON],MEM_SIZE_SLON);
			size=MEM_SIZE_SLON;
			break;
		case BLE_DTA_SLAT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SLAT],MEM_SIZE_SLAT);
			size=MEM_SIZE_SLAT;
			break;
		case BLE_DTA_SALT:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SALT],MEM_SIZE_SALT);
			size=MEM_SIZE_SALT;
			break;
		case BLE_DTA_SSTM:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SSTM],MEM_SIZE_SSTM);
			size=MEM_SIZE_SSTM;
			break;
		case BLE_DTA_SGPS:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SGPS],MEM_SIZE_SGPS);
			size=MEM_SIZE_SGPS;
			break;
		case BLE_DTA_SSPE:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_SSPE],MEM_SIZE_SSPE);
			size=MEM_SIZE_SSPE;
			break;
		/*case BLE_DTA_ACYC:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_ACYC],MEM_SIZE_ACYC);
			size=MEM_SIZE_ACYC;
			break;*/

		case BLE_DIA_CV01:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV01],MEM_SIZE_CV01);
			size=MEM_SIZE_CV01;
			break;
		case BLE_DIA_CV02:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV02],MEM_SIZE_CV02);
			size=MEM_SIZE_CV02;
			break;
		case BLE_DIA_CV03:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV03],MEM_SIZE_CV03);
			size=MEM_SIZE_CV03;
			break;
		case BLE_DIA_CV04:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV04],MEM_SIZE_CV04);
			size=MEM_SIZE_CV04;
			break;
		case BLE_DIA_CV05:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV05],MEM_SIZE_CV05);
			size=MEM_SIZE_CV05;
			break;
		case BLE_DIA_CV06:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV06],MEM_SIZE_CV06);
			size=MEM_SIZE_CV06;
			break;
		case BLE_DIA_CV07:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV07],MEM_SIZE_CV07);
			size=MEM_SIZE_CV07;
			break;
		#ifdef E_MOB48V_PROJECT
		case BLE_DIA_CV08:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV08],MEM_SIZE_CV08);
			size=MEM_SIZE_CV08;
			break;
		case BLE_DIA_CV09:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV09],MEM_SIZE_CV09);
			size=MEM_SIZE_CV09;
			break;
		case BLE_DIA_CV10:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV10],MEM_SIZE_CV10);
			size=MEM_SIZE_CV10;
			break;
		case BLE_DIA_CV11:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV11],MEM_SIZE_CV11);
			size=MEM_SIZE_CV11;
			break;
		case BLE_DIA_CV12:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV12],MEM_SIZE_CV12);
			size=MEM_SIZE_CV12;
			break;
		case BLE_DIA_CV13:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV13],MEM_SIZE_CV13);
			size=MEM_SIZE_CV13;
			break;
		case BLE_DIA_CV14:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV14],MEM_SIZE_CV14);
			size=MEM_SIZE_CV14;
			break;
		case BLE_DIA_CV15:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV15],MEM_SIZE_CV15);
			size=MEM_SIZE_CV15;
			break;		
		case BLE_DIA_CV16:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV16],MEM_SIZE_CV16);
			size=MEM_SIZE_CV16;
			break;	
		#ifdef BMS_JBD_SUPPROT
		case BLE_DIA_CV17:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV17],MEM_SIZE_CV17);
			size=MEM_SIZE_CV17;
			break;
		case BLE_DIA_CV18:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV18],MEM_SIZE_CV18);
			size=MEM_SIZE_CV18;
			break;
		case BLE_DIA_CV19:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV19],MEM_SIZE_CV19);
			size=MEM_SIZE_CV19;
			break;		
		case BLE_DIA_CV20:
			ack=TRUE;
			memcpy(buffer,(uint8_t*)&g_GattMem[MEM_ADDR_CV20],MEM_SIZE_CV20);
			size=MEM_SIZE_CV20;
			break;
		#endif
		#endif
		#endif
		default:
			break;
		}


	
	if(ack)
	{	
		//memset(g_Uart3Buf,0x00,UART3_RX_BUF_SIZE);
		//huart3.RxXferCount=UART3_RX_BUF_SIZE;
		///huart3.pRxBuffPtr=g_Uart3Buf; 
		
		BlePacktSend(cmd+128,buffer,size);
		}
	#endif

}

void BleCmdProc(void)
{
	uint8_t buffer[32];
	uint8_t size=0,ret_len=0;
	uint8_t ack=FALSE,i=0;
	uint16_t temp16=0;
	uint8_t *uartbuff=(uint8_t*)g_Uart1Buf;

	memset(buffer,0x00,32);

	#ifdef BLE_ENABLE

	BleDataCheck();

	if(BlePacktParse(uartbuff,buffer,&ret_len))
	{
		#ifdef BLE_MASTER_ENABLE
		//g_bleCmd_State+=128;
		g_bleCmd_State++;
		
		if(g_bleAck_State>=128)
		{	
			g_bleAck_State-=128;
		
			switch(g_bleAck_State)
			{
				case BLE_ATT_OPID:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OPID],buffer,MEM_SIZE_OPID);
					break;
				case BLE_ATT_PPID:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PPID],buffer,MEM_SIZE_PPID);
					break;
				case BLE_ATT_FLID:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_FLID],buffer,MEM_SIZE_FLID);
					break;
				case BLE_ATT_FRMV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_FRMV],buffer,MEM_SIZE_FRMV);
					break;
				case BLE_CMD_PUBK: 
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PUBK],buffer,MEM_SIZE_PUBK);
					break;
				case BLE_CMD_GSTW:	
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GSTW],buffer,MEM_SIZE_GSTW);
					break;
				case BLE_CMD_GCTW:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GCTW],buffer,MEM_SIZE_GCTW);
					break;
				case BLE_CMD_NAPN:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_NAPN],buffer,MEM_SIZE_NAPN);
					break;
				case BLE_CMD_SWCH:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SWCH],buffer,MEM_SIZE_SWCH);
					break;
				case BLE_CMD_READ:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_READ],buffer,MEM_SIZE_READ);
					break;
				case BLE_CMD_RPTM:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RPTM],buffer,MEM_SIZE_RPTM);
					break;
				case BLE_CMD_HBFQ:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_HBFQ],buffer,MEM_SIZE_HBFQ);
					break;	
				#ifdef P10KW_PROJECT	
				case BLE_CMD_AOCT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOCT],buffer,MEM_SIZE_AOCT);
					break;
				case BLE_CMD_AOSS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOSS],buffer,MEM_SIZE_AOSS);
					break;
				case BLE_CMD_BCSS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BCSS],buffer,MEM_SIZE_BCSS);
					break;
				case BLE_CMD_UOST:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UOST],buffer,MEM_SIZE_UOST);
					break;
				case BLE_CMD_UOET:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UOET],buffer,MEM_SIZE_UOET);
					break;
				case BLE_CMD_UCST:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UCST],buffer,MEM_SIZE_UCST);
					break;
				case BLE_CMD_UCET:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UCET],buffer,MEM_SIZE_UCET);
					break;
				case BLE_CMD_PIMD:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PIMD],buffer,MEM_SIZE_PIMD);
					break;
				case BLE_CMD_AIMD:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AIMD],buffer,MEM_SIZE_AIMD);
					break;
				case BLE_CMD_LNGS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_LNGS],buffer,MEM_SIZE_LNGS);
					break;
				case BLE_CMD_AOVS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOVS],buffer,MEM_SIZE_AOVS);
					break;
				case BLE_CMD_AOFS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOFS],buffer,MEM_SIZE_AOFS);
					break;
				case BLE_CMD_OLRS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OLRS],buffer,MEM_SIZE_OLRS);
					break;
				case BLE_CMD_OTRS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OTRS],buffer,MEM_SIZE_OTRS);
					break;
				case BLE_CMD_BZON:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BZON],buffer,MEM_SIZE_BZON);
					break;
				case BLE_CMD_CASS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CASS],buffer,MEM_SIZE_CASS);
					break;
				case BLE_CMD_MXCC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_MXCC],buffer,MEM_SIZE_MXCC);
					break;
				case BLE_CMD_BBCV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BBCV],buffer,MEM_SIZE_BBCV);
					break;
				case BLE_CMD_BFCV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BFCV],buffer,MEM_SIZE_BFCV);
					break;
				case BLE_CMD_LBUS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_LBUS],buffer,MEM_SIZE_LBUS);
					break;
				case BLE_CMD_ACCS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACCS],buffer,MEM_SIZE_ACCS);
					break;
				case BLE_CMD_AGMD:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AGMD],buffer,MEM_SIZE_AGMD);
					break;
				case BLE_CMD_STYS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STYS],buffer,MEM_SIZE_STYS);
					break;
				case BLE_CMD_STMS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STMS],buffer,MEM_SIZE_STMS);
					break;
				case BLE_CMD_STDS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STDS],buffer,MEM_SIZE_STDS);
					break;
				case BLE_CMD_STHS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STHS],buffer,MEM_SIZE_STHS);
					break;
				case BLE_CMD_STMM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STMM],buffer,MEM_SIZE_STMM);
					break;
				case BLE_CMD_STSS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STSS],buffer,MEM_SIZE_STSS);
					break;
				#endif		
				case BLE_STS_SSTC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SSTC],buffer,MEM_SIZE_SSTC);
					break;
				case BLE_STS_CRTM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CRTM],buffer,MEM_SIZE_CRTM);
					break;
				case BLE_STS_UDTM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UDTM],buffer,MEM_SIZE_UDTM);
					break;
				case BLE_STS_TIME:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_TIME],buffer,MEM_SIZE_TIME);
					break;
				case BLE_STS_PLAT:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PLAT],buffer,MEM_SIZE_PLAT);
					break;
				case BLE_STS_PLON:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PLON],buffer,MEM_SIZE_PLON);
					break;
				case BLE_STS_GPSS:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GPSS],buffer,MEM_SIZE_GPSS);
					break;
				case BLE_STS_GPFC:
					//memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GPFC],buffer,MEM_SIZE_GPFC);
					break;
				case BLE_STS_RCRD:	
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RCRD],buffer,MEM_SIZE_RCRD);
					break;
				case BLE_STS_TRHD:	
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_TRHD],buffer,MEM_SIZE_TRHD);
					break;
				case BLE_STS_TPGD:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_TPGD],buffer,MEM_SIZE_TPGD);
					break;
				case BLE_STS_PGST:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PGST],buffer,MEM_SIZE_PGST);
					break;
				case BLE_STS_TKRE:	
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_TKRE],buffer,MEM_SIZE_TKRE);
					g_bletkre=TRUE;
					break;
				case BLE_STS_OCST:	
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OCST],buffer,MEM_SIZE_OCST);
					break;
				#ifdef P10KW_PROJECT
				case BLE_DTA_BATP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BATP],buffer,MEM_SIZE_BATP);
					break;
				case BLE_DTA_INPP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_INPP],buffer,MEM_SIZE_INPP);
					break;
				case BLE_DTA_OUTP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OUTP],buffer,MEM_SIZE_OUTP);
					break;
				case BLE_DTA_AENG:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AENG],buffer,MEM_SIZE_AENG);
					break;
				case BLE_DTA_PCKV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKV],buffer,MEM_SIZE_PCKV);
					break;
				case BLE_DTA_PCKC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKC],buffer,MEM_SIZE_PCKC);
					break;
				case BLE_DTA_RSOC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RSOC],buffer,MEM_SIZE_RSOC);
					break;
				case BLE_DTA_RCAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RCAP],buffer,MEM_SIZE_RCAP);
					break;
				case BLE_DTA_FCCP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_FCCP],buffer,MEM_SIZE_FCCP);
					break;
				case BLE_DTA_PCKT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKT],buffer,MEM_SIZE_PCKT);
					break;
				case BLE_DTA_ACYC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACYC],buffer,MEM_SIZE_ACYC);
					break;
				case BLE_DTA_PV1V:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PV1V],buffer,MEM_SIZE_PV1V);
					break;
				case BLE_DTA_PV2V:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PV2V],buffer,MEM_SIZE_PV2V);
					break;
				case BLE_DTA_PV1P:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PV1P],buffer,MEM_SIZE_PV1P);
					break;
				case BLE_DTA_PV2P:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PV2P],buffer,MEM_SIZE_PV2P);
					break;
				case BLE_DTA_CG1C:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CG1C],buffer,MEM_SIZE_CG1C);
					break;
				case BLE_DTA_CG2C:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CG2C],buffer,MEM_SIZE_CG2C);
					break;
				case BLE_DTA_AOAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOAP],buffer,MEM_SIZE_AOAP);
					break;
				case BLE_DTA_AOVA:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOVA],buffer,MEM_SIZE_AOVA);
					break;
				case BLE_DTA_ACAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACAP],buffer,MEM_SIZE_ACAP);
					break;
				case BLE_DTA_ACVA:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACVA],buffer,MEM_SIZE_ACVA);
					break;
				case BLE_DTA_BPKV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BPKV],buffer,MEM_SIZE_BPKV);
					break;
				case BLE_DTA_ACOV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACOV],buffer,MEM_SIZE_ACOV);
					break;
				case BLE_DTA_ACOF:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACOF],buffer,MEM_SIZE_ACOF);
					break;
				case BLE_DTA_DCOV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DCOV],buffer,MEM_SIZE_DCOV);
					break;
				case BLE_DTA_LDPP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_LDPP],buffer,MEM_SIZE_LDPP);
					break;
				case BLE_DTA_OUTC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OUTC],buffer,MEM_SIZE_OUTC);
					break;
				case BLE_DTA_AIPP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AIPP],buffer,MEM_SIZE_AIPP);
					break;
				case BLE_DTA_FLTB:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_FLTB],buffer,MEM_SIZE_FLTB);
					break;
				case BLE_DTA_WRNB:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_WRNB],buffer,MEM_SIZE_WRNB);
					break;
				case BLE_DTA_ACDE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACDE],buffer,MEM_SIZE_ACDE);
					break;
				case BLE_DTA_ACTE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACTE],buffer,MEM_SIZE_ACTE);
					break;
				case BLE_DTA_BDDE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BDDE],buffer,MEM_SIZE_BDDE);
					break;
				case BLE_DTA_BDTE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BDTE],buffer,MEM_SIZE_BDTE);
					break;
				case BLE_DTA_AODE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AODE],buffer,MEM_SIZE_AODE);
					break;
				case BLE_DTA_AOTE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOTE],buffer,MEM_SIZE_AOTE);
					break;
				case BLE_DTA_BTNP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BTNP],buffer,MEM_SIZE_BTNP);
					break;
				case BLE_DTA_BOCF:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BOCF],buffer,MEM_SIZE_BOCF);
					break;
				//DIA

				case BLE_DIA_BUSV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BUSV],buffer,MEM_SIZE_BUSV);
					break;
				case BLE_DIA_ACIV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACIV],buffer,MEM_SIZE_ACIV);
					break;
				case BLE_DIA_ACIF:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACIF],buffer,MEM_SIZE_ACIF);
					break;
				case BLE_DIA_IVTT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_IVTT],buffer,MEM_SIZE_IVTT);
					break;
				case BLE_DIA_CVTT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CVTT],buffer,MEM_SIZE_CVTT);
					break;
				case BLE_DIA_BPTV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BPTV],buffer,MEM_SIZE_BPTV);
					break;
				case BLE_DIA_BBSV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BBSV],buffer,MEM_SIZE_BBSV);
					break;
				case BLE_DIA_WKTM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_WKTM],buffer,MEM_SIZE_WKTM);
					break;
				case BLE_DIA_BK1T:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BK1T],buffer,MEM_SIZE_BK1T);
					break;
				case BLE_DIA_BK2T:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BK2T],buffer,MEM_SIZE_BK2T);
					break;
				case BLE_DIA_INTC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_INTC],buffer,MEM_SIZE_INTC);
					break;
				case BLE_DIA_AIAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AIAP],buffer,MEM_SIZE_AIAP);
					break;
				case BLE_DIA_AIVA:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AIVA],buffer,MEM_SIZE_AIVA);
					break;
				case BLE_DIA_WRNV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_WRNV],buffer,MEM_SIZE_WRNV);
					break;
				case BLE_DIA_DVTC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DVTC],buffer,MEM_SIZE_DVTC);
					break;
				case BLE_DIA_DVCS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DVCS],buffer,MEM_SIZE_DVCS);
					break;
				case BLE_DIA_PRLM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PRLM],buffer,MEM_SIZE_PRLM);
					break;
				case BLE_DIA_CPOK:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CPOK],buffer,MEM_SIZE_CPOK);
					break;
				case BLE_DIA_P1DE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_P1DE],buffer,MEM_SIZE_P1DE);
					break;
				case BLE_DIA_P1TE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_P1TE],buffer,MEM_SIZE_P1TE);
					break;
				case BLE_DIA_P2DE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_P2DE],buffer,MEM_SIZE_P2DE);
					break;
				case BLE_DIA_P2TE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_P2TE],buffer,MEM_SIZE_P2TE);
					break;
				case BLE_DIA_ABCC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ABCC],buffer,MEM_SIZE_ABCC);
					break;
				case BLE_DIA_ACDP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACDP],buffer,MEM_SIZE_ACDP);
					break;
				case BLE_DIA_ADVA:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ADVA],buffer,MEM_SIZE_ADVA);
					break;
				case BLE_DIA_BDAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BDAP],buffer,MEM_SIZE_BDAP);
					break;
				case BLE_DIA_BDVA:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BDVA],buffer,MEM_SIZE_BDVA);
					break;
				case BLE_DIA_MCFS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_MCFS],buffer,MEM_SIZE_MCFS);
					break;
				case BLE_DIA_IVFS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_IVFS],buffer,MEM_SIZE_IVFS);
					break;
				case BLE_DIA_CCRT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CCRT],buffer,MEM_SIZE_CCRT);
					break;
				case BLE_DIA_DCCT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DCCT],buffer,MEM_SIZE_DCCT);
					break;
				case BLE_DIA_DCDE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DCDE],buffer,MEM_SIZE_DCDE);
					break;
				case BLE_DIA_DCTE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_DCTE],buffer,MEM_SIZE_DCTE);
					break;
				case BLE_DIA_PCCR:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCCR],buffer,MEM_SIZE_PCCR);
					break;
				#else
				case BLE_DTA_BATP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BATP],buffer,MEM_SIZE_BATP);
					break;
				case BLE_DTA_INPP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_INPP],buffer,MEM_SIZE_INPP);
					break;
				case BLE_DTA_OUTP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OUTP],buffer,MEM_SIZE_OUTP);
					break;
				case BLE_DTA_AENG:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AENG],buffer,MEM_SIZE_AENG);
					break;
				case BLE_DTA_PCKV:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKV],buffer,MEM_SIZE_PCKV);
					break;
				case BLE_DTA_PCKC:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKC],buffer,MEM_SIZE_PCKC);
					break;
				case BLE_DTA_RSOC:
					ack=TRUE;
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RSOC],buffer,MEM_SIZE_RSOC);
					break;
				case BLE_DTA_RCAP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RCAP],buffer,MEM_SIZE_RCAP);
					break;
				case BLE_DTA_FCCP:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_FCCP],buffer,MEM_SIZE_FCCP);
					break;
				case BLE_DTA_PCKT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PCKT],buffer,MEM_SIZE_PCKT);
					break;
				case BLE_DTA_SLON:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SLON],buffer,MEM_SIZE_SLON);
					break;
				case BLE_DTA_SLAT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SLAT],buffer,MEM_SIZE_SLAT);
					break;
				case BLE_DTA_SALT:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SALT],buffer,MEM_SIZE_SALT);
					break;
				case BLE_DTA_SSTM:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SSTM],buffer,MEM_SIZE_SSTM);
					break;
				case BLE_DTA_SGPS:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SGPS],buffer,MEM_SIZE_SGPS);
					break;
				case BLE_DTA_SSPE:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_SSPE],buffer,MEM_SIZE_SSPE);
					break;
				//case BLE_DTA_ACYC:
				//	memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACYC],buffer,MEM_SIZE_ACYC);
				//	break;
				case BLE_DIA_CV01:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV01],buffer,MEM_SIZE_CV01);
					break;
				case BLE_DIA_CV02:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV02],buffer,MEM_SIZE_CV02);
					break;
				case BLE_DIA_CV03:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV03],buffer,MEM_SIZE_CV03);
					break;
				case BLE_DIA_CV04:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV04],buffer,MEM_SIZE_CV04);
					break;
				case BLE_DIA_CV05:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV05],buffer,MEM_SIZE_CV05);
					break;
				case BLE_DIA_CV06:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV06],buffer,MEM_SIZE_CV06);
					break;
				case BLE_DIA_CV07:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV07],buffer,MEM_SIZE_CV07);
					break;
				#ifdef E_MOB48V_PROJECT
				case BLE_DIA_CV08:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV08],buffer,MEM_SIZE_CV08);
					break;
				case BLE_DIA_CV09:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV09],buffer,MEM_SIZE_CV09);
					break;
				case BLE_DIA_CV10:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV10],buffer,MEM_SIZE_CV10);
					break;
				case BLE_DIA_CV11:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV11],buffer,MEM_SIZE_CV11);
					break;
				case BLE_DIA_CV12:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV12],buffer,MEM_SIZE_CV12);
					break;
				case BLE_DIA_CV13:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV13],buffer,MEM_SIZE_CV13);
					break;
				case BLE_DIA_CV14:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV14],buffer,MEM_SIZE_CV14);
					break;
				case BLE_DIA_CV15:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV15],buffer,MEM_SIZE_CV15);
					break;		
				case BLE_DIA_CV16:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV16],buffer,MEM_SIZE_CV16);
					break;	
				#ifdef BMS_JBD_SUPPROT
				case BLE_DIA_CV17:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV17],buffer,MEM_SIZE_CV17);
					break;
				case BLE_DIA_CV18:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV18],buffer,MEM_SIZE_CV18);
					break;
				case BLE_DIA_CV19:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV19],buffer,MEM_SIZE_CV19);
					break;		
				case BLE_DIA_CV20:
					memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CV20],buffer,MEM_SIZE_CV20);
					break;	
				#endif
				#endif
				#endif
				}

				memset((void*)g_Uart1Buf,0x00,UART1_RX_BUF_SIZE);
				huart1.RxXferCount=0;
				huart1.pRxBuffPtr=(uint8_t*)g_Uart1Buf; 
				//BleDataReprot(g_bleCmd_State);

			}

		#else
		{

			if(ret_len)
			{
				switch(g_bleCmd_State)
				{
					case BLE_CMD_PUBK: 
						{
						uint8_t token[64];

						if(ret_len<MEM_SIZE_PUBK)
						{	
							sprintf((char*)token,"/cmd/code/\"%s\"",buffer);
							AtCmdTokenParse(token,"/cmd/code/\"*0");
							}
						
						if( g_tokenState==TOKEN_OK)
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PUBK],buffer,MEM_SIZE_PUBK);
						else if( g_tokenState==TOKEN_USEED)
							;
						else
							;

						g_GattMem[MEM_ADDR_TKRE]=g_tokenState;

						g_bleCmd_State=BLE_STS_TKRE;

						g_bleReport_Counter=BLE_STS_TKRE;
						
						HAL_Delay(100);
						}
						break;
					case BLE_CMD_GSTW:	
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GSTW],buffer,MEM_SIZE_GSTW);
						
						GattGetData( LIST_CMD, CMD_GSTW, (uint8_t*)&temp16);
					    g_UserSet.sleeptime=temp16;
					    EEpUpdateEnable();
						break;
					case BLE_CMD_GCTW:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GCTW],buffer,MEM_SIZE_GCTW);
						GattGetData( LIST_CMD, CMD_GCTW, (uint8_t*)&temp16);
					    g_UserSet.onlinetime=temp16;
					    EEpUpdateEnable();
						break;
					case BLE_CMD_NAPN:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_NAPN],buffer,MEM_SIZE_NAPN);
						GattGetData( LIST_CMD, CMD_NAPN, (uint8_t*)&g_UserSet.NetInfor.apn);
						EEpUpdateEnable();
						break;
					case BLE_CMD_SWCH:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_GCTW],buffer,MEM_SIZE_GCTW);
						break;
					case BLE_CMD_READ:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_READ],buffer,MEM_SIZE_READ);
						break;
					case BLE_CMD_RPTM:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_RPTM],buffer,MEM_SIZE_RPTM);
						GattGetData( LIST_CMD, CMD_RPTM, (uint8_t*)&temp16);
					    g_UserSet.reportt_auto=temp16;
					    EEpUpdateEnable();
						break;
					case BLE_CMD_HBFQ:
						memcpy((uint8_t*)&g_GattMem[MEM_ADDR_HBFQ],buffer,MEM_SIZE_HBFQ);
						GattGetData( LIST_CMD, CMD_HBFQ, (uint8_t*)&temp16);
					    g_UserSet.heartbeat=temp16;
					    EEpUpdateEnable();
						break;	
					#ifdef P10KW_PROJECT	
					case BLE_CMD_AOCT:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOCT],buffer,MEM_SIZE_AOCT);
							GattGetData( LIST_CMD, CMD_AOCT, (uint8_t*)&temp16);
							OffGrid_SetReg(0x00,temp16);
							break;
					    case BLE_CMD_AOSS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOSS],buffer,MEM_SIZE_AOSS);
							GattGetData( LIST_CMD, CMD_AOSS, (uint8_t*)&temp16);
							if(temp16>3)
								temp16=3;
							OffGrid_SetReg(0x01,temp16);
							break;
					    case BLE_CMD_BCSS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BCSS],buffer,MEM_SIZE_BCSS);
							GattGetData( LIST_CMD, CMD_BCSS, (uint8_t*)&temp16);
							if(temp16>2)
								temp16=2;
							OffGrid_SetReg(0x02,temp16);
							break;
					    case BLE_CMD_UOST:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UOST],buffer,MEM_SIZE_UOST);
							GattGetData( LIST_CMD, CMD_UOST, (uint8_t*)&temp16);
							if(temp16>23)
								temp16=23;
							OffGrid_SetReg(0x03,temp16);
							break;
					    case BLE_CMD_UOET:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UOET],buffer,MEM_SIZE_UOET);
							GattGetData( LIST_CMD, CMD_UOET, (uint8_t*)&temp16);
							if(temp16>23)
								temp16=23;
							OffGrid_SetReg(0x04,temp16);
							break;
					    case BLE_CMD_UCST:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UCST],buffer,MEM_SIZE_UCST);
							GattGetData( LIST_CMD, CMD_UCST, (uint8_t*)&temp16);
							if(temp16>23)
								temp16=23;
							OffGrid_SetReg(0x05,temp16);
							break;
					    case BLE_CMD_UCET:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_UCET],buffer,MEM_SIZE_UCET);
							GattGetData( LIST_CMD, CMD_UCET, (uint8_t*)&temp16);
							if(temp16>23)
								temp16=23;
							OffGrid_SetReg(0x06,temp16);
							break;
					    case BLE_CMD_PIMD:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_PIMD],buffer,MEM_SIZE_PIMD);
							GattGetData( LIST_CMD, CMD_PIMD, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(0x07,temp16);
							break;
					    case BLE_CMD_AIMD:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AIMD],buffer,MEM_SIZE_AIMD);
							GattGetData( LIST_CMD, CMD_AIMD, (uint8_t*)&temp16);
							OffGrid_SetReg(0x08,temp16);
							break;
					    case BLE_CMD_LNGS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_LNGS],buffer,MEM_SIZE_LNGS);
							GattGetData( LIST_CMD, CMD_LNGS, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(15,temp16);
							break;
					    case BLE_CMD_AOVS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOVS],buffer,MEM_SIZE_AOVS);
							GattGetData( LIST_CMD, CMD_AOVS, (uint8_t*)&temp16);
							if(temp16>6)
								temp16=6;
							OffGrid_SetReg(18,temp16);
							break;
					    case BLE_CMD_AOFS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AOFS],buffer,MEM_SIZE_AOFS);
							GattGetData( LIST_CMD, CMD_AOFS, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(19,temp16);
							break;
					    case BLE_CMD_OLRS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OLRS],buffer,MEM_SIZE_OLRS);
							GattGetData( LIST_CMD, CMD_OLRS, (uint8_t*)&temp16);
							if(temp16>2)
								temp16=2;
							OffGrid_SetReg(20,temp16);
							break;
					    case BLE_CMD_OTRS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_OTRS],buffer,MEM_SIZE_OTRS);
							GattGetData( LIST_CMD, CMD_OTRS, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(21,temp16);
							break;
					    case BLE_CMD_BZON:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BZON],buffer,MEM_SIZE_BZON);
							GattGetData( LIST_CMD, CMD_BZON, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(22,temp16);
							break;
					    case BLE_CMD_CASS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_CASS],buffer,MEM_SIZE_CASS);
							GattGetData( LIST_CMD, CMD_CASS, (uint8_t*)&temp16);
							if(temp16>254)
								temp16=254;
							OffGrid_SetReg(30,temp16);
							break;
					    case BLE_CMD_MXCC:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_MXCC],buffer,MEM_SIZE_MXCC);
							GattGetData( LIST_CMD, CMD_MXCC, (uint8_t*)&temp16);
							if(temp16>180)
								temp16=180;
							OffGrid_SetReg(34,temp16);
							break;
					    case BLE_CMD_BBCV:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BBCV],buffer,MEM_SIZE_BBCV);
							GattGetData( LIST_CMD, CMD_BBCV, (uint8_t*)&temp16);
							if(temp16>640)
								temp16=640;
							if(temp16<500)
								temp16=500;
							
							OffGrid_SetReg(35,temp16);
							break;
					    case BLE_CMD_BFCV:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_BFCV],buffer,MEM_SIZE_BFCV);
							GattGetData( LIST_CMD, CMD_BFCV, (uint8_t*)&temp16);
							if(temp16>560)
								temp16=560;
							if(temp16<500)
								temp16=500;
							OffGrid_SetReg(36,temp16);
							break;
					    case BLE_CMD_LBUS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_LBUS],buffer,MEM_SIZE_LBUS);
							GattGetData( LIST_CMD, CMD_LBUS, (uint8_t*)&temp16);
							if(temp16>640)
								temp16=640;
							if(temp16<300)
								temp16=300;
							OffGrid_SetReg(37,temp16);
							break;
					    case BLE_CMD_ACCS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_ACCS],buffer,MEM_SIZE_ACCS);
							GattGetData( LIST_CMD, CMD_ACCS, (uint8_t*)&temp16);
							if(temp16>100)
								temp16=100;
							OffGrid_SetReg(38,temp16);
							break;
					    case BLE_CMD_AGMD:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_AGMD],buffer,MEM_SIZE_AGMD);
							GattGetData( LIST_CMD, CMD_AGMD, (uint8_t*)&temp16);
							if(temp16>1)
								temp16=1;
							OffGrid_SetReg(40,temp16);
							break;
					    case BLE_CMD_STYS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STYS],buffer,MEM_SIZE_STYS);
							GattGetData( LIST_CMD, CMD_STYS, (uint8_t*)&temp16);
							if(temp16>99)
								temp16=99;
							OffGrid_SetReg(45,temp16);
							break;
					    case BLE_CMD_STMS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STMS],buffer,MEM_SIZE_STMS);
							GattGetData( LIST_CMD, CMD_STMS, (uint8_t*)&temp16);
							if(temp16>12)
								temp16=12;

							if(temp16<1)
								temp16=1;
							OffGrid_SetReg(46,temp16);
							break;
					    case BLE_CMD_STDS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STDS],buffer,MEM_SIZE_STDS);
							GattGetData( LIST_CMD, CMD_STDS, (uint8_t*)&temp16);

							if(temp16>31)
								temp16=31;

							if(temp16<1)
								temp16=1;
							OffGrid_SetReg(47,temp16);
							break;
					    case BLE_CMD_STHS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STHS],buffer,MEM_SIZE_STHS);
							GattGetData( LIST_CMD, CMD_STHS, (uint8_t*)&temp16);
							if(temp16>23)
								temp16=23;
							OffGrid_SetReg(48,temp16);
							break;
					    case BLE_CMD_STMM:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STMM],buffer,MEM_SIZE_STMM);
							GattGetData( LIST_CMD, CMD_STMM, (uint8_t*)&temp16);
							if(temp16>59)
								temp16=59;
							OffGrid_SetReg(49,temp16);
							break;
					    case BLE_CMD_STSS:
							memcpy((uint8_t*)&g_GattMem[MEM_ADDR_STSS],buffer,MEM_SIZE_STSS);
							GattGetData( LIST_CMD, CMD_STSS, (uint8_t*)&temp16);
							if(temp16>59)
								temp16=59;
							OffGrid_SetReg(50,temp16);
							break;
					#endif		

					}
				}

			memset((void*)g_Uart1Buf,0x00,UART1_RX_BUF_SIZE);
			huart1.RxXferCount=0;
			huart1.pRxBuffPtr=(uint8_t*)g_Uart1Buf; 
			BleDataReprot(g_bleCmd_State);

			HAL_Delay(100);
		}
		
		#endif
	
		}

	#endif
}


