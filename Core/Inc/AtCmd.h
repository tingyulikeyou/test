#ifndef __ATCMD_H__
#define __ATCMD_H__

#define AT_RETRY_CNT  6
#define GSM_BUFFER 1024

enum
{
	AT_ACK_NONE,
	AT_ACK_OK,
	AT_ACK_SEND_OK,
	AT_ACK_ERROR,
	AT_ACK_TIMEOUT
};

typedef enum
{
	AT_CMD_NONE,
	AT_CMD_WKUP,	
	AT_CMD_AT,
	AT_CMD_ATE0,
	AT_CMD_CIPCLOSE,
	AT_CMD_CIPSHUT,
	AT_CMD_CPIN,
	AT_CMD_CSQ,
	AT_CMD_CREG,
	AT_CMD_CGATT,
	AT_CMD_CIICR,
	AT_CMD_CMGF,
	AT_CMD_CMGF_0,
	AT_CMD_CMGDA, //DEL ALL SMS
	AT_CMD_CMGR,  //READ SMS 
	AT_CMD_CMGS,  //SEND SMS
	AT_CMD_CMGS_SMS,  //SEND SMS
	AT_CMD_CPMS,
	AT_CMD_CSTT,
	AT_CMD_CSTT_Q,
	AT_CMD_CIFSR,
	AT_CMD_CIPSTART,
	AT_CMD_CIPSEND,
	AT_CMD_CIPSEND_DATA,
	AT_CMD_SAPBR,
	AT_CMD_CLBSCFG,
	AT_CMD_CLBS

}AT_CMD_DEF;


typedef struct
{
	uint32_t cmd;
	uint8_t *cmdstr;
	uint8_t *ackstr;
	//uint8_t *ackstr1;
	void (*send)(uint8_t* str);
	void (*ackfun)(uint8_t* str);
}AT_DEAL_TypeDef;

typedef enum
{
	TAG_NONE,
	TAG_APN,	
	TAG_APN_USERNAME,
	TAG_APN_PASSWORD,
	TAG_TOKEN,
	TAG_MEANS_FRQ,
	TAG_TRANS_FRQ,
	TAG_BROKER,
	TAG_PORT,
	TAG_USERNAME,
	TAG_PASSWORD,
	TAG_PHONE_NO,
	//TAG_AUTO_REPORT,
	TAG_SWTICH


}TAG_TypeDef;

typedef struct
{
	uint32_t index;
	uint8_t *tag_str;
	void (*parse)(uint8_t* str,uint8_t* tag_str);
}AT_PARSE_TypeDef;

uint32_t AtouI(uint8_t *ascii);

void AtCmdInit(void);
void AtCmdSend(uint8_t * buffer);
void AtCmdCipSend(uint8_t * buffer);
void AtCmdCipStart(uint8_t * buffer);
uint16_t  AtCmdGetValueLen(char *str,uint8_t end_tag);
void AtCmdMerge(uint8_t cmd);
void AtCmdPaser(uint8_t *buffer,uint8_t cmd);
void AtCmdProc(void);

void AtCmdCsttSend(uint8_t * buffer);
void AtCmdCmgrSend(uint8_t * buffer);
void AtCmdCmgsSend(uint8_t * buffer);
void AtCmdCmgsSMSSend(uint8_t * buffer);
void AtCmdCpmsAck(uint8_t * buffer);

uint8_t*AtStrStr(uint8_t *src,uint8_t *dst);

void AtCmdApnParse(uint8_t*str,uint8_t*tag);
void AtCmdApnUserParse(uint8_t*str,uint8_t*tag);
void AtCmdApnPswdParse(uint8_t*str,uint8_t*tag);
void AtCmdTokenParse(uint8_t*str,uint8_t*tag);
void AtCmdMeansFrqParse(uint8_t*str,uint8_t*tag);
void AtCmdTransFrqParse(uint8_t*str,uint8_t*tag);
void AtCmdBrokerParse(uint8_t*str,uint8_t*tag);
void AtCmdPortParse(uint8_t*str,uint8_t*tag);
void AtCmdMqttUsrNameParse(uint8_t*str,uint8_t*tag);
void AtCmdMqttPswdParse(uint8_t*str,uint8_t*tag);
void AtCmdSwitchParse(uint8_t*str,uint8_t*tag);
void AtCmdPhoneNoParse(uint8_t*str,uint8_t*tag);
void AtSetTopicId(uint8_t * ext_topic);
void AtSetSubscribeId(void);
void AtCmdCipSendOk(uint8_t * buffer);

#endif



