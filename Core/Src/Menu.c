
#include "main.h"
#include "string.h"
//_TIMER_ g_Timeout_timer;

UI_MENU_TypeDef g_UiMenu;
uint8_t g_Ui_Refresh=FALSE;
uint8_t g_ErrBlinkCnt=0;
uint8_t g_PwrOnCnt=0;
uint8_t g_ErrorClrCnt=0;
uint8_t g_PpidShowPos=0;
uint8_t g_PpidHoldDelay=0;


extern GATT_PARA_Def g_gatt_value;
extern PAYG_TypeDef payg;
extern BQ40Z50_TypeDef g_bq40z50_state;

extern KEY_INPUT_TypeDef g_PaygInput;
extern USER_SET_TypeDef g_UserSet;

//extern _KARAOKE_CFG_ g_karaokSet;
extern __IO uint8_t g_tokenState;
extern __IO uint8_t  g_AcState;
extern __IO uint8_t  g_DcState;
extern __IO uint8_t  g_UsbState;

extern  SYS_STATE_CODE_TypeDef  g_sysStateCode;



#ifdef DC_PUMP_SUPPORT

extern uint8_t g_waterFull;
extern uint8_t g_waterEmpty;

extern LED_STATE_TypeDef g_PumpLedState;

extern uint16_t g_MotorSpeed;
extern uint16_t g_MotorCurrent;
extern uint16_t g_MotorPower;

extern uint8_t g_led_enable;

#endif

void TimeOutSet(void)
{
	//g_Timeout_timer.count=SYS_8S;
	//g_Timeout_timer.enable=TRUE;
}

void TimeOutTimer(void)
{
 /*   if(g_Timeout_timer.enable)
	{
	     g_Timeout_timer.count--;
	     if(g_Timeout_timer.count==TIME_OUT)
	     	{
			g_Timeout_timer.enable=FALSE;
			UserCfgChange();
			MenuInit();
	     	}
	          
    	}*/
}

void MenuSaverSet(uint8_t enable)
{
	g_UiMenu.saver=enable;
	g_UiMenu.saver_index=IDLE_ITEM_LOGO;
	MenuRefresh();

	#ifdef DC_PUMP_SUPPORT
	if(g_PumpLedState.ledbitstate.MPPT_state==FALSE&&enable)
		g_led_enable=FALSE;
	#endif
}
void MenuFlashSet(uint8_t flash)
{
	 g_UiMenu.flash=flash;
	 
}
uint8_t MenuGetFlashState(void)
{
	return  g_UiMenu.flash;
	 
}

void MenuPopSet(uint8_t id)
{
	 g_UiMenu.pop_menu=id;
	 g_UiMenu.pop_cnt=10;
}

void MenuSaverUpdate(void)
{
	//g_UiMenu.saver_index=g_UiMenu.menu;
	if(g_UiMenu.saver_index<IDLE_ITEM_POWER)
		g_UiMenu.saver_index++;
	else
		g_UiMenu.saver_index=IDLE_ITEM_LOGO;
	MenuRefresh();
}

void MenuInit(void)
{
	 #ifdef DC_PUMP_SUPPORT
	 g_UiMenu.menu=MENU_SPEED;
	 g_UiMenu.submenu=0;
	 #else
	 g_UiMenu.menu=MENU_IDLE;
	 g_UiMenu.submenu=IDLE_ITEM_LOGO;
	 #endif
	 g_Ui_Refresh=TRUE;
}

void MenuRefresh(void)
{
	g_Ui_Refresh=TRUE;
}

