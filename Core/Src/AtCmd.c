#include"main.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdarg.h>


AT_CMD_DEF g_AtCmdState=AT_CMD_NONE;
uint8_t g_AtAckState=AT_ACK_NONE;
uint8_t g_GsmRbuffer[GSM_BUFFER];
uint32_t g_RxGsmCounter=0;
uint32_t g_RxGsmParseSize=0;
uint32_t g_RxGsmParsePos=0;
//uint32_t g_RxGsmParseSize=0;

uint8_t msgid=0;
uint8_t g_endmark[]={0x1a,0X00,0x0d,0x0a};


#ifdef E_SIM_SUPPORT
char *Defualt_APN= "AT+CSTT=\"orange.m2m.spec\",\"\",\"\"\r\n"; 
#else
char *Defualt_APN= "AT+CSTT=\"internet.gma.iot\",\"\",\"\"\r\n";
#endif
char *Defualt_URL= "AT+CIPSTART=\"TCP\",\"mqtt-2.omnivoltaic.com\",\"1883\"\r\n";

//char Set_Apn[SETAPN_LEN];

const uint8_t g_MqttPubFixedPath[]="dt/V01/GPRSV2/";
const uint8_t g_MqttSubFixedPath[]="cmd/V01/GPRSV2/+/";
const uint8_t g_MqttComparePath[]="cmd/V01/GPRSV2/\0";


#define SMS_INDEX_SIZE 14
#define SMS_REPLY_SIZE 128
#define PHONE_NO_SIZE 20
#define CELL_ID_SIZE 24


static uint8_t g_SmsIndex[SMS_INDEX_SIZE];
static uint8_t  g_SmsReply[SMS_REPLY_SIZE];
uint8_t  g_PhoneNo[PHONE_NO_SIZE];
uint8_t  g_CellID[CELL_ID_SIZE];
uint8_t g_GsmExist=0;


 char message_start[47]={
  0x10,0x2C,0x00,0x06,0x4D,0x51,0x49,0x73,0x64,0x70,
  0x03,  //version
  0xC2, //flag
  0x10,0x3C,  //keep alive
  0x00,0x06,0x41,0x42,0x43,0x44,0x45,0x46,
  0x00,0x08,0x64,0x78,0x78,0x6B,0x67,0x6B,0x70,0x70,
  0x00,0x0C,0x71,0x41,0x55,0x5A,0x42,0x64,0x61,0x53,0x49,0x55,0x4C,0x78,
  0x30
  };//0x13,0x00,0x08


/*uint8_t g_testcmd[67]={
	0x10,0x2C,0x00,0x06,0x4D,0x51,0x49,0x73,0x64,0x70,0x03,0xC2,0x00,0x3C,0x00,
	0x06,0x41,0x42,0x43,0x44,0x45,0x46,0x00,0x08,0x64,0x78,0x78,0x6B,0x67,0x6B,
	0x70,0x70,0x00,0x0C,0x71,0x41,0x55,0x5A,0x42,0x64,0x61,0x53,0x49,0x55,0x4C,
	0x78
	,0x30,0x13,0x00,0x08,0x76,0x61,0x6C,0x65,0x74,0x72,0x6F,0x6E,0x68,0x65,0x6C,0x6C,0x6F,0x72,0x61,0x76,0x69
};*/

#define TOPIC_LEN    64

char MQTT_ClienID[60];
char MQTT_topic_ppid[TOPIC_LEN];
//char MQTT_topic_ppid_ext[20];

char MQTT_topic_ppid_Subscribe[60];
//extern char SMS_SaveNum[4];


extern TIMER_TypeDef g_AtAckTout_timer;

//extern UART_HandleTypeDef huart1;   
extern UART_HandleTypeDef huart2;   //SIM COM

extern uint8_t g_Mqtt_hold;
extern uint8_t g_SimCard_State;

extern MQTT_STATE_DEF g_Mqtt_State;
extern SMS_STATE_DEF g_Sms_State;
extern USER_SET_TypeDef g_UserSet;

extern PAYG_TypeDef payg;

extern uint8_t g_tokenState;

extern uint8_t g_MqttReconnect;
extern uint8_t g_NetConnect_State;


extern SYS_STATE_CODE_TypeDef  g_sysStateCode;


