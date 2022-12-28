#include"main.h"
#include"string.h"


 __IO double g_lon=0.0f,g_lat=0.0f;

 __IO uint8_t latstr[16];
 __IO uint8_t lonstr[16];

 uint8_t g_GpsWorkEnable=FALSE;


 
 uint8_t g_gpsRateCfg[]="$PCAS01,3*1F\r\n";
 //$PCAS01,3*1F


extern UART_HandleTypeDef huart4;
extern uint8_t g_GattMem[MEM_GATT_SIZE];
 __IO uint8_t g_UartGpsBuf[UART4_RX_BUF_SIZE];


uint16_t  GetAsciiValueLen(uint8_t *str,uint8_t end_tag)
{
	uint16_t ret=0,i=0,j=0;//,len=0;
	uint16_t len=strlen((char*)str);

	for(i=0;str[i]!=end_tag;i++)
	{	
		ret++;
		j++;

		if(i>=len)
			return 0;
		}
	return ret;
}

uint8_t*mStrStr(uint8_t *src,uint8_t *dst,uint16_t size)
{
	uint16_t src_len=size;
	uint16_t dst_len=strlen((char*)dst);
	uint16_t i,j;

	for(i=0;i<src_len-dst_len;i++)
	{
		for(j=0;j<dst_len;j++)
		{
			if(src[i+j]!=dst[j])
				break;
			}

		if(j==dst_len)
			return &src[i];
		}

	return NULL;
}



double StrToDouble(uint8_t *str)
{
	uint32_t temp1=0,temp2=0,base=1;
	uint8_t len,lenf,i=0;

	len=GetAsciiValueLen(str,'.');

	if(len>=20)
		return 0.0f;

	lenf=GetAsciiValueLen(str+len+1,0x00);

	for(i=0;i<len;i++)
	{	temp1=temp1*10+str[i]-'0';
		if(str[i]<'0'&&str[i]>'9')
			return 0.0f;
		}

	for(i=0;i<lenf;i++)
	{	temp2=temp2*10+str[len+1+i]-'0';
		base=base*10;
		
		if(str[i]<'0'&&str[i]>'9')
			return 0.0f;
		}

	return(double) ((double)temp1+(double)temp2/(double)base);
}

void GpsInit(void)
{
	
	huart4.pRxBuffPtr=(uint8_t*)g_UartGpsBuf;
	
	huart4.RxXferCount=0;
	huart4.RxXferSize=UART4_RX_BUF_SIZE;

	

}


//RTC_TimeTypeDef g_gpstime;
//RTC_DateTypeDef g_gpssdate;

