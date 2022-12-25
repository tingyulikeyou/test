
#ifndef __BMS_SMARTLI_SUPPROT_H__
#define __BMS_SMARTLI_SUPPROT_H__

enum
{
	SBMS_CMD_NONE,
	SBMS_CMD_PROTECT,
	SBMS_CMD_RTDATA,
	SBMS_CMD_REVERED0,
	SBMS_CMD_REVERED1,
	SBMS_CMD_SET,
	SBMS_CMD_FET,
	SBMS_CMD_VERSION=9
};


typedef struct
{
	uint8_t header;
	uint8_t addr;
	uint8_t cmd;
	uint8_t ver;
	
	uint16_t len;
	uint8_t crc;
	uint8_t end;

	uint8_t *infor;
	
}SBMS_CMD_TypeDef;

void HextoHexAscii(uint8_t hex,uint8_t*hexAscii);
uint8_t HexAsciitoHex(uint8_t*hexAscii);
void SmartBmsInit(void);
uint8_t SBms_GetInfo(void);
void SBmsCmdConvert(SBMS_CMD_TypeDef pack,uint8_t *cmd);
void SBmsTask(void);
void SbmsWakeupRelease(void);
void Smart485Send(uint8_t *buffer,uint16_t size);

#endif

