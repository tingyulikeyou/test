 #ifndef __GSMCOM_H__
#define __GSMCOM_H__


enum
{
	MQTT_REQ_NONE,
	MQTT_REQ_ALLFIELD,
	MQTT_REQ_DTTYPE,
	MQTT_REQ_SINGLEFIELD,
	MQTT_REQ_CMD,
	MQTT_REQ_RAML,
};

typedef enum
{
	MQTT_STATE_IDLE,   //0
	MQTT_STATE_INIT,
	MQTT_STATE_AT,	  //
	MQTT_STATE_ATE0,  // 2
	MQTT_STATE_CIPCLOSE,// 3
	MQTT_STATE_CIPSHUT,   // 4
	MQTT_STATE_CPIN,  //5
	MQTT_STATE_CSQ, //6
	MQTT_STATE_CREG,  //7
	MQTT_STATE_CGATT,	  //8
	MQTT_STATE_CSTT,	  //9
	MQTT_STATE_CSTT_Q,	  //9
	MQTT_STATE_CIICR,     // 0x0a
	MQTT_STATE_CIFSR,  //0xb
	MQTT_STATE_CIPSTAR,   //c
	MQTT_STATE_CIPSEND,  //d
	MQTT_STATE_CIPSEND_PUB,  //d
	MQTT_STATE_WAIT,
	MQTT_STATE_SUBLISH,  //d
	MQTT_STATE_CONNECT,
	MQTT_STATE_SUBSCRIBE,
	MQTT_STATE_SUBSCRIBEACK,
	MQTT_STATE_PUBLISH,
	MQTT_STATE_PUBLISHACK,
	MQTT_STATE_CHK_REQUEST,
	MQTT_STATE_SAPBR,
	MQTT_STATE_CLBSCFG,
	MQTT_STATE_CLBS,
	MQTT_STATE_END,  //d
	MQTT_STATE_ERROR
}MQTT_STATE_DEF;

typedef enum
{
	SMS_STATE_IDLE,   //0
	SMS_STATE_INIT,	  // 1
	SMS_STATE_AT,	  // 1
	SMS_STATE_ATE0,  // 2
	SMS_STATE_CIPCLOSE,// 3
	SMS_STATE_CIPSHUT,   // 4
	SMS_STATE_CPIN,  //5
	SMS_STATE_CSQ, //6
	SMS_STATE_CREG,  //7
	SMS_STATE_CGAT,	  //8
	SMS_STATE_CMGF,	  //8
	SMS_STATE_CSTT,	  //9
	SMS_STATE_WAIT,     // 0x0a
	SMS_STATE_JUMP,     // 0x0a
	SMS_STATE_CPMS,  //0xb
	SMS_STATE_CMGR,   //c
	SMS_STATE_CMGR_PARSE,   //c
	SMS_STATE_READ,   //c
	SMS_STATE_CMGS,   //c
	SMS_STATE_CMGS_SMS,   //c
	SMS_STATE_CMGF_0,   //c
	SMS_STATE_CMGDA,   //DEL ALL SMS
	SMS_STATE_CMGF_1,  //c
	SMS_STATE_END,  //c
	SMS_STATE_ERROR
}SMS_STATE_DEF;


uint8_t  GmsNetConnectState(void);
void  GmsSetNetConnectState(uint8_t state);
void GsmComInit(void);
void GsmComProc(void);
uint8_t GmsGetSimcardState(void);
void  GmsSetSimcardState(uint8_t state);
void MqttInit(void);
void MqttProc(void);
void MqttSetRequest(uint8_t request);
uint8_t MqttGetRequest(void);
uint8_t MqttGetState(void);
void GsmComIdle(void);
void  GmsWakeup(void);

void SmsInit(void);
void SmsProc(void);
#endif


