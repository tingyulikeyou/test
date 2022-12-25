
#ifndef __MENU_H__
#define __MENU_H__

#ifdef DC_PUMP_SUPPORT
  enum
 {
	 MENU_SPEED,  
	 MENU_POWER,  
	 MENU_VOLTAGE,
	 MENU_RM_DAY,
	 MENU_NONE
 };

#else
 enum
{
	MENU_IDLE,	
	MENU_LOGO,	
	MENU_CHARGE,
	MENU_BATTERY,
	MENU_TIME,
	MENU_BLE,
	MENU_PAYG,
	MENU_POWER,
	MENU_PAYG_INPUT,
	MENU_NONE
};
#endif	

enum
{
	IDLE_ITEM_LOGO,
	IDLE_ITEM_CHARGE,
	IDLE_TTEM_BATTERY,
	IDLE_ITEM_TIME,
	IDLE_ITEM_BLE,
	IDLE_ITEM_PAYG,
	IDLE_ITEM_POWER
};

enum
{
	LOGO_ITEM_VOLT,
	LOGO_ITEM_SOC,
	LOGO_TTEM_PAYG,
	LOGO_ITEM_CHRGEFULL_TIME,
	LOGO_ITEM_REMAIN_TIME,
	LOGO_ITEM_PPID,
	LOGO_ITEM_ESC
};

enum
{
	CHRGE_ITEM_CAP, //W
	CHRGE_ITEM_CHRGEFULL_TIME,
	CHRGE_ITEM_VOLT,
	CHRGE_ITEM_CURRENT,
	CHRGE_ITEM_ESC
};

enum
{
	BAT_ITEM_SOC,
	BAT_ITEM_REMAIN_CAP,
	BAT_ITEM_TEMP,
	BAT_ITEM_REMAIN_TIME,
	BAT_ITEM_VOLTAGE,
	BAT_ITEM_CURRENT,
	BAT_ITEM_CYC_TIME,
	BAT_ITEM_ESC
};

enum
{
	TIME_ITEM_CHRGE_FULL,
	TIME_ITEM_RECHRGE_EMPTY,
	TIME_ITEM_RUN_TIME,
	TIME_ITEM_TOTAL_RUN_TIME,
	TIME_ITEM_ESC
};


enum
{
	BLE_ITEM_ON, 
	BLE_ITEM_OFF,
	BLE_ITEM_ESC
};

enum
{
	PAYG_ITEM_REMAIN_DAYS, 
	//PAYG_ITEM_INPUT,
	PAYG_ITEM_FEEDBACK,
	PAYG_ITEM_ESC
};

enum
{
#ifdef ROLL_SWITCH
	PWR_ITEM_AC, 
	PWR_ITEM_DC, 
	PWR_ITEM_USB, 
	PWR_ITEM_OFF,
	PWR_ITEM_ESC,
#else
	PWR_ITEM_ON, 
	PWR_ITEM_OFF,
	PWR_ITEM_ESC,
#endif	
};

enum
{
	POP_MENU_NONE, 
	POP_MENU_AC,
	POP_MENU_DC,
	POP_MENU_USB
};



typedef struct
{
    uint8_t menu;
    uint8_t submenu;
    uint8_t pop_menu:4;	
    uint8_t pop_cnt:4;		
    uint8_t saver:1;
    uint8_t flash:1;	
    uint8_t saver_index:6;	
}UI_MENU_TypeDef;


void TimeOutSet(void);
void TimeOutTimer(void);
void MenuSaverSet(uint8_t enable);
void MenuSaverUpdate(void);
void MenuFlashSet(uint8_t flash);
uint8_t  MenuGetFlashState(void);
void MenuPopSet(uint8_t id);

void MenuInit(void);
void MenuShow(void);
void MenuRefresh(void);
#ifndef DC_PUMP_SUPPORT
void MenuShowIdle(uint8_t submenu,uint8_t *tempStr);
void MenuShowLogo(uint8_t submenu,uint8_t *tempStr);
void MenuShowcharge(uint8_t submenu,uint8_t *tempStr);
void MenuShowTime(uint8_t submenu,uint8_t *tempStr);
void MenuShowBle(uint8_t submenu,uint8_t *tempStr);
void MenuShowPayg(uint8_t submenu,uint8_t *tempStr);
void MenuShowPower(uint8_t submenu,uint8_t *tempStr);
void MenuShowBattery(uint8_t submenu,uint8_t *tempStr);
void MenuShowPaygInput(uint8_t submenu,uint8_t *tempStr);
#endif

#endif