const AT_DEAL_TypeDef  g_atcmd_deal[AT_CMD_MAX_COUNT]=
{
	{AT_CMD_NONE,NULL,NULL,NULL,NULL},
	{AT_CMD_WKUP,"START SIM800C \r\n","OK",AtCmdSend,NULL},
	{AT_CMD_AT,"AT\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_ATE0,"ATE0\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CIPCLOSE,"AT+CIPCLOSE\r\n","ERROR",AtCmdSend,NULL},
	{AT_CMD_CIPSHUT,"AT+CIPSHUT\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CPIN,"AT+CPIN?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CSQ,"AT+CSQ\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CREG,"AT+CREG?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CGATT,"AT+CGATT?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CIICR,"AT+CIICR\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMGF,"AT+CMGF=1\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMGF_0,"AT+CMGF=0\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMGDA,"AT+CMGDA=6\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMGR,"AT+CMGR=\r\n","OK",AtCmdCmgrSend,NULL},
	{AT_CMD_CMGS,"AT+CMGS=\r\n",">",AtCmdCmgsSend,NULL},
	{AT_CMD_CMGS_SMS,"AT\r\n","OK",AtCmdCmgsSMSSend,NULL},
	{AT_CMD_CPMS,"AT+CPMS?\r\n","OK",AtCmdSend,AtCmdCpmsAck},
	{AT_CMD_CSTT,"AT+CSTT=\r\n","OK",AtCmdCsttSend,NULL},
	{AT_CMD_CSTT_Q,"AT+CSTT?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CIFSR,"AT+CIFSR\r\n",".",AtCmdSend,NULL},
	{AT_CMD_CIPSTART,"AT+CIPSTART=\r\n","CONNECT OK",AtCmdCipStart,NULL},
	{AT_CMD_CIPSEND,"AT+CIPSEND\r\n",">",AtCmdSend,AtCmdCipSend},
	{AT_CMD_CIPSEND_DATA,"AT\r\n","SEND OK",AtCmdCipSendOk,NULL},
	{AT_CMD_SAPBR,"AT+SAPBR=1,1\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CLBSCFG,"AT+CLBSCFG=0,3\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CLBS,"AT+CLBS=1,1\r\n","OK",AtCmdSend,NULL},
	#ifdef MODULE_4G
	{AT_CMD_COPS_Q,"AT+COPS?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_COPS,"AT+COPS=0,2\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CGREG,"AT+CGREG?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CPSI,"AT+CPSI?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CGDCOUNT,"AT+CGDCOUNT\r\n","OK",AtCmdCgdcontSend,NULL},
	{AT_CMD_CGACT,"AT+CGACT\r\n","OK",AtCmdCgactSend,NULL},
	{AT_CMD_CMQTTSTART,"AT+CMQTTSTART\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMQTTACCQ,"AT+CMQTTACCQ\r\n","OK",AtCmdMqttAccqSend,NULL},
	{AT_CMD_CMQTTCONNECT,"AT+CMQTTCONNECT\r\n","OK",AtCmdMqttConnectSend,NULL},
	{AT_CMD_CMQTTTOPIC,"AT+CMQTTTOPIC\r\n",">",AtCmdMqttTopicSend,AtCmdMqttTopicAck},
	{AT_CMD_CMQTTPAYLOAD,"AT+CMQTTPAYLOAD\r\n",">",AtCmdMqttPayloadSend,AtCmdMqttPayloadAck},
	{AT_CMD_CMQTTPUB,"AT+CMQTTPUB\r\n","OK",AtCmdMqttPubSend,NULL},
	{AT_CMD_CMQTTSUB,"AT+CMQTTSUB\r\n",">",AtCmdMqttSubSend,AtCmdMqttSubAck},
	{AT_CMD_CMQTTSUBTOPIC,"AT+CMQTTSUBTOPIC\r\n",">",AtCmdMqttSubtopicSend,AtCmdMqttSubtopicAck},
	{AT_CMD_CMQTTUNSUB,"AT+CMQTTUNSUB\r\n",">",AtCmdMqttUnSubSend,AtCmdMqttUnSubAck},
	{AT_CMD_CMQTTUNSUBTOPIC,"AT+CMQTTUNSUBTOPIC\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMQTTDISC,"AT+CMQTTDISC=0,120\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CMQTTSTOP,"AT+CMQTTSTOP\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CGPSINFOR,"AT+CGPSINFO=?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_CGNSSPWR,"AT+CGNSSPWR=1\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_SIMSWITCH,"AT+SWITCHSIM=?\r\n","OK",AtCmdSend,NULL},
	{AT_CMD_DUALSIM,"AT+DUALSIM?\r\n","OK",AtCmdSend,NULL},
	#endif
};

const AT_PARSE_TypeDef  g_attag_parse[]= 
{
	{TAG_NONE,NULL,NULL},
	{TAG_APN,"\"apn\":\"",AtCmdApnParse},
	{TAG_APN_USERNAME,"\"apn_name\":\"",AtCmdApnUserParse},
	{TAG_APN_PASSWORD,"\"apn_psw\":\"",AtCmdApnPswdParse},
	{TAG_TOKEN,"\"Token\":\"*0",AtCmdTokenParse},
	{TAG_MEANS_FRQ,"\"m_freq\":\"",AtCmdMeansFrqParse},
	{TAG_TRANS_FRQ,"\"t_freq\":\"",AtCmdTransFrqParse},
	{TAG_BROKER,"\"broker\":\"",AtCmdBrokerParse},
	{TAG_PORT,"\"port\":\"",AtCmdPortParse},
	{TAG_USERNAME,"\"mqttname\":\"",AtCmdMqttUsrNameParse},
	{TAG_PASSWORD,"\"mqttpsw\":\"",AtCmdMqttPswdParse},
	{TAG_PHONE_NO,"+CMGR:",AtCmdPhoneNoParse}
	#ifdef PUMP_PROJECT
	,{TAG_SWTICH,"\"switch\":\"",AtCmdSwitchParse}
	#endif
};




#if 0
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */

	/* NOTE: This function should not be modified, when the callback is needed,
	the HAL_UART_RxCpltCallback could be implemented in the user file
	*/

	if(huart->Instance==USART1 )
	{
		   CampInit();
	}

	if(huart->Instance==USART2 )
	{
		g_RxGsmParsePos=0;
		memset(g_GsmRbuffer,0x00,GSM_BUFFER);
		HAL_UART_Receive_IT(&huart2,(uint8_t*)g_GsmRbuffer,GSM_BUFFER);
	}

	if(huart->Instance==USART3 )
	{
		// g_RxGsmParsePos=0;
		// memset(g_RxM5311Buffer,0x00,UART_M5311_RXSIZE);
		// HAL_UART_Receive_IT(&huart1,(uint8_t*)g_RxM5311Buffer,UART_M5311_RXSIZE);
		//   memset(g_Uart3Buf,0x00,UART3_RX_BUF_SIZE);
		//   HAL_UART_Receive_IT(&huart1,g_Uart3Buf,UART3_RX_BUF_SIZE);
	}	 
}
#endif


uint32_t AtouI(uint8_t *ascii)
{
	uint8_t i=0,len=strlen((char*)ascii);
	uint32_t value=0;

	for(i=0;i<len;i++)
	{
		if(ascii[i]<'0'||ascii[i]>'9')
			break;
		value*=10;
		value+=ascii[i]-'0';
		}
	return value;
}

void AtCmdInit(void)
{
 	g_AtCmdState=AT_CMD_NONE;
 	g_AtAckState=AT_ACK_NONE;

	memset(g_GsmRbuffer,0x00,GSM_BUFFER);
        g_RxGsmParsePos=0;
        g_RxGsmParseSize=0;	


	memset( g_SmsReply,0x00,SMS_REPLY_SIZE);	
	memcpy(g_SmsReply,"reply:{",7);

	memset( g_PhoneNo,0x00,PHONE_NO_SIZE);	

	
    huart2.pRxBuffPtr=g_GsmRbuffer;
	huart2.RxXferCount=0;
	huart2.RxXferSize=GSM_BUFFER;
	//HAL_UART_Abort(&huart2);
		
	//HAL_UART_Receive_IT(&huart2,g_GsmRbuffer,GSM_BUFFER);

	//memcpy(Set_Apn, Defualt_APN, strlen((char*)Defualt_APN));
}

uint16_t  AtCmdGetValueLen(char *str,uint8_t end_tag)
{
	uint16_t ret=0,i=0;//,len=0;

	for(i=0;str[i]!=end_tag&&str[i];i++)
	{	
		ret++;
		}
	return ret;
}

void AtCmdSend(uint8_t * buffer)
{
    uint16_t size=(uint16_t)strlen((char*)buffer);

  //   HAL_UART_Transmit(&huart2,buffer,size,size*20);
    Uart2Send(buffer,size);
	 #ifdef DEBUG_AT_LOG
	  LogPrintf("MCU:",4);
	 //HAL_UART_Transmit(&huart3,buffer,size,1000);
	  LogPrintf(buffer,size);
     #endif
}

void AtCmdLenSend(uint8_t * buffer,uint16_t size)
{
   //  HAL_UART_Transmit(&huart2,buffer,size,size*20);
   Uart2Send(buffer,size);
	 #ifdef DEBUG_AT_LOG
	  LogPrintf("MCU:",4);
	// HAL_UART_Transmit(&huart3,buffer,size,1000);
	  LogPrintf(buffer,size);
     #endif
}

void AtCmdCsttSend(uint8_t * buffer)
{
	// "AT+CSTT=\"internet.gma.iot\",\"\",\"\"\r\n";
	uint8_t temp_apn[128];

	if(strlen((char*)g_UserSet.NetInfor.apn)>0)
	{	

		if(strlen(g_UserSet.NetInfor.apn_usename)>0&&strlen(g_UserSet.NetInfor.apn_password))
		{	sprintf((char*)temp_apn,"AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,\
			g_UserSet.NetInfor.apn_password);
			}
		else
		{	sprintf((char*)temp_apn,"AT+CSTT=\"%s\",\"\",\"\"\r\n",g_UserSet.NetInfor.apn);
			}
		AtCmdSend(temp_apn);
		}
	else
	{	AtCmdSend((uint8_t*)Defualt_APN);
		}
}

void AtCmdCmgrSend(uint8_t * buffer)
{
	//uint16_t size=(uint16_t)strlen((char*)(const char*)buffer);
	//AtCmdSend(/*Defualt_APN*/Set_Apn);
	uint8_t temp[64]={0};

	sprintf((char*)temp,"AT+CMGR=%s\r\n",g_SmsIndex);

	AtCmdSend(temp);
}
#ifdef MODULE_4G

void AtCmdCgdcontSend(uint8_t *buffer)
{
	uint8_t temp[64]={0};

	memset(temp,0x00,64);
	
	sprintf((char*)temp,"AT+CGDCONT=1,\"IP\",\"%s\"\r\n",g_UserSet.NetInfor.apn);

	AtCmdSend(temp);
}

uint8_t g_mqtt_broker_index=0;

void AtCmdCgactSend(uint8_t *buffer)
{
	uint8_t temp[64]={0};

	memset(temp,0x00,64);
	
	sprintf((char*)temp,"AT+CGACT=1,%d\r\n",1);

	AtCmdSend(temp);
}

void AtCmdMqttAccqSend(uint8_t *buffer)
{
	uint8_t temp[64]={0};

	memset(temp,0x00,64);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTACCQ=%d,\"%s\"\r\n",1,"testmqtt2");
	else
		sprintf((char*)temp,"AT+CMQTTACCQ=%d,\"%s\"\r\n",0,"testmqtt1");

	AtCmdSend(temp);

    //memset(temp,0x00,64);
	
	//sprintf((char*)temp,"AT+CMQTTACCQ=%d,\"%s\"\r\n",1,"testmqtt2");

	//AtCmdSend(temp);
}

void AtCmdMqttConnectSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};

	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
	{
		if(strlen(g_UserSet.NetInfor.mqtt_usename)==0)
			sprintf((char*)temp,"AT+CMQTTCONNECT=%d,\"tcp://%s:%s\",7200,1,,\r\n",1,g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port);
		else	
			sprintf((char*)temp,"AT+CMQTTCONNECT=%d,\"tcp://%s:%s\",7200,1,\"%s\",\"%s\"\r\n",1,g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);
		}
	else
	{
		if(strlen(g_UserSet.NetInforFactory.mqtt_usename)==0)
			sprintf((char*)temp,"AT+CMQTTCONNECT=%d,\"tcp://%s:%s\",7200,1,,\r\n",0,g_UserSet.NetInforFactory.mqtt_broker,g_UserSet.NetInforFactory.mqtt_port);
		else	
			sprintf((char*)temp,"AT+CMQTTCONNECT=%d,\"tcp://%s:%s\",7200,1,\"%s\",\"%s\"\r\n",0,g_UserSet.NetInforFactory.mqtt_broker,g_UserSet.NetInforFactory.mqtt_port,g_UserSet.NetInforFactory.mqtt_usename,g_UserSet.NetInforFactory.mqtt_password);
		}

	AtCmdSend(temp);

	//HAL_Delay(500);
	//memset(temp,0x00,128);

	//sprintf((char*)temp,"AT+CMQTTCONNECT=%d,\"tcp://%s:%d\",7200,1,\"%s\",\"%s\"\r\n",1,"mqtt-factory.omnivoltaic.com",1883,"Admin","7xzUV@MT");

	//AtCmdSend(temp);
}

void AtCmdMqttTopicSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};

	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTTOPIC=%d,%d\r\n",1,strlen(MQTT_topic_ppid));
	else
		sprintf((char*)temp,"AT+CMQTTTOPIC=%d,%d\r\n",0,strlen(MQTT_topic_ppid));

	AtCmdSend(temp);
}

void AtCmdMqttTopicAck(uint8_t *buffer)
{
	AtCmdSend((uint8_t*)MQTT_topic_ppid);
	AtCmdSend("\r\n");
}

void AtCmdMqttSubSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};

	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTSUB=%d,%d,1\r\n",1,strlen(MQTT_topic_ppid_Subscribe));
	else
		sprintf((char*)temp,"AT+CMQTTSUB=%d,%d,1\r\n",0,strlen(MQTT_topic_ppid_Subscribe));

	AtCmdSend(temp);
}

void AtCmdMqttSubAck(uint8_t *buffer)
{

	AtCmdSend((uint8_t*)MQTT_topic_ppid_Subscribe);
	AtCmdSend("\r\n");
}

void AtCmdMqttSubtopicSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};

	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTSUBTOPIC=%d,%d,1\r\n",1,strlen(MQTT_topic_ppid_Subscribe));
	else
		sprintf((char*)temp,"AT+CMQTTSUBTOPIC=%d,%d,1\r\n",0,strlen(MQTT_topic_ppid_Subscribe));

	AtCmdSend(temp);
}

void AtCmdMqttSubtopicAck(uint8_t *buffer)
{

	AtCmdSend((uint8_t*)MQTT_topic_ppid_Subscribe);
	AtCmdSend("\r\n");
}


void AtCmdMqttPubSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};
	uint8_t *json;
	
	memset(temp,0x00,128);

	json=GattGetJsonBuff();

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTPUB=1,1,%d\r\n",80);
	else
		sprintf((char*)temp,"AT+CMQTTPUB=0,1,%d\r\n",80);

	AtCmdSend(temp);
}


void AtCmdMqttUnSubSend(uint8_t *buffer)
{
	uint8_t temp[128]={0};

	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTUNSUB=%d,%d,0\r\n",1,strlen(MQTT_topic_ppid_Subscribe));
	else
		sprintf((char*)temp,"AT+CMQTTUNSUB=%d,%d,0\r\n",0,strlen(MQTT_topic_ppid_Subscribe));

	AtCmdSend(temp);
}

void AtCmdMqttUnSubAck(uint8_t *buffer)
{

	AtCmdSend((uint8_t*)MQTT_topic_ppid_Subscribe);
	AtCmdSend("\r\n");
}


void AtCmdMqttPayloadSend(uint8_t *buffer)
{
	uint8_t *json;
	uint8_t temp[128]={0};

	json=GattGetJsonBuff();
	memset(temp,0x00,128);

	if(g_mqtt_broker_index>=1)
		sprintf((char*)temp,"AT+CMQTTPAYLOAD=%d,%d\r\n",1,strlen((char*)json));
	else
		sprintf((char*)temp,"AT+CMQTTPAYLOAD=%d,%d\r\n",0,strlen((char*)json));

	AtCmdSend(temp);

	HAL_Delay(1000);
}

void AtCmdMqttPayloadAck(uint8_t *buffer)
{
	uint8_t *json;

	json=GattGetJsonBuff();

	AtCmdSend(json);

	AtCmdSend("\r\n");
}

#endif



void AtCmdCpmsAck(uint8_t * buffer)
{
	
	int temp = 0;
	//Creat SMS Readnum

	HAL_Delay(10);
	
	/*SMS_SaveNum[0] = g_GsmRbuffer[temp + 7];
	if(g_GsmRbuffer[temp + 8] != ',')
		SMS_SaveNum[1] = g_GsmRbuffer[temp + 8];
	//Crear SMS get Cmd
	memcpy(SMS_GET, SMS_GET_CMD_Head, 8);
	memcpy(SMS_GET + 8, SMS_SaveNum, strlen((char*)SMS_SaveNum));
	memcpy(SMS_GET + 8 + strlen((char*)SMS_SaveNum), "\r\n", 2);*/
}