void MenuShow(void)
{
	uint8_t tempStr[12];
	uint8_t i,flag=0;
	uint16_t volt=0,current=0,power=0,temp=0;
	
	if(g_Ui_Refresh)
	{
		LcdClearAll();

		memset(tempStr,0x00,12);

		#ifdef DC_PUMP_SUPPORT

		if(g_led_enable)
		{
			LcdSetIcon(ICON_TCTH,g_PumpLedState.ledbitstate.tank_state);
			LcdSetIcon(ICON_WCWH,g_PumpLedState.ledbitstate.well_state);

			LcdSetIcon(ICON_SOLAR,g_PumpLedState.ledbitstate.MPPT_state);

			if(g_PumpLedState.ledbitstate.MPPT_state)
				LcdSetIcon(ICON_RUN,g_PumpLedState.ledbitstate.pump_run_state);
			
			LcdSetIcon(ICON_FAULT,g_PumpLedState.ledbitstate.fault_state);
			}
		
		//if(g_PumpLedState.ledbitstate.MPPT_state)
		{
			switch(g_UiMenu.menu)
			{
				case MENU_SPEED:
					if(g_PumpLedState.ledbitstate.MPPT_state)
						sprintf((char*)tempStr,"%d",g_MotorSpeed);
					else
						memcpy(tempStr,"----",4);
					break;
		 		case MENU_POWER:
					if(g_PumpLedState.ledbitstate.MPPT_state)
					     sprintf((char*)tempStr,"%3dp",g_MotorPower/100);
					else
						 memcpy(tempStr,"---p",4);
					break;
		 		case MENU_VOLTAGE:
					if(g_PumpLedState.ledbitstate.MPPT_state)
					{	sprintf((char*)tempStr,"%3dU",g_MotorCurrent);
						LcdSetIcon(ICON_P_2,TRUE);
						}
					else
						memcpy(tempStr,"---U",4);
					
					
					break;
				case MENU_RM_DAY:

					if(PaygGetFreeState())
						memcpy(tempStr,"zzd",3);
					else	
						sprintf((char*)tempStr,"%3dd",payg.remaing_days);
					break;
				default:
					memcpy(tempStr,"----",4);
					break;
				}
			}
		//else 
		{	//memcpy(tempStr,"----",4);
			}

		
		if(g_led_enable)
			ShowStr(tempStr);	
		

		LedUpdateAll();
		#else

		if(g_UiMenu.saver)
		{
			MenuShowIdle(g_UiMenu.saver_index,tempStr);
			
			}
		else
		{
			switch(g_UiMenu.menu)
			{
				case MENU_IDLE:
					MenuShowIdle(g_UiMenu.submenu,tempStr);
					break;
				case MENU_LOGO:	
					MenuShowLogo(g_UiMenu.submenu,tempStr);
					break;
				case MENU_CHARGE:
					MenuShowcharge(g_UiMenu.submenu,tempStr);
					break;
				case MENU_BATTERY:
					MenuShowBattery(g_UiMenu.submenu,tempStr);
					break;
				case MENU_TIME:
					MenuShowTime(g_UiMenu.submenu,tempStr);
					break;
				case MENU_BLE:
					MenuShowBle(g_UiMenu.submenu,tempStr);
					break;
				case MENU_PAYG:
					MenuShowPayg(g_UiMenu.submenu,tempStr);
					break;
				case MENU_POWER:
					MenuShowPower(g_UiMenu.submenu,tempStr);
					break;
				case MENU_PAYG_INPUT:
					MenuShowPaygInput(g_UiMenu.submenu,tempStr);
					break;
				case MENU_NONE:
					break;	
				}
			}

		ShowStr(tempStr);
		#ifdef UI1K_V13_PROJECT
		if(g_AcState/*&&PaygGetFreeState()*/
			#ifdef ROLL_SWITCH
			||g_DcState||g_UsbState
			#endif
			)
		{
			if(MenuGetFlashState())
				LcdSetIcon(ICON_POWER,TRUE);
			else
				LcdSetIcon(ICON_POWER,FALSE);
			}
		if(AdcGetChrgeState())
		{
			if(MenuGetFlashState())
				LcdSetIcon(ICON_CHARGE,TRUE);
			else
				LcdSetIcon(ICON_CHARGE,FALSE);
			}

		if(GmsNetConnectState())
		{
			if(MenuGetFlashState())
			{	LcdSetIcon(ICON_WIFI,TRUE);
				LcdSetIcon(ICON_BLE,TRUE);
				}
			else
			{	LcdSetIcon(ICON_WIFI,FALSE);
				LcdSetIcon(ICON_BLE,FALSE);
				}
			}
		
		if(MenuGetFlashState())
		{	

			if(g_UiMenu.menu==MENU_LOGO&&g_UiMenu.submenu==LOGO_ITEM_PPID)
			{	

				g_PpidHoldDelay++;

				if(g_PpidHoldDelay>=5)
				{
					uint8_t len;
					uint8_t p[32];
					GattGetOpid(p);
					
					len=strlen(p);

					g_PpidHoldDelay=0;

					g_PpidShowPos+=4;
					
					if(g_PpidShowPos>=len)
					{	
						g_PpidShowPos=0;
						}
					
					}

				}
			else
			{
				g_PpidHoldDelay=0;
				g_PpidShowPos=0;
				}
			
			if(g_sysStateCode.error.pswd_error||g_sysStateCode.error.pwd_len_error)
			{	g_ErrorClrCnt++;

				if(g_ErrorClrCnt>25)
				{
					g_sysStateCode.error.pswd_error=0;
					g_sysStateCode.error.pwd_len_error=0;
					}
				}
			else
				g_ErrorClrCnt=0;
			
			if(g_PwrOnCnt<10)
				g_PwrOnCnt++;

			if(g_PwrOnCnt>=10)
			{
				g_ErrBlinkCnt++;

				}

			if(g_UiMenu.pop_cnt)
			{
				g_UiMenu.pop_cnt--;

				if(g_UiMenu.pop_cnt==0)
					g_UiMenu.pop_menu=POP_MENU_NONE;
				}
			
			}

			if(g_ErrBlinkCnt>=8)
			{     
			       if(g_ErrBlinkCnt>=10)
					g_ErrBlinkCnt=0;

				  

				if(g_sysStateCode.error.temp_over)
				{	 LcdClearAll();
					ShowStr(" E8 ");
					}

				if(g_sysStateCode.error.chg_current_over)
				{	 LcdClearAll();
					ShowStr(" E7 ");
					}

				if(g_sysStateCode.error.dc_current_over)
				{	 LcdClearAll();
					ShowStr(" E6 ");
					}
				
				if(g_sysStateCode.error.coulom_com)
				{	 LcdClearAll();
					ShowStr(" E1 ");
					}

				if(g_sysStateCode.error.rtc_error&&PaygGetFreeState()==FALSE)
				{	LcdClearAll();
					ShowStr(" E3 ");
					}
				
				if(g_sysStateCode.error.ac_fault)
				{	
					 LcdClearAll();
					ShowStr(" E14");
					}

				if(g_sysStateCode.error.pswd_error)
				{	 LcdClearAll();
					ShowStr(" E4 ");
					}

				if(g_sysStateCode.error.pwd_len_error)
				{	 LcdClearAll();
					ShowStr(" E2 ");
					}
				
					 
				/*switch(GetSysErrorCode())
				{
					case 0x00:
						break;
					case 0xe1:
						ShowStr(" E1 ");
						break;
					case 0xe2:
						ShowStr(" E2 ");
						break;
					case 0xe3:
						if(PaygGetFreeState()==FALSE)
							ShowStr(" E3 ");
						break;
					}*/
				}


			if(g_UiMenu.pop_cnt)
			{
				LcdClearAll();
				
			        LcdSetIcon(ICON_POINT,TRUE);
				
				switch(g_UiMenu.pop_menu)
				{
					case POP_MENU_NONE:
						break;
					case POP_MENU_AC:
						if(g_AcState)
							ShowStr("ac i");
						else
							ShowStr("ac o");
						break;
					case POP_MENU_DC:
						if(g_DcState)
							ShowStr("dc i");
						else
							ShowStr("dc o");
						break;
					case POP_MENU_USB:
						if(g_UsbState)
							ShowStr("usbi");
						else
							ShowStr("usbo");
						break;
					}
			
				}
		#endif

		LcdUpdateAll();

		#endif

		g_Ui_Refresh=FALSE;	
		}
		
}

