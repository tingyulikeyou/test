
#include "main.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>



static uint32_t PageError = 0;
//static FLASH_EraseInitTypeDef EraseCfg={FLASH_TYPEERASE_PAGES,FLASH_START_ADDR+EEP_START_ADDR,4/*,1*/};

USER_SET_TypeDef g_UserSet;
static uint8_t g_eepupdate=0;

ABACUS_COUNTER_TypeDef  g_AbacusLeder[MAX_ABACUS_NUMBER];

	
void EEpInit(void)
{

	memset((uint8_t*)&g_UserSet,0x00,sizeof(USER_SET_TypeDef));

	EEpReadPage(0x0000,sizeof(USER_SET_TypeDef),(uint8_t*)&g_UserSet);

	if(g_UserSet.endmark!=0x01234567)
	{

		if(g_UserSet.Payg.oem_id[0]==0xff)
			memset((uint8_t*)&g_UserSet,0x00,sizeof(USER_SET_TypeDef));
		else	
			memset((uint8_t*)&g_UserSet.NetInfor,0x00,sizeof(USER_SET_TypeDef)-sizeof(PAYG_TypeDef));
		
		memset((uint8_t*)&g_UserSet.NetInfor,0x00,sizeof(NET_INFOR_TypeDef));
		memset((uint8_t*)&g_UserSet.CampFreq,0x00,sizeof(CAMP_FREQ_TypeDef));
		memset((uint8_t*)&g_UserSet.NetInforFactory,0x00,sizeof(NET_INFOR_TypeDef));
	

		//memcpy(g_UserSet.NetInfor.apn,Defualt_APN,strlen(Defualt_APN));
		#ifdef E_SIM_SUPPORT
		memcpy(g_UserSet.NetInfor.apn,"orange.m2m.spec",15);
		#else
		memcpy(g_UserSet.NetInfor.apn,"internet.gma.iot",16);
		#endif
		
		
		/*memcpy(g_UserSet.NetInfor.mqtt_broker,"mqtt-2.omnivoltaic.com",22);
		memcpy(g_UserSet.NetInfor.mqtt_port,"1883",4);
		
		memcpy(g_UserSet.CampFreq.M_frequen,"180",3);
		memcpy(g_UserSet.CampFreq.T_frequen,"5",1);*/
		
		//memset((uint8_t*)&g_UserSet.NetInforFactory,0x00,sizeof(NET_INFOR_TypeDef));

		memcpy(g_UserSet.NetInfor.mqtt_broker,"mqtt-client1.omnivoltaic.com",28);
		memcpy(g_UserSet.NetInfor.mqtt_port,"18884",5);
 		memcpy(g_UserSet.NetInfor.mqtt_usename,"Client1",7);
		memcpy(g_UserSet.NetInfor.mqtt_password,"3QtpFnDS",8); 

		memcpy(g_UserSet.NetInforFactory.mqtt_broker,"mqtt-factory.omnivoltaic.com",28);
		memcpy(g_UserSet.NetInforFactory.mqtt_port,"18883",5);
		memcpy(g_UserSet.NetInforFactory.mqtt_usename,"Admin",5);
		memcpy(g_UserSet.NetInforFactory.mqtt_password,"7xzUV@MT",8);

		g_UserSet.reportt_auto=TRUE;
		g_UserSet.ble_state=5; 
		g_UserSet.sleeptime=5; 
		g_UserSet.onlinetime=2; 
		g_UserSet.heartbeat=1; 
		g_UserSet.wakeup_cnt=0; 
		g_UserSet.ramt=30; 
		g_UserSet.raml_num=0; 

		#ifdef ABACUSLEDER_SUPPORT
		g_UserSet.abacus_num=0;
		memset(g_UserSet.abacuslist,0x00,MAX_ABACUS_NUMBER*6);
		#endif
		g_UserSet.endmark=0x01234567;

		EEpUpdateEnable();

		//EEpWritePage(0x0000,sizeof(USER_SET_TypeDef),(uint8_t*)&g_UserSet);
		}


		#ifdef PUMP_TEST
		memcpy(g_UserSet.Payg.oem_id,"40AH2022136000\0",15);
		memcpy(g_UserSet.Payg.payg_id,"40AH2022136000\0",15);
		//memcpy(g_UserSet.Payg.oem_id,"CN202108171234\0",15);
		//memcpy(g_UserSet.Payg.payg_id,"CN202108171234\0",15);
		#endif

		#if defined(CAMP_PROJECT )||defined(PUMP_PROJECT)	
		 EEpSetWakeupCnt(EEpGetWakeupCnt()+1);
		#endif
		//memset(g_UserSet.Payg.oem_id,0x00,20);
		//memcpy(g_UserSet.Payg.oem_id,"00AH210300000311",15);
		
		//memcpy(g_UserSet.Payg.oem_id,"07AH2112028888\0",15);
		//memcpy(g_UserSet.Payg.payg_id,"20210712\0",9);

		//g_UserSet.reportt_auto=1;


		
		//memset(g_UserSet.NetInfor.mqtt_usename,0x00,MQTT_USENAME_LEN);
		//memset(g_UserSet.NetInfor.mqtt_password,0x00,MQTT_PASSWORD_LEN);
}


