
#include "main.h"


__IO ADC_SIMPLE_TypeDef g_SampelTable[SIMPLE_NUM];
__IO uint16_t g_VefIntAvergeVolt;
__IO uint16_t g_BatAvergeVolt=12000u;
__IO uint16_t g_BmsAvergeVolt=24000u;

uint8_t g_ChargeIn=0;

uint8_t g_dc12v_overload=0;
uint8_t g_dianyan_overload=0;

#ifdef PUMP_PROJECT
int16_t g_Temperature=25;
#endif

__IO uint16_t g_PvCAvergeAdc=0;
__IO uint16_t g_PvCChrgeCurrent=0;

uint8_t g_PvChrgProtect=FALSE;

uint16_t g_UsbInsertAdc=0;
uint16_t g_UsbInsertVolt=23000u;
uint16_t g_DcCurrent=0;

uint8_t g_ChargeInConFirmCnt=0;

__IO uint8_t g_AdcDetectEnable=FALSE;

//extern ADC_HandleTypeDef hadc;
extern  __IO uint8_t  g_DcState;


extern SYS_STATE_CODE_TypeDef  g_sysStateCode;



void AdcInit(void)
{
	#if 0
	uint8_t i;
	 #if defined(PUMP_PROJECT)||defined(UI1K_V13_PROJECT)
	 #ifdef UI1K_V13_PROJECT
	 if(HAL_ADC_Start_DMA(&hadc,(uint32_t*)g_SampelTable,5*SIMPLE_NUM)!=HAL_OK);
	 #else
	 if(HAL_ADC_Start_DMA(&hadc,(uint32_t*)g_SampelTable,3*SIMPLE_NUM)!=HAL_OK);
	 #endif
	 #else
	 if(HAL_ADC_Start_DMA(&hadc,(uint32_t*)g_SampelTable,2*SIMPLE_NUM)!=HAL_OK);
	 //HAL_Delay(100);
	 #endif
       for(i=0;i<SIMPLE_NUM;i++)	
       {
   		#ifdef UI1K_V13_PROJECT
   		g_SampelTable[i].dc12v_adc=0xfff;
   		g_SampelTable[i].diany_adc=0xfff;
   		#else
   		g_SampelTable[i].bat_adc=0x0AE4;
   		#endif
       		g_SampelTable[i].verf_adc=0x05DB;
   		#ifdef PUMP_PROJECT
   		g_SampelTable[i].temp_adc=0xfff;
   		#endif
       	}
	   #endif
	 
}

void AdcDetectEnable(void)
{
	g_AdcDetectEnable=TRUE;
}


uint16_t  AdcGetVerfIntAvgVolt(uint32_t adc,uint8_t size )
{
	uint32_t voltaverg;
	voltaverg=1200*4096/adc+45;
	
	return voltaverg;
		
}


uint16_t  AdcGetBatAvgVolt(uint32_t adc,uint8_t size )
{

	uint32_t voltaverg;
	
	voltaverg=/*(270+71.5)**/adc*g_VefIntAvergeVolt*61/4096/10;///71.5;//00;
	
	return voltaverg;
		
}

#ifdef UI1K_V13_PROJECT
uint16_t  AdcGetBmsAvgVolt(uint32_t adc,uint8_t size )
{

	uint32_t voltaverg;

	
	//voltaverg=/*(270+71.5)**/adc*g_VefIntAvergeVolt*8.5f/4096+200;///71.5;//00;
	//voltaverg=/*(270+71.5)**/(uint32_t)((float)((uint32_t)adc*(uint32_t)g_VefIntAvergeVolt/(uint32_t)4096)/**16.46f*/);///71.5;//00;

	voltaverg=(uint32_t)((float)((uint32_t)adc*(uint32_t)g_VefIntAvergeVolt/(uint32_t)4096)*16.46f);
	
	return voltaverg;
		
}
uint16_t  AdcGetDcCurrentAvgVolt(uint32_t adc,uint8_t size )
{

	uint32_t voltaverg;

	
	//voltaverg=/*(270+71.5)**/adc*g_VefIntAvergeVolt*8.5f/4096+200;///71.5;//00;
	voltaverg=/*(270+71.5)**/(uint32_t)((float)((uint32_t)adc*(uint32_t)g_VefIntAvergeVolt/(uint32_t)4096)/**16.46f*/);///71.5;//00;
	
	return voltaverg;
		
}


