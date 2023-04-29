#include "main.h"

TIMER_TypeDef g_GprsUpload_timer;
TIMER_TypeDef g_Mqtt_timer;
TIMER_TypeDef g_Camp_timer;
TIMER_TypeDef g_Pump_timer;
TIMER_TypeDef g_Payg_timer;
TIMER_TypeDef g_BmsCom_timer;
TIMER_TypeDef g_Backlight_timer;
TIMER_TypeDef g_LcdSaver_timer;


TIMER_TypeDef g_AtAckTout_timer;
TIMER_TypeDef g_GsmCom_timer;
TIMER_TypeDef g_KeyTimer;
TIMER_TypeDef g_KbPwrHoldTimer;

TIMER_TypeDef g_SleepTimer;
TIMER_TypeDef g_UiFlashTimer;
TIMER_TypeDef g_ClFullTimer;
TIMER_TypeDef g_ClChrgeOverTimer;
TIMER_TypeDef g_ClChrgeLowTimer;
TIMER_TypeDef g_SysInitTimer;
TIMER_TypeDef g_DemoTimer;
TIMER_TypeDef g_GpsSampleTimer;
TIMER_TypeDef g_RamLReprotTimer;
TIMER_TypeDef g_AbacSampleTimer;


__IO uint32_t uwTick;

extern uint8_t g_KeyboardDetectLockRst;


const TIMER_LIST_TypeDef g_Timer[]=
{
	{TIMER_UPLOAD,COUNT_SUB,&g_GprsUpload_timer,TimerGprsUploadCallback},
	{TIMER_CAMP,COUNT_SUB,&g_Camp_timer,TimerCampCallback},
	{TIMER_PUMP,COUNT_SUB,&g_Pump_timer,TimerPumpCallback},
	{TIMER_PAYG,COUNT_SUB,&g_Payg_timer,TimerPaygCallback},
	{TIMER_BMS_COM,COUNT_SUB,&g_BmsCom_timer,TimerBmsComCallback},
	{TIMER_AT_ACKOUT,COUNT_SUB,&g_AtAckTout_timer,NULL},
	{TIMER_MQTT,COUNT_SUB,&g_Mqtt_timer,NULL},
	{TIMER_GSMCOM,COUNT_SUB,&g_GsmCom_timer,NULL},
	{TIMER_KEY,COUNT_ADD,&g_KeyTimer,TimerKeyScan},
	{TIMER_SLEEP,COUNT_SUB,&g_SleepTimer,NULL},
	{TIMER_LCD_BL,COUNT_SUB,&g_Backlight_timer,TimeBacklightCallback},
	{TIMER_LCDSAVER,COUNT_SUB,&g_LcdSaver_timer,TimeSaverCallback},
	{TIMER_KB_INSERT,COUNT_SUB,&g_KbPwrHoldTimer,TimeKbPwrHoldCallback},
	{TIMER_UI_FLASH,COUNT_SUB,&g_UiFlashTimer, TimeUiFlashCallback},
	{TIMER_CL_FULL,COUNT_SUB,&g_ClFullTimer, TimeCoulomCallback},
	{TIMER_CL_CHRGE_OVER,COUNT_SUB,&g_ClChrgeOverTimer, NULL},
	{TIMER_CL_CHRGE_LOW,COUNT_SUB,&g_ClChrgeLowTimer, NULL},
	{TIMER_SYS_INIT,COUNT_SUB,&g_SysInitTimer, NULL},
	{TIMER_DEMO,COUNT_SUB,&g_DemoTimer, NULL},
	{TIMER_GPS_SAMPLE,COUNT_SUB,&g_GpsSampleTimer, NULL},
	{TIMER_RAML_REPROT,COUNT_SUB,&g_RamLReprotTimer, TimeRamlReportCallback},
	{TIMER_ABAC_SAMPLE,COUNT_SUB,&g_AbacSampleTimer, TimeAbacSampleCallback},
};

uint32_t HAL_GetTick(void)
{
  return uwTick;
}


void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a period to guaranty minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += 1;
  }

  while((HAL_GetTick() - tickstart) < wait)
  {
  }
}


