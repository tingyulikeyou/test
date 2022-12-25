
#ifndef __ADC_H__
#define __ADC_H__

#define SIMPLE_NUM  30

typedef struct
{
	#ifdef UI1K_V13_PROJECT
	uint16_t dc12v_adc;
	#else
	uint16_t bat_adc;
	#endif
	uint16_t verf_adc;
	#ifdef PUMP_PROJECT
	uint16_t temp_adc;
	#endif
	#ifdef UI1K_V13_PROJECT
	uint16_t diany_adc;
	uint16_t pv_0c_adc;
	uint16_t usb_in_adc;
	#endif
	
}ADC_SIMPLE_TypeDef;

void AdcInit(void);

uint16_t  AdcGetVerfIntAvgVolt(uint32_t adc,uint8_t size );
uint16_t  AdcGetBatAvgVolt(uint32_t adc,uint8_t size );
void AdcProc(void);

uint8_t AdcDianyanOverState(void);
uint8_t AdcDc12vOverState(void);
uint16_t AdcGetBatVolt(void);
uint8_t AdcPvChrgOverState(void);
uint8_t AdcGetChrgeState(void);
void AdcDetectEnable(void);

#ifdef UI1K_V13_PROJECT
uint16_t  AdcGetBmsAvgVolt(uint32_t adc,uint8_t size );
uint16_t  AdcGetDcCurrentAvgVolt(uint32_t adc,uint8_t size );

uint16_t AdcGetBmsVolt(void);
uint16_t AdcGetChrgeCurrent(void);   //unit mA
#endif
#endif