uint32_t EEpGetAccuCap(void)
{
	return ( g_UserSet.accuCap);
}

uint32_t EEpGetAccuPower(void)
{
	return ( g_UserSet.accuPower);
}

uint32_t EEpGetAccuSolarGen(void)
{
	return ( g_UserSet.accuSolarGen);
}

uint32_t EEpGetAccuRuntime(void)
{
	return ( g_UserSet.accuRuntime);
}

void EEpSetAccuCap(uint32_t cap)
{	g_UserSet.accuCap=cap;
	EEpUpdateEnable();
}

void EEpSetAccuPower(uint32_t power)
{
	g_UserSet.accuPower=power;
	EEpUpdateEnable();
}

void EEpSetAccuSolarGen(uint32_t solar)
{
	g_UserSet.accuSolarGen=solar;
	EEpUpdateEnable();
}

void EEpSetAccuRuntime(uint32_t runtime)
{
	g_UserSet.accuRuntime=runtime;
	EEpUpdateEnable();
}

uint32_t EEpGetOnlineTime(void)
{
	return ( g_UserSet.onlinetime*60*1000);
}

uint32_t EEpGetRamLRptTime(void)
{
	return ( g_UserSet.ramt*1000);
}

uint32_t EEpGetRamLRptNum(void)
{
	return ( g_UserSet.raml_num);
}


uint32_t EEpGetTransFreq(void)
{
	uint32_t frq=atoi( g_UserSet.CampFreq.T_frequen);
	return frq*60*1000;
}

uint32_t EEpGetSleepTime(void)
{
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
	if(g_UserSet.sleeptime)
		return g_UserSet.sleeptime*60*1000;
	else
		return ( 5*60*1000);
	#else
	if(g_UserSet.sleeptime>g_UserSet.onlinetime)
		return ( (g_UserSet.sleeptime-g_UserSet.onlinetime)*60);
	else
		return ( 5*60);
	#endif
}

uint8_t EEpGetLowBatState(void)
{
	return g_UserSet.lowbat;
}

void EEpSetLowBatFlag(uint8_t flag)
{
	g_UserSet.lowbat=flag;
	EEpUpdateEnable();
}

uint32_t EEpGetDemoCnt(void )
{
	return g_UserSet.demo_Cnt;
}
void EEpSetDemoCnt(uint8_t cnt)
{
	g_UserSet.demo_Cnt=cnt;
	EEpUpdateEnable();
}

void EEpSetHeartbeat(uint32_t heartbeat)
{
	g_UserSet.heartbeat=heartbeat;
	EEpUpdateEnable();
}

void EEpSetWakeupCnt(uint32_t wkcnt)
{
	g_UserSet.wakeup_cnt=wkcnt;
	EEpUpdateEnable();
}

uint32_t EEpGetHeartbeat(void )
{
	return g_UserSet.heartbeat;
}

uint32_t EEpGetWakeupCnt(void)
{
	return g_UserSet.wakeup_cnt;
}

uint8_t EEpGetAttState(void)
{
	return 0;
}
uint8_t EEpGetStsState(void)
{
	return 0;
}	
uint8_t EEpGetStaState(void)
{
	return 0;
}	

uint8_t EEpGetAutoReportState(void)
{
	return g_UserSet.reportt_auto;
}	

void EEpSetJtag(uint8_t jtag)
{
	g_UserSet.jtag=jtag;
}	