void AtCmdCmgsSend(uint8_t * buffer)
{
	//uint16_t size=(uint16_t)strlen((char*)(const char*)buffer);
	//AtCmdSend(/*Defualt_APN*/Set_Apn);
	
	//AtCmdSend("AT+CMGS=\"18924610909\"\r\n");
	AtCmdSend("AT+CMGS=\"");
	AtCmdSend(g_PhoneNo);
	AtCmdSend("\"\r\n");

	memset( g_PhoneNo,0x00,PHONE_NO_SIZE);	
	//AtCmdSend("AT+CMGS=\"18938917953\"\r\n");
}
void AtCmdCmgsSMSSend(uint8_t * buffer)
{
	if(strlen((char*)g_SmsReply)<=7)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1),"error}\r\n");
	else	
	{
		if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-3)
			sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1),"}\r\n");
		}
	AtCmdSend(g_SmsReply);	
	//AtCmdSend("Config_OK\r\n");
	HAL_Delay(2);
	AtCmdLenSend(g_endmark,1);

	memset(g_SmsReply,0x00,SMS_REPLY_SIZE);
	memcpy(g_SmsReply,"reply:{",7);
}
void AtCmdCipStart(uint8_t * buffer)
{
	//"AT+CIPSTART=\"TCP\",\"mqtt-2.omnivoltaic.com\",\"1883\"\r\n";
	uint8_t temp_uarl[128];

	if(strlen(g_UserSet.NetInfor.mqtt_broker)>8)
	{	

		if(strlen(g_UserSet.NetInfor.mqtt_usename)>0&&strlen(g_UserSet.NetInfor.mqtt_password))
		{	sprintf((char*)temp_uarl,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\",\"%s\",\"%s\"\r\n",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,\
			g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);
			}
		else
		{	sprintf((char*)temp_uarl,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r\n",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port);
			}
		AtCmdSend(temp_uarl);
		}
	else
	{	AtCmdSend((uint8_t*)Defualt_URL);
		}
}

void AtCmdCipSendOk(uint8_t * buffer)
{
	g_AtAckState=AT_ACK_OK;
	g_AtCmdState=AT_CMD_NONE;
}
void AtCmdCipSend(uint8_t * buffer)
{
	uint16_t size=(uint16_t)strlen((char*)buffer);

	char packLengthInterval=0x00;
	char message_end = 0x1A;
	char length_message_flag=0x01;
	char topicLength ;//= strlen((char*)topic);
	uint8_t * json;

	uint16_t messageLength ;//= strlen((char*)mqtt_message);
	uint16_t packetLength;// = messageLength+topicLength+2;//31


	//AtCmdLenSend(message_start,47);



	switch(MqttGetState()-1)
	{
		case MQTT_STATE_CONNECT:
			AtCmdLenSend((uint8_t*)message_start,46);
			AtCmdLenSend((uint8_t*)&message_end,1);
			break;
		case MQTT_STATE_SUBSCRIBE:
			AtSetSubscribeId();

			packLengthInterval=0x82;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1); //FIEXED HEADER

			topicLength=strlen(MQTT_topic_ppid_Subscribe);
			packetLength= topicLength+4+1;//31
			// packLengthInterval=0x00;
			// AtCmdLenSend((uint8_t*)&packLengthInterval,1); ///LENGHT HIGHT

			AtCmdLenSend((uint8_t*)&packetLength,1);  //LENGHT LOW

			packLengthInterval=0x00;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1); //PACKET ID msb
			packLengthInterval=0x01;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1); //PACKET ID lsb

			packLengthInterval=0x00;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1); //top lenght high

			AtCmdLenSend((uint8_t*)&topicLength,1); //top lenght low

			AtCmdLenSend((uint8_t*)MQTT_topic_ppid_Subscribe,topicLength);

			packLengthInterval=0x00;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1);// QOS

			HAL_Delay(10);

			AtCmdLenSend((uint8_t*)&message_end,1);//	AtCmdLenSend(&message_end,1);

			break;
		case MQTT_STATE_PUBLISH:

				switch(MqttGetRequest())
				{
					case MQTT_REQ_NONE:
						GattAllFieldJsonMerge();	
						break;
					case MQTT_REQ_ALLFIELD:
						GattAllFieldJsonMerge();
						break;
					case MQTT_REQ_DTTYPE:
						break;
					case MQTT_REQ_SINGLEFIELD:
						break;
					case MQTT_REQ_CMD:
						break;	
					case MQTT_REQ_RAML:
						GattMultiFieldMerge(); 
						break;
					case MQTT_REQ_ABAC:
						GattAbacFieldMerge(); 
						break;
					}
			
			
			//memcpy(MQTT_topic_ppid_Subscribe, MQTT_topic_ppid, 48);
			//memcpy(MQTT_topic_ppid_Subscribe + strlen((char*)MQTT_topic_ppid_Subscribe),"/#", 2);

			json=GattGetJsonBuff();//GattAllFieldJsonMerge();	 

			packLengthInterval=0x30;
		        AtCmdLenSend((uint8_t*)&packLengthInterval,1); //FIEXED HEADER

			topicLength=strlen(MQTT_topic_ppid);
			messageLength= strlen((char*)json);

			packetLength= messageLength+topicLength+2;//31

			if(packetLength>127)
			{
				length_message_flag = (packetLength/128);
				packetLength = packetLength-(length_message_flag-1)*128;
				AtCmdLenSend((uint8_t*)&packetLength,1);
				AtCmdLenSend((uint8_t*)&length_message_flag,1);
			}else
			{
				AtCmdLenSend((uint8_t*)&packetLength,1);
			}
			packLengthInterval=0;
			AtCmdLenSend((uint8_t*)&packLengthInterval,1);
			AtCmdLenSend((uint8_t*)&topicLength,1);
			AtCmdLenSend((uint8_t*)MQTT_topic_ppid,topicLength);
			HAL_Delay(10);
			AtCmdLenSend(json,messageLength);

			 AtCmdLenSend((uint8_t*)&message_end,1);
			break;
		}

	
	

      // AtSetTopicId();
	//Creat username and PSWD
	// memcpy(MQTTusename, "admin", 5);
	// memcpy(MQTTpassword, "admin123", 8); 
	//Creat Subscribe topic



/*	AtCmdLenSend(message_start,47);

	//AtCmdLenSend(&message_end,1);
//HAL_Delay(2000);

	//AtCmdSend("AT+CIPSEND\r\n");
		//HAL_Delay(3000);
	HAL_Delay(1);


HAL_Delay(2000);
	 AtCmdSend("AT+CIPSEND\r\n");
		HAL_Delay(3000);
        //HAL_Delay(2000);*/



	   
	/*memset(Deagnostic_Message_Json, 0x00, 512);
	   sprintf((char*)Deagnostic_Message_Json,"{\"msgid\":\"%d\",\"clientid\":\"%s\",\"ack\":\"%s\"}",msgid++,MQTT_ClienID,"test4");

		messageLength= strlen((char*)Deagnostic_Message_Json);
		topicLength=strlen((char*)MQTT_topic_ppid);
			   
	       packetLength= messageLength+topicLength+2;//31

		   AtCmdLenSend(message_start,47);
	HAL_Delay(1);

		//packLengthInterval=0x30;
		//AtCmdLenSend((uint8_t*)&packLengthInterval,1);//header
		
		AtCmdLenSend((uint8_t*)&packetLength,1);  //lenght

		packLengthInterval=0x00;
		
		AtCmdLenSend((uint8_t*)&packLengthInterval,1);  //len msb
		AtCmdLenSend((uint8_t*)&topicLength,1);    //len lsb
		
		AtCmdLenSend(MQTT_topic_ppid,topicLength);
		HAL_Delay(10);
		AtCmdLenSend(Deagnostic_Message_Json,messageLength);
		HAL_Delay(100);

		 AtCmdLenSend(&message_end,1);*/

}



void AtSetTopicId(uint8_t * ext_topic)
{

	//Creat Client ID
	memset(MQTT_ClienID,0x00,20);
	#ifdef PUMP_PROJECT
	sprintf((char*)MQTT_ClienID,"ovPump%s",payg.oem_id);
	#elif defined(CAMP_PROJECT)
	//memcpy(MQTT_ClienID, GattGetPpid(), 14);
	GattGetPpid((uint8_t*)MQTT_ClienID);
	#elif defined(BLE_MASTER_ENABLE)
	GattGetData( LIST_ATT, ATT_OPID, MQTT_ClienID);
	#else
	memcpy(MQTT_ClienID,payg.oem_id/*json+ 32"99AH1712099999"*/, strlen((char*)payg.oem_id));
	#endif
	//memcpy(MQTT_ClienID, /*json+ 32*/"99AH1712099999", 14);
	//Creat Topic
	memset(MQTT_topic_ppid, 0x00,TOPIC_LEN);
	//memcpy(MQTT_topic_ppid, "_OVES/GPRSV1/1/", 15);
	memcpy(MQTT_topic_ppid, g_MqttPubFixedPath, strlen((char*)g_MqttPubFixedPath));
	memcpy(MQTT_topic_ppid + strlen((char*)MQTT_topic_ppid), MQTT_ClienID, strlen((char*)MQTT_ClienID));


	if(ext_topic!=NULL)
	{
		sprintf(MQTT_topic_ppid + strlen(MQTT_topic_ppid),"%s",ext_topic);
		}
		
}

void AtSetSubscribeId(void)
{

	#ifdef PUMP_PROJECT
	//sprintf((char*)MQTT_ClienID,"ovPump%s",g_UserSet.Payg.oem_id);
	sprintf((char*)MQTT_ClienID,"ovPump%s",payg.oem_id);
	#elif defined(CAMP_PROJECT)
	//memcpy(MQTT_ClienID, GattGetPpid(), 14);
	GattGetPpid(MQTT_ClienID);
	#elif defined(BLE_MASTER_ENABLE)
	GattGetData( LIST_ATT, ATT_OPID, MQTT_ClienID);
	#else
	//memcpy(MQTT_ClienID, /*json+ 32*/"99AH1712099999", 14);
	memcpy(MQTT_ClienID, payg.oem_id/*json+ 32"99AH1712099999"*/, strlen((char*)payg.oem_id));
	#endif
	//memcpy(MQTT_ClienID, /*json+ 32*/"99AH1712099999", 14);
	
	memset(MQTT_topic_ppid_Subscribe, 0x00,60);
	memcpy(MQTT_topic_ppid_Subscribe, g_MqttSubFixedPath, strlen((char*)g_MqttSubFixedPath));
	//memcpy(MQTT_topic_ppid_Subscribe, "_OVES/GPRSV1/1/", 15);
	memcpy(MQTT_topic_ppid_Subscribe + strlen(MQTT_topic_ppid_Subscribe),MQTT_ClienID, strlen(MQTT_ClienID));
	memcpy(MQTT_topic_ppid_Subscribe + strlen(MQTT_topic_ppid_Subscribe),"/#", 2);
}

