/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

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

#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f10x_eval.h"



UART_HandleTypeDef huart1;//BLE
UART_HandleTypeDef huart2;//4G
UART_HandleTypeDef huart3;//485	DI
UART_HandleTypeDef huart4;//GPS
UART_HandleTypeDef huart5;//UART





/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/


void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal){

        if(timingdelaylocal < 500U){
            gd_eval_led_on(LED2);
        }else{
            gd_eval_led_off(LED2);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 1000U;
    }
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

void Uart1Init(void);
void Uart2Init(void);
void Uart3Init(void);
void Uart4Init(void);
void Uart5Init(void);

void UartConfig(uint32_t uart_no,uint32_t bandrate);

int main(void)
{
    /* configure systick */
	//SystemInit();

    //systick_config();
    /* initilize the LEDs, USART and key */
   // gd_eval_led_init(LED2); 
   // gd_eval_led_init(LED3); 
   // gd_eval_led_init(LED4);
    //gd_eval_com_init(EVAL_COM0);
    //gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);
	//

	Uart1Init(); //BLE
	Uart2Init(); //4G
	Uart3Init(); //485  DI
	Uart4Init(); //GPS
	Uart5Init(); //UART

	

	
#ifdef PUMP_PROJECT
	 HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);  //debug
	 HAL_Delay(2000);
 #else
	  //HAL_Delay(2000);
 #endif
	 EEpInit();
	
	 LcdInit();
	 TimerInit();
	
 #ifdef BMS_309_SUPPORT
	 bms309Init();
 #endif
	 
 #ifdef CAMP_PROJECT
	 CampInit();
 #endif
	 
	 GsmComInit();
	 GattInit();
 #if defined(UI1K_V2_PROJECT)||defined(P10KW_PROJECT)
	 MenuInit();
	 KeyInit();
	
	 //TimerKbInsertSet(2*60*1000);
 #endif
	
	 
 #if defined(UI1K_V2_PROJECT)||defined(PUMP_PROJECT)||defined( E_MOB48V_PROJECT)||defined( P10KW_PROJECT)
	 
	 HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
	 
	 HAL_Delay(200);
	 PaygInit();
	 KeyBoardInit();
 #endif
	  AdcInit();
	
 #ifdef PUMP_PROJECT
	  PumInit();
 #endif
	
	 debug_printf("main","system reset",0);
	
 #ifdef GPS_SUPPORT
	 //GpsInit();
 #endif
	
	 // HAL_Delay(500);
	 // HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_SET);
#ifdef UI1K_V2_PROJECT
	
	/* while(i<60)
	 {
	   if(HAL_GPIO_ReadPin(PWR_KEY_GPIO_Port,PWR_KEY_Pin)==GPIO_PIN_RESET)
		   continue;
		HAL_Delay(50);
	   i++;
	   }*/
	 HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
	 //HAL_GPIO_WritePin(BL_CTRL_GPIO_Port, BL_CTRL_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_SET);
	
	 TimeBacklightSet();
	 HAL_Delay(1000);
 #endif
 #ifdef UI1K_V13_PROJECT
	 CoulomInit();
 #endif
 #ifdef BMS_SMARTLI_SUPPROT
	 SmartBmsInit();
 #endif
 #ifdef BMS_JBD_SUPPROT
	 JbsBmsInit();
 #endif
	
 #ifdef DC_PUMP_SUPPORT
	 TimeBacklightSet();
 #endif
	
 #ifdef P10KW_PROJECT
	 TimeBacklightSet();
	 OffGridBmsInit();
 #endif
	
	 
	 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_SET);

    
    /* print out the clock frequency of system, AHB, APB1 and APB2 */
   // printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
   // printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
   // printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
   // printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));

    while(1){
				/* USER CODE END WHILE */
	  	#ifdef WDG_ENABLE
				HAL_IWDG_Refresh(&hiwdg);
		#endif
		
				/* USER CODE BEGIN 3 */
				 GsmComProc();
	     #ifdef PUMP_PROJECT
				  AdcProc();		 
				  PumpProc();
				  PKeybordProc();
				  PaygProcess(); 
				  //HAL_Delay(1000);
		  #ifdef DC_PUMP_SUPPORT
				  KeyEventProcess();
				  MenuShow();
		  #endif
	     #endif
				 
	      #ifdef CAMP_PROJECT 
				 AdcProc();   
				 CampGetTask();
	      #endif
			
		  #ifdef P10KW_PROJECT
				  AdcProc();
				  PaygProcess(); 
				  PKeybordProc();
				  
				  KeyEventProcess();
				  MenuShow();
			
		  #ifdef BMS_OFFGRID_SUPPROT
				  OffGridBmsTask(); 
		  #endif	
			
		  #ifdef BMS_JBD_SUPPROT
				  JbsBmsTask(); 	
		  #endif
		  #endif
			
		  #ifdef BMS_309_SUPPORT
				//  bms309Proc();
		  #endif
			
		  #ifdef E_MOB48V_PROJECT
				  AdcProc();
				  
				  PaygProcess(); 
				  PKeybordProc();
		  #ifndef BLE_MASTER_ENABLE
		  #ifdef BMS_SMARTLI_SUPPROT
				  SBmsTask();	
		  #endif
			
		  #ifdef BMS_JBD_SUPPROT
				  JbsBmsTask(); 	
		  #endif
		  #endif
		  #endif
			
				  
	     #ifdef  UI1K_V2_PROJECT  
				 AdcProc();  
				 
				 PaygProcess(); 
				 KeyEventProcess();
				 MenuShow();
				 PKeybordProc();
				 
	     #ifdef BMS_SMARTLI_SUPPROT
				  SBmsTask();
	     #else
	      #ifdef UI1K_V13_PROJECT
				   CoulomProc();
	      #else	  
				   BmsProcess();
	       #endif	  
	     #endif	 
	    #endif	 
			
				 EEpProcess();
			
				 if(TimerSleepState()
	 	#ifdef CAMP_PROJECT 
				||AdcGetBatVolt()<=10500u||GmsGetSimcardState()==FALSE
	 	#endif
		#if defined(E_MOB48V_PROJECT )||defined(P10KW_PROJECT)||defined( UI1K_V13_PROJECT)
				||GmsGetSimcardState()==FALSE
	 	#endif
					)
	    #if defined( UI1K_V13_PROJECT)||defined( DC_PUMP_SUPPORT)||defined( E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
					{
			
					if(TimerSleepState())
					{	 EEpSetWakeupCnt(EEpGetWakeupCnt()+1);
					
						 TimerEventClear(TIMER_SLEEP);
						}
					 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
					 GmsSetNetConnectState(FALSE);	 
					 GsmComIdle();
			
			 #if defined( E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
						GmsSetSimcardState(TRUE);
			 #endif
			
					
			
					}
	    #else	
				 {
						GPIO_InitTypeDef GPIO_InitStruct = {0};
					
						HAL_ADC_Stop_DMA(&hadc);
					HAL_ADC_DeInit(&hadc);
			
					HAL_UART_DeInit(&huart1);
					HAL_UART_DeInit(&huart2);
					HAL_UART_DeInit(&huart3);
			
					  /*Configure GPIO pin : DS_SDA_Pin */
					  GPIO_InitStruct.Pin = DS_IO_Pin;
					  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
					  GPIO_InitStruct.Pull = GPIO_NOPULL;
					  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
					  HAL_GPIO_Init(DS_IO_GPIO_Port, &GPIO_InitStruct);
			
					  /*Configure GPIO pin : DS_SCLK_Pin */
					  GPIO_InitStruct.Pin = DS_SCLK_Pin;
					  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
					  GPIO_InitStruct.Pull = GPIO_NOPULL;
					  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
					  HAL_GPIO_Init(DS_SCLK_GPIO_Port, &GPIO_InitStruct);
			
					  /*Configure GPIO pin : DS_SCLK_Pin */
					  GPIO_InitStruct.Pin = DS_CE_Pin;
					  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
					  GPIO_InitStruct.Pull = GPIO_NOPULL;
					  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
					  HAL_GPIO_Init(DS_CE_GPIO_Port, &GPIO_InitStruct);
			
					//	HAL_GPIO_WritePin(GPIOB, DS_IO_Pin|DS_SCLK_Pin, GPIO_PIN_RESET);
					//	HAL_GPIO_WritePin(DS_CE_GPIO_Port, DS_CE_Pin, GPIO_PIN_RESET);
			
			#ifdef CAMP_PROJECT
					
					  GPIO_InitStruct.Pin = GPIO_PIN_10;
					  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
					  GPIO_InitStruct.Pull = GPIO_NOPULL;
						  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
					  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
					  /* EXTI interrupt init*/
					 HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
					 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
			 #endif
			
					  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, EEpGetSleepTime(), RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
					  {
						Error_Handler();
					  }
			
					 HAL_GPIO_WritePin(GSM_EN_GPIO_Port, GSM_EN_Pin, GPIO_PIN_RESET);
			
					 __HAL_RCC_DMA1_CLK_DISABLE();
					 __HAL_RCC_GPIOH_CLK_DISABLE();
					// __HAL_RCC_GPIOA_CLK_ENABLE();
					 __HAL_RCC_GPIOB_CLK_DISABLE();
			
			
					HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);	
			
					HAL_Delay(1);
												
					__disable_fault_irq(); 
					NVIC_SystemReset();
					}
		#endif

	    /*usart_data_transmit(USART0, 0xaa);
    	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));

		usart_data_transmit(USART1, 0xaa);
    	while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));

		usart_data_transmit(USART2, 0xaa);
    	while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));

		usart_data_transmit(UART3, 0xaa);
    	while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));

		usart_data_transmit(UART4, 0xaa);
    	while(RESET == usart_flag_get(UART4, USART_FLAG_TBE));

		
		
        if(RESET == gd_eval_key_state_get(KEY_WAKEUP)){
            gd_eval_led_on(LED3);
            delay_1ms(500);
            gd_eval_led_off(LED3);
            gd_eval_led_toggle(LED4);
        }*/
    }
}