#ifndef DC_PUMP_SUPPORT

void MenuShowIdle(uint8_t submenu,uint8_t *tempStr)
{
	uint8_t i,flag=1;
	uint16_t volt=0,current=0,power=0,temp=0;

	#ifdef UI1K_V13_PROJECT
	current= GattGetBattCurrent();// (g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8);
	volt = (g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8);
	power=CoulomGetRtPower();

	if(CoulomGetChrgState()==CHRGE_STATE_CHRGE)
		flag = 0;
	#else
	current= g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8;
	volt = g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8;

	power = (uint16_t)(current * CURRENT_CAL * volt / 10000);
	#endif
	
	switch(submenu)
	{
		case IDLE_ITEM_LOGO:
			LcdSetIcon(ICON_LOGO,TRUE);
			sprintf((char*)tempStr,"%d%d",volt/1000,(volt/100)%10);
			LcdSetIcon(ICON_V,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case IDLE_ITEM_CHARGE:
			LcdSetIcon(ICON_CHARGE,TRUE);

			if(flag&&power)
				sprintf((char*)tempStr,"-%d",power/10 );
			else
				sprintf((char*)tempStr,"%d",power/10 );
			LcdSetIcon(ICON_W,TRUE);
			break;
		case IDLE_TTEM_BATTERY:
			LcdSetIcon(ICON_BAT_FRAME,TRUE);
			LcdSetIcon(ICON_BAT_20,TRUE);
			LcdSetIcon(ICON_BAT_40,TRUE);
			LcdSetIcon(ICON_BAT_60,TRUE);
			LcdSetIcon(ICON_BAT_80,TRUE);
			sprintf((char*)tempStr,"%d",(g_bq40z50_state.Relative_State_of_Charge[0] | g_bq40z50_state.Relative_State_of_Charge[1]<<8));
			LcdSetIcon(ICON_PRECENT,TRUE);
			break;
		case IDLE_ITEM_TIME:
			LcdSetIcon(ICON_CLOCK,TRUE);
			temp=(g_bq40z50_state.Run_Time_To_Full[0] | g_bq40z50_state.Run_Time_To_Full[1]<< 8);
			if(temp==0xffff)
				memcpy(tempStr,"----",4);
			else
				//sprintf(tempStr,"%d%d",temp/6/10,(temp/6)%10);
				sprintf((char*)tempStr,"%d",/*(temp/6)/10,(temp/6)%10*/temp);
			LcdSetIcon(ICON_h,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case IDLE_ITEM_BLE:
			LcdSetIcon(ICON_BLE,TRUE);
			LcdSetIcon(ICON_WIFI,TRUE);
			//sprintf(tempStr,"%s",payg.remaing_days);
			if(g_UserSet.ble_state)
				memcpy(tempStr,"on  ",4);
			else
				memcpy(tempStr,"off ",4);
			break;
		case IDLE_ITEM_PAYG:
			LcdSetIcon(ICON_LOCK,TRUE);
			LcdSetIcon(ICON_UNLOCK,TRUE);
			if(/*payg.remaing_days>=1096||*/payg.free == YES)
				memcpy(tempStr,"free",4);
			else	
				sprintf((char*)tempStr,"%d",payg.remaing_days);
			LcdSetIcon(ICON_d,TRUE);
			break;
		case IDLE_ITEM_POWER:
			LcdSetIcon(ICON_POWER,TRUE);
			#ifdef ROLL_SWITCH
			 if(g_AcState)
				memcpy(tempStr,"ac  ",4);
			 else if(g_DcState)
				memcpy(tempStr,"dc  ",4);
			else if(g_UsbState)
				memcpy(tempStr,"usb ",4);
			else
				memcpy(tempStr,"off ",4);
			#else
			memcpy(tempStr,"on  ",4);
			#endif
			break;
		}
}

void MenuShowLogo(uint8_t submenu,uint8_t *tempStr)
{
	uint8_t i,flag=1;
	uint16_t volt=0,current=0,power=0,temp=0;

	#ifdef UI1K_V13_PROJECT
	current= GattGetBattCurrent();// (g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8);
	volt = (g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8);
	power=CoulomGetRtPower();
	if(CoulomGetChrgState()==CHRGE_STATE_CHRGE)
		flag = 0;
	#else
	current= g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8;
	volt = g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8;

	if(current > 32768) //充电电流 < 放电电流  0w
	{
		current = 65535 - current;
		flag = 1;
	}

	power = (uint16_t)(current * CURRENT_CAL * volt / 10000);
	#endif
	
	LcdSetIcon(ICON_LOGO,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);

	switch(submenu)
	{
		case LOGO_ITEM_VOLT:
			sprintf((char*)tempStr,"%d%d",volt/1000,(volt/100)%10);
			LcdSetIcon(ICON_V,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case LOGO_ITEM_SOC:
			sprintf((char*)tempStr,"%d",(g_bq40z50_state.Relative_State_of_Charge[0] | g_bq40z50_state.Relative_State_of_Charge[1]<<8));
			LcdSetIcon(ICON_PRECENT,TRUE);
			break;
		case LOGO_TTEM_PAYG:
			if(/*payg.remaing_days>=1096||*/payg.free == YES)
				memcpy(tempStr,"free",4);
			else	
				sprintf((char*)tempStr,"%d",payg.remaing_days);
			LcdSetIcon(ICON_d,TRUE);
			break;
		case LOGO_ITEM_CHRGEFULL_TIME:

			temp=(g_bq40z50_state.Run_Time_To_Full[0] | g_bq40z50_state.Run_Time_To_Full[1]<< 8);
			
			if(temp==0xFFFF)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",temp);
			
			LcdSetIcon(ICON_h,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case LOGO_ITEM_REMAIN_TIME:

			temp=(g_bq40z50_state.Run_Time_To_Empty[0] | g_bq40z50_state.Run_Time_To_Empty[1]<< 8);
			
			if(temp==0xFFFF)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",temp);
			LcdSetIcon(ICON_H,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case LOGO_ITEM_PPID:
			{
				uint8_t len;
				uint8_t p[32];
				GattGetOpid(p);
				
				len=strlen((char*)p);

				if(g_PpidShowPos+4<len)
				{	
					memcpy(tempStr,&p[g_PpidShowPos],4);
					}
				else
				{
					memcpy(tempStr,"    ",4);
					memcpy(tempStr,&p[g_PpidShowPos],len-g_PpidShowPos);
					}
			
			}
			break;	
		case LOGO_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}


void MenuShowcharge(uint8_t submenu,uint8_t *tempStr)
{
	uint8_t i,flag=1;
	uint16_t volt=0,current=0,power=0,temp=0;

	#ifdef UI1K_V13_PROJECT
	
	current= GattGetBattCurrent();//(g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8);
	volt = (g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8);
	power=CoulomGetRtPower();

	if(CoulomGetChrgState()==CHRGE_STATE_CHRGE)
		flag = 0;
	
	#else

	current= g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8;
	volt = g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8;

	if(current > 32768) //充电电流 < 放电电流  0w
	{
		current = 65535 - current;
		flag = 1;
	}

	power = (uint16_t)(current * CURRENT_CAL * volt / 10000);
	#endif
	
	LcdSetIcon(ICON_CHARGE,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(g_UiMenu.submenu)
	{
		case CHRGE_ITEM_CAP:
			if(flag&&power)
				sprintf((char*)tempStr,"-%d",power /10);
			else
				sprintf((char*)tempStr,"%d",power/10 );
			LcdSetIcon(ICON_W,TRUE);
			break;//W
		case CHRGE_ITEM_CHRGEFULL_TIME:

			temp=(g_bq40z50_state.Run_Time_To_Full[0] | g_bq40z50_state.Run_Time_To_Full[1]<< 8);
			
			if(temp==0xffff)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",temp/*/6/10,(temp/6)%10*/);
			LcdSetIcon(ICON_h,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case CHRGE_ITEM_VOLT:
			sprintf((char*)tempStr,"%d%d",volt/1000,(volt/100)%10);
			LcdSetIcon(ICON_V,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case CHRGE_ITEM_CURRENT:
			 
			if(flag&&current)
				sprintf((char*)tempStr,"-%d%d",(current /1000),(current /100)%10);
			else
				sprintf((char*)tempStr,"%d%d",(current /1000),(current /100)%10);

			LcdSetIcon(ICON_A,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case CHRGE_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}


void MenuShowBattery(uint8_t submenu,uint8_t *tempStr)
{

	uint8_t i,flag=1;
	uint16_t volt=0,current=0,power=0,temp=0;

	#ifdef UI1K_V13_PROJECT
	current=  GattGetBattCurrent();//(g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8);
	volt = (g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8);
	power=CoulomGetRtPower();
	if(CoulomGetChrgState()==CHRGE_STATE_CHRGE)
		flag = 0;
	
	#else
	current= g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8;
	volt = g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8;

	if(current > 32768) //充电电流 < 放电电流  0w
	{
		current = 65535 - current;
		flag = 1;
	}

	power = (uint16_t)(current * CURRENT_CAL * volt / 10000);
	#endif
	
	LcdSetIcon(ICON_BAT_FRAME,TRUE);
	LcdSetIcon(ICON_BAT_20,TRUE);
	LcdSetIcon(ICON_BAT_40,TRUE);
	LcdSetIcon(ICON_BAT_60,TRUE);
	LcdSetIcon(ICON_BAT_80,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(g_UiMenu.submenu)
	{
		case BAT_ITEM_SOC:
			sprintf((char*)tempStr,"%d",(g_bq40z50_state.Relative_State_of_Charge[0] | g_bq40z50_state.Relative_State_of_Charge[1]<<8));
			LcdSetIcon(ICON_PRECENT,TRUE);
			break;
		case BAT_ITEM_REMAIN_CAP:
			#ifdef UI1K_V13_PROJECT
			temp =(g_bq40z50_state.Remaning_Capacity[0]| g_bq40z50_state.Remaning_Capacity[1]<<8) ;
			#else
			temp =(g_bq40z50_state.Remaning_Capacity[0]| g_bq40z50_state.Remaning_Capacity[1]<<8) / 100 * CAP_CAP;
			#endif
			
			sprintf((char*)tempStr,"%d%d",temp/1000,(temp%1000)/100);
			LcdSetIcon(ICON_A,TRUE);
			LcdSetIcon(ICON_H,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case BAT_ITEM_TEMP:
			#ifdef UI1K_V13_PROJECT
			temp=GattGetBattTemp();
			#else
			temp=((g_bq40z50_state.Temperature[0] | g_bq40z50_state.Temperature[1]<<8) - 2731);
			#endif
			//if(temp-9>=0)
				sprintf((char*)tempStr,"%d",(temp));
			/*else
				sprintf(tempStr,"%d%d",(temp-9)/10,(temp-9)%10);*/
			
			LcdSetIcon(ICON_CELSIUS,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case BAT_ITEM_REMAIN_TIME:
			temp=(g_bq40z50_state.Run_Time_To_Empty[0] | g_bq40z50_state.Run_Time_To_Empty[1]<< 8);
			
			if(temp==0xffff)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",temp/*/6/10,(temp/6)%10*/);
			LcdSetIcon(ICON_H,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case BAT_ITEM_VOLTAGE:
			sprintf((char*)tempStr,"%d%d",volt/1000,(volt/100)%10);
			LcdSetIcon(ICON_V,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case BAT_ITEM_CURRENT:
		 
			if(flag&&current)
				sprintf((char*)tempStr,"-%d%d",(current /1000),(current /100)%10);
			else
				sprintf((char*)tempStr,"%d%d",(current /1000),(current /100)%10);
			LcdSetIcon(ICON_A,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case BAT_ITEM_CYC_TIME:
			sprintf((char*)tempStr,"%d",g_bq40z50_state.Cycle_Count[0] | g_bq40z50_state.Cycle_Count[1]<<8);
			break;	
		case BAT_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}


void MenuShowTime(uint8_t submenu,uint8_t *tempStr)
{
	
	uint8_t i,flag=1;
	uint16_t volt=0,current=0,power=0,temp=0;

	#ifdef UI1K_V13_PROJECT
	current= GattGetBattCurrent();// (g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8);
	volt = (g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8);
	power=CoulomGetRtPower();
	if(CoulomGetChrgState()==CHRGE_STATE_CHRGE)
		flag = 0;
	#else
	current= g_bq40z50_state.Current[0] | g_bq40z50_state.Current[1]<<8;
	volt = g_bq40z50_state.Voltage[0] | g_bq40z50_state.Voltage[1]<<8;

	if(current > 32768) //充电电流 < 放电电流  0w
	{
		current = 65535 - current;
		flag = 1;
	}

	power = (uint16_t)(current * CURRENT_CAL * volt / 10000);
	#endif
	
	LcdSetIcon(ICON_CLOCK,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(g_UiMenu.submenu)
	{
		case TIME_ITEM_CHRGE_FULL:
			temp=(g_bq40z50_state.Run_Time_To_Full[0] | g_bq40z50_state.Run_Time_To_Full[1]<< 8);
			
			if(temp==0xffff)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",/*(temp/6)/10,(temp/6)%10*/temp);
			LcdSetIcon(ICON_h,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case TIME_ITEM_RECHRGE_EMPTY:
			temp=(g_bq40z50_state.Run_Time_To_Empty[0] | g_bq40z50_state.Run_Time_To_Empty[1]<< 8);
			
			if(temp==0xffff)
				memcpy(tempStr,"----",4);
			else
				sprintf((char*)tempStr,"%d",temp/*/6/10,(temp/6)%10*/);
			LcdSetIcon(ICON_H,TRUE);
			//LcdSetIcon(ICON_POINT,TRUE);
			break;
		case TIME_ITEM_RUN_TIME:

			sprintf((char*)tempStr,"%d%d",(CoulomGetRuntime()/3600),(CoulomGetRuntime()*10/3600)%10);
			//LcdSetIcon(ICON_h,TRUE);
			LcdSetIcon(ICON_H,TRUE);
			LcdSetIcon(ICON_h,TRUE);
			LcdSetIcon(ICON_POINT,TRUE);
			break;
		case TIME_ITEM_TOTAL_RUN_TIME:
			sprintf((char*)tempStr,"%d",(EEpGetAccuRuntime()+CoulomGetRuntime())/3600/24);
			LcdSetIcon(ICON_d,TRUE);
			break;
		case TIME_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}

void MenuShowBle(uint8_t submenu,uint8_t *tempStr)
{
	LcdSetIcon(ICON_BLE,TRUE);
	LcdSetIcon(ICON_WIFI,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(submenu)
	{
		case BLE_ITEM_ON:
			memcpy(tempStr,"ON  ",4);
			break;
		case BLE_ITEM_OFF:
			memcpy(tempStr,"OFF ",4);
			break;
		case BLE_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}


void MenuShowPayg(uint8_t submenu,uint8_t *tempStr)
{
	LcdSetIcon(ICON_LOCK,TRUE);
	LcdSetIcon(ICON_UNLOCK,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(g_UiMenu.submenu)
	{
		case PAYG_ITEM_REMAIN_DAYS:
			if(/*payg.remaing_days>=1096||*/payg.free == YES)
				memcpy(tempStr,"free",4);
			else	
				sprintf((char*)tempStr,"%d",payg.remaing_days);
			LcdSetIcon(ICON_d,TRUE);
			break; 
		/*case PAYG_ITEM_INPUT:
			memcpy(tempStr,"in-p",4);
			break;*/
		case PAYG_ITEM_FEEDBACK:
			//memcpy(tempStr,"fb %d",4);
			if(g_tokenState==TOKEN_USEED)
				memcpy(tempStr,"  C0",4);
			else 
				memcpy(tempStr,"  E4",4);
			break;
		case PAYG_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}


void MenuShowPower(uint8_t submenu,uint8_t *tempStr)
{
	LcdSetIcon(ICON_POWER,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);
	
	switch(g_UiMenu.submenu)
	{
		#ifdef ROLL_SWITCH
		
		case PWR_ITEM_AC:
			LcdSetIcon(ICON_POINT,TRUE);
			if(g_AcState)
				memcpy(tempStr,"ac i",4);
			else
				memcpy(tempStr,"ac o",4);
			
			break;
		case PWR_ITEM_DC:
			LcdSetIcon(ICON_POINT,TRUE);
			if(g_DcState)
				memcpy(tempStr,"dc i",4);
			else
				memcpy(tempStr,"dc o",4);
			break;	
		case PWR_ITEM_USB:
			LcdSetIcon(ICON_POINT,TRUE);
			if(g_UsbState)
				memcpy(tempStr,"usbi",4);
			else
				memcpy(tempStr,"usbo",4);
			break;	
		#else
		case PWR_ITEM_ON:
			memcpy(tempStr,"ON  ",4);
			break; 
		#endif	
		case PWR_ITEM_OFF:
			memcpy(tempStr,"OFF ",4);
			break;
		case PWR_ITEM_ESC:
			memcpy(tempStr,"ESC ",4);
			break;
		}
}

void MenuShowPaygInput(uint8_t submenu,uint8_t *tempStr)
{
	uint8_t i,n,m=0;
	
	LcdSetIcon(ICON_LOCK,TRUE);
	LcdSetIcon(ICON_UNLOCK,TRUE);
	LcdSetIcon(ICON_LINE,TRUE);

	if(g_PaygInput.len>=4)
		m=3;
	if(g_PaygInput.len>=7)
		m=6;
	if(g_PaygInput.len>=10)
		m=9;
	if(g_PaygInput.len>=13)
		m=12;
	if(g_PaygInput.len>=16)
		m=15;
	if(g_PaygInput.len>=19)
		m=18;


	n=g_PaygInput.len-m;
		
	memcpy(tempStr,"----",4);

	//if(g_PaygInput.len>=21)
	if(g_PaygInput.key[g_PaygInput.pos]==12)		
	{
		tempStr[3]='z';
		}
	 else if(g_PaygInput.key[g_PaygInput.pos]==10)	
		tempStr[3]='-';
	else if(g_PaygInput.key[g_PaygInput.pos]==11)	
		tempStr[3]='y';
	else
		tempStr[3]=g_PaygInput.key[g_PaygInput.pos]+'0';
		
		
	
	for(i=0;i<n;i++)
	{
		tempStr[i]=g_PaygInput.key[m+i]+'0';
		}
}
#endif

