#include"main.h"
#include"string.h"
#include <stdio.h>
#include <stdarg.h>

MQTT_STATE_DEF g_Mqtt_State=MQTT_STATE_IDLE;
SMS_STATE_DEF g_Sms_State=SMS_STATE_IDLE;
uint8_t  g_Mqtt_Request=MQTT_REQ_NONE;


uint8_t g_Mqtt_hold=FALSE;
uint8_t g_SimCard_State=TRUE;
uint8_t g_NetConnect_State=FALSE;
__IO uint8_t g_GsmWkup_Event=FALSE;
uint8_t g_MqttReconnect=FALSE;


extern uint8_t g_AtAckState;
extern TIMER_TypeDef g_AtTimeout_timer;

extern UART_HandleTypeDef huart1;
extern uint8_t g_AtCmdState;
extern uint8_t g_AtAckState;

extern TIMER_TypeDef g_AtAckTout_timer;


extern USER_SET_TypeDef g_UserSet;


uint8_t GmsGetSimcardState(void)
{
	return g_SimCard_State;
}

void  GmsSetSimcardState(uint8_t state)
{
	 g_SimCard_State=state;
}

uint8_t  GmsNetConnectState(void)
{
	return  g_NetConnect_State;
}

void  GmsSetNetConnectState(uint8_t state)
{
	  g_NetConnect_State=state;
}


void  GmsWakeup(void)
{
	  g_GsmWkup_Event=TRUE;
}


void GsmComIdle(void)
{
	g_Mqtt_State=MQTT_STATE_IDLE;
	g_Sms_State=SMS_STATE_IDLE;
	TimerAtTOutStop();
}

void GsmComInit(void)
{
	AtCmdInit();
	MqttInit();
	SmsInit();
}

void GsmComProc(void)
{
	SmsProc();
	MqttProc();
	AtCmdProc();

	if(g_GsmWkup_Event)
	{
		g_GsmWkup_Event=FALSE;
		HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_SET);
		}

	if(GetTimerGsmComEvent())
	{
		GsmComInit();
		//HAL_Delay(2000); 
		TimerSleepSet();

		TimerEventClear(TIMER_GSMCOM);
		}
}

void MqttInit(void)
{
	g_Mqtt_State=MQTT_STATE_IDLE;
}

void SmsInit(void)
{
	#ifdef CAMP_PROJECT
	if(EEpGetLowBatState()==FALSE)
	#endif
	{	g_Sms_State=SMS_STATE_INIT;
		HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_SET);
		}
}

void MqttSetRequest(uint8_t request)
{
	g_Mqtt_Request=request;
}

uint8_t MqttGetRequest(void)
{
	return g_Mqtt_Request;
}

uint8_t MqttGetState(void)
{
	return g_Mqtt_State;
}


