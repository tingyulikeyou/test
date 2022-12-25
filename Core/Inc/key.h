
#ifndef __KEY_H__
#define __KEY_H__


#define KEY_TIMER_COUNT  (uint32_t)(60*1000)	// 20ms


#define DEBOUNCE_TIME					(uint32_t)(200)	// 20ms
#define SHORT_PRESSED_TIME		(uint32_t)(500)	// 500ms
#define LONG_PRESSED_TIME			(uint32_t)(750)	// 750ms
#define HOLD_TIME							(uint32_t)(2000)	// 3s

#define EVENT_NONE	0x00
#define EVENT_CLICK	0x01
#define EVENT_DOWN	0x02
#define EVENT_UP	0x04
#define EVENT_HOLD	0x08
#define EVENT_PRESS	0x10

#define BTN_EVENT_IS(btn,event) (uint32_t)(((uint32_t)event<<8)|(uint32_t)btn)


enum
{
  SUBMENU_MIN,
  SUBMENU_MAX
};

enum
{
  KEY_NONE=0,
  KEY_POWER,
  KEY_ENTER,
  KEY_PREV,
  KEY_NEXT,
  KEY_AC,
  KEY_DC,
  KEY_USB
};

enum
{
  ROLL_IDLE_INIT,	
  ROLL_IDLE_LOW,
  ROLL_IDLE_HIGH,
  ROLL_RUN_LEFT,
  ROLL_RUN_RIGHT,
  ROLL_RUN_LOW,
  ROLL_RUN_HIGH
};

typedef struct
{
	uint8_t pos;
	uint8_t len;
	uint8_t key[30];
}KEY_INPUT_TypeDef;



uint8_t KeyboardPlugIn(void);
void  KeyboardUsbPwrSet(uint8_t enable);

void  KeyInit(void);
uint8_t  IsKeyEvent(void);
void RollSwitchScan(void);
void KeyScanTimer(void);
void KeyScan(void);
void KeyEventProcess(void);
void KeyEventStbyProcess(void);
uint8_t GetUserAcState(void);
uint8_t GetUserDcState(void);
uint8_t GetUserUsbState(void);
uint8_t GetDemoState(void);
void  DemoModeClose(void);
void  DemoModeStart(void);

#endif




