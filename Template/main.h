/*!
    \file    main.h
    \brief   the header file of main

    \version 2014-12-26, V1.0.0, firmware for GD32F10x
    \version 2017-06-20, V2.0.0, firmware for GD32F10x
    \version 2018-07-31, V2.1.0, firmware for GD32F10x
    \version 2020-09-30, V2.2.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef MAIN_H
#define MAIN_H

#include "gd32f10x.h"

#include "stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include "string.h"


//#define PUMP_PROJECT
//#define DC_PUMP_SUPPORT 
//#define CAMP_PROJECT
//#define UI1K_V2_PROJECT
//#define UI1K_V13_PROJECT
//#define P10KW_PROJECT    //5KW  10KW
#define E_MOB48V_PROJECT

//bms  


#ifdef P10KW_PROJECT
#define BMS_OFFGRID_SUPPROT
#elif defined(E_MOB48V_PROJECT)
//#define BMS_SMARTLI_SUPPROT
#define BMS_JBD_SUPPROT
//#define BMS_JBD_14_SUPPORT

#ifdef BMS_JBD_14_SUPPORT
#define BMS_JBD_SUPPROT
#endif
#endif

//others 
#define GPS_SUPPORT
#define E_SIM_SUPPORT 
#define ROLL_SWITCH
#define MODULE_4G

#define ABACUSLEDER_SUPPORT

#define DEBUG_AT_LOG

//#define DUBUG_LOG 
//#define PUMP_TEST


#define CAN0_USED
//#define CAN1_USED

#ifdef  CAN0_USED
    #define CANX CAN0
#else 
    #define CANX CAN1
#endif



#include"gatt.h"
#include"payg.h"
#include "eeprom.h"
#include "gsmcom.h"
#include "timer.h"
#include "atcmd.h"
#include "camp.h"
#include "mqtt.h"
#include "menu.h"
#include "key.h"
#include "lcd.h"
#include "pump.h"
#include "Keyboard.h"
#include "DS1302.h"
#include "BQ40Z50.h"
#include "SmartBms.h"
#include "string.h"
#include "adc.h"
#include "temp.h"
#include "coulom.h"
#include "ble.h"
#include "OffGrid.h"
#include "Jbsbms.h"

#include "i2c.h"
#include "at24cxx.h"


#ifdef GPS_SUPPORT
#include "gps.h"
#endif

#ifdef BMS_309_SUPPORT
#include "bms309proc.h"
#endif

#define DS_SCLK_Pin GPIO_PIN_13
#define DS_SCLK_GPIO_Port GPIOC
#define DS_IO_Pin GPIO_PIN_14
#define DS_IO_GPIO_Port GPIOC
#define DS_CE_Pin GPIO_PIN_15
#define DS_CE_GPIO_Port GPIOC
#define GSM_EN_Pin GPIO_PIN_0
#define GSM_EN_GPIO_Port GPIOA
#define GSM_RST_Pin GPIO_PIN_1
#define GSM_RST_GPIO_Port GPIOA
#define GSM_PWR_Pin GPIO_PIN_4
#define GSM_PWR_GPIO_Port GPIOC
#define CAN_INH_Pin GPIO_PIN_7
#define CAN_INH_GPIO_Port GPIOC
#define CAN_EN_Pin GPIO_PIN_8
#define CAN_EN_GPIO_Port GPIOC
#define GPS_PWR_Pin GPIO_PIN_15
#define GPS_PWR_GPIO_Port GPIOA
#define BAT_PWR_Pin GPIO_PIN_8
#define BAT_PWR_GPIO_Port GPIOA

#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOA
#define LCD_CLK_Pin GPIO_PIN_5
#define LCD_CLK_GPIO_Port GPIOA
#define LCD_DAT_Pin GPIO_PIN_7
#define LCD_DAT_GPIO_Port GPIOA
#define BL_CTRL_Pin GPIO_PIN_6
#define BL_CTRL_GPIO_Port GPIOA
#define BLE_RESET_Pin GPIO_PIN_1
#define BLE_RESET_GPIO_Port GPIOB
#define BLE_CS_Pin GPIO_PIN_2
#define BLE_CS_GPIO_Port GPIOB
#define ENTER_KEY_Pin GPIO_PIN_1
#define ENTER_KEY_GPIO_Port GPIOB
#define SW_A_Pin GPIO_PIN_5
#define SW_A_GPIO_Port GPIOC
//#define SW_A_EXTI_IRQn EXTI15_10_IRQn
#define SW_B_Pin GPIO_PIN_0
#define SW_B_GPIO_Port GPIOB
//#define SW_B_EXTI_IRQn EXTI15_10_IRQn
#define PWR_KEY_Pin GPIO_PIN_14
#define PWR_KEY_GPIO_Port GPIOB
#define PWR_CTRL_Pin GPIO_PIN_13
#define PWR_CTRL_GPIO_Port GPIOB
#define UART_SEL_Pin GPIO_PIN_11
#define UART_SEL_GPIO_Port GPIOA
#define KEY_ENTER_Pin GPIO_PIN_15
#define KEY_ENTER_GPIO_Port GPIOA
#define KEY_UP_Pin GPIO_PIN_3
#define KEY_UP_GPIO_Port GPIOB
#define KEY_DOWN_Pin GPIO_PIN_4
#define KEY_DOWN_GPIO_Port GPIOB
#define LCD_CSB5_Pin GPIO_PIN_5
#define LCD_CSB5_GPIO_Port GPIOB
#define LCD_CLKB8_Pin GPIO_PIN_8
#define LCD_CLKB8_GPIO_Port GPIOB
#define LCD_DATB9_Pin GPIO_PIN_9
#define LCD_DATB9_GPIO_Port GPIOB

#define RELAY_EN_Pin GPIO_PIN_1
#define RELAY_EN_GPIO_Port GPIOA



#define GPIO_PIN_SET  SET
#define GPIO_PIN_RESET  RESET
#define  HAL_GPIO_WritePin(port,pin,value)     gpio_bit_write(port, pin, value)
#define HAL_GPIO_ReadPin(port,pin)   gpio_input_bit_get(port,pin)

typedef struct
{
	uint8_t *pRxBuffPtr;		/*!< Pointer to UART Rx transfer Buffer */
	
	__IO uint16_t RxXferSize;		/*!< UART Rx Transfer size				*/
	
	__IO uint16_t RxXferCount;		/*!< UART Rx Transfer Counter			*/

}UART_HandleTypeDef;

void Uart1Send(uint8_t *buffer,uint16_t size);
void Uart2Send(uint8_t *buffer,uint16_t size);
void Uart3Send(uint8_t *buffer,uint16_t size);
void Uart4Send(uint8_t *buffer,uint16_t size);
void Uart5Send(uint8_t *buffer,uint16_t size);





/* led spark function */
void led_spark(void);

#endif /* MAIN_H */
