
#include "main.h"


#define KEY_OFFSET  100

//_TIMER_ g_key_timer;




//const u16 KeyVoltTable[3]={0x00C0,0x052A,0x099a };

uint8_t g_led_enable=1;

uint8_t g_oldkey_value;
uint32_t g_u16KeyEvent  ;
uint8_t g_Roll_State=0;
__IO uint8_t  g_AcState=0;
__IO uint8_t  g_DcState=0;
__IO uint8_t  g_UsbState=0;

__IO uint8_t  g_DemoState=0;


__IO uint8_t  g_RollSwitch=0;
KEY_INPUT_TypeDef g_PaygInput;
__IO uint8_t g_KeyboardPwrEnable=TRUE;

const uint8_t  g_SubMenuRange[][2]=
{
	{IDLE_ITEM_LOGO,IDLE_ITEM_POWER},
	{LOGO_ITEM_VOLT,LOGO_ITEM_ESC},
	{CHRGE_ITEM_CAP,CHRGE_ITEM_ESC},
	{BAT_ITEM_SOC,BAT_ITEM_ESC},
	{TIME_ITEM_CHRGE_FULL,TIME_ITEM_ESC},
	{BLE_ITEM_ON,BLE_ITEM_ESC},
	{PAYG_ITEM_REMAIN_DAYS,PAYG_ITEM_ESC},
	#ifdef ROLL_SWITCH
	{PWR_ITEM_AC,PWR_ITEM_ESC},
	#else
	{PWR_ITEM_ON,PWR_ITEM_ESC},
	#endif
};



extern __IO uint16_t ADC1ConvertedValue[1] ;
extern UI_MENU_TypeDef g_UiMenu;
extern TIMER_TypeDef g_KeyTimer;
extern PAYG_TypeDef payg;
extern USER_SET_TypeDef g_UserSet;
extern uint8_t g_tokenState;
extern TIMER_TypeDef g_SleepTimer;
extern SYS_STATE_CODE_TypeDef  g_sysStateCode;
//extern IWDG_HandleTypeDef hiwdg;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
   if(GPIO_Pin==GPIO_PIN_13)
   {
	Delay(10);
	
   	if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
   	{
   		g_RollSwitch=ROLL_RUN_RIGHT;
   		}
	else
	{
		g_RollSwitch=ROLL_RUN_LEFT;
		}
   	}

   //if(GPIO_Pin==GPIO_PIN_13)
   {
	 // g_KeyboardPwrEnable=TRUE;

	//  TimerKbInsertSet(2*60*1000);
   	}

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */

 	}

uint8_t KeyboardPlugIn(void)
{
	return g_KeyboardPwrEnable;
}
void  KeyboardUsbPwrSet(uint8_t enable)
{
	 g_KeyboardPwrEnable=enable;
}
void  KeyInit(void)
{
	//memset((u8*)&g_key_timer,0x00,sizeof(_TIMER_));
	g_oldkey_value=KEY_NONE;
	g_u16KeyEvent=0;

	g_Roll_State=ROLL_IDLE_INIT;
	g_RollSwitch=ROLL_IDLE_INIT;

	memset((uint8_t*)&g_PaygInput,0x00,sizeof(g_PaygInput));
}


uint8_t  IsKeyEvent(void)
{
    if(g_oldkey_value!=KEY_NONE)
	 return TRUE;
    else
	 return FALSE;
}




//u16 adcbuffer[1024]={0};
//u16 cnt=0;