uint8_t EEpGetJtagState(void)
{
	return g_UserSet.jtag;
}	


void EEpWritePage(uint32_t PageAddress,uint32_t size,uint8_t* buffer)
{
	uint32_t i=0;
	uint32_t page=size/EEP_PAGE_SIZE;

	
	/*HAL_FLASH_Unlock();

	EraseCfg.PageAddress=EEP_START_ADDR+FLASH_START_ADDR;
	HAL_FLASHEx_Erase(&EraseCfg, &PageError);

	//EraseCfg.PageAddress=EEP_START_ADDR;
	//HAL_FLASHEx_Erase(&EraseCfg, &PageError);
	HAL_Delay(2);


	for(i=0;i<size/4;i++)
	{	
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,FLASH_START_ADDR+EEP_START_ADDR+PageAddress+i*4,((uint32_t*)buffer)[i]);
		}


	HAL_FLASH_Lock();*/
	//EEpWrite(PageAddress,size,buffer);
	for(i=0;i<page;i++)
	{	
		I2c_PageWrite(I2CX_SLAVE_ADDRESS7,PageAddress+i*EEP_PAGE_SIZE,EEP_PAGE_SIZE,&buffer[i*EEP_PAGE_SIZE]);

		HAL_Delay(5);
		}

	if(size%EEP_PAGE_SIZE)
		I2c_PageWrite(I2CX_SLAVE_ADDRESS7,PageAddress+i*EEP_PAGE_SIZE,(size%EEP_PAGE_SIZE),&buffer[i*EEP_PAGE_SIZE]);

	
}
	

void EEpReadPage(uint32_t PageAddress,uint32_t size,uint8_t* buffer)
{
	//FlashRead(FLASH_START_ADDR+EEP_START_ADDR+PageAddress,size/4,(uint32_t*)buffer);
	//EEpRead(PageAddress,size,buffer);
	I2c_PageRead(I2CX_SLAVE_ADDRESS7,PageAddress,size,buffer);
}




 void FlashPageErase(uint32_t PageAddress)
{

	/*EraseCfg.PageAddress=PageAddress;
	
	HAL_FLASHEx_Erase(&EraseCfg, &PageError);*/

	uint32_t erase_counter;

    /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    
    /* erase the flash pages */
   // for(erase_counter = 0; erase_counter < page_num; erase_counter++){
        fmc_page_erase(PageAddress /*+ (FMC_PAGE_SIZE * erase_counter)*/);
        fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    //}

    /* lock the main FMC after the erase operation */
    fmc_lock();
	
}

 uint8_t FlashPageProgram(uint32_t PageAddress,uint32_t size,uint32_t* buffer)
{
	uint32_t i;

	fmc_unlock();
	
	for(i=0;i<size;i++)
	{	
		//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,PageAddress+i*4,buffer[i]);
		fmc_word_program(PageAddress+i*4,buffer[i]);
		if(buffer[i]!=*((__IO uint32_t*)(PageAddress+i*4)))	
			return FALSE;

		fmc_flag_clear(FMC_FLAG_BANK0_END);
        fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
        fmc_flag_clear(FMC_FLAG_BANK0_PGERR); 
		}

	 /* lock the main FMC after the erase operation */
    fmc_lock();

	return TRUE;
}

static void FlashRead(uint32_t PageAddress,uint32_t size,uint32_t* buffer)
{
	uint32_t i;
	for(i=0;i<size;i++)
	{	
		buffer[i]=*((__IO uint32_t*)(PageAddress+i*4));
		}
}


static void EEpWrite(uint32_t Address,uint16_t len,uint8_t  *buf)
{
 	uint16_t i;
	
	/*HAL_FLASHEx_DATAEEPROM_Unlock();
	
	for(i=0;i<len;)
	{
		if(*((__IO uint32_t*)eeprom_addr(i+Address))!=((uint32_t*)buf)[i/4])
		{
			HAL_FLASHEx_DATAEEPROM_Erase(FLASH_TYPEERASEDATA_WORD,eeprom_addr(i+Address));
			HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,eeprom_addr(i+Address),((uint32_t*)buf)[i/4]);
			}
		i+=4;
		
		}
	HAL_FLASHEx_DATAEEPROM_Lock();*/
}