uint8_t*AtStrStr(uint8_t *src,uint8_t *dst)
{
	uint16_t src_len=g_RxGsmParseSize;
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

void AtCmdMerge(uint8_t cmd)
{
	AT_DEAL_TypeDef *p=(AT_DEAL_TypeDef*)&g_atcmd_deal[cmd];


	
	g_AtCmdState=(AT_CMD_DEF)cmd;
	g_AtAckState=AT_ACK_NONE;

	if(g_AtCmdState==AT_CMD_NONE)
		return ;

	memset(g_GsmRbuffer,0x00,GSM_BUFFER);
   	huart2.RxXferCount=0;
	huart2.RxXferSize=GSM_BUFFER;
        huart2.pRxBuffPtr=g_GsmRbuffer; 
		
        g_RxGsmParsePos=0;
        g_RxGsmParseSize=0;	

	
	if(p->send!=NULL&&p->cmd!=NULL&&cmd<AT_CMD_MAX_COUNT)
		p->send(p->cmdstr);
}

void AtCmdApnParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"'); 

	if(value_len>=APN_LEN)
		value_len=APN_LEN;
	memset(g_UserSet.NetInfor.apn,0x00,APN_LEN);
	memcpy(g_UserSet.NetInfor.apn,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.apn)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.apn);

	EEpUpdateEnable();
}

void AtCmdApnUserParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');
	if(value_len>=APN_USENAME_LEN)
		value_len=APN_USENAME_LEN;
	memset(g_UserSet.NetInfor.apn_usename,0x00,APN_USENAME_LEN);
	memcpy(g_UserSet.NetInfor.apn_usename,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.apn_usename)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.apn_usename);
	
	EEpUpdateEnable();
}

void AtCmdApnPswdParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=APN_PASSWORD_LEN)
		value_len=APN_PASSWORD_LEN;
	memset(g_UserSet.NetInfor.apn_password,0x00,APN_PASSWORD_LEN);
	memcpy(g_UserSet.NetInfor.apn_password,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.apn_password)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.apn_password);

	EEpUpdateEnable();
}


void AtCmdTokenParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;//,token_len=0;
	uint8_t /*token[32],*/*p,*p_target;
	uint32_t token_hi,token_lo;
	uint8_t temp[12];
	uint8_t token[32];
	uint8_t tokenstate=0,breakfor=0;
	//uint8_t mqtt_cmd=FALSE;
	uint8_t mqtt_cmd=FALSE,token_len=0;
	uint16_t i,j;
	uint16_t tkre=0;
	#ifdef BLE_MASTER_ENABLE
	extern uint8_t g_bleCmd_State;
	extern uint16_t RxUart3Counter;
	extern uint8_t g_Uart3Buf[UART3_RX_BUF_SIZE];
	extern uint8_t g_bleAck_State;
	extern uint8_t g_bletkre;
	#endif

	if(strstr((char*)str,"/cmd/code")!=NULL)
		mqtt_cmd=TRUE;

	
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'#');
	memset(token,0x00,32);

	//memcpy(token,str+tag_len,value_len);

	p=str+tag_len;

	#ifdef BLE_MASTER_ENABLE
	g_bleCmd_State=BLE_CMD_PUBK;
	p-=2;
					
	BlePacktSend(g_bleCmd_State,p,value_len+3);
	//RxUart3Counter=0;
	//memset(g_Uart3Buf,0x00,UART3_RX_BUF_SIZE);


	//g_bleAck_State=BLE_COM_IDLE;
	g_bletkre=0;
	
	for(j=0;j<2000;j++)
	{
		HAL_Delay(5);
		PKeybordProc();

		#ifdef WDG_ENABLE
	    HAL_IWDG_Refresh(&hiwdg);
		#endif
		if(g_bletkre)
			break;
		}
         
    #else
	
	p_target=token;

	for(i=0;i<value_len;i++)
	{
		if(p[i]!=' ')
		{	*p_target=p[i];
			p_target++;
			token_len++;
			}
		}

	if((strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)token)-2)&&mqtt_cmd==FALSE)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,token);

	if(token_len<20&&mqtt_cmd==FALSE)
	{
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1),"is not correct format. ");	
		return ;
		}

	if(token_len<20)
		g_sysStateCode.error.pwd_len_error=1;
	
		
    token_lo = AtouI(&token[10]);
	token[10]='\0';
	token_hi = AtouI(token);

	
	memset(token,0x00,32);

	token[13] = 0xc5;
        token[14] = 0x6a;
        token[15] = 0x29;
        token[16] = 0x0e;
        token[17] = 0xfa;

        token[21]= token_lo>>24;
        token[20]= token_lo>>16;
        token[19]= token_lo>>8;
        token[18]= token_lo;
        
        token[25]= token_hi>>24;
        token[24]= token_hi>>16;
        token[23]= token_hi>>8;
        token[22]= token_hi;
		
	#ifdef CAMP_PROJECT	
	token[26]= CRC8(token + 13 ,13);	

	CampSend(token,28);

	for(i=0;i<500;i++)
	{	
		HAL_Delay(10);
		tokenstate=CampTokenState();
		if(tokenstate!=0xff)
			break;
		}

	
	if(mqtt_cmd==FALSE)
	{
		switch(tokenstate)
		{
			case 0:
				sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1),"tops up succeeded ");	
				g_sysStateCode.error.pswd_error=0;
				g_sysStateCode.error.pwd_len_error=0;
				break;
			case 1:
				sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1),"is latest and expired ");
				g_sysStateCode.error.pswd_error=1;
				break;
			case 2:
				sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)-1)," tops up Failed ");
				g_sysStateCode.error.pswd_error=1;
				break;
			}
		}
		
	
	return ;
	#endif

	p=&token[13] ;

        temp[3] = p[12];
        temp[2] = p[11];
        temp[1] = p[10];
        temp[0] = p[9];
        temp[7] = p[8];
        temp[6] = p[7];
        temp[5] = p[6];
        temp[4] = p[5];

	memcpy((uint8_t*)payg.hast_input, temp, 8);

	//payg.hast_input[0]=token_lo;
	//payg.hast_input[1]=token_hi;

	PaygOvesInput();
	

	#ifdef E_MOB48V_PROJECT

	if( g_tokenState==TOKEN_OK)
	{
		#ifdef BMS_SMARTLI_SUPPROT
		SbmsWakeupRelease();
		#endif
		#ifdef BMS_JBD_SUPPROT
		//JbsMosCtrl(TRUE);
		JbsBmsInit();
		#endif
		}
	#endif

	#endif

	#ifdef DC_PUMP_SUPPORT
	if(/*PaygGetPayState()&&*/g_tokenState==TOKEN_OK)
		PumpOn(TRUE);
    #endif

	#ifdef BLE_MASTER_ENABLE
	 GattGetData( LIST_STS, STS_TKRE, (uint8_t*)&tkre);
	 g_tokenState=(uint8_t)tkre;
	#endif


	 if( g_tokenState==TOKEN_USEED)
	 {
	 	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-4-2)
		       memcpy(g_SmsReply+strlen((char*)g_SmsReply)-1," use\"",5);

		
	 	}
	else if( g_tokenState==TOKEN_ERROR)
	 {
	 	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-4-2)
		       memcpy(g_SmsReply+strlen((char*)g_SmsReply)-1," error\"",7);
	 	}

	#ifndef BLE_MASTER_ENABLE

	tkre=g_tokenState;

	GattSetData( LIST_STS, STS_TKRE, (uint8_t*)&tkre);
	
	
	EEpUpdateEnable();
	#endif

}

void AtCmdMeansFrqParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=MEARSURE_FRE_LEN)
		value_len=MEARSURE_FRE_LEN;
	memset(g_UserSet.CampFreq.M_frequen,0x00,MEARSURE_FRE_LEN);
	memcpy(g_UserSet.CampFreq.M_frequen,str+tag_len,value_len);


	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.CampFreq.M_frequen)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.CampFreq.M_frequen);

	EEpUpdateEnable();

}

void AtCmdTransFrqParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=TRANSMISSION_FRE_LEN)
		value_len=TRANSMISSION_FRE_LEN;
	memset(g_UserSet.CampFreq.T_frequen,0x00,TRANSMISSION_FRE_LEN);
	memcpy(g_UserSet.CampFreq.T_frequen,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.CampFreq.T_frequen)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.CampFreq.T_frequen);

	EEpUpdateEnable();

}

void AtCmdBrokerParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=MQTT_BROKER_LEN)
		value_len=MQTT_BROKER_LEN;
	
	memset(g_UserSet.NetInfor.mqtt_broker,0x00,MQTT_BROKER_LEN);
	memcpy(g_UserSet.NetInfor.mqtt_broker,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.mqtt_broker)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.mqtt_broker);

	EEpUpdateEnable();

}

void AtCmdPortParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=MQTT_PORT_LEN)
		value_len=MQTT_PORT_LEN;
	memset(g_UserSet.NetInfor.mqtt_port,0x00,MQTT_PORT_LEN);
	memcpy(g_UserSet.NetInfor.mqtt_port,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.mqtt_port)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.mqtt_port);

	EEpUpdateEnable();

}

void AtCmdMqttUsrNameParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=MQTT_USENAME_LEN)
		value_len=MQTT_USENAME_LEN;
	
	memset(g_UserSet.NetInfor.mqtt_usename,0x00,MQTT_USENAME_LEN);
	memcpy(g_UserSet.NetInfor.mqtt_usename,str+tag_len,value_len);

	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.mqtt_usename)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.mqtt_usename);

	EEpUpdateEnable();
}

void AtCmdPhoneNoParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len,*p;
	tag_len=strlen((char*)tag);
	

	p=AtStrStr(str, "\"+");

	if(p!=NULL)
	{
		p++;
		value_len=AtCmdGetValueLen((char*)p,'\"');	

		memcpy(g_PhoneNo,p,value_len);
		
		}

}


void AtCmdMqttPswdParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=MQTT_PASSWORD_LEN)
		value_len=MQTT_PASSWORD_LEN;
	memset(g_UserSet.NetInfor.mqtt_password,0x00,MQTT_PASSWORD_LEN);
	memcpy(g_UserSet.NetInfor.mqtt_password,str+tag_len,value_len);
	if(strlen((char*)g_SmsReply)<SMS_REPLY_SIZE-strlen((char*)tag)-strlen((char*)g_UserSet.NetInfor.mqtt_password)-2)
		sprintf((char*)(g_SmsReply+strlen((char*)g_SmsReply)),"%s%s\",",tag,g_UserSet.NetInfor.mqtt_password);

	EEpUpdateEnable();
}