void Uart1Init(void)
{
	/* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    /* configure COM0 */
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    UartConfig(USART0,115200U);

}

void Uart2Init(void)
{
	/* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* configure COM0 */
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    UartConfig(USART1,115200U);

}

void Uart3Init(void)
{
	/* USART interrupt configuration */
    nvic_irq_enable(USART2_IRQn, 0, 0);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART2);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    UartConfig(USART2,115200U);

}

void Uart4Init(void)
{
	/* USART interrupt configuration */
    nvic_irq_enable(UART3_IRQn, 0, 0);
    /* configure COM0 */
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART3);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    UartConfig(UART3,115200U);

}

void Uart5Init(void)
{
	/* USART interrupt configuration */
    nvic_irq_enable(UART4_IRQn, 0, 0);
    /* configure COM0 */
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART4);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    UartConfig(UART4,115200U);

}





void UartConfig(uint32_t uart_no,uint32_t bandrate)
{
	/* USART configure */
	usart_deinit(uart_no);
	usart_baudrate_set(uart_no, bandrate);
	usart_word_length_set(uart_no, USART_WL_8BIT);
	usart_stop_bit_set(uart_no, USART_STB_1BIT);
	usart_parity_config(uart_no, USART_PM_NONE);
	usart_hardware_flow_rts_config(uart_no, USART_RTS_DISABLE);
	usart_hardware_flow_cts_config(uart_no, USART_CTS_DISABLE);
	usart_receive_config(uart_no, USART_RECEIVE_ENABLE);
	usart_transmit_config(uart_no, USART_TRANSMIT_ENABLE);
	usart_enable(uart_no);

	/* enable USART TBE interrupt */  
	//usart_interrupt_enable(uart_no, USART_INT_FLAG_RBNE);

}


void Uart1Send(uint8_t *buffer,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		 usart_data_transmit(USART0, buffer[i]);
   		 while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
		}
}

void Uart2Send(uint8_t *buffer,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		 usart_data_transmit(USART1, buffer[i]);
   		 while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
		}
}

void Uart3Send(uint8_t *buffer,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		 usart_data_transmit(USART2, buffer[i]);
   		 while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
		}
}

void Uart4Send(uint8_t *buffer,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		 usart_data_transmit(UART3, buffer[i]);
   		 while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));
		}
}


void Uart5Send(uint8_t *buffer,uint16_t size)
{
	uint16_t i;
	
	for(i=0;i<size;i++)
	{
		 usart_data_transmit(UART4, buffer[i]);
   		 while(RESET == usart_flag_get(UART4, USART_FLAG_TBE));
		}
}





/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));

    return ch;
}
