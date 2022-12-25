//lcd.h
#ifndef _holtek_h_
#define _holtek_h_



#ifdef DC_PUMP_SUPPORT
#define P_Strobe(fgFlag)  HAL_GPIO_WritePin(LCD_CSB5_GPIO_Port, LCD_CSB5_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);
#define P_Clk(fgFlag)    HAL_GPIO_WritePin(LCD_CLKB8_GPIO_Port, LCD_CLKB8_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);
#define P_Data(fgFlag)  HAL_GPIO_WritePin(LCD_DATB9_GPIO_Port, LCD_DATB9_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);

#else

#define P_Strobe(fgFlag)  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);
#define P_Clk(fgFlag)    HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);
#define P_Data(fgFlag)  HAL_GPIO_WritePin(LCD_DAT_GPIO_Port, LCD_DAT_Pin, fgFlag?GPIO_PIN_SET:GPIO_PIN_RESET);
#endif

#define LCD_CMD               0x04
#define LCD_WRITE             0x05
#define LCD_READ              0x06

#define LCD_SYS_DIS           0x00
#define LCD_SYS_EN            0x01
#define LCD_OFF               0x02
#define LCD_ON                0x03
#define LCD_TIMER_DIS         0x04
#define LCD_WDT_DIS           0x05
#define LCD_TIMER_EN          0x06
#define LCD_WDT_EN            0x07
#define LCD_TONE_OFF          0x08
#define LCD_TONE_ON           0x09
#define LCD_CLR_TIMER         0x0c
#define LCD_CLR_WDT           0x0e
#define LCD_XTAL_32K          0x14
#define LCD_RC_256K           0x18
#define LCD_EXT_256K          0x1c
#define LCD_BIAS12_COM2       0x20  // 1/2 bias, 2 commons
#define LCD_BIAS12_COM3       0x24  // 1/2 bias, 3 commons
#define LCD_BIAS12_COM4       0x28  // 1/2 bias, 4 commons
#define LCD_BIAS13_COM2       0x21  // 1/3 bias, 2 commons
#define LCD_BIAS13_COM3       0x25  // 1/3 bias, 3 commons
#define LCD_BIAS13_COM4       0x29  // 1/3 bias, 4 commons
#define LCD_TONE_4K           0x40
#define LCD_TONE_2K           0x60
#define LCD_IRQ_DIS           0x80
#define LCD_IRQ_EN            0x88
#define LCD_F1                0xa0
#define LCD_F2                0xa1
#define LCD_F4                0xa2
#define LCD_F8                0xa3
#define LCD_F16               0xa4
#define LCD_F32               0xa5
#define LCD_F64               0xa6
#define LCD_F128              0xa7
#define LCD_TOPT              0xe0
#define LCD_TNORMAL           0xe3

#ifdef DC_PUMP_SUPPORT
enum 
{
	ICON_P_4,
	ICON_P_3,
	ICON_P_2,
	ICON_P_1,
	ICON_TCTH,
	ICON_SOLAR,
	ICON_RUN,
	ICON_FAULT,
	ICON_WCWH,
	ICON_NONE
};

#else
enum
{
	ICON_BAT_FRAME,// 0%
	ICON_BAT_20,// 20%
	ICON_BAT_40,// 40%
	ICON_BAT_60,// 60%
	ICON_BAT_80,//80%
	ICON_CLOCK,// CLOCK
	ICON_BLE,// BLE
	ICON_WIFI,// WIFI
	ICON_UNLOCK,// UNLOCK
	ICON_CELSIUS,// C
	ICON_H,// H our
	ICON_h,// h our
	ICON_POWER,// POWER
	ICON_d,// d
	ICON_A,// A
	ICON_W,// W
	ICON_LOCK,// LOCK
	ICON_PRECENT,// %
	ICON_V,// V
	ICON_k,// k
	ICON_LINE,// LINE
	ICON_POINT,// point
	ICON_CHARGE,// charge
	ICON_LOGO,// eng
	ICON_NONE
};
#endif


/******************TM1621模块命令定义*********************/
#define SYSDIS   0x00	 //关系统振荡器和LCD偏压发生器	
#define SYSEN    0x02    //打开系统振荡器	

#define LCDOFF   0x04     //关LCD偏压
#define LCDON    0x06     //开LCD偏压

#define TONEON   0x12     //打开声音输出
#define TONEOFF  0x10     //关闭声音输出

#define XTAL     0x28     //外部接晶振								 
#define RC       0x30     //内部RC振荡	

#define BIAS     0x52    // //1/3偏压 4公共口	

#define WDTDIS   0x0a     //禁止看门狗
#define WDTEN    0x0e     //开启看门狗

 void LCDSendCom(uint8_t bVal, uint8_t bCnt) ;
 void LCDSendAddr(uint8_t Addr) ;
 void LCDSendData(uint8_t bData,uint8_t bCnt);




#define DIGIT_CURRENT_TEMP_0  0
#define DIGIT_CURRENT_TEMP_1 1
#define DIGIT_SET_TEMP_0     2
#define DIGIT_SET_TEMP_1    3
#define DIGIT_DELAY_0	4
#define DIGIT_DELAY_1	5
#define DIGIT_DELAY_2	6
#define DIGIT_DELAY_3	7
#define DIGIT_CLOCK_0     8
#define DIGIT_CLOCK_1     9
#define DIGIT_CLOCK_2     10
#define DIGIT_CLOCK_3     11
#define DIGIT_START         8
#define DIGIT_STOP           11
#define DIGIT_WATER_0     8
#define DIGIT_WATER_1     9
#define DIGIT_WATER_2      10


#define DIGIT_ON_CLOCK_0 4
#define DIGIT_ON_CLOCK_1 5
#define DIGIT_ON_CLOCK_2 6
#define DIGIT_ON_CLOCK_3 7


#define DOT_A      0x00
#define DOT_B      0x01
#define DOT_C      0x02
#define DOT_D      0x03
#define DOT_E      0x04
#define DOT_F      0x05
#define DOT_G      0x06
#define DOT_H      0x07

#define Line_Pos   0
#define Space_Pos  1
#define Number_Pos  2
#define Letter_Pos  12

#define SEG_SIZE  7

#define LCD_BUFFER_SIZE 32


#define DATA_SET_INC   0x40        // increamental address(to display memory)
#define DATA_SET_FIX   0x44       // fixed address(to display memory

#define ADDR_SET       0xC0

#define LED_CONTROL   0x8c
#define MODESET    0x01

#define LED_BUFFER_SIZE  9


void LcdInit(void);
void LcdClearAll(void);
void LcdSetIcon(uint8_t point,uint8_t enble);
void ShowStr(uint8_t *str);
void LcdShowChar(uint8_t pos,uint8_t str);
void LcdShowNumber(uint8_t pos,uint8_t number);
void LcdUpdateAll(void);

void LcdTestPoint(uint8_t addr,uint8_t test);
void LcdFillAll(void);

void LedWrite(uint8_t bTmp);
void LedWriteRam(uint8_t	 bAddr, uint8_t bData) ;
void LedUpdateAll(void);

#endif

