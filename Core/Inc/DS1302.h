#ifndef __DS1302_H__
#define __DS1302_H__


//#define PAYG_TEST

/*#define DS_RST_Pin GPIO_PIN_0
#define DS_RST_GPIO_Port GPIOA
#define DS_SDA_Pin GPIO_PIN_2
#define DS_SDA_GPIO_Port GPIOB
#define DS_SCLK_Pin GPIO_PIN_12
#define DS_SCLK_GPIO_Port GPIOB*/

#define DS1302_ERRCNT  6


#define DS1302_SEC_REG 0x80 
#define DS1302_MIN_REG 0x82 
#define DS1302_HR_REG 0x84 
#define DS1302_DATE_REG 0x86 
#define DS1302_MONTH_REG 0x88 
#define DS1302_DAY_REG 0x8a 
#define DS1302_YEAR_REG 0x8c 
#define DS1302_CONTROL_REG 0x8e 
#define DS1302_CHARGER_REG 0x90 
#define DS1302_CLKBURST_REG 0xbe

#define DS1302_SEC_REG_R 0x81 
#define DS1302_MIN_REG_R 0x83 
#define DS1302_HR_REG_R 0x85 
#define DS1302_DATE_REG_R 0x87 
#define DS1302_MONTH_REG_R 0x89 
#define DS1302_DAY_REG_R 0x8B 
#define DS1302_YEAR_REG_R 0x8D 

#define DS1302_RAM_REG 0xc0

typedef struct
{
  unsigned char year;
  unsigned char month;
  unsigned char day;
  unsigned char week;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  unsigned char reserved;
}DS1302_TIME_DEF;

typedef struct _DS1302_TIME
{
  unsigned char year;
  unsigned char month;
  unsigned char day;
  unsigned char week;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;

  unsigned char reserved;
  
}DS1302_TIME;

void InitDs1302(void);//用17年1月1日初始化DS1302
void Check_Ds1302Runs(void);
void ds1302_read_time(DS1302_TIME* time) ;
void ds1302_write_time(DS1302_TIME* time) ;
void SetSysErrorCode(uint8_t error);
uint8_t GetSysErrorCode(void);
unsigned int GetDs1302Day(void);
void Delay(__IO uint16_t nCount);

#endif