#endif



//uint8_t sizetest=sizeof(ADC_SIMPLE_TypeDef);
   uint32_t volt_dc12v=0,volt_1=0,volt_diany=0  ;
void AdcProc(void)
{

    uint8_t i;
    uint32_t total0=0,total1=0,total2=0,total3=0,total4=0; ;

	//sizetest=sizeof(ADC_SIMPLE_TypeDef);
	//total0=total1=total2=0;

    for(i=0;i<SIMPLE_NUM;i++)	
    {
		#ifdef UI1K_V13_PROJECT
		total0+=g_SampelTable[i].dc12v_adc;
		total2+=g_SampelTable[i].diany_adc;
		total3+=g_SampelTable[i].pv_0c_adc;
		total4+=g_SampelTable[i].usb_in_adc;
		#else
		total0+=g_SampelTable[i].bat_adc;
		#endif
    		total1+=g_SampelTable[i].verf_adc;
		#ifdef PUMP_PROJECT
		total2+=g_SampelTable[i].temp_adc;
		#endif
    	}

    total0=total0/SIMPLE_NUM;
    total1=total1/SIMPLE_NUM;

	
    #ifdef UI1K_V13_PROJECT	
    volt_dc12v=total0;	
    volt_diany=total2/SIMPLE_NUM;

    // charge over protect	
    g_PvCAvergeAdc=total3/SIMPLE_NUM;	

     g_PvCChrgeCurrent=	g_PvCAvergeAdc*1000/73;	

     g_UsbInsertAdc=total4/SIMPLE_NUM;
	

   if(g_ChargeIn)	 
   {
	    if(/*g_PvCAvergeAdc>=1015*/g_PvCChrgeCurrent>=14500u/*||g_PvCChrgeCurrent<=200u*/)  //70*14.5
	   { 	
	   	g_PvChrgProtect=TRUE;
		 g_sysStateCode.error.chg_current_over=1;
		TimerCoulomChrgeOverSet(10000);	

		debug_printf("adc","Charge current over,protect ,I=",g_PvCChrgeCurrent);
	    	}

	    if(GetTimerCoulomChrgeOverEvent())
	    {		g_PvChrgProtect=FALSE;
			TimerEventClear(TIMER_CL_CHRGE_OVER);
			 g_sysStateCode.error.chg_current_over=0;
			 debug_printf("adc","Charge recover ,I=",g_PvCChrgeCurrent);
	    	}

	    if(g_PvCChrgeCurrent<=500u)	
	    {
		if(GetTimerCoulomChrgeLowEvent())	
		{	g_PvChrgProtect=TRUE;
			TimerEventClear(TIMER_CL_CHRGE_LOW);
			TimerCoulomChrgeOverSet(10000);
			TimerCoulomChrgeLowStop();

			debug_printf("adc","Charge current <500,protect ,I=",g_PvCChrgeCurrent);
			}
		if(GetTimerCoulomChrgeLowState()==FALSE&&g_PvChrgProtect==FALSE)  //low current keep 5s
	    		TimerCoulomChrgeLowSet(5000);
	    	}
	      else
	      {
	      		TimerCoulomChrgeLowStop();
			debug_printf("adc","Charging ,I=",g_PvCChrgeCurrent);	
	      	}
   	}

    
    #endif	

   	


   g_VefIntAvergeVolt=(uint16_t)AdcGetVerfIntAvgVolt(total1,SIMPLE_NUM);	
   g_BatAvergeVolt=(uint16_t)AdcGetBatAvgVolt(total0,SIMPLE_NUM);

  #ifdef UI1K_V13_PROJECT 
  g_BmsAvergeVolt=(uint16_t)AdcGetBmsAvgVolt(volt_diany,1);
 // g_UsbInsertVolt=(uint16_t)AdcGetBmsAvgVolt(g_UsbInsertAdc,1);

  g_DcCurrent=(uint16_t)AdcGetDcCurrentAvgVolt(g_UsbInsertAdc,1);	

  #endif

 if(g_DcCurrent>=g_VefIntAvergeVolt-50)
 {
 	g_DcState=FALSE;
	g_sysStateCode.error.dc_current_over=TRUE;
 	}
 
     #ifdef PUMP_PROJECT
     total2=total2/SIMPLE_NUM;	
     g_Temperature=TempCal(total2,10000);

     if(g_Temperature<0)
	 	g_Temperature=0;
	 
     GattSetBattTemp(g_Temperature);
    #endif	


  #ifdef UI1K_V2_PROJECT
  #ifdef UI1K_V13_PROJECT 
  if(g_AdcDetectEnable)
  {

	  if(HAL_GPIO_ReadPin(/*BAT_C_CHRG_IN_GPIO_Port,BAT_C_CHRG_IN_Pin*/PV_ON_CHRGE_IN_GPIO_Port, PV_ON_CHRGE_IN_Pin)==GPIO_PIN_RESET)
	  {	

	  	if(g_ChargeInConFirmCnt<5)
			g_ChargeInConFirmCnt++;

		if(g_ChargeInConFirmCnt>=5)
		{
		  	if(g_ChargeIn==FALSE)
		  	{
		  		g_PvChrgProtect=FALSE;
				TimerCoulomChrgeLowStop();
				TimerCoulomChrgeOverStop();
		  		}
		  	g_ChargeIn=TRUE;
			}

		/*if(g_BatAvergeVolt<15000u)
	  		 HAL_GPIO_WritePin(CEN_GPIO_Port,CEN_Pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(CEN_GPIO_Port,CEN_Pin,GPIO_PIN_SET);*/
	  	}
	  else
	  {	g_ChargeIn=FALSE;
	  	 //HAL_GPIO_WritePin(CEN_GPIO_Port,CEN_Pin,GPIO_PIN_SET);
	  	 g_ChargeInConFirmCnt=0;
	  	}
  	}
  #endif
  #endif

  #ifdef CAMP_PROJECT
  if(AdcGetBatVolt()<=10500u)
  {
  	if(EEpGetLowBatState()==FALSE)
  		EEpSetLowBatFlag(TRUE);
  	}
  else
  {
	if(EEpGetLowBatState())
  	{	EEpSetLowBatFlag(FALSE);
		GsmComInit();
		}
  	}
  #endif

  #ifdef UI1K_V13_PROJECT
 /* if(volt_dc12v>26)
  	g_dc12v_overload=TRUE;
  if(volt_diany>34)
  	g_dianyan_overload=TRUE;*/
  #endif
   
}


uint8_t AdcDianyanOverState(void)
{
	return g_dianyan_overload;
}

uint8_t AdcPvChrgOverState(void)
{
	return g_PvChrgProtect;	
		
}

uint16_t AdcGetBatVolt(void)
{

#ifdef UI1K_V13_PROJECT
	return g_BatAvergeVolt;
#else
	return g_BatAvergeVolt+200;
#endif
}

#ifdef UI1K_V13_PROJECT
uint16_t AdcGetBmsVolt(void)
{
	return g_BmsAvergeVolt;

}
uint16_t AdcGetChrgeCurrent(void)   //unit mA
{
	return g_PvCChrgeCurrent;
}
#endif

uint8_t AdcDc12vOverState(void)
{
	return g_dc12v_overload;
}

uint8_t AdcGetChrgeState(void)
{
	return g_ChargeIn;
}



