
#ifndef __TIMER_H__
#define __TIMER_H__


#define GPRS_UPLOAD_PRIOD   (EEpGetTransFreq())// (uint32_t)(3u*60000u)

#define SLEEP_PRIOD  (uint32_t)(2*60000u)


#define DEMO_PRIOD  (uint32_t)(15*60000u)


#define GPS_SAMPLE_TIME 15000u


#define COUNT_ADD 0
#define COUNT_SUB 1

#define HAL_MAX_DELAY      0xFFFFFFFFU


enum
{
	TIMER_UPLOAD,
	TIMER_CAMP,
	TIMER_PUMP,
	TIMER_PAYG,
	TIMER_BMS_COM,
	TIMER_AT_ACKOUT,
	TIMER_MQTT,
	TIMER_GSMCOM,
	TIMER_KEY,
	TIMER_SLEEP,
	TIMER_LCD_BL,
	TIMER_LCDSAVER,
	TIMER_KB_INSERT,
	TIMER_UI_FLASH,
	TIMER_CL_FULL,
	TIMER_CL_CHRGE_OVER,
	TIMER_CL_CHRGE_LOW,
	TIMER_SYS_INIT,
	TIMER_DEMO,
	TIMER_GPS_SAMPLE,
	TIMER_RAML_REPROT,
	TIMER_ABAC_SAMPLE,
	TIMER_COUNT
};

typedef struct
{
    uint8_t enable:1;
    uint8_t period:1;	
    uint8_t holdlock:1;
    uint8_t event:5;	
	
    uint8_t retry;	

    uint16_t reserved;
    uint32_t count;
}TIMER_TypeDef;

typedef struct
{
    uint16_t timer_id;
    uint16_t cnt_type;	
    TIMER_TypeDef *p;
    void  (*callback)(TIMER_TypeDef *p);
	
}TIMER_LIST_TypeDef;

void TimerInit(void);
void TimerAtTOutStart(uint32_t ms,uint8_t retry_clr);
void TimerAtTOutStop(void);
void TimerMqttHrbtSet(uint16_t ms);
void TimerGsmComSet(uint16_t delay);
void TimerGprsUploadCallback(TIMER_TypeDef*p);
void TimerCampCallback(TIMER_TypeDef *p);
void TimerPumpCallback(TIMER_TypeDef *p);
void TimerPaygCallback(TIMER_TypeDef *p);
void TimerBmsComCallback(TIMER_TypeDef *p);
void TimeBacklightCallback(TIMER_TypeDef *p);
void TimeSaverCallback(TIMER_TypeDef *p);
void TimeKbPwrHoldCallback(TIMER_TypeDef *p);
void TimeRamlReportCallback(TIMER_TypeDef *p);

void TimerKeyScan(TIMER_TypeDef *p);
void TimeBacklightSet(void);
void TimeUiFlashCallback(TIMER_TypeDef *p);
void TimeCoulomCallback(TIMER_TypeDef *p);
void TimerCoulomFullSet(uint16_t ms);
uint8_t GetTimerCoulomFullState(void);
uint8_t GetTimerCoulomFullEvent(void);
void TimeCoulomChrgeOverCallback(TIMER_TypeDef *p);
void TimerCoulomChrgeOverSet(uint16_t ms);
void TimerCoulomChrgeOverStop(void);

uint8_t GetTimerCoulomChrgeLowState(void);
void TimerCoulomChrgeLowSet(uint16_t ms);
void TimerCoulomChrgeLowStop(void);
uint8_t GetTimerCoulomChrgeLowEvent(void);
uint8_t GetTimerCoulomChrgeOverEvent(void);
void TimerCoulomStop(void);

uint8_t  TimerSleepState(void);
void TimerKbPwrHoldSet(uint32_t delay);
void TimerDemoSet(void);
void  TimerEventClear(uint8_t timer);
uint8_t GetTimerDemoEvent(void);
void  TimerSleepSet(void);
uint8_t  GetTimerGsmComEvent(void);
uint8_t  TimerGetEventState(uint8_t timer);
void  TimerSet(uint8_t timer,uint32_t count);


uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);
void HAL_SYSTICK_Callback(void);
void TimeAbacSampleCallback(TIMER_TypeDef *p);


#endif

