/*!
    \file    i2c.c
    \brief   I2C configuration file

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

#include "gd32f10x.h"
#include "main.h"

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIO_I2C);

    /* connect PB6 to I2C_SCL */
    /* connect PB7 to I2C_SDA */
    gpio_init(I2C_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
}

/*!
    \brief      configure the I2CX interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void)
{
    /* enable I2C clock */
    rcu_periph_clock_enable(RCU_I2C);
    /* configure I2C clock */
    i2c_clock_config(I2CX, I2C_SPEED, I2C_DTCY_2);
    /* configure I2C address */
    i2c_mode_addr_config(I2CX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2CX_SLAVE_ADDRESS7);
    /* enable I2CX */
    i2c_enable(I2CX);
    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
}

/*!
    \brief      reset i2c bus
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_bus_reset()
{
    i2c_deinit(I2CX);
    /* configure SDA/SCL for GPIO */
    GPIO_BC(I2C_SCL_PORT) |= I2C_SCL_PIN;
    GPIO_BC(I2C_SDA_PORT) |= I2C_SDA_PIN;
    gpio_init(I2C_SCL_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SCL_PORT) |= I2C_SCL_PIN;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(I2C_SDA_PORT) |= I2C_SDA_PIN;
    /* connect I2C_SCL_PIN to I2C_SCL */
    /* connect I2C_SDA_PIN to I2C_SDA */
    gpio_init(I2C_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN);
    gpio_init(I2C_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, I2C_SDA_PIN);
    /* configure the I2CX interface */
    i2c_config();
}


 /**
  ******************************************************************************
  * @file  I2c.c
  * @author  Langaopeng
  * @version  --
  * @date  03/19/2011
  * @brief  This file provides all the I2c firmware functions.
  *****************************************************************************/

/*******************************************************************************
* Function Name  : I2c_Delay2us
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


void I2c_Delay2us(void)// 2us
{
        uint8_t i;
        for(i=0;i<50;i++);
}


 
uint8_t I2c_Send(uint8_t bValue) 
{
 
  uint8_t bBitMask = 0x80;
  uint16_t delay_i2c=1000;
 
// step 1 : 8-bit data transmission


  while(bBitMask)
  {
    if(bBitMask & bValue)
    {
      P_SET_SDA();
    }
    else
    {
      P_CLR_SDA();
    }

   I2c_Delay2us();

   I2c_Delay2us();
    P_SET_SCL();                    // data clock in
    I2c_Delay2us();

    P_CLR_SCL();                    // ready for next clock in

  I2c_Delay2us();
    bBitMask = bBitMask >> 1;   // MSB first & timing delay
  }

  // step 2 : slave acknowledge check
  P_SET_SDA();                      // release SDA for ACK polling
  I2c_Delay2us();

  P_SET_SCL();                      // start ACK polling
  bBitMask = ACK_DELAY;         // time out protection

  I2c_Delay2us();
  

  while(P_SDA() && --delay_i2c)
  {
    ;  // wait for ACK, SDA=0 or bitMask=0->jump to this loop
  }
 
  I2c_Delay2us();

   // timing delay 2us
  P_CLR_SCL();                     // end ACK polling
  I2c_Delay2us();

  if(delay_i2c)
  {
    return(TRUE);              // return TRUE if ACK detected
  }
  else
  {
    return(FALSE);             // return FALSE if time out
  }
}

/************************************************************************
     Function : void vI2CRead(uint8_t *prValue, BOOL fgSeq_Read)
  Description : Read Routine
                timing : SCL ___|^|___|^|__~__|^|___|^|__
                         SDA __/D7 \_/D6 \_~_/D0 \_/ACK\_
    Parameter : *prValue(8-bit input pointer value)
    Return    : NONE
************************************************************************/
void I2c_Read(uint8_t *prValue, uint8_t fgSeq_Read) 
{
  uint8_t bBitMask = 0x80;


  *prValue = 0;                 // reset data buffer
  P_SET_SDA();                      // make sure SDA released
  I2c_Delay2us();


   I2c_Delay2us();
   
// step 1 : 8-bit data reception
  while(bBitMask)
  {
    P_SET_SCL();                    // data clock out
    I2c_Delay2us();

    if(P_SDA())
    {
      *prValue = *prValue | bBitMask;   // Get all data
    }                                   // non-zero bits to buffer


    P_CLR_SCL();                            // ready for next clock out
    I2c_Delay2us();

    bBitMask = bBitMask >> 1;           // shift bit mask & clock delay
  }


  I2c_Delay2us();

// step 2 : acknowledgement to slave
  if(fgSeq_Read)
  {
    P_CLR_SDA();                            // ACK here for Sequential Read
  }
  else
  {
    P_SET_SDA();                            // NACK here (for single uint8_t read)
  }

    I2c_Delay2us();
  P_SET_SCL();                    // NACK clock out
    I2c_Delay2us();
  P_CLR_SCL();                    // ready for next clock out
  I2c_Delay2us();
  P_SET_SDA();                    // release SDA
   I2c_Delay2us();

}