static void EEpRead(uint32_t Address,uint16_t len,uint8_t  *buf)
{
	uint16_t i;

	for(i=0;i<len;)
	{	
		((uint32_t*)buf)[i/4]=*((__IO uint32_t*)eeprom_addr(i+Address));
		i+=4;
		}

		
}

/*void EEpUpdatePayg(PAYG_TypeDef* payg)
{
	memcpy((uint8_t*)&g_UserSet.payg,(uint8_t*)payg,sizeof(PAYG_TypeDef));

	EEpUpdateEnable();
}*/

void EEpUpdateEnable(void)
{
	g_eepupdate=TRUE;	
}
void EEpProcess(void)
{
	if(g_eepupdate)
	{
		g_eepupdate=FALSE;
		memcpy((uint8_t*)&g_UserSet.Payg,(uint8_t*)PaygGetPaygBuf(),sizeof(PAYG_TypeDef));
		EEpWritePage(0x0000,sizeof(USER_SET_TypeDef),(uint8_t*)&g_UserSet);
		}
}


#ifdef ABACUSLEDER_SUPPORT
ABACUS_IMAGE_TypeDef g_AbacusLeder_Image[MAX_ABACUS_NUMBER];
uint16_t g_AbacSaveCounter=0;
uint16_t g_AbacSaveAddr=0;
uint16_t g_AbacTableIndex=0;


void RamShift(uint32_t *buf,uint16_t size)
{
	uint16_t i=0;

	for(i=0;i<size;i++)
		buf[i]=buf[i+1];
}

void AbacusLederInit(void)
{
	uint8_t i=0;
	

	for(i=0;i</*g_UserSet.abacus_num*/MAX_ABACUS_NUMBER;i++)
	{	
		memset((uint8_t*)&g_AbacusLeder_Image[i],0x00,sizeof(ABACUS_IMAGE_TypeDef));
		EEpReadPage(ABACUSLEDER_START_ADDR+i*ABACUSLEDED_SIZE,sizeof(ABACUS_IMAGE_TypeDef),(uint8_t*)&g_AbacusLeder_Image[i]);
		HAL_Delay(5);
		}

	if(g_AbacusLeder_Image[0].sec[0]==0xffffffff)
	{	memset((uint8_t*)&g_AbacusLeder_Image,0x00,MAX_ABACUS_NUMBER*sizeof(ABACUS_IMAGE_TypeDef));
		}
}

