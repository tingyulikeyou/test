#ifndef __PUMP_H__
#define __PUMP_H__

#define UART485_RX_BUF_SIZE  512

#ifdef DC_PUMP_SUPPORT
typedef enum
{
	PUMP_NONE,
	PUMP_GET_RMP,
	PUMP_GET_POWER,
	PUMP_GET_WATERLEVEL,
	PUMP_GET_LED_STATUS,
	PUMP_SET_ON,
	PUMP_SET_OFF,
	PUMP_SET_LOCK,
	PUMP_SET_UNLOCK
}PUMP_CMD_TypeDef;


typedef struct  
{
	uint8_t  header;
	uint8_t  device;
	uint8_t  target;
	uint8_t  addr;
	uint8_t  cmd;
	uint8_t  data;
	uint8_t  checksum[2];
	uint8_t  end;
	uint8_t  reserved[3];
} PUMP_PACKET_TypeDef;

#else
typedef enum
{
	PUMP_NONE,
	PUMP_SET_MODUBUS,
	PUMP_PWR_TIME,
	PUMP_PWR_OV_TIME,
	PUMP_GET_STATUS,
	PUMP_CHECK_RUN,
	PUMP_SET_PWR_CMD,
	PUMP_RESET,
	PUMP_ON,
	PUMP_OFF,
	PUMP_FREE
}PUMP_CMD_TypeDef;

#endif

#ifdef DC_PUMP_SUPPORT
typedef enum
{
	LED_EER_I,
	LED_TANK_F,
	LED_WELL_L,
	LED_MPPT,
	LED_LOW_P,
	LED_PUMP,
	LED_SOL_AR,
	LED_ALL
}LED_TypeDef;

typedef struct
{
	uint8_t pump_run_state:1;
	uint8_t MPPT_state:1;
	uint8_t lowpress_state:1;
	uint8_t fault_state:1;
	
	uint8_t tank_state:1;
	uint8_t well_state:1;
	uint8_t dc_state:1;
	uint8_t reserved:1;
	}LED_BIT_TypeDef;


typedef union
{
	uint8_t ledstate;
	LED_BIT_TypeDef ledbitstate;
	
}LED_STATE_TypeDef;

#endif
typedef struct  
{
  unsigned int mFrq;
  unsigned int busV;
  unsigned int outV;
  unsigned int outC;
  unsigned int outP;
  unsigned int mTrq;
  unsigned int mSpd;
  unsigned int runS;
} PUMP_STATE_TypeDef;

void PumInit(void);
void PumpProc(void);
unsigned int PumpParseModbusState(unsigned char *buf);
unsigned int PumpParseModbus(unsigned char *buf);
void PumpTxCmd(uint8_t cmd);
void PumpComEnable(void);
uint8_t  BuffCompare(uint8_t *src,uint8_t *dst,uint8_t size);

void PumpComStart(void);
void PumpSend(const uint8_t * buffer,uint16_t size);
uint16_t crc_chk_value(unsigned char *data_value,unsigned char length);
#ifdef DC_PUMP_SUPPORT
void PumpOn(uint8_t on);
void PumpLedShow(LED_TypeDef led,uint8_t on );
#endif
#endif

