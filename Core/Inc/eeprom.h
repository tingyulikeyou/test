
#ifndef __EEPROM_H__
#define __EEPROM_H__

#define RAML_SIZE   20

#ifdef ABACUSLEDER_SUPPORT
#define MAX_ABACUS_NUMBER  8

#define ABACUSLEDER_START_ADDR  0x400
#define ABACUSLEDED_SIZE   3*1024u	
#endif


typedef struct
{

	PAYG_TypeDef  Payg;  //104
	NET_INFOR_TypeDef NetInfor;  //152
	CAMP_FREQ_TypeDef CampFreq;//16
	NET_INFOR_TypeDef NetInforFactory;
	uint8_t reportt_auto;// 0,manu,1 auto
	uint8_t ble_state;
	uint8_t lowbat;
	uint8_t demo_Cnt;

	uint32_t sleeptime;   //min
	uint32_t onlinetime;   //min
	uint32_t report_fail_cnt;
	uint32_t heartbeat;
	uint32_t wakeup_cnt;
	
	uint32_t accuCap;
	uint32_t accuPower;
	uint32_t accuSolarGen;
	uint32_t accuRuntime;

	uint32_t ramt;
	uint32_t raml_num;
	uint8_t raml[RAML_SIZE][6];

	
#ifdef ABACUSLEDER_SUPPORT
	//uint32_t abacus;
	uint32_t abacus_num;
	uint8_t abacuslist[MAX_ABACUS_NUMBER][6];
#endif

	uint8_t jtag;
	uint8_t reserved[3];
	
	uint32_t endmark;  // 4
}USER_SET_TypeDef;

typedef struct
{
	uint32_t sec[60]; 
	uint32_t min[60]; 
	uint32_t hour[24]; 
	uint32_t day[365];
	uint32_t year[10];
}ABACUS_IMAGE_TypeDef;

typedef struct
{
	uint32_t sec_counter; 
	uint32_t min_counter; 
	uint32_t hour_counter; 
	uint16_t day_counter;
	uint16_t year_counter;
}ABACUS_COUNTER_TypeDef;


#define EEPROM_START_ADDR  0x801f800//FLASH_EEPROM_BASE
#define eeprom_addr(a) ((uint32_t)(a+EEPROM_START_ADDR))

#define FLASH_START_ADDR   (uint32_t)0x8000000

#define PAGE_SIZE (uint32_t)2048
#define FMC_PAGE_SIZE (uint32_t)2048

#define EEP_PAGE_SIZE 32u

#define ApplicationAddress    (uint32_t)0x8002000
#define UpgradeflagAddress FLASH_START_ADDR+0xfcc0+3*64*1024
#define BootModeflag           0x1a1a1a1a



#define EEP_START_ADDR  (uint32_t)1024* (uint32_t)(63)
void EEpInit(void);
void EEpWritePage(uint32_t PageAddress,uint32_t size,uint8_t* buffer);
void EEpReadPage(uint32_t PageAddress,uint32_t size,uint8_t* buffer);

static void EEpWrite(uint32_t Address,uint16_t len,uint8_t  *buf);
static void EEpRead(uint32_t Address,uint16_t len,uint8_t  *buf);
void FlashPageErase(uint32_t PageAddress);
 uint8_t FlashPageProgram(uint32_t PageAddress,uint32_t size,uint32_t* buffer);
static void FlashRead(uint32_t PageAddress,uint32_t size,uint32_t* buffer);

//void EEpUpdatePayg(PAYG_TypeDef* payg);
uint32_t EEpGetAccuCap(void);
uint32_t EEpGetAccuPower(void);
uint32_t EEpGetAccuSolarGen(void);
uint32_t EEpGetAccuRuntime(void);

void EEpSetAccuCap(uint32_t cap);
void EEpSetAccuPower(uint32_t power);
void EEpSetAccuSolarGen(uint32_t solar);
void EEpSetAccuRuntime(uint32_t solar);

void EEpUpdateEnable(void);
void EEpProcess(void);
uint32_t EEpGetTransFreq(void);
uint8_t EEpGetAttState(void);
uint8_t EEpGetStsState(void);
uint8_t EEpGetStaState(void);
uint32_t EEpGetOnlineTime(void);
uint32_t EEpGetSleepTime(void);
uint8_t EEpGetAutoReportState(void);
uint8_t EEpGetLowBatState(void);
void EEpSetLowBatFlag(uint8_t flag);
void EEpSetHeartbeat(uint32_t heartbeat);
void EEpSetWakeupCnt(uint32_t wkcnt);
uint32_t EEpGetHeartbeat(void );
uint32_t EEpGetWakeupCnt(void);

uint32_t EEpGetDemoCnt(void );
void EEpSetDemoCnt(uint8_t cnt);
uint32_t EEpGetRamLRptTime(void);
uint32_t EEpGetRamLRptNum(void);
uint8_t EEpGetJtagState(void);
void EEpSetJtag(uint8_t jtag);
#ifdef ABACUSLEDER_SUPPORT
void RamShift(uint32_t *buf,uint16_t size);
void AbacusLederInit(void);
void AbacusLederProc(void);
#endif
#endif