void RollSwitchScan(void)
{

	//if(g_systate==SYS_RUN)
	{
		switch(g_Roll_State)
		{
			/*case ROLL_IDLE_INIT:
				if(GPIO_ReadInputDataBit(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_SET
					&&GPIO_ReadInputDataBit(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)
				{	g_Roll_State=ROLL_RUN_LOW;
					g_RollSwitch=ROLL_RUN_LEFT;
					}

				if(GPIO_ReadInputDataBit(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_RESET
					&&GPIO_ReadInputDataBit(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
				{	g_Roll_State=ROLL_RUN_LOW;
					g_RollSwitch=ROLL_RUN_RIGHT;
					}
				break;
			case ROLL_RUN_LOW:
				if(GPIO_ReadInputDataBit(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_RESET
					&&GPIO_ReadInputDataBit(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)
				{
					g_Roll_State=ROLL_RUN_HIGH;
					}
				break;
			case ROLL_RUN_HIGH:
				if(GPIO_ReadInputDataBit(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_SET
					&&GPIO_ReadInputDataBit(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
				{
					g_Roll_State=ROLL_IDLE_INIT;
					}
				break;*/
			case ROLL_IDLE_INIT:
				if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_SET&&HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
					g_Roll_State=ROLL_IDLE_HIGH;
				else
					g_Roll_State=ROLL_IDLE_LOW;
				break;
			case ROLL_IDLE_LOW:
				if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_SET)
				{
					if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)
					 {	g_RollSwitch=ROLL_RUN_RIGHT;
						g_Roll_State=ROLL_RUN_RIGHT;
						}
					}
				else
				{
					if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
					 {	g_RollSwitch=ROLL_RUN_LEFT;
						g_Roll_State=ROLL_RUN_LEFT;
						}
					}
				break;
			case ROLL_IDLE_HIGH:
				if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_RESET)
				{
					if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
					 {	g_RollSwitch=ROLL_RUN_RIGHT;
						g_Roll_State=ROLL_RUN_RIGHT;
						}
					}
				else
				{
					if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)
					 {	g_RollSwitch=ROLL_RUN_LEFT;
						g_Roll_State=ROLL_RUN_LEFT;
						}
					}
				break;
				
			case ROLL_RUN_RIGHT:
			case ROLL_RUN_LEFT:	
				if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_RESET
					&&HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)
					g_Roll_State=ROLL_IDLE_LOW;

				if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_SET
					&&HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_SET)
					g_Roll_State=ROLL_IDLE_HIGH;
				break;

		}
	}
	

}

uint8_t GetUserAcState(void)
{
	return g_AcState;
}

uint8_t GetUserDcState(void)
{
	return g_DcState;
}

uint8_t GetUserUsbState(void)
{
	return g_UsbState;
}

uint8_t GetDemoState(void)
{
	return g_DemoState;
}

void  DemoModeClose(void)
{
	 g_DemoState=FALSE;
}

void  DemoModeStart(void)
{
	 g_DemoState=TRUE;
}