void HAL_SYSTICK_Callback(void)
{

	uint8_t i;

	uwTick++;

	for(i=0;i<sizeof(g_Timer)/sizeof(TIMER_LIST_TypeDef);i++)
	{
		if(g_Timer[i].p->count&&g_Timer[i].p->enable&&g_Timer[i].cnt_type==COUNT_SUB)
		{
			g_Timer[i].p->count--;

			if(g_Timer[i].p->count==0)
			{
				g_Timer[i].p->event=TRUE;

				if(g_Timer[i].p->period==FALSE)
					g_Timer[i].p->enable=FALSE;
				
				if(g_Timer[i].callback!=NULL)
					g_Timer[i].callback(g_Timer[i].p); 
				}
			}

		if(g_Timer[i].p->enable&&g_Timer[i].cnt_type==COUNT_ADD)
		{
			if(g_Timer[i].timer_id==TIMER_KEY)
			{
				if(g_Timer[i].callback!=NULL)
					g_Timer[i].callback(g_Timer[i].p); 
				}
			}
		}
	


}


void TimerInit(void)
{
	g_GprsUpload_timer.enable=TRUE;
	g_GprsUpload_timer.period=TRUE;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
	g_GprsUpload_timer.count=EEpGetSleepTime();
	#else
	g_GprsUpload_timer.count=GPRS_UPLOAD_PRIOD;
	#endif
	g_GprsUpload_timer.event=FALSE;


	g_AtAckTout_timer.enable=FALSE;
	g_AtAckTout_timer.period=FALSE;
	g_AtAckTout_timer.count=0u;
	g_AtAckTout_timer.event=FALSE;

	g_Camp_timer.enable=TRUE;
	g_Camp_timer.period=TRUE;
	g_Camp_timer.count=100u;
	g_Camp_timer.period=TRUE;
	g_Camp_timer.event=FALSE;

	g_Pump_timer.enable=TRUE;
	g_Pump_timer.period=TRUE;
	g_Pump_timer.count=100u;
	g_Pump_timer.event=FALSE;

	g_Payg_timer.enable=TRUE;
	g_Payg_timer.period=TRUE;
	g_Payg_timer.count=100u;
	g_Payg_timer.event=FALSE;
	
	g_BmsCom_timer.enable=TRUE;
	g_BmsCom_timer.period=TRUE;
	g_BmsCom_timer.count=1000u;
	g_BmsCom_timer.event=FALSE;

    /*g_BmsCom_timer.enable=TRUE;
	g_BmsCom_timer.period=FALSE;
	g_BmsCom_timer.count=100u;
	g_BmsCom_timer.event=FALSE;*/

	g_SleepTimer.enable=TRUE;
	g_SleepTimer.period=FALSE;
	
	g_SleepTimer.count=EEpGetOnlineTime();
	
	g_SleepTimer.event=FALSE;

	g_Backlight_timer.enable=FALSE;
	g_Backlight_timer.period=FALSE;
	g_Backlight_timer.count=0;
	g_Backlight_timer.event=FALSE;

	g_LcdSaver_timer.enable=FALSE;
	g_LcdSaver_timer.period=FALSE;
	g_LcdSaver_timer.count=0;
	g_LcdSaver_timer.event=FALSE;

	g_KbPwrHoldTimer.enable=FALSE;
	g_KbPwrHoldTimer.period=FALSE;
	g_KbPwrHoldTimer.count=0;
	g_KbPwrHoldTimer.event=FALSE;

	g_UiFlashTimer.enable=TRUE;
	g_UiFlashTimer.period=TRUE;
	g_UiFlashTimer.count=100u;
	g_UiFlashTimer.event=FALSE;

	
	g_ClFullTimer.enable=FALSE;
	g_ClFullTimer.period=FALSE;
	g_ClFullTimer.count=30000u;
	g_ClFullTimer.event=FALSE;	

	g_ClChrgeOverTimer.enable=FALSE;
	g_ClChrgeOverTimer.period=FALSE;
	g_ClChrgeOverTimer.count=10000u;
	g_ClChrgeOverTimer.event=FALSE;	

	g_ClChrgeLowTimer.enable=FALSE;
	g_ClChrgeLowTimer.period=FALSE;
	g_ClChrgeLowTimer.count=10000u;
	g_ClChrgeLowTimer.event=FALSE;

	g_DemoTimer.enable=FALSE;
	g_DemoTimer.period=FALSE;
	g_DemoTimer.count=DEMO_PRIOD;
	g_DemoTimer.event=FALSE;

	g_GpsSampleTimer.enable=TRUE;
	g_GpsSampleTimer.period=FALSE;
	g_GpsSampleTimer.count=GPS_SAMPLE_TIME;
	g_GpsSampleTimer.event=FALSE;

	g_RamLReprotTimer.enable=TRUE;
	g_RamLReprotTimer.period=TRUE;
	g_RamLReprotTimer.count=EEpGetRamLRptTime();
	g_RamLReprotTimer.event=FALSE;

	g_AbacSampleTimer.enable=TRUE;
	g_AbacSampleTimer.period=TRUE;
	g_AbacSampleTimer.count=1000u;
	g_AbacSampleTimer.event=FALSE;

	

}