void AtCmdSwitchParse(uint8_t*str,uint8_t*tag)
{
	uint8_t tag_len,value_len;
	uint8_t temp[8];
	tag_len=strlen((char*)tag);
	value_len=AtCmdGetValueLen((char*)(str+tag_len),'\"');

	if(value_len>=8)
		value_len=8;
	memset(temp,0x00,8);

	memcpy(temp,str+tag_len,value_len);

	if(strstr((char*)temp,"on")!=NULL)
	{
		#ifdef DC_PUMP_SUPPORT
		PumpTxCmd(PUMP_SET_ON);
		#else
		PumpTxCmd(PUMP_RESET);
		HAL_Delay(50);
		PumpTxCmd(PUMP_ON);
		#endif
		}

	if(strstr((char*)temp,"off")!=NULL)
	{	
		#ifdef DC_PUMP_SUPPORT
		PumpTxCmd(PUMP_SET_OFF);
		#else
		PumpTxCmd(PUMP_OFF);
		#endif
		}
}

void AtCmdPaser(uint8_t *buffer,uint8_t cmd)
{
	uint16_t pos=0,len_value=0,len_spec=0;
	char *p=NULL,*p_buf=NULL;
	int temp;
	uint8_t i,meta=0,value_len=0;
	uint8_t tempBuff[128];
	char*str=(char*)buffer;

	AT_DEAL_TypeDef *pdeal=NULL;
	AT_PARSE_TypeDef *ptag=NULL;

	GATT1_Def *p_gattlist=NULL;
	uint16_t tempU16;
	int16_t tempInt16;

	if(cmd<AT_CMD_MAX_COUNT)
		pdeal=(AT_DEAL_TypeDef*)&g_atcmd_deal[cmd];

	//if(pdeal[cmd].senddeal!=NULL)
	//	pdeal[cmd].ackdeal(pdeal[cmd].ackstr);

	#ifdef MODULE_4G

	if(strstr(str,"ERROR")!=NULL)
	{
		if(g_Mqtt_State>=MQTT_STATE_MQTTSTART)
		{
			//AtCmdMerge(AT_CMD_AT);	
			//g_Mqtt_State=MQTT_STATE_MQTTCONNECT;
			HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
			HAL_Delay(600);
			MqttInit();
			}
		}
	
	if(strstr(str,"+CMQTTCONNLOST")!=NULL)
	{
		if(g_Mqtt_State>=MQTT_STATE_MQTTSTART)
		{
			AtCmdMerge(AT_CMD_AT);	
			g_Mqtt_State=MQTT_STATE_MQTTCONNECT;
			//HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
			//HAL_Delay(600);
			//MqttInit();
			}
		}

	 if(strstr(str,"*ATREADY: 1")!=NULL)  //+CREG: 0,5
	 {
		AtCmdMerge(AT_CMD_CSQ);
		g_Mqtt_State=MQTT_STATE_CSQ;	
	 	}

	 
	 if(strstr(str,"+CREG:")!=NULL)  //+CREG: 0,5
	 {
	 	p=strstr(str,"+CREG:");

		//if(p[9]=='1'||p[9]=='5')
		//	g_Mqtt_State=MQTT_STATE_CGREG;
			
	 	}

    if(strstr(str,"+CPSI:")!=NULL)
	{

		if(strstr(str,"NO SERVICE")==NULL)
		{
			//	AtCmdSend("AT+SWITCHSIM=?\r\n");
			/*if(strstr(str,"LTE")!=NULL)
			{
				g_GsmExist=FALSE;
				}
			else
			{
				g_GsmExist=TRUE;
				}*/
			
			if(g_GsmExist)
				g_Mqtt_State=MQTT_STATE_CONNECT_CSQ;//MQTT_STATE_CGDCONT;
			else
				g_Mqtt_State=MQTT_STATE_MQTTSTART;
			}

		if(strstr(str,"Online")!=NULL)
		{
			}
		
		
		p=strstr(str,"+CPSI:");
		p+=6;

		for(i=0;i<5;i++)
		{
			len_value=AtCmdGetValueLen(p,',');
			p+=len_value+1;
			
			}
		len_value=AtCmdGetValueLen(p,',');	

		if(len_value<CELL_ID_SIZE)
		{  
			memset(g_CellID,0x00,CELL_ID_SIZE);
			memcpy(g_CellID,p,len_value);
			}
		
		}

		if(strstr(str,"+CPIN:")!=NULL)
		{
			if(strstr(str,"READY")!=NULL)
				GmsSetSimcardState(TRUE);
			else 
				GmsSetSimcardState(FALSE);
			}
	
	#endif


	if(strstr(str,"+CMTI")!=NULL)
	{
		g_Sms_State=SMS_STATE_READ;
		//g_Mqtt_State=MQTT_STATE_INIT;
		g_Mqtt_hold=TRUE;
		}

	if((strstr(str,"+CPIN:")!=NULL||strstr(str,"ERROR")!=NULL)&&g_Sms_State==MQTT_STATE_CSQ)
	{
		if(strstr(str,"READY")!=NULL)
			GmsSetSimcardState(TRUE);
		else 
			GmsSetSimcardState(FALSE);
		}



       for(i=0;i<sizeof(g_attag_parse)/sizeof(AT_PARSE_TypeDef);i++)
       	{
       		ptag=(AT_PARSE_TypeDef*)&g_attag_parse[i];
			
       		if(ptag->tag_str!=NULL&&ptag->parse!=NULL)
       		{
       			p=(char*)AtStrStr((uint8_t*)str,ptag->tag_str);
       			if(p!=NULL)
       				ptag->parse((uint8_t*)p,ptag->tag_str);
       			}
       	}


	if(strstr(str,"\"SM_P\"")!=NULL)
	{
		p=strstr(str,"\"SM_P\"");
		
		memset(g_SmsIndex,0x00,SMS_INDEX_SIZE);

		len_value=AtCmdGetValueLen(p+7,',');
		memcpy(g_SmsIndex,p+7,len_value);
		}

	if(strstr(str,"+CPMS")!=NULL)
	{

		}

	
	if(strstr(str,"+IPD")!=NULL)
	{
		}
	
	if(strstr(str,"CLOSED")!=NULL)
	{
		len_value=AtCmdGetValueLen(p+7,',');

		g_NetConnect_State=FALSE;
		g_MqttReconnect=TRUE;


		}

	p=strstr(str,"+CLBS:");	
	if(p!=NULL)
	{
		uint8_t latbuf[16];
		
		len_value=AtCmdGetValueLen(p,',');
		p+=len_value+1;
		len_value=AtCmdGetValueLen(p,',');

		memset(latbuf,0x00,16);
		if(len_value<=16)
			memcpy(latbuf,p,len_value);
		GattSetGpsCordLon(latbuf);

		p+=len_value+1;
		len_value=AtCmdGetValueLen(p,',');
		memset(latbuf,0x00,16);
		if(len_value<=16)
			memcpy(latbuf,p,len_value);
		
		GattSetGpsCordLat(latbuf);
		
		}

	p=strstr(str,"+CSQ: ");
	
	if(p!=NULL)
	{
		//AtCmdSend(Defualt_URL);	
		uint8_t temp[8];
		memset(temp,0x00,8);
		len_value=AtCmdGetValueLen(p+6,',');

		if(len_value>8)
			len_value=8;

		memcpy(temp,p+6,len_value);

		GattSetGprsRssi(atoi((char*)temp));
		}


	//p=strstr(str,"cmd/ov/1/GPRS/");
	p_buf=(char*)AtStrStr((uint8_t*)str,(uint8_t*)g_MqttComparePath);

	if(p_buf!=NULL)
	{

		if(strstr(p_buf,MQTT_ClienID)!=NULL)
		{
			if(strstr(p_buf,"+CMQTTRXPAYLOAD: 0")!=NULL)
				g_mqtt_broker_index=0;
			else
				g_mqtt_broker_index=1;
			//cmd parse
			p=strstr(p_buf,"/cmd/nbroker/");  //"ip ,port,username,password"
			if(p!=NULL)
			{
				HAL_Delay(20);
				p=NULL;
				p=strstr(p_buf,"\r\n\"");  

				if(p!=NULL)
				{

					if(strstr(p_buf,"+CMQTTRXPAYLOAD: 0")!=NULL)
						g_mqtt_broker_index=0;
					else
						g_mqtt_broker_index=1;
					
					p=p+3;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_BROKER_LEN)
						value_len=MQTT_BROKER_LEN;
					memset(g_UserSet.NetInfor.mqtt_broker,0x00,MQTT_BROKER_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_broker,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_PORT_LEN)
						value_len=MQTT_PORT_LEN;
					memset(g_UserSet.NetInfor.mqtt_port,0x00,MQTT_PORT_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_port,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_USENAME_LEN)
						value_len=MQTT_USENAME_LEN;
					memset(g_UserSet.NetInfor.mqtt_usename,0x00,MQTT_USENAME_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_usename,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,'\"');

					if(value_len>=MQTT_PASSWORD_LEN)
						value_len=MQTT_PASSWORD_LEN;
					memset(g_UserSet.NetInfor.mqtt_password,0x00,MQTT_PASSWORD_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_password,p,value_len);


					AtSetTopicId(/*"/cmd/nbroker"*/NULL);
					memset(tempBuff,0x00,128);

					sprintf((char*)tempBuff,"\"nbroker\":\"%s,%s,%s,%s\"",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);
					GattSetCmdUplinkData(tempBuff);
	//				GattSetUplinkData(tempBuff);
					MqttSetRequest(MQTT_REQ_CMD);
					EEpUpdateEnable();

					
					}
				
				}
			
			p=strstr(p_buf,"/cmd/napn/"); //"apn,username,passowrd"
			if(p!=NULL)
			{
				p=p+10+1;
				value_len=AtCmdGetValueLen(p,',');

				if(value_len>=APN_LEN)
					value_len=APN_LEN;
				memset(g_UserSet.NetInfor.apn,0x00,APN_LEN);
				memcpy(g_UserSet.NetInfor.apn,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen(p,',');

				if(value_len>=APN_USENAME_LEN)
					value_len=APN_USENAME_LEN;
				memset(g_UserSet.NetInfor.apn_usename,0x00,APN_USENAME_LEN);
				memcpy(g_UserSet.NetInfor.apn_usename,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen(p,'\"');

				if(value_len>=APN_PASSWORD_LEN)
					value_len=APN_PASSWORD_LEN;
				memset(g_UserSet.NetInfor.apn_password,0x00,APN_PASSWORD_LEN);
				memcpy(g_UserSet.NetInfor.apn_password,p,value_len);

				AtSetTopicId(/*"/cmd/napn"*/NULL);
				memset(tempBuff,0x00,128);

				//sprintf((char*)tempBuff,"\"%s,%s,%s'\'",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,g_UserSet.NetInfor.apn_password);
				sprintf((char*)tempBuff,"\"napn\":\"%s,%s,%s\"",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,g_UserSet.NetInfor.apn_password);

				GattSetCmdUplinkData(tempBuff);
				//GattSetUplinkData(tempBuff);
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
				}

			p=strstr(p_buf,"/cmd/gstw/");  //sleep
			if(p!=NULL)
			{
				g_UserSet.sleeptime=atoi(p+10);
				AtSetTopicId(/*"/cmd/gstw"*/NULL);

				if(g_UserSet.sleeptime>720)
				{	g_UserSet.sleeptime=720;
					//GattSetUplinkData("720");
					}
				//else
					//GattSetUplinkData(p+10);
				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"gstw\":\"%d\"",g_UserSet.sleeptime);
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
				}

			p=strstr(p_buf,"/cmd/gctw/");  //on line time
			if(p!=NULL)
			{
  				AtSetTopicId(/*"/cmd/gctw"*/NULL);
				g_UserSet.onlinetime=atoi(p+10);
				//GattSetUplinkData(p+10);
				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"gctw\":\"%d\"",g_UserSet.onlinetime);
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();

				
				}

			p=strstr(p_buf,"/cmd/code/");  //token
			if(p!=NULL)
			{
				AtSetTopicId("/cmd/code");
				AtCmdTokenParse((uint8_t*)p,"/cmd/code/\"*0");
				
				memset(tempBuff,0x00,128);

				if( g_tokenState==TOKEN_OK)
				{	//GattSetUplinkData(p+10);
					sprintf((char*)tempBuff,"\"code\":%s",p+10);

					
					}
				else if( g_tokenState==TOKEN_USEED)
					//GattSetUplinkData("password use");
					sprintf((char*)tempBuff,"\"code\":\"%s\"","password use");
				else
					//GattSetUplinkData("password error");
					sprintf((char*)tempBuff,"\"code\":\"%s\"","password error");

				GattSetCmdUplinkData(tempBuff);
					
				MqttSetRequest(MQTT_REQ_CMD);
				}

			p=strstr(p_buf,"/cmd/mfrq/");  //token
			if(p!=NULL)
			{
				AtSetTopicId("/cmd/mfrq");
				p+=10;
				value_len=strlen((char*)p);
				if(value_len>=MEARSURE_FRE_LEN)
					value_len=MEARSURE_FRE_LEN;
				memset(g_UserSet.CampFreq.M_frequen,0x00,MEARSURE_FRE_LEN);
				memcpy(g_UserSet.CampFreq.M_frequen,p,value_len);

				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"mfrq\":\"%s\"",g_UserSet.CampFreq.M_frequen);
				GattSetCmdUplinkData(tempBuff);

				//GattSetUplinkData(p);
				MqttSetRequest(MQTT_REQ_CMD);

				EEpUpdateEnable();
				}

			p=strstr(p_buf,"/cmd/tfrq/");  
			if(p!=NULL)
			{
				AtSetTopicId(/*"/cmd/tfrq"*/NULL);
				p+=10;
				value_len=strlen(p);
				if(value_len>=TRANSMISSION_FRE_LEN)
					value_len=TRANSMISSION_FRE_LEN;
				memset(g_UserSet.CampFreq.T_frequen,0x00,MEARSURE_FRE_LEN);
				memcpy(g_UserSet.CampFreq.T_frequen,p,value_len);
				MqttSetRequest(MQTT_REQ_CMD);

				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"tfrq\":\"%s\"",g_UserSet.CampFreq.T_frequen);
				GattSetCmdUplinkData(tempBuff);

				//GattSetUplinkData(p);

				EEpUpdateEnable();
				}

			p=strstr(p_buf,"/cmd/updt/"); 
			if(p!=NULL)
			{
				AtSetTopicId(/*"/cmd/updt"*/NULL);
				memset(tempBuff,0x00,128);
				if(strstr(p,"\"auto\"")!=NULL)
				{	g_UserSet.reportt_auto=1;
					//GattSetUplinkData("auto");
					sprintf((char*)tempBuff,"\"updt\":\"%s\"","auto");
					}
				else if(strstr(p,"\"manu\"")!=NULL)
				{	g_UserSet.reportt_auto=0;
					//GattSetUplinkData("manu");
					sprintf((char*)tempBuff,"\"updt\":\"%s\"","manu");
					}
				else
				{
					//GattSetUplinkData("error cmd");
					sprintf((char*)tempBuff,"\"updt\":\"%s\"","error cmd");
					}

				
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
				}
			p=strstr(p_buf,"/cmd/rptm/"); 
			if(p!=NULL)
			{
				AtSetTopicId(/*"/cmd/rptm"*/NULL);

				memset(tempBuff,0x00,128);

				if(strstr(p,"0")!=NULL)
				{	g_UserSet.reportt_auto=1;
					//GattSetUplinkData("\"mode 0\"");
					sprintf((char*)tempBuff,"\"rptm\":\"%d\"",0);
					}
				else if(strstr(p,"1")!=NULL)
				{	g_UserSet.reportt_auto=0;
					//GattSetUplinkData("\"mode 1\"");
					sprintf((char*)tempBuff,"\"rptm\":\"%d\"",1);
					}
				else
				{
					sprintf((char*)tempBuff,"\"rptm\":\"%s\"","error");
					}
				
				
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
				}

			p=strstr(p_buf,"/cmd/hbfq/");  //hbfq
			if(p!=NULL)
			{
				g_UserSet.heartbeat=atoi(p+10);
				AtSetTopicId(/*"/cmd/hbfq"*/NULL);

				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"hbrq\":\"%d\"",g_UserSet.heartbeat);
				GattSetCmdUplinkData(tempBuff);
				//GattSetUplinkData(p+10);

				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
				}

			#ifdef PUMP_PROJECT
			p=strstr(p_buf,"/cmd/swch/"); 
			if(p!=NULL)
			{
				AtSetTopicId("/cmd/swch");

				

				if(strstr(p,"\"on\"")!=NULL)
				{	
					GattSetUplinkData("on");
					#ifdef DC_PUMP_SUPPORT
					//PumpTxCmd(PUMP_SET_ON);
					PumpOn(TRUE);
					#else
					PumpTxCmd(PUMP_RESET);
					HAL_Delay(50);
					PumpTxCmd(PUMP_ON);
					#endif
					}
				else if(strstr(p,"\"off\"")!=NULL)
				{	
					GattSetUplinkData("off");
					#ifdef DC_PUMP_SUPPORT
					//PumpTxCmd(PUMP_SET_OFF);
					PumpOn(FALSE);
					#else
					PumpTxCmd(PUMP_OFF);
					#endif
					}
				else
				{
					GattSetUplinkData("error cmd");
					}
				MqttSetRequest(MQTT_REQ_CMD);
				}
			#endif
			
			
			
			p=strstr(p_buf,"/att");

			if(p!=NULL)
			{
				if(strstr(p_buf,"/att/meta")!=NULL)
					meta=1;
				
				 MqttSetRequest(MQTT_REQ_DTTYPE);
				if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
					MqttSetRequest(MQTT_REQ_SINGLEFIELD);
				else
					GattDtTypeFieldJsonMerge(DT_TYPE_ATT,meta);
				}

			p=strstr(p_buf,"/sts");
			if(p!=NULL)
			{
				if(strstr(p_buf,"/sts/meta")!=NULL)
					meta=1;
				
			          MqttSetRequest(MQTT_REQ_DTTYPE);
				if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
					MqttSetRequest(MQTT_REQ_SINGLEFIELD);
				else
					GattDtTypeFieldJsonMerge(DT_TYPE_STS,meta);
				}

			p=strstr(p_buf,"/sta");
			if(p!=NULL)
			{
				if(strstr(p_buf,"/sta/meta")!=NULL)
					meta=1;
				
				 MqttSetRequest(MQTT_REQ_DTTYPE);
				 
				if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
					MqttSetRequest(MQTT_REQ_SINGLEFIELD);
				else
					GattDtTypeFieldJsonMerge(DT_TYPE_STA,meta);
				}

			p=strstr(p_buf,"\"get\":");
			if(p!=NULL)
			{
				 
				 if(strstr(p_buf,"/meta")!=NULL)
					meta=1;

				 if(strstr(p_buf,"fulldata")!=NULL)
				 {
					MqttSetRequest(MQTT_REQ_ALLFIELD);
				 	}
				 else if(strstr(p_buf,"\"att")!=NULL)
				 {
				 	MqttSetRequest(MQTT_REQ_DTTYPE);
					
				 	if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
					else
						GattDtTypeFieldJsonMerge(LIST_ATT,meta);
				 	}
				 else if(strstr(p_buf,"\"cmd")!=NULL)
				 {
				 	MqttSetRequest(MQTT_REQ_DTTYPE);
				 	if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
					else
						GattDtTypeFieldJsonMerge(LIST_CMD,meta);
				 	}
				 else if(strstr(p_buf,"\"sts")!=NULL)
				 {
				 	MqttSetRequest(MQTT_REQ_DTTYPE);
					
				 	if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
					else
						GattDtTypeFieldJsonMerge(LIST_STS,meta);
				 	}
				 else if(strstr(p_buf,"\"dta")!=NULL)
				 {
				 	MqttSetRequest(MQTT_REQ_DTTYPE);
					
				 	if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
					else
						GattDtTypeFieldJsonMerge(LIST_DTA,meta);
				 	}
				 else if(strstr(p_buf,"\"dia")!=NULL)
				 {
				 	MqttSetRequest(MQTT_REQ_DTTYPE);
					
				 	if(strlen((char*)GattSingleFieldMerge((uint8_t*)p))>0)
						MqttSetRequest(MQTT_REQ_SINGLEFIELD);
					else
						GattDtTypeFieldJsonMerge(LIST_DIA,meta);
				 	}
				 else if(strstr(p_buf,"\"abac")!=NULL)
				 {
				 	
				 	for(i=0;i<g_UserSet.abacus_num;i++)
				 	{
				 		if(strstr(p_buf,g_UserSet.abacuslist[i])!=NULL)
						{	GattAbacSetReprotIndex(i);
							break;
				 			}
				 		}
					if(i>=g_UserSet.abacus_num)
						GattAbacSetReprotIndex(0xff);
					
				 	MqttSetRequest(MQTT_REQ_ABAC);
					
				 	}
									 
				}
			}
		}

		p=strstr(p_buf,"\"set\":");
		if(p!=NULL)
		{
			p=strstr(p_buf,"brok\":");
			if(p!=NULL)
			{
				HAL_Delay(20);
				//p=NULL;
				//p=strstr(p_buf,"\r\n\"");  

				//if(p!=NULL)
				{

					if(strstr(p_buf,"+CMQTTRXPAYLOAD: 0")!=NULL)
						g_mqtt_broker_index=0;
					else
						g_mqtt_broker_index=1;
					
					p=p+6+1;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_BROKER_LEN)
						value_len=MQTT_BROKER_LEN;
					memset(g_UserSet.NetInfor.mqtt_broker,0x00,MQTT_BROKER_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_broker,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_PORT_LEN)
						value_len=MQTT_PORT_LEN;
					memset(g_UserSet.NetInfor.mqtt_port,0x00,MQTT_PORT_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_port,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,',');

					if(value_len>=MQTT_USENAME_LEN)
						value_len=MQTT_USENAME_LEN;
					memset(g_UserSet.NetInfor.mqtt_usename,0x00,MQTT_USENAME_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_usename,p,value_len);

					p=p+value_len+1;
					value_len=AtCmdGetValueLen(p,'\"');

					if(value_len>=MQTT_PASSWORD_LEN)
						value_len=MQTT_PASSWORD_LEN;
					memset(g_UserSet.NetInfor.mqtt_password,0x00,MQTT_PASSWORD_LEN);
					memcpy(g_UserSet.NetInfor.mqtt_password,p,value_len);


					AtSetTopicId(/*"/cmd/nbroker"*/NULL);
					memset(tempBuff,0x00,128);

					sprintf((char*)tempBuff,"\"brok\":\"%s,%s,%s,%s\"",g_UserSet.NetInfor.mqtt_broker,g_UserSet.NetInfor.mqtt_port,g_UserSet.NetInfor.mqtt_usename,g_UserSet.NetInfor.mqtt_password);
					GattSetCmdUplinkData(tempBuff);
	//				GattSetUplinkData(tempBuff);
					MqttSetRequest(MQTT_REQ_CMD);
					EEpUpdateEnable();

					
					}
				
				}

			p=strstr(p_buf,"raml\":");
			if(p!=NULL)
			 {
				p=strstr(p_buf,"raml\":[]");
			 	if(p!=NULL)
			 	{
			 		memset(tempBuff,0x00,128);
			 		memcpy(tempBuff,"\"raml\":[]",9);
					GattSetCmdUplinkData(tempBuff);
					g_UserSet.raml_num=0;
					memset(g_UserSet.raml,0x00,RAML_SIZE*4);
			 		}
				else
				{
					uint8_t list=LIST_ATT,id=0,prop[8];
					extern const uint8_t g_GattlistMemberNum[LIST_COUNT];
					g_UserSet.raml_num=0;
					memset(g_UserSet.raml,0x00,RAML_SIZE*4);

					for(list=LIST_ATT;list<LIST_COUNT;list++)
					{
						for(id=0;id<g_GattlistMemberNum[list];id++)
						{    
							memset(prop,0x00,8);
							{	
								if(GattGetListProp(list,id,prop))
								{
									p=strstr(p_buf+7,(char*)prop);
									if(p!=NULL&&strstr((char*)g_UserSet.raml,(char*)prop)==NULL)
									{
										memcpy(g_UserSet.raml[g_UserSet.raml_num++],prop,4);
										}
									}
							
								}
							}
						}

					memset(tempBuff,0x00,128);

					memcpy(tempBuff,"\"raml\":[",8);
					if(g_UserSet.raml_num)
					{
						for(i=0;i<g_UserSet.raml_num;i++)
						{
							sprintf((char*)(tempBuff+strlen((char*)tempBuff)),"\"%s\"",g_UserSet.raml[i]);
							memcpy(tempBuff+strlen((char*)tempBuff),",",1);
							}

						tempBuff[strlen((char*)tempBuff)-1]=0;
						}
					//else
					{
						memcpy(tempBuff+strlen((char*)tempBuff),"]",1);
						}

					GattSetCmdUplinkData(tempBuff);
					}
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}
			
			p=strstr(p_buf,"ramt\":");
			if(p!=NULL)
			 {
			 	tempInt16=atoi(p+6);

				if(tempInt16>=1&&tempInt16<720)
					g_UserSet.ramt=tempInt16;

				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"ramt\":%d",g_UserSet.ramt);
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}

			p=strstr(p_buf,"abac\":");
			if(p!=NULL)
			 {
				p=strstr(p_buf,"abac\":[]");
			 	if(p!=NULL)
			 	{
			 		memset(tempBuff,0x00,128);
			 		memcpy(tempBuff,"\"abac\":[]",9);
					GattSetCmdUplinkData(tempBuff);
					g_UserSet.abacus_num=0;
					memset(g_UserSet.abacuslist,0x00,MAX_ABACUS_NUMBER*6);
			 		}
				else
				{
					uint8_t list=LIST_ATT,id=0,prop[8];
					extern const uint8_t g_GattlistMemberNum[LIST_COUNT];
					g_UserSet.abacus_num=0;
					memset(g_UserSet.abacuslist,0x00,MAX_ABACUS_NUMBER*6);

					for(list=LIST_ATT;list<LIST_COUNT;list++)
					{
						for(id=0;id<g_GattlistMemberNum[list];id++)
						{    
							memset(prop,0x00,8);
							{	
								if(GattGetListProp(list,id,prop))
								{
									p=strstr(p_buf+7,(char*)prop);
									if(p!=NULL&&strstr((char*)g_UserSet.abacuslist,(char*)prop)==NULL)
									{
										memcpy(g_UserSet.abacuslist[g_UserSet.abacus_num++],prop,4);
										}
									}
							
								}
							}
						}

					memset(tempBuff,0x00,128);

					memcpy(tempBuff,"\"abac\":",8);
					if(g_UserSet.abacus_num)
					{
						for(i=0;i<g_UserSet.abacus_num;i++)
						{
							sprintf((char*)(tempBuff+strlen((char*)tempBuff)),"\"%s\"",g_UserSet.abacuslist[i]);
							memcpy(tempBuff+strlen((char*)tempBuff),",",1);
							}

						tempBuff[strlen((char*)tempBuff)-1]=0;
						}
					//else
					{
						//memcpy(tempBuff+strlen((char*)tempBuff),"]",1);
						}

					GattSetCmdUplinkData(tempBuff);
					}
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}
			p=strstr(p_buf,"pubk\":\"");
			if(p!=NULL)
			 {
			 	uint8_t len=0,pbukbuf[64];
				
				AtCmdTokenParse((uint8_t*)p,"pubk\":\"*0");

				memset(pbukbuf,0x00,64);
				
				memset(tempBuff,0x00,128);

				if( g_tokenState==TOKEN_OK)
				{
					len=AtCmdGetValueLen(p+7,'#')+1;

					memcpy(pbukbuf,p+7,len);  //for 4G
					
					sprintf((char*)tempBuff,"\"pubk\":\"%s\"",pbukbuf);

					len=strlen((char*)tempBuff);

					/*if(len)
					{	tempBuff[len-1]=0;
					    tempBuff[len-2]=0;
						}*/
					}
				else if( g_tokenState==TOKEN_USEED)
					sprintf((char*)tempBuff,"\"pubk\":\"%s\"","password use");
				else
					sprintf((char*)tempBuff,"\"pubk\":\"%s\"","password error");

				GattSetCmdUplinkData(tempBuff);
					
				MqttSetRequest(MQTT_REQ_CMD);
			 	}

			p=strstr(p_buf,"gstw\":");
			if(p!=NULL)
			 {
			 	tempInt16=atoi(p+6);

				if(tempInt16>0&&tempInt16<720)
					g_UserSet.sleeptime=tempInt16;

				/*if(g_UserSet.sleeptime>720)
				{	g_UserSet.sleeptime=720;
					}*/
				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"gstw\":%d",g_UserSet.sleeptime);
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}

			p=strstr(p_buf,"gctw\":");
			if(p!=NULL)
			 {
			 	
				tempInt16=atoi(p+6);
				if(tempInt16>0&&tempInt16<g_UserSet.sleeptime)
					g_UserSet.onlinetime=tempInt16;
				
				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"gctw\":%d",g_UserSet.onlinetime);
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}

			p=strstr(p_buf,"napn\":\"");
			if(p!=NULL)
			 {
				p=p+7;
				value_len=AtCmdGetValueLen(p,',');

				if(value_len>=APN_LEN)
					value_len=APN_LEN;
				memset(g_UserSet.NetInfor.apn,0x00,APN_LEN);
				memcpy(g_UserSet.NetInfor.apn,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen(p,',');

				if(value_len>=APN_USENAME_LEN)
					value_len=APN_USENAME_LEN;
				memset(g_UserSet.NetInfor.apn_usename,0x00,APN_USENAME_LEN);
				memcpy(g_UserSet.NetInfor.apn_usename,p,value_len);

				p=p+value_len+1;
				value_len=AtCmdGetValueLen(p,'\"');

				if(value_len>=APN_PASSWORD_LEN)
					value_len=APN_PASSWORD_LEN;
				memset(g_UserSet.NetInfor.apn_password,0x00,APN_PASSWORD_LEN);
				memcpy(g_UserSet.NetInfor.apn_password,p,value_len);

				memset(tempBuff,0x00,128);
				
				sprintf((char*)tempBuff,"\"napn\":\"%s,%s,%s\"",g_UserSet.NetInfor.apn,g_UserSet.NetInfor.apn_usename,g_UserSet.NetInfor.apn_password);

				GattSetCmdUplinkData(tempBuff);
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}
			p=strstr(p_buf,"swch\":\"");
			if(p!=NULL)
			 {
			 	memset(tempBuff,0x00,128);
				
				 if(strstr(p,"\"on\"")!=NULL)
				{	
					sprintf((char*)tempBuff,"\"swch\":\"on\"");
					#ifdef DC_PUMP_SUPPORT
					//PumpTxCmd(PUMP_SET_ON);
					PumpOn(TRUE);
					#else
					PumpTxCmd(PUMP_RESET);
					HAL_Delay(50);
					PumpTxCmd(PUMP_ON);
					#endif
					}
				else if(strstr(p,"\"off\"")!=NULL)
				{	
					sprintf((char*)tempBuff,"\"swch\":\"off\"");
					#ifdef DC_PUMP_SUPPORT
					//PumpTxCmd(PUMP_SET_OFF);
					PumpOn(FALSE);
					#else
					PumpTxCmd(PUMP_OFF);
					#endif
					}
				else
				{
					sprintf((char*)tempBuff,"\"swch\":\"error\"");
					}
				GattSetUplinkData(tempBuff);
				MqttSetRequest(MQTT_REQ_CMD);
			 	}
			p=strstr(p_buf,"read\":\"");
			if(p!=NULL)
			 {
				memset(tempBuff,0x00,128);

				memcpy(tempBuff,p,strlen(p));
				
			 	GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}
			p=strstr(p_buf,"rptm\":");
			if(p!=NULL)
			 {
				memset(tempBuff,0x00,128);

				if(strstr(p,":0")!=NULL)
				{	g_UserSet.reportt_auto=1;
					sprintf((char*)tempBuff,"\"rptm\":%d",0);
					}
				else if(strstr(p,":1")!=NULL)
				{	g_UserSet.reportt_auto=0;
					sprintf((char*)tempBuff,"\"rptm\":%d",1);
					}
				else
				{
					sprintf((char*)tempBuff,"\"rptm\":\"%s\"","error");
					}
				
				
				GattSetCmdUplinkData(tempBuff);
				
				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}
			p=strstr(p_buf,"hbfq\":");
			if(p!=NULL)
			 {
			 	g_UserSet.heartbeat=atoi(p+6);
				memset(tempBuff,0x00,128);
				sprintf((char*)tempBuff,"\"hbfq\":%d",g_UserSet.heartbeat);
				GattSetCmdUplinkData(tempBuff);

				MqttSetRequest(MQTT_REQ_CMD);
				EEpUpdateEnable();
			 	}

			#ifdef P10KW_PROJECT
			p_gattlist=GattGetList();
			
			for(i=0;i<CMD_COUNT-CMD_AOCT;i++)
			{
				p=strstr(p_buf,p_gattlist->cmd[CMD_AOCT+i].prop);
				if(p!=NULL)
				 {
					memset(tempBuff,0x00,128);
					tempU16=(uint16_t)atoi(p+6);
					
				 	switch(CMD_AOCT+i)
				 	{
				 		case CMD_AOCT:
							//tempU16=(uint16_t)atoi(p+6);
							sprintf((char*)tempBuff,"\"aoct\":%d",tempU16);
							OffGrid_SetReg(0x00,tempU16);
							break;
					    case CMD_AOSS:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>3)
								tempU16=3;
							sprintf((char*)tempBuff,"\"aoss\":%d",tempU16);
							OffGrid_SetReg(0x01,tempU16);
							break;
					    case CMD_BCSS:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>2)
								tempU16=2;
							sprintf((char*)tempBuff,"\"bcss\":%d",tempU16);
							OffGrid_SetReg(0x02,tempU16);
							break;
					    case CMD_UOST:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>23)
								tempU16=23;
							sprintf((char*)tempBuff,"\"uost\":%d",tempU16);
							OffGrid_SetReg(0x03,tempU16);
							break;
					    case CMD_UOET:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>23)
								tempU16=23;
							sprintf((char*)tempBuff,"\"uoet\":%d",tempU16);
							OffGrid_SetReg(0x04,tempU16);
							break;
					    case CMD_UCST:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>23)
								tempU16=23;
							sprintf((char*)tempBuff,"\"ucst\":%d",tempU16);
							OffGrid_SetReg(0x05,tempU16);
							break;
					    case CMD_UCET:
							//tempU16=(uint16_t)atoi(p+6);
							if(tempU16>23)
								tempU16=23;
							sprintf((char*)tempBuff,"\"ucet\":%d",tempU16);
							OffGrid_SetReg(0x06,tempU16);
							break;
					    case CMD_PIMD:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"pimd\":%d",tempU16);
							OffGrid_SetReg(0x07,tempU16);
							break;
					    case CMD_AIMD:
							sprintf((char*)tempBuff,"\"aimd\":%d",tempU16);
							OffGrid_SetReg(0x08,tempU16);
							break;
					    case CMD_LNGS:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"lngs\":%d",tempU16);
							OffGrid_SetReg(15,tempU16);
							break;
					    case CMD_AOVS:
							if(tempU16>6)
								tempU16=6;
							sprintf((char*)tempBuff,"\"aovs\":%d",tempU16);
							OffGrid_SetReg(18,tempU16);
							break;
					    case CMD_AOFS:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"aofs\":%d",tempU16);
							OffGrid_SetReg(19,tempU16);
							break;
					    case CMD_OLRS:
							if(tempU16>2)
								tempU16=2;
							sprintf((char*)tempBuff,"\"olrs\":%d",tempU16);
							OffGrid_SetReg(20,tempU16);
							break;
					    case CMD_OTRS:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"otrs\":%d",tempU16);
							OffGrid_SetReg(21,tempU16);
							break;
					    case CMD_BZON:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"bzon\":%d",tempU16);
							OffGrid_SetReg(22,tempU16);
							break;
					    case CMD_CASS:
							if(tempU16>254)
								tempU16=254;
							sprintf((char*)tempBuff,"\"cass\":%d",tempU16);
							OffGrid_SetReg(30,tempU16);
							break;
					    case CMD_MXCC:
							if(tempU16>180)
								tempU16=180;
							sprintf((char*)tempBuff,"\"mxcc\":%d",tempU16);
							OffGrid_SetReg(34,tempU16);
							break;
					    case CMD_BBCV:
							if(tempU16>640)
								tempU16=640;
							if(tempU16<500)
								tempU16=500;
							
							sprintf((char*)tempBuff,"\"bbcv\":%d",tempU16);
							OffGrid_SetReg(35,tempU16);
							break;
					    case CMD_BFCV:
							if(tempU16>560)
								tempU16=560;
							if(tempU16<500)
								tempU16=500;
							sprintf((char*)tempBuff,"\"bfcv\":%d",tempU16);
							OffGrid_SetReg(36,tempU16);
							break;
					    case CMD_LBUS:
							if(tempU16>640)
								tempU16=640;
							if(tempU16<300)
								tempU16=300;
							sprintf((char*)tempBuff,"\"lbus\":%d",tempU16);
							OffGrid_SetReg(37,tempU16);
							break;
					    case CMD_ACCS:
							if(tempU16>100)
								tempU16=100;
							sprintf((char*)tempBuff,"\"accs\":%d",tempU16);
							OffGrid_SetReg(38,tempU16);
							break;
					    case CMD_AGMD:
							if(tempU16>1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"agmd\":%d",tempU16);
							OffGrid_SetReg(40,tempU16);
							break;
					    case CMD_STYS:
							if(tempU16>99)
								tempU16=99;
							sprintf((char*)tempBuff,"\"stys\":%d",tempU16);
							OffGrid_SetReg(45,tempU16);
							break;
					    case CMD_STMS:
							if(tempU16>12)
								tempU16=12;

							if(tempU16<1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"stms\":%d",tempU16);
							OffGrid_SetReg(46,tempU16);
							break;
					    case CMD_STDS:

							if(tempU16>31)
								tempU16=31;

							if(tempU16<1)
								tempU16=1;
							sprintf((char*)tempBuff,"\"stds\":%d",tempU16);
							OffGrid_SetReg(47,tempU16);
							break;
					    case CMD_STHS:
							if(tempU16>23)
								tempU16=23;
							sprintf((char*)tempBuff,"\"sths\":%d",tempU16);
							OffGrid_SetReg(48,tempU16);
							break;
					    case CMD_STMM:
							if(tempU16>59)
								tempU16=59;
							sprintf((char*)tempBuff,"\"stmm\":%d",tempU16);
							OffGrid_SetReg(49,tempU16);
							break;
					    case CMD_STSS:
							if(tempU16>59)
								tempU16=59;
							sprintf((char*)tempBuff,"\"stss\":%d",tempU16);
							OffGrid_SetReg(50,tempU16);
							break;
				 		}
				 	
					GattSetCmdUplinkData(tempBuff);

					MqttSetRequest(MQTT_REQ_CMD);
				 	}
				}

		  #endif
			}

		

		if(pdeal!=NULL)	
		{	
			p=strstr(str,(char*)pdeal->ackstr);
			
			if(p!=NULL)
			{
				/*Uart5Send(" ACK:",6);
				Uart5Send(p,strlen(p));
				Uart5Send(str,strlen(str));
				
				Uart5Send("\r\n",2);*/
				if(pdeal->cmd==AT_CMD_CIPSEND)
				{
					g_AtCmdState=AT_CMD_CIPSEND_DATA;
					}
				else	
				{	
					g_AtAckState=AT_ACK_OK;
					g_AtCmdState=AT_CMD_NONE;
					}
				
				if(pdeal->ackfun!=NULL)
				{
					pdeal->ackfun((uint8_t*)str);
					}
				
				}
		}
	
}