void MqttProc(void)
{

	if(g_Mqtt_hold)
		return ;
	

	switch(g_Mqtt_State)
	{
		case MQTT_STATE_IDLE:  
			break;
		case MQTT_STATE_INIT:
			AtCmdMerge(AT_CMD_AT);
			g_Mqtt_State=MQTT_STATE_AT;
			TimerAtTOutStart(5000U,TRUE);
			break;
		case MQTT_STATE_AT:
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_AT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_ATE0;
				}
			break;
		case MQTT_STATE_ATE0:  
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_ATE0);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CIPCLOSE;
				}
			break;
		case MQTT_STATE_CIPCLOSE:
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CIPCLOSE);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CIPSHUT;
				}
			break;
		case MQTT_STATE_CIPSHUT: 
			if(g_AtAckState==AT_ACK_OK)
			{	
				AtCmdMerge(AT_CMD_CIPSHUT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CPIN;
				}
			break;
		case MQTT_STATE_CPIN:  
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CPIN);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CSQ;
				}
			break;
		case MQTT_STATE_CSQ: 
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CSQ);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CREG;
				
				CampStart();
				}
			break;
		case MQTT_STATE_CREG:
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CREG);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CGATT;
				}
			break;
		case MQTT_STATE_CGATT: 
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CGATT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CSTT;
				}
			break;
		case MQTT_STATE_CSTT: 
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CSTT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CSTT_Q;
				}
			break;
		case MQTT_STATE_CSTT_Q: 
			if(g_AtAckState==AT_ACK_OK)
			{	AtCmdMerge(AT_CMD_CSTT_Q);
				TimerAtTOutStart(3000U,TRUE);
				g_Mqtt_State=MQTT_STATE_SAPBR;
				}
			break;	
		//GPS DATA 
		case MQTT_STATE_SAPBR:
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{	AtCmdMerge(AT_CMD_SAPBR);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CLBSCFG;
				}
			break;
		case MQTT_STATE_CLBSCFG:
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{	AtCmdMerge(AT_CMD_CLBSCFG);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CLBS;
				}
			break;
		case MQTT_STATE_CLBS:
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{	AtCmdMerge(AT_CMD_CLBS);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CIICR;
				}
			break;	
		case MQTT_STATE_CIICR:   
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{
				AtCmdMerge(AT_CMD_CIICR);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CIFSR;
			}
			break;
		case MQTT_STATE_CIFSR:  
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{
				AtCmdMerge(AT_CMD_CIFSR);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CIPSTAR;
			}
			break;
		case MQTT_STATE_CIPSTAR: 
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{
				AtCmdMerge(AT_CMD_CIPSTART);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CONNECT;
			}
			break;
		case MQTT_STATE_CONNECT:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CIPSEND);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_SUBSCRIBE;
				}

			if(g_AtAckTout_timer.retry>=AT_RETRY_CNT)//error counter
			{	
				if(g_SimCard_State)
				{	g_UserSet.report_fail_cnt++;
					EEpUpdateEnable();
					}

				AtCmdMerge(AT_CMD_AT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_AT;
				}

			break;
		case MQTT_STATE_SUBSCRIBE:	
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CIPSEND);
				TimerAtTOutStart(5000U,TRUE);
				 g_Mqtt_State=MQTT_STATE_SUBSCRIBEACK;

				/*if(g_UserSet.reportt_auto)
					g_Mqtt_State=MQTT_STATE_PUBLISH;
				else
					g_Mqtt_State=MQTT_STATE_CHK_REQUEST;//MQTT_STATE_PUBLISH;*/
				}
			break;
		case MQTT_STATE_SUBSCRIBEACK:	
			if(g_AtAckState==AT_ACK_OK)
			{
				g_NetConnect_State=TRUE;
				
				if(g_UserSet.reportt_auto&&g_MqttReconnect==FALSE)
				{	
					/*AtCmdMerge(AT_CMD_AT);
					TimerAtTOutStart(5000U,TRUE);
					g_Mqtt_State=MQTT_STATE_PUBLISH;*/
					AtCmdMerge(AT_CMD_CIPSEND);
					TimerAtTOutStart(5000U,TRUE);
					g_Mqtt_State=MQTT_STATE_PUBLISHACK;
					}
				else
				{	
					if(EEpGetWakeupCnt()>=EEpGetHeartbeat()&&g_MqttReconnect==FALSE)
					{	
						GattSingleFieldMerge("ppid");
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
						EEpSetWakeupCnt(0);
						}
					else
						g_Mqtt_Request=MQTT_REQ_NONE;
					AtCmdMerge(AT_CMD_NONE);
					TimerAtTOutStart(180000U,TRUE);
					
					g_Mqtt_State=MQTT_STATE_CHK_REQUEST;//MQTT_STATE_PUBLISH;
					g_MqttReconnect=FALSE;
					
				}
				}
			break;	
		case MQTT_STATE_PUBLISH:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CIPSEND);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_PUBLISHACK;
				}
			break;
		case MQTT_STATE_PUBLISHACK:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_NONE);
				TimerAtTOutStart(180000U,TRUE);
				g_Mqtt_Request=MQTT_REQ_NONE;
				g_Mqtt_State=MQTT_STATE_CHK_REQUEST;
				}
			break;	
		case MQTT_STATE_CHK_REQUEST:
			if(g_AtAckTout_timer.retry)
			{
				//AtCmdMerge(AT_CMD_NONE);
				TimerAtTOutStop();
				//g_Mqtt_State=MQTT_STATE_END;
			}
			
			if(g_Mqtt_Request)
			{
				AtCmdMerge(AT_CMD_AT);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_PUBLISH;
				}

			if(g_MqttReconnect)
			{
				//g_Mqtt_State=MQTT_STATE_CIPSTAR;
				/*AtCmdMerge(AT_CMD_CIPSEND);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_SUBSCRIBE;*/

				AtCmdMerge(AT_CMD_CIPSTART);
				TimerAtTOutStart(5000U,TRUE);
				g_Mqtt_State=MQTT_STATE_CONNECT;
				}
			break;	
		case MQTT_STATE_END:
			g_Mqtt_State=MQTT_STATE_IDLE;
			//HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
			break;
		}
	
}