void TimerMqttSet(uint16_t ms)
{
	g_Mqtt_timer.enable=TRUE;
	g_Mqtt_timer.count=ms;
	g_Mqtt_timer.event=FALSE;
}

void TimerCoulomFullSet(uint16_t ms)
{
	g_ClFullTimer.enable=TRUE;
	g_ClFullTimer.count=ms;
	g_ClFullTimer.event=FALSE;
}

void TimerCoulomChrgeOverSet(uint16_t ms)
{
	g_ClChrgeOverTimer.enable=TRUE;
	g_ClChrgeOverTimer.count=ms;
	g_ClChrgeOverTimer.event=FALSE;
}

void TimerCoulomChrgeOverStop(void)
{
	g_ClChrgeOverTimer.enable=FALSE;
	//g_ClChrgeOverTimer.count=ms;
	g_ClChrgeOverTimer.event=FALSE;
}

uint8_t GetTimerCoulomChrgeLowState(void)
{
	return g_ClChrgeLowTimer.enable;
}
void TimerCoulomChrgeLowSet(uint16_t ms)
{
	g_ClChrgeLowTimer.enable=TRUE;
	g_ClChrgeLowTimer.count=ms;
	g_ClChrgeLowTimer.event=FALSE;
}

void TimerDemoSet(void)
{
	g_DemoTimer.enable=TRUE;
	g_DemoTimer.count=DEMO_PRIOD;
	g_DemoTimer.event=FALSE;
}
  
uint8_t GetTimerDemoEvent(void)
{
       // uint8_t event=g_DemoTimer.event;

	return g_DemoTimer.event;	
}


void TimerCoulomChrgeLowStop(void)
{
	g_ClChrgeLowTimer.enable=FALSE;
	g_ClChrgeLowTimer.count=0;
	g_ClChrgeLowTimer.event=FALSE;
}


uint8_t GetTimerCoulomChrgeLowEvent(void)
{
        uint8_t event=g_ClChrgeLowTimer.event;

	//g_ClChrgeLowTimer.event=FALSE;	
	
	return event;	
}

void TimerCoulomStop(void)
{
	g_ClFullTimer.enable=FALSE;
	g_ClFullTimer.count=0;
	g_ClFullTimer.event=FALSE;
}


uint8_t GetTimerCoulomFullState(void)
{
	return g_ClFullTimer.enable;
}
uint8_t GetTimerCoulomFullEvent(void)
{
        uint8_t event=g_ClFullTimer.event;

	//g_ClFullTimer.event=FALSE;	
	return event;	
}

uint8_t GetTimerCoulomChrgeOverEvent(void)
{
        uint8_t event=g_ClChrgeOverTimer.event;

	//g_ClChrgeOverTimer.event=FALSE;	
	return event;	
}

void TimerAtTOutStart(uint32_t ms,uint8_t retry_clr)
{
	g_AtAckTout_timer.enable=TRUE;
	g_AtAckTout_timer.period=FALSE;
	g_AtAckTout_timer.count=ms;
	g_AtAckTout_timer.event=FALSE;

	if(retry_clr)
		g_AtAckTout_timer.retry=0;

}

void TimerAtTOutStop(void)
{
	g_AtAckTout_timer.enable=FALSE;
	g_AtAckTout_timer.count=0;
	g_AtAckTout_timer.event=FALSE;
	g_AtAckTout_timer.retry=0;
}

void TimerGsmComSet(uint16_t delay)
{
	
	g_GsmCom_timer.count=delay;
	g_GsmCom_timer.enable=TRUE;
	g_GsmCom_timer.event=FALSE;

}

void TimerKbPwrHoldSet(uint32_t delay)
{
	
	g_KbPwrHoldTimer.count=delay;
	g_KbPwrHoldTimer.enable=TRUE;
	g_KbPwrHoldTimer.event=FALSE;

}


void TimerGprsUploadCallback(TIMER_TypeDef *p)
{
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
	p->count=EEpGetSleepTime();
	#else
	p->count=GPRS_UPLOAD_PRIOD;
	#endif

	//SmsInit();
	
	PumpComStart();
	//GsmComInit();
	GmsWakeup();

	TimerGsmComSet(2000);
	//if(EEpGetAutoReportState())
	//	MqttSetRequest(MQTT_REQ_NONE);
}

void TimerCampCallback(TIMER_TypeDef *p)
{
	p->count=100;
	CampGetEnable();

}

