/*!
    \file    i2c.h
    \brief   the header file of I2C

    \version 2020-09-30, V1.0.0, firmware for GD32F10x
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

#ifndef I2C_H
#define I2C_H

#include "gd32f10x.h"

#define I2C_SPEED               100000
#define I2CX_SLAVE_ADDRESS7     0xA0
#define I2C_PAGE_SIZE           8

#define I2CX                    I2C0
#define RCU_GPIO_I2C            RCU_GPIOB
#define RCU_I2C                 RCU_I2C0
#define I2C_SCL_PORT            GPIOB
#define I2C_SDA_PORT            GPIOB
#define I2C_SCL_PIN             GPIO_PIN_6
#define I2C_SDA_PIN             GPIO_PIN_7

/* configure the GPIO ports */
void gpio_config(void);
/* configure the I2C0 interfaces */
void i2c_config(void);
/* reset i2c bus */
void i2c_bus_reset(void);

#define   P_SCL(a)     HAL_GPIO_ReadPin(I2C_SCL_PORT,I2C_SCL_PIN)
#define   P_SET_SCL(a)  HAL_GPIO_WritePin(I2C_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_SET);
#define   P_CLR_SCL(a)  HAL_GPIO_WritePin(I2C_SCL_PORT,I2C_SCL_PIN,GPIO_PIN_RESET);

#define   P_SDA(a)      HAL_GPIO_ReadPin(I2C_SDA_PORT,I2C_SDA_PIN)
#define   P_SET_SDA(a)   HAL_GPIO_WritePin(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_SET);
#define   P_CLR_SDA(a)   HAL_GPIO_WritePin(I2C_SDA_PORT,I2C_SDA_PIN,GPIO_PIN_RESET);



#define   FG_SEQREAD    1
#define   FG_RANDREAD   0

#define 	ACK_DELAY 200
#define 	BUS_DELAY  255 





void I2c_Read(uint8_t *prValue, uint8_t fgSeq_Read) ;
void I2c_Stop(void) ;

void I2c_Delay2us(void);
uint8_t I2c_Send(uint8_t bValue) ;
uint8_t I2c_Start(uint8_t bDevice) ;
uint8_t I2c_PageWrite(uint8_t bDevice, uint16_t bData_Addr,uint16_t bDataCount, uint8_t *prData) ;                   
uint8_t I2c_PageRead(uint8_t bDevice, uint16_t bData_Addr, uint16_t bDataCount,uint8_t *prData) ;


#endif  /* I2C_H */