void AtCmdProc(void)
{

	uint32_t i;
	uint8_t testprintf[128]={0};

	//if(huart2.RxState==HAL_UART_STATE_READY)
	//	HAL_UART_Receive_IT(&huart2,g_GsmRbuffer,GSM_BUFFER);
	
	if(g_AtAckTout_timer.event)
	{
		g_AtAckTout_timer.event=0;
		g_AtAckTout_timer.retry++;
		if(g_AtAckTout_timer.retry<AT_RETRY_CNT)
		{	
			AtCmdMerge(g_AtCmdState);
			TimerAtTOutStart(5000U,FALSE);
			}
		}
	g_RxGsmCounter=/*huart2.RxXferSize-*/huart2.RxXferCount;

	for(i=g_RxGsmParsePos;i<g_RxGsmCounter;i++)
	{
		if((g_GsmRbuffer[i-1]==0x0d&&g_GsmRbuffer[i]==0x0a)&&i||g_GsmRbuffer[i]=='>'||g_GsmRbuffer[i]=='}'
			||g_GsmRbuffer[i-2]==MQTT_ClienID[0]&&g_GsmRbuffer[i-1]==MQTT_ClienID[1]&&g_GsmRbuffer[i]==MQTT_ClienID[2])
		{

			HAL_Delay(10);
		
			g_RxGsmParseSize=i+1-g_RxGsmParsePos;	

#ifdef DEBUG_AT_LOG
			LogPrintf("GSM:",4);

			//sprintf(testprintf,"%02x,%02x,%02x %02x  ",g_GsmRbuffer[i-2],g_GsmRbuffer[i-1],g_GsmRbuffer[i],g_GsmRbuffer[i+1]);
			LogPrintf(testprintf,strlen((char*)testprintf));
			//HAL_UART_Transmit(&huart1,&g_GsmRbuffer[g_RxGsmParsePos],strlen((char*)&g_GsmRbuffer[g_RxGsmParsePos]),1000);
			LogPrintf(&g_GsmRbuffer[g_RxGsmParsePos],strlen((char*)&g_GsmRbuffer[g_RxGsmParsePos]));
#endif
			
			AtCmdPaser(&g_GsmRbuffer[g_RxGsmParsePos],g_AtCmdState);
			

			g_RxGsmParsePos=i+1;
			 
			}
		}

	  
	
	
}