void TimerPumpCallback(TIMER_TypeDef *p)
{
	
	#ifdef PUMP_PROJECT
	p->count=1000;
	PumpComEnable();
	#else
	p->count=1000;
	CoulomComEnable();
	AdcDetectEnable();
	#endif
	#ifdef BLE_ENABLE
	p->count=300;
	BleComEnable();
	#endif
}

void TimerPaygCallback(TIMER_TypeDef *p)
{
	p->count=1000;
	PaygUpdateEnable();

	#ifdef BMS_309_SUPPORT
	bms309TimerSet(1);
	#endif
}

void TimerBmsComCallback(TIMER_TypeDef *p)
{
	#ifdef BMS_OFFGRID_SUPPROT
	p->count=3000;
	#elif defined(BMS_JBD_SUPPROT)
	p->count=1000;
	#else
	p->count=2000;
	#endif
	BmsComEnable();

}

void TimeBacklightCallback(TIMER_TypeDef *p)
{
	p->enable=FALSE;
	#ifndef DC_PUMP_SUPPORT
	HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_RESET);
	#endif

	MenuSaverSet(TRUE);
	g_LcdSaver_timer.enable=TRUE;
	g_LcdSaver_timer.count=2000;
	g_LcdSaver_timer.period=TRUE;

}

void TimeSaverCallback(TIMER_TypeDef *p)
{
	p->count=2000;
	MenuSaverUpdate();
}
void TimeKbPwrHoldCallback(TIMER_TypeDef *p)
{
	KeyboardUsbPwrSet(FALSE);
	//g_KeyboardInsertLock=FALSE;
	g_KeyboardDetectLockRst=FALSE;
}

void TimeUiFlashCallback(TIMER_TypeDef *p)
{
	p->count=400;
	MenuFlashSet(!MenuGetFlashState());
	MenuRefresh();

}

void TimeCoulomCallback(TIMER_TypeDef *p)
{
	//p->count=200;
	//MenuFlashSet(!MenuGetFlashState());
	//MenuRefresh();

}

void TimeRamlReportCallback(TIMER_TypeDef *p)
{
	p->count=EEpGetRamLRptTime();

	if(GmsNetConnectState()&&EEpGetRamLRptNum()>0)
		MqttSetRequest(MQTT_REQ_RAML);
}

void TimeAbacSampleCallback(TIMER_TypeDef *p)
{
	p->count=1000u;

}


void TimeCoulomChrgeOverCallback(TIMER_TypeDef *p)
{
	//p->count=200;
	//MenuFlashSet(!MenuGetFlashState());
	//MenuRefresh();

}


void TimeBacklightSet(void)
{
	g_Backlight_timer.enable=TRUE;
	#ifdef DC_PUMP_SUPPORT
	g_Backlight_timer.count=30000;
	#else
	g_Backlight_timer.count=60000;
	HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_SET);
	#endif
	MenuSaverSet(FALSE);

}

void TimerKeyScan(TIMER_TypeDef *p)
{
   if(p->enable&&IsKeyEvent())
	{
	     if(p->count<0xffff)
	          p->count++;
    	}
}

uint8_t  TimerSleepState(void)
{
        uint8_t event=g_SleepTimer.event;

	//g_SleepTimer.event=FALSE;	
	return event;
}

uint8_t  GetTimerGsmComEvent(void)
{
	return g_GsmCom_timer.event;
}



void  TimerSleepSet(void)
{
	g_SleepTimer.enable=TRUE;
	g_SleepTimer.period=FALSE;
	g_SleepTimer.count=EEpGetOnlineTime();
	g_SleepTimer.event=FALSE;
}


void  TimerSet(uint8_t timer,uint32_t count)
{
	uint8_t i=0;
	
    for(i=0;i<TIMER_COUNT;i++)
	{	
		if(g_Timer[i].timer_id==timer)
		{
			g_Timer[i].p->enable=TRUE;
			g_Timer[i].p->count=count;
			g_Timer[i].p->event=FALSE;

			break;
			}
        }

}

void  TimerEventClear(uint8_t timer)
{
	uint8_t i=0;
	
    for(i=0;i<TIMER_COUNT;i++)
	{	
		if(g_Timer[i].timer_id==timer)
		{
			g_Timer[i].p->event=FALSE;
			break;
			}
        }
}

uint8_t  TimerGetEventState(uint8_t timer)
{
	uint8_t i=0;
	
    for(i=0;i<TIMER_COUNT;i++)
	{	
		if(g_Timer[i].timer_id==timer)
		{
			return g_Timer[i].p->event;
			}
        }

	return FALSE;
}