void GpsProc(void)
{
	uint8_t value_len=0;
	uint8_t* gpsbuff=(uint8_t*)g_UartGpsBuf;

	uint8_t *p=NULL,*endmark=NULL;//,value_len=0;

	uint32_t i=0;
	uint8_t *p_gps=NULL,clear=0;
	uint8_t *p_buff_addr=(uint8_t*)g_UartGpsBuf;
	uint16_t speed=0;
	uint8_t tempstr[32];




	{
	
		{
				p=mStrStr(gpsbuff,"$GNRMC",UART4_RX_BUF_SIZE/*-(gpsbuff-p_buff_addr)*/);
				endmark=mStrStr(p,"\r\n",strlen((char*)p));

				if(p!=NULL&&endmark!=NULL)
				{
				//HAL_Delay(50);
					p=p+7;
					value_len=GetAsciiValueLen(p,',');//utc hhmmss 1

					if(value_len>=6)
					{
						//g_gpstime.Hours=(p[0]-'0')*10+(p[1]-'0');
						//g_gpstime.Minutes=(p[2]-'0')*10+(p[3]-'0');
						//g_gpstime.Seconds=(p[4]-'0')*10+(p[5]-'0');

						}

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,',');//pos valid 2

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,',');//ddmm.mmmm 3

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,',');//N OR S  4


					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //dddmm.mmmm 5


					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //E OR W 6

					
					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //地面速度speed  7

				    speed=atoi((char*)p);

					#ifdef E_MOB48V_PROJECT
					GattSetData( LIST_DTA, DTA_SSPE,(uint8_t*)&speed);
					#endif


					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //地面航向方向8

					//g_MotionDirect=atoi(p);


					p=p+value_len+1;	
					value_len=GetAsciiValueLen(p,','); //日期 ddmmyy  9

					if(value_len==6)
					{
						//g_gpssdate.Date=(p[0]-'0')*10+(p[1]-'0');
						//g_gpssdate.Month=(p[2]-'0')*10+(p[3]-'0');
						//g_gpssdate.Year=(p[4]-'0')*10+(p[5]-'0');

						}

					memset(tempstr,0x00,32);
					//sprintf((char*)tempstr,"20%d-%d-%d %d:%d:%d",g_gpssdate.Year,g_gpssdate.Month,g_gpssdate.Date,g_gpstime.Hours,g_gpstime.Minutes,g_gpstime.Seconds);


					
					#ifdef E_MOB48V_PROJECT
					GattSetData( LIST_DTA, DTA_SSTM,tempstr);
					#endif
					p=NULL;

					clear=TRUE;
					
					}
				

			p=NULL;
			
			p=mStrStr(gpsbuff,"GNGLL",UART4_RX_BUF_SIZE/*-(gpsbuff-p_buff_addr)*/);
			
			if(p!=NULL)
			{
				HAL_Delay(50);
				if(mStrStr(p,"\r\n",UART4_RX_BUF_SIZE-(p-p_buff_addr))!=NULL)
				{
					p=p+6;
					value_len=GetAsciiValueLen(p,',');//ddmm.mmmm  维度

					

					memset((uint8_t*)latstr,0x00,16);
					memset((uint8_t*)lonstr,0x00,16);

					if(value_len>16)
						memcpy((uint8_t*)latstr,p,16);
					else 	if(value_len>0)
						memcpy((uint8_t*)latstr,p,value_len);

					g_lat=StrToDouble((uint8_t*)latstr)/100.0f;
					memset(tempstr,0x00,32);
					sprintf((char*)tempstr,"%f",g_lat);

					//if(g_lat!=0.0f)
					//	GattSetGpsCordLat(latstr);
					#ifdef E_MOB48V_PROJECT
					if(g_lat!=0.0f)
						GattSetData( LIST_DTA, DTA_SLAT,tempstr);
					#endif

					// printf("gps lat=%s \r\n",latstr);
					
					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,',');//N OR S

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //dddmm.mmmm  经度

					if(value_len>16)
						memcpy((uint8_t*)lonstr,p,16);
					else	if(value_len>0)
						memcpy((uint8_t*)lonstr,p,value_len);

					g_lon=StrToDouble((uint8_t*)lonstr)/100.0f;
					memset(tempstr,0x00,32);
					sprintf((char*)tempstr,"%f",g_lon);
					
					//if(g_lon!=0.0f)
					//	GattSetGpsCordLon(latstr);
					#ifdef E_MOB48V_PROJECT
					if(g_lon!=0.0f)
						GattSetData( LIST_DTA, DTA_SLON, tempstr);
					#endif

					//printf("gps lon=%s \r\n",lonstr);

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,','); //E OR W

					p=p+value_len+1;
					value_len=GetAsciiValueLen(p,',');//utc hhmmss


					p=p+value_len+1;
					value_len=GetAsciiValueLen(p+6,',');//定位有效 A=VALILD  V=无效

					
					}

			if(clear)
			{
				memset((uint8_t*)g_UartGpsBuf,0x00,UART4_RX_BUF_SIZE);
		   	    huart4.RxXferCount=0;
				huart4.RxXferSize=UART4_RX_BUF_SIZE;
			    huart4.pRxBuffPtr=(uint8_t*)g_UartGpsBuf; 

				g_GpsWorkEnable=FALSE;

				HAL_GPIO_WritePin(UART_SEL_GPIO_Port, UART_SEL_Pin, GPIO_PIN_RESET);

				//HAL_UART_DeInit(&huart3);

				//MX_USART3_UART_Init();

				TimerSet(TIMER_GPS_SAMPLE,GPS_SAMPLE_TIME);
				}	

				}

			}
		}
}




