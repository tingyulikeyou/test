#ifndef __COULOM_H__
#define __COULOM_H__

//#define UART485_RX_BUF_SIZE  256


#define NORMAL_CAPACITY (uint32_t)43000u    //48 Ah

 enum
{      CHRGE_STATE_IDLE,
	CHRGE_STATE_RECHRG,
	CHRGE_STATE_CHRGE,
};

typedef enum
{      LOWBAT_NONE,
	LOWBAT_LEVEL1,
	LOWBAT_LEVEL2,
}LOWBAT_LEVEL_TypeDef;

typedef enum
{      COULOM_IDLE,
	COULOM_SOC,
	COULOM_TEMP,
	COULOM_VOLTAGE, //V
	COULOM_CURRENT,  //A
	COULOM_RT_PWR,   //KW
	COULOM_ACCU_ENERGE,//KWH
	COULOM_ACCU_CAP,   //mAH
	COULOM_WARN,
	COULOM_RECHRGESTATE,
	COULOM_FULLSET
}COULOM_CMD_TypeDef;

typedef struct
{
	uint8_t bus_addr;
	uint8_t func;
	uint8_t regH;
	uint8_t regL;
	uint8_t numH;
	uint8_t numL;
	uint16_t crc;
}MODBUS_READ_TypeDef;

typedef struct
{
	uint8_t reserved:5;
	uint8_t ac_fault:1;
	uint8_t volt_over:1;
	uint8_t volt_low:1;
	
	uint8_t coulom_com:1;
	uint8_t pwd_len_error:1;
	uint8_t rtc_error:1;
	uint8_t pswd_error:1;
	
	uint8_t reserved1:1;
	uint8_t dc_current_over:1;
	uint8_t chg_current_over:1;
	uint8_t temp_over:1;
}ERR_CODE_TypeDef;

typedef union
{
	uint8_t state[4];
	uint16_t statecode;
	ERR_CODE_TypeDef error;
}SYS_STATE_CODE_TypeDef;


void SysPowerOff(void);

uint16_t CRC16(uint8_t* pData, uint16_t len);
uint8_t CrcCheck(uint8_t *pSrc, uint16_t size);
uint16_t CoulomGetRtPower(void);
uint16_t CoulomGetChrgState(void);
void CoulomSleep(void);
uint16_t CoulomGetRuntime(void);

void CoulomInit(void);
void CoulomProc(void);
void CoulomTxCmd(uint8_t cmd);
void CoulomComEnable(void);
void CoulomComStart(void);
void CoulomComMerge(uint8_t reg,uint8_t size);
uint8_t CoulomVoltOver(void);
uint8_t CoulomLowbat(void);
void CoulomSend( uint8_t * buffer,uint16_t size);

void CoulomConfig(void);

#endif