void SmsProc(void)
{

	switch(g_Sms_State)
	{
		case SMS_STATE_IDLE:  
			break;
		case SMS_STATE_INIT:
			AtCmdMerge(AT_CMD_AT);
			g_Sms_State=SMS_STATE_AT;
			TimerAtTOutStart(10000U,TRUE);
			break;	
		case SMS_STATE_AT:
			if(/*g_AtAckState==AT_ACK_OK&&*/g_AtAckTout_timer.retry)
			{
				AtCmdMerge(AT_CMD_AT);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_ATE0;
			}
			break;
		case SMS_STATE_ATE0:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_ATE0);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CIPCLOSE;
			}
			break;
		case SMS_STATE_CIPCLOSE:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CIPCLOSE);
				TimerAtTOutStart(5000U,TRUE);
				//g_Sms_State=MQTT_STATE_CIPSHUT;
				g_Sms_State=SMS_STATE_CPIN;
			}
			break;
		case SMS_STATE_CIPSHUT: 
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{
				AtCmdMerge(AT_CMD_CIPSHUT);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CPIN;
			}
			break;
		case SMS_STATE_CPIN:  
			if(g_AtAckState==AT_ACK_OK||g_AtAckTout_timer.retry>=AT_RETRY_CNT)
			{
				AtCmdMerge(AT_CMD_CPIN);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CSQ;
			}
			break;
		case SMS_STATE_CSQ: 
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CSQ);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CREG;
			}
			break;
		case SMS_STATE_CREG:  
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CREG);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CGAT;
			}
			break;
		case SMS_STATE_CGAT:  
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CGATT);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGF;
			}
			break;	
		case SMS_STATE_CMGF:  
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGF);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_WAIT;
			}
			break;
		case SMS_STATE_WAIT:   
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_NONE);
				TimerAtTOutStart(10000U,TRUE);
				g_Sms_State=SMS_STATE_JUMP;
				}
			break;
		case SMS_STATE_JUMP:      // NORMAL JUMP TO MQTT INIT
			if(g_AtAckTout_timer.retry)
			{
				TimerAtTOutStop();
				g_Sms_State=SMS_STATE_IDLE;//SMS_STATE_IDLE;
				g_Mqtt_State=MQTT_STATE_INIT;  //ENTER TO MQTT SEND  PUBLISH
				}
			break;			
		case SMS_STATE_CPMS:                       //CHECK SMS
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CPMS);
				TimerAtTOutStart(30000U,TRUE);
				g_Sms_State=SMS_STATE_CMGR;
			}
			break;
		case SMS_STATE_CMGR: 				//READ AND PARSE SMS
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGR);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGR_PARSE;//SMS_STATE_CMGS;    //ENTER SMS SEND MODE
			}
			break;
		case SMS_STATE_CMGR_PARSE:
			{
				if(g_AtAckState==AT_ACK_OK)
				{
					AtCmdMerge(AT_CMD_NONE);
					TimerAtTOutStart(3000U,TRUE);
					}
				
				if(g_AtAckTout_timer.retry)
				{
					AtCmdMerge(AT_CMD_AT);
					TimerAtTOutStart(5000U,TRUE);
					g_Sms_State=SMS_STATE_CMGS;    //ENTER SMS SEND MODE
					}
			}
			break;
		case SMS_STATE_CMGS:    //SEND  SMS START
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGS); //SMS ADDR
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGS_SMS;
			}
			break;
		case SMS_STATE_CMGS_SMS:  // SMS  TXT
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGS_SMS);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGF_0;
			}
			break;	
		case SMS_STATE_READ:    // READ SMS START
			{
				AtCmdMerge(AT_CMD_AT);
				TimerAtTOutStart(5000U,TRUE);
			         g_Sms_State=SMS_STATE_CPMS;
			}
			break;
		case SMS_STATE_CMGF_0:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGF_0);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGDA;  	//ENTER DELAY MODE
			}
			break;
		case SMS_STATE_CMGDA:  //DEL  ALL SMS
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGDA);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_CMGF_1;	
			}
			break;	
		case SMS_STATE_CMGF_1:
			if(g_AtAckState==AT_ACK_OK)
			{
				AtCmdMerge(AT_CMD_CMGF);
				TimerAtTOutStart(5000U,TRUE);
				g_Sms_State=SMS_STATE_END;
				}
			break;	
		case SMS_STATE_END:
			if(g_AtAckState==AT_ACK_OK)
			{
				TimerAtTOutStop();
				g_Sms_State=SMS_STATE_IDLE;
				g_Mqtt_hold=FALSE;
				}
			break;		
		}
	
}