/************************************************************************
     Function : BOOL fgI2CStart(uint8_t bValue, BOOL fgRead)
  Description : Start Routine
                timing : SCL ^^^^|___|^|___|^|__~__|^|___|^|___|^|__~
                         SDA ^^|____/A6 \_/A5 \_~_/A0 \_/R/W\_/ACK\_~
                              (S)
                               |<--- start condition
    Parameter : bDevice(7-bit slave address) + fgRead(R/W bit)
    Return    : TRUE  : successful with ACK from slave
                FALSE  : bus (SCL = 0) or ACK failure
************************************************************************/
uint8_t I2c_Start(uint8_t bDevice) 
{
  uint16_t bBusDelayTemp = 1000;


			  
  P_SET_SDA();            // make sure SDA released
  I2c_Delay2us();

  P_SET_SCL();            // make sure SCL released
  I2c_Delay2us();


  
  while((!P_SCL()) && (--bBusDelayTemp))
  {
    ;  // simple bus abritration
  }

 I2c_Delay2us();
 
  if(!bBusDelayTemp)
  {
    return(FALSE);           // time out protection & timing delay
  }


  P_CLR_SDA();          // start condition here
  I2c_Delay2us();
  P_CLR_SCL();          // ready for clocking
  I2c_Delay2us();

  return(I2c_Send(bDevice));// slave address & R/W transmission
}

/************************************************************************
     Function : void vI2CStop(void)
  Description : Stop Routine
                timing : SCL ___|^^^^^
                         SDA xx___|^^^
                                 (P)
                                  |<--- stop condition
    Parameter : NONE
    Return    : NONE
************************************************************************/
void I2c_Stop(void) 
{
  P_CLR_SDA();          // ready for stop condition
  I2c_Delay2us();
  P_SET_SCL();          // ready for stop condition
  I2c_Delay2us();
  P_SET_SDA();          // stop condition here
 I2c_Delay2us();

}


uint8_t I2c_PageWrite(uint8_t bDevice, uint16_t bData_Addr,uint16_t bDataCount, uint8_t *prData)            
{

  //bDevice = bDevice << 1;      // Shift the 7-bit address to 7+1 format

  if(!I2c_Start(bDevice))     // Write Command
  {
  

    I2c_Stop();
    return(FALSE);             // Device Address exceeds the range
  }

  if(!I2c_Send(bData_Addr>>8))   // Word Address
  {
    I2c_Stop();
    return(FALSE);             // Device Address exceeds the range
  }

  if(!I2c_Send(bData_Addr))   // Word Address
  {
    I2c_Stop();
    return(FALSE);             // Device Address exceeds the range
  }

  while(bDataCount)
  {
    if(!I2c_Send(*(prData++))) // Data Content Write
    { 
      I2c_Stop();
      return(FALSE);            // Device Address exceeds the range
    }
    bDataCount--;
  }

  I2c_Stop();
  return(TRUE);
}

/************************************************************************
     Function : BOOL fgI2CDataRead(uint8_t bDevice, uint8_t bData_Addr,
                                    uint8_t bDataCount, uint8_t *prData)
  Description : DataRead Routine
    Parameter : bDevice -> Device Address
                bData_Addr -> Data Address
                bDataCount -> Data Content Cont
                *prData -> Data Content Pointer
    Return    : TRUE  : successful with ACK from slave
                FALSE  : bus (SCL = 0) or ACK failure
************************************************************************/
uint8_t I2c_PageRead(uint8_t bDevice, uint16_t bData_Addr, uint16_t bDataCount,uint8_t *prData) 
{

  {
    //bDevice = bDevice << 1;   // Shift the 7-bit address to 7+1 format

    if(!I2c_Start(bDevice))  // Write Command
    {
    
      I2c_Stop();
      return(FALSE);          // Start fail
    }

    if(!I2c_Send(bData_Addr>>8))// Word Address
    {
      I2c_Stop();
      return(FALSE);          // Data Address Fail
    }

	if(!I2c_Send(bData_Addr))// Word Address
    {
      I2c_Stop();
      return(FALSE);          // Data Address Fail
    }

    // Step 2 : Real Read
    bDevice = bDevice + 1;    // Shift the 7-bit address to 7+1 format

    if(!I2c_Start(bDevice))  // Read Command
    {
      I2c_Stop();
      return(FALSE);          // Start fail
    }
  }

  while (bDataCount)
  {
    if (bDataCount == 1)
    {
      I2c_Read(prData++, FG_RANDREAD);  // Data Content Read
    }
    else
    {
      I2c_Read(prData++, FG_SEQREAD);  // Data Content Read
    }
    bDataCount--;
  }

  // Step 3 : Stop
  I2c_Stop();


  return (TRUE);
}