void KeyScan(void)
{
    uint8_t keyBtn=KEY_NONE;

	


	
	#ifdef UI1K_V13_PROJECT
	if(AdcGetBatVolt()<=1000U)
	{
		keyBtn=KEY_ENTER;
		}
	else
	#endif
	{

		#ifndef ROLL_SWITCH
		if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_POWER;
		else if(HAL_GPIO_ReadPin(SW_B_GPIO_Port,SW_B_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_NEXT;
		else if(HAL_GPIO_ReadPin(SW_A_GPIO_Port,SW_A_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_PREV;
		#else

		#ifdef UI1K_V13_PROJECT
		if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_USB;
		else if(HAL_GPIO_ReadPin(/*PWR_KEY_GPIO_Port,PWR_KEY_Pin*/BAT_C_CHRG_IN_GPIO_Port,BAT_C_CHRG_IN_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_POWER;
		else if(HAL_GPIO_ReadPin(DC_KEY_GPIO_Port,DC_KEY_Pin)==GPIO_PIN_RESET)
		    keyBtn=KEY_AC;	
		else if(HAL_GPIO_ReadPin(USB_KEY_GPIO_Port,USB_KEY_Pin)==GPIO_PIN_RESET)
		    keyBtn=KEY_DC;	
		#elif defined(DC_PUMP_SUPPORT)
		if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_ENTER;
		else if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_NEXT;
		else if(HAL_GPIO_ReadPin(KEY_ENTER_GPIO_Port,KEY_ENTER_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_PREV;
		#else
		if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==GPIO_PIN_RESET)	
	   	    keyBtn=KEY_POWER;
		else if(HAL_GPIO_ReadPin(ENTER_KEY_GPIO_Port,ENTER_KEY_Pin)==GPIO_PIN_RESET)
		    keyBtn=KEY_ENTER;	
		#endif
		#endif
		}
	



	if(keyBtn!=KEY_NONE)
	{
		TimeBacklightSet();
		
		if(g_KeyTimer.count == 0)		//first press key
		{
			g_oldkey_value =keyBtn;
			g_KeyTimer.enable=TRUE;
			//g_key_timer.downlock=FALSE;
		}
		else		//check key pressed
		{
			if(g_oldkey_value!=keyBtn)		//if the same key
			{
				g_KeyTimer.count = 0;
				//g_key_timer.downlock=FALSE;
				g_oldkey_value = keyBtn;
				g_KeyTimer.holdlock=FALSE;
			}
		}
	}
	else
	{
			#ifdef ROLL_SWITCH
			if(g_RollSwitch==ROLL_RUN_RIGHT)
			{	g_u16KeyEvent = (EVENT_CLICK << 8)|KEY_PREV;
				g_RollSwitch=0;
				TimeBacklightSet();
				}
			else if(g_RollSwitch==ROLL_RUN_LEFT)
			{	g_u16KeyEvent = (EVENT_CLICK << 8)|KEY_NEXT;
				g_RollSwitch=0;
				TimeBacklightSet();
				}
			#endif
		}

	if(g_KeyTimer.count > DEBOUNCE_TIME)		//key press active
	{
		
		if(keyBtn==KEY_NONE)
		{     
		      
			if(g_KeyTimer.count <= SHORT_PRESSED_TIME)		//short press
			{
				g_u16KeyEvent = EVENT_CLICK;
			}
			else if(g_KeyTimer.count < LONG_PRESSED_TIME)		//long press
			{
				g_u16KeyEvent = EVENT_UP;
			}
			g_KeyTimer.count = 0;
			//g_key_timer.downlock=FALSE;
			g_KeyTimer.holdlock=FALSE;
			g_KeyTimer.enable=FALSE;
			
			g_u16KeyEvent = (g_u16KeyEvent << 8)|g_oldkey_value;
			g_oldkey_value=KEY_NONE;
			
		}
		else
		{
			if(g_KeyTimer.count >= HOLD_TIME)
			{
				if(g_KeyTimer.holdlock==FALSE)
				{
					g_u16KeyEvent = EVENT_HOLD;
 					g_KeyTimer.holdlock=TRUE;
					}
				else
				{    g_u16KeyEvent=EVENT_PRESS;
					}

				g_u16KeyEvent = (g_u16KeyEvent << 8)|g_oldkey_value;
			}
			/*else if(g_key_timer.count  >=10&&g_key_timer.downlock==FALSE)
			{
				g_u16KeyEvent = EVENT_DOWN;
				g_u16KeyEvent = (g_u16KeyEvent << 8)|g_oldkey_value;
				g_key_timer.downlock=TRUE;
				}*/
			else
			{
				g_u16KeyEvent = EVENT_PRESS;
				g_u16KeyEvent = (g_u16KeyEvent << 8)|g_oldkey_value;
				}
	
		}
	
}

	
}




void KeyEventProcess(void)
{
	uint8_t i,temp=0;

	KeyScan();
	
	if(g_u16KeyEvent==0)
	{
		return ;
		}	


	TimeOutSet();

	
	switch(g_u16KeyEvent)
	{
	
		case BTN_EVENT_IS(KEY_ENTER, EVENT_CLICK):
			{
				#ifdef DC_PUMP_SUPPORT
				if(g_led_enable)
				{	
					g_led_enable=0;
					}
				else
				{	g_led_enable=1;

					if(GmsNetConnectState()==FALSE)
					{	//g_UserSet.ble_state=TRUE;
						EEpUpdateEnable();

						g_SleepTimer.enable=TRUE;
						g_SleepTimer.period=FALSE;
						g_SleepTimer.count=EEpGetOnlineTime();
						g_SleepTimer.event=FALSE;

						GsmComInit();
						}
					}
				
				#else
				switch(g_UiMenu.menu)
				{
					case MENU_IDLE:
					{	g_UiMenu.menu=MENU_LOGO+g_UiMenu.submenu;
						g_UiMenu.submenu=0;
						}
						break;
					case MENU_LOGO:	
					case MENU_CHARGE:
					case MENU_BATTERY:
					case MENU_TIME:
					case MENU_BLE:
					case MENU_PAYG:

						if(g_UiMenu.menu==MENU_BLE)
						{
							if(g_UiMenu.submenu==BLE_ITEM_ON)
							{	g_UserSet.ble_state=TRUE;
								EEpUpdateEnable();

								g_SleepTimer.enable=TRUE;
								g_SleepTimer.period=FALSE;
								g_SleepTimer.count=EEpGetOnlineTime();
								g_SleepTimer.event=FALSE;

								GsmComInit();
								}
							if(g_UiMenu.submenu==BLE_ITEM_OFF)
							{	g_UserSet.ble_state=FALSE;
								EEpUpdateEnable();
								if(GmsNetConnectState())
								{
									HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
			 						GmsSetNetConnectState(FALSE);	 
									MqttInit();
									MqttSetRequest(MQTT_REQ_ALLFIELD);
									}
								}
							
							}
							
						
						if(g_UiMenu.submenu==g_SubMenuRange[g_UiMenu.menu][SUBMENU_MAX])
						{	g_UiMenu.submenu=g_UiMenu.menu-MENU_LOGO;
							g_UiMenu.menu=MENU_IDLE;
							break;
							}
						if(g_UiMenu.menu==MENU_PAYG)
						{
							g_UiMenu.menu=MENU_PAYG_INPUT;
							g_UiMenu.submenu=0;
							memset((uint8_t*)&g_PaygInput,0x00,sizeof(g_PaygInput));
							g_PaygInput.len=0;
							}
						break;
					case MENU_POWER:
						
						if(g_UiMenu.submenu==g_SubMenuRange[g_UiMenu.menu][SUBMENU_MAX])
						{	
								g_UiMenu.submenu=g_UiMenu.menu-MENU_LOGO;
								g_UiMenu.menu=MENU_IDLE;
							}
						else if(g_UiMenu.submenu==PWR_ITEM_OFF)
						{	
						     #ifdef UI1K_V13_PROJECT
							 	SysPowerOff();
						     #endif
							}
						break;
					case MENU_PAYG_INPUT:

						if(g_PaygInput.key[g_PaygInput.pos]==10&&g_PaygInput.len>0)
						{
							
							g_PaygInput.len--;
							g_PaygInput.pos--;
							g_PaygInput.key[g_PaygInput.pos]=10;
							break;
							}
						else if(g_PaygInput.key[g_PaygInput.pos]==11||g_PaygInput.key[g_PaygInput.pos]==10&&g_PaygInput.len==0)
						{
							g_UiMenu.menu=MENU_PAYG;
							g_UiMenu.submenu=PAYG_ITEM_REMAIN_DAYS;
							}

						if(g_PaygInput.len>=21&&g_PaygInput.key[g_PaygInput.pos]==12)
						{	
							uint8_t token[32],i;
							uint32_t token_hi,token_lo;
							uint8_t temp[12],*p;
							memset(token,0x00,32);

							for(i=0;i<21;i++)
								token[i]=g_PaygInput.key[i]+'0';

							//memcpy(token,"031093657401791773705",21);

						        token_lo = AtouI(&token[11]);
							token[11]=0x00;	
							token_hi = AtouI(token);

							memset(token,0x00,32);

							token[21]= token_lo>>24;
						        token[20]= token_lo>>16;
						        token[19]= token_lo>>8;
						        token[18]= token_lo;
						        
						        token[25]= token_hi>>24;
						        token[24]= token_hi>>16;
						        token[23]= token_hi>>8;
						        token[22]= token_hi;

								
							p=&token[18] ;   // ±£³ÖÔ­×´

							temp[3] = p[7];
						        temp[2] = p[6];
						        temp[1] = p[5];
						        temp[0] = p[4];
						        temp[7] = p[3];
						        temp[6] = p[2];
						        temp[5] = p[1];
						        temp[4] = p[0];

							memcpy((uint8_t*)payg.hast_input, temp, 8);
							/*payg.hast_input[0]=token_lo;
							payg.hast_input[1]=token_hi;*/

							PaygOvesInput();

							EEpUpdateEnable();

							g_UiMenu.menu=MENU_PAYG;
							
							if(g_tokenState==TOKEN_OK)
								g_UiMenu.submenu=PAYG_ITEM_REMAIN_DAYS;
							else
								g_UiMenu.submenu=PAYG_ITEM_FEEDBACK;	

							break;
							}

						g_PaygInput.len++;
						g_PaygInput.pos++;
						g_PaygInput.key[g_PaygInput.pos]=g_PaygInput.key[g_PaygInput.pos-1];

						if(g_PaygInput.len>=21)
						{
							g_PaygInput.key[g_PaygInput.pos]=12;
							}
						break;
					}
				#endif
				MenuRefresh();
				break;
			}
		#ifdef ROLL_SWITCH
	       case BTN_EVENT_IS(KEY_AC, EVENT_CLICK):
			{
				if((payg.remaing_days>0||payg.free == YES||GetDemoState())&&CoulomLowbat()==LOWBAT_NONE)
				{
					if(g_AcState==FALSE)
					{
						//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);
						g_AcState=TRUE;
						g_sysStateCode.error.ac_fault=FALSE;
						}
					else
					{	//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);
						g_AcState=FALSE;
						}

					//MenuPopSet(POP_MENU_AC);
					}
				//else
					MenuPopSet(POP_MENU_AC);
			break;
			}
		    case BTN_EVENT_IS(KEY_DC, EVENT_CLICK):
			{
				if((payg.remaing_days>0||payg.free == YES||GetDemoState())&&CoulomLowbat()<LOWBAT_LEVEL2)
				{
					if(g_DcState==FALSE)
					{
						//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);
						g_DcState=TRUE;
						g_sysStateCode.error.dc_current_over=FALSE;
						}
					else
					{	//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);
						g_DcState=FALSE;
						}

					//MenuPopSet(POP_MENU_DC);
					}	
				MenuPopSet(POP_MENU_DC);
			break;
			}
		 case BTN_EVENT_IS(KEY_USB, EVENT_CLICK):
			{
				if((payg.remaing_days>0||payg.free == YES||GetDemoState())&&CoulomLowbat()<LOWBAT_LEVEL2)
				{
					if(g_UsbState==FALSE)
					{
						//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);
						g_UsbState=TRUE;
						}
					else
					{	//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);
						g_UsbState=FALSE;
						}
					}	
				MenuPopSet(POP_MENU_USB);
			break;
			}	
		 case BTN_EVENT_IS(KEY_POWER, EVENT_HOLD):
			{
				uint16_t counter_delay=0;
			   LcdClearAll();
			   LcdUpdateAll();
			   HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_RESET);
			   while(counter_delay<1000)
			   {
			   	#ifdef WDG_ENABLE
			    HAL_IWDG_Refresh(&hiwdg);
				#endif
				HAL_Delay(100);
			  	HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);
				
			   	}
			  break;
			}
		#else
		case BTN_EVENT_IS(KEY_POWER, EVENT_HOLD):
			{
				#ifdef UI1K_V13_PROJECT	
				if(payg.free == YES||payg.remaing_days>0)
				{
					if(g_AcState==FALSE)
					{
						//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);
						g_AcState=TRUE;
						}
					else
					{	//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);
						g_AcState=FALSE;
						}
					}
				#else
				HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);
				#endif
					
			break;
			}
		#endif
		//case BTN_EVENT_IS(KEY_ENTER, EVENT_CLICK):
		case BTN_EVENT_IS(KEY_ENTER, EVENT_HOLD):
			{
				 #ifdef UI1K_V13_PROJECT

				 SysPowerOff();
				 /*HAL_GPIO_WritePin(PV_CHRG_CTRL_GPIO_Port, PV_CHRG_CTRL_Pin, GPIO_PIN_RESET);
				 
				// if(HAL_GPIO_ReadPin(PV_ON_CHRGE_IN_GPIO_Port, PV_ON_CHRGE_IN_Pin)==GPIO_PIN_SET)
				 {
					 LcdClearAll();
					 LcdUpdateAll();
					 HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_RESET);
					 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET); 

					  CoulomSleep();

					  while(AdcGetBatVolt()<=5000U)
					 {
					 	AdcProc();
					 	}
					  HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);

					 HAL_Delay(2000);
				 	}*/
				  #endif
				break; 
			}

		case BTN_EVENT_IS(KEY_PREV, EVENT_CLICK):
			{
				#ifdef DC_PUMP_SUPPORT
				if(g_UiMenu.menu>MENU_SPEED)
				{
					g_UiMenu.menu--;
					}
				else
				{
					g_UiMenu.menu=MENU_RM_DAY;
					}
				#else
				switch(g_UiMenu.menu)
				{
					case MENU_IDLE:
					case MENU_LOGO:
					case MENU_CHARGE:
					case MENU_BATTERY:
					case MENU_TIME:
					case MENU_BLE:
					//case MENU_PAYG:
					case MENU_POWER:
						{
							if(g_UiMenu.submenu>g_SubMenuRange[g_UiMenu.menu][SUBMENU_MIN] )
								g_UiMenu.submenu--;
							else
								g_UiMenu.submenu=g_SubMenuRange[g_UiMenu.menu][SUBMENU_MAX];
						}
						break;
					case MENU_PAYG:
							if(g_UiMenu.submenu==PAYG_ITEM_REMAIN_DAYS)
								g_UiMenu.submenu=PAYG_ITEM_ESC;
							else
								g_UiMenu.submenu=PAYG_ITEM_REMAIN_DAYS;
						break;
					case MENU_PAYG_INPUT:
						if(g_PaygInput.len>=21)
						{
							if(g_PaygInput.key[g_PaygInput.pos]>10)
								g_PaygInput.key[g_PaygInput.pos]--;
							else
								g_PaygInput.key[g_PaygInput.pos]=12;
							}
						else
						{
							if(g_PaygInput.key[g_PaygInput.pos]>0)
								g_PaygInput.key[g_PaygInput.pos]--;
							else
								g_PaygInput.key[g_PaygInput.pos]=11;
							}
						break;
					}
				#endif
				MenuRefresh();
				break; 
			}
		  case BTN_EVENT_IS(KEY_NEXT, EVENT_CLICK):
			{
				#ifdef DC_PUMP_SUPPORT
				if(g_UiMenu.menu<MENU_RM_DAY)
				{
					g_UiMenu.menu++;
					}
				else
				{
					g_UiMenu.menu=MENU_SPEED;
					}
				#else
				switch(g_UiMenu.menu)
				{
					case MENU_NONE:
						break;
					case MENU_IDLE:
					case MENU_LOGO:	
					case MENU_CHARGE:
					case MENU_BATTERY:
					case MENU_TIME:
					case MENU_BLE:
					//case MENU_PAYG:
					case MENU_POWER:
						if(g_UiMenu.submenu<g_SubMenuRange[g_UiMenu.menu][SUBMENU_MAX])
							g_UiMenu.submenu++;
						else
							g_UiMenu.submenu=g_SubMenuRange[g_UiMenu.menu][SUBMENU_MIN];
						break;
					case MENU_PAYG:
							if(g_UiMenu.submenu==PAYG_ITEM_REMAIN_DAYS)
								g_UiMenu.submenu=PAYG_ITEM_ESC;
							else
								g_UiMenu.submenu=PAYG_ITEM_REMAIN_DAYS;
						break;	
					case MENU_PAYG_INPUT:
						if(g_PaygInput.len>=21)
						{
							if(g_PaygInput.key[g_PaygInput.pos]<12)
								g_PaygInput.key[g_PaygInput.pos]++;
							else
								g_PaygInput.key[g_PaygInput.pos]=10;
							}
						else
						{
							if(g_PaygInput.key[g_PaygInput.pos]<11)
								g_PaygInput.key[g_PaygInput.pos]++;
							else
								g_PaygInput.key[g_PaygInput.pos]=0;
							}
						break;
					}	
				#endif
				MenuRefresh();
				break; 
			}
		}

	g_u16KeyEvent=0;
}





void KeyEventStbyProcess(void)
{
//	uint8_t i;
	
	if(g_u16KeyEvent==0)
	{
		return ;
		}	

	
	switch(g_u16KeyEvent)
	{
	
		case BTN_EVENT_IS(KEY_POWER, EVENT_CLICK):
			{
				//g_systate=SYS_INIT;
				MenuRefresh();
				break;
			}

			
	
		}

	g_u16KeyEvent=0;
}
