void AbacusLederProc(void)
{
	uint16_t i,m,j;
	uint32_t sum=0;
	uint32_t value32=0;
	uint16_t value16=0;


	if(TimerGetEventState(TIMER_ABAC_SAMPLE))
	{
	    TimerEventClear(TIMER_ABAC_SAMPLE);

		 if(g_UserSet.abacus_num)
		 {	
		 	g_AbacSaveCounter++;

		 	if(g_AbacSaveCounter>=3600)//1 hour
			 {
				uint8_t abacbuf[EEP_PAGE_SIZE]={0};
				uint8_t *pram=(uint8_t*)&g_AbacusLeder_Image[g_AbacTableIndex];

				memset(abacbuf,0x00,EEP_PAGE_SIZE);
				
			 	EEpReadPage(ABACUSLEDER_START_ADDR+g_AbacTableIndex*ABACUSLEDED_SIZE+g_AbacSaveAddr,EEP_PAGE_SIZE,abacbuf);
				pram+=g_AbacSaveAddr;
				
				for(i=0;i<EEP_PAGE_SIZE;i++)
				{
					if(abacbuf[i]!=pram[i])
					{
						HAL_Delay(2);
						EEpWritePage(ABACUSLEDER_START_ADDR,EEP_PAGE_SIZE,pram);
						break;
						}
					}

				g_AbacSaveAddr+=EEP_PAGE_SIZE;

				if(g_AbacSaveAddr>=sizeof(ABACUS_IMAGE_TypeDef))
				{
					g_AbacSaveAddr=0;
					g_AbacTableIndex++;
					
					if(g_AbacTableIndex>=g_UserSet.abacus_num)
					{
						g_AbacSaveAddr=0;
						g_AbacTableIndex=0;
						g_AbacSaveCounter=0;
						}
					}
			 	}
		 	}
		/* else
		 {
		 	g_AbacSaveAddr=0;
			g_AbacSaveIndex=0;
		 	}*/
		
		for(i=0;i<MAX_ABACUS_NUMBER&&i<g_UserSet.abacus_num;i++)
		{
			uint8_t list=LIST_STS,id=0,prop[8];	
			uint8_t exit=0;
			extern const uint8_t g_GattlistMemberNum[LIST_COUNT];
			
			for(list=LIST_STS;list<LIST_COUNT;list++)
			{
				for(id=0;id<g_GattlistMemberNum[list];id++)
				{    
					memset(prop,0x00,8);
					{	
						if(GattGetListProp(list,id,prop))
						{
							if(strstr((char*)g_UserSet.abacuslist[i],(char*)prop)!=NULL)
							{
								value32=0;
								value16=0;
								if(list==LIST_DTA&&id==DTA_PCKV)
								{	GattGetData(list,id,(uint8_t*)&value32);
									}
								else
								{	GattGetData(list,id,(uint8_t*)&value16);
									value32=value16;
									}
							
								exit=TRUE;
								break;
								}
							}
					
						}
					}
					if(exit)
						break;
				}

			
			if(g_AbacusLeder[i].sec_counter>=60)
			{
					RamShift(g_AbacusLeder_Image[i].sec,59);
					g_AbacusLeder_Image[i].sec[59]=value32;//g_AbacusLeder[i].sec_counter%60;
				}
		        else
				{	
					g_AbacusLeder_Image[i].sec[g_AbacusLeder[i].sec_counter%60]=value32;//g_AbacusLeder[i].sec_counter%60;
		        	}

			g_AbacusLeder[i].sec_counter++;
			
			if(g_AbacusLeder[i].sec_counter>0&&((g_AbacusLeder[i].sec_counter%60)==0))
			{
				sum=0;
				for(m=0;m<60;m++)
				{
					sum+=g_AbacusLeder_Image[i].sec[m];
					}

				if(g_AbacusLeder[i].min_counter>=60)
				{
						RamShift(g_AbacusLeder_Image[i].min,59);
						g_AbacusLeder_Image[i].min[59]=sum/60;
					}
			        else
					{	
						g_AbacusLeder_Image[i].min[g_AbacusLeder[i].min_counter%60]=sum/60;
			        	}

				g_AbacusLeder[i].min_counter++;

				if(g_AbacusLeder[i].min_counter>0&&((g_AbacusLeder[i].min_counter%60)==0))
				{
					sum=0;
					for(m=0;m<60;m++)
					{
						sum+=g_AbacusLeder_Image[i].min[m];
						}

					if(g_AbacusLeder[i].hour_counter>=24)
					{
							RamShift(g_AbacusLeder_Image[i].hour,23);
							g_AbacusLeder_Image[i].hour[23]=sum/60;
						}
				        else
						{	
							 g_AbacusLeder_Image[i].hour[g_AbacusLeder[i].hour_counter%24]=sum/60;
				        	}
				   
					g_AbacusLeder[i].hour_counter++;

					if(g_AbacusLeder[i].hour_counter>0&&((g_AbacusLeder[i].hour_counter%24)==0))
					{
						sum=0;
						for(m=0;m<24;m++)
						{
							sum+=g_AbacusLeder_Image[i].hour[m];
							}

						if(g_AbacusLeder[i].day_counter>=365)
						{
								RamShift(g_AbacusLeder_Image[i].day,364);
								g_AbacusLeder_Image[i].day[364]=sum/24;
							}
					        else
							{	
								 g_AbacusLeder_Image[i].day[g_AbacusLeder[i].day_counter%365]=sum/24;
					        	}

						g_AbacusLeder[i].day_counter++;

						if(g_AbacusLeder[i].day_counter>0&&((g_AbacusLeder[i].day_counter%365)==0))
						{
							sum=0;
							for(m=0;m<365;m++)
							{
								sum+=g_AbacusLeder_Image[i].day[m];
								}

							if(g_AbacusLeder[i].year_counter>=10)
							{
									RamShift(g_AbacusLeder_Image[i].year,9);
									g_AbacusLeder_Image[i].year[9]=sum/365;
								}
						        else
								{	
									 g_AbacusLeder_Image[i].year[g_AbacusLeder[i].year_counter]=sum/365;
						        	}

								 g_AbacusLeder[i].year_counter++;

							
						}
						
					}
					
					}

				
				
				}
			}

		}
}

#endif

