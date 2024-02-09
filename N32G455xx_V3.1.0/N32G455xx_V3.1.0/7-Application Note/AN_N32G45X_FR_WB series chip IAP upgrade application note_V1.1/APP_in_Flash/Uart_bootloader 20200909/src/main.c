/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file main.c
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "n32g4fr_rcc.h"
#include "string.h"
#include "bsp_usart.h"
#include "iap.h"
#include "string.h"




#define app_update_flag_addr 0x08005800 //The address must exceed the content range of the APP. The bin content of the demo ends at 0x080054F8


uint32_t receive_cnt = 0; // Used to count the length of uart received data
uint8_t receive_app_done = 0; // Used to mark the completion of uart data reception
uint8_t f_IAP_flashing = 0;
uint8_t f_IAP_start = 0;
uint16_t slot_timer = 0;
uint8_t f_receive_frame = 0;
uint8_t f_final_frame = 0;

extern uint8_t RX_buf[];
extern uint8_t pages_number;


void IAP_UPDATE_APP(void);
int32_t app_flag_write(uint32_t data ,uint32_t start_add);






/**============================================== =================
        Delay function
================================================== ==============*/
void m_delay_ms(uint32_t ms)
{
	uint32_t us = 0;
	
	for(; ms!=0; ms--)
	{
		us = 1440;
		while(us--);
	}
	
}
/**============================================== =================
        Read Flash
================================================== ==============*/
uint32_t FLASH_ReadWord(uint32_t address)
{
  return *(__IO uint32_t*)address; 
}
/**
 * @brief  Timer3 initialization configuration.
 */
void tim3_init(uint16_t Period, uint16_t Prescaler)
{
    TIM_TimeBaseInitType timInitStruct;
    NVIC_InitType NVIC_InitStructure;

    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);

    timInitStruct.Period    = Period;
    timInitStruct.Prescaler = Prescaler;
    timInitStruct.ClkDiv    = 0;
    timInitStruct.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM3, &timInitStruct);

    TIM_ConfigInt(TIM3, TIM_INT_UPDATE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Enable(TIM3, ENABLE);
}
/**
  * @brief  TIM3 interrupt process function each 100us.Get the TIM2->CNT value, and Calculate the number of pulses by
  the difference of two times. Then take the delta pules into the buffer of last channel and toggle to new channel.
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetIntStatus(TIM3, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_UPDATE);
        
        //
        if(f_IAP_start==1) //Start IAP update
        {
            slot_timer++;
            if(slot_timer>=2000) //200ms no data is issued, clear RX_buf data
            {
                memset(RX_buf,0,256);
                slot_timer = 0;
                receive_cnt = 0;
            }
        }
        else
        {
            slot_timer = 0;
        }
    }
}
/**================================================================
 * @brief  Main program.
 ================================================================*/
int main(void)
{
    tim3_init(99, 71); //72MH/(71+1)=1M Hz; 1M Hz/(99+1)=100us
    USART_Config();
    printf("NZ3601_init success! \r\n");
    while(1)
    {
        if(FLASH_ReadWord(app_update_flag_addr) == 0x12345678) //Whether the power-on detection needs to jump directly
        {
            receive_app_done = 1;
        }
        while(receive_app_done == 0) //No APP program, waiting to receive updates
        {
            if(f_IAP_flashing == 1)
            {
                TIM_Enable(TIM3, DISABLE);
                USART_Enable(DEBUG_USARTx, DISABLE);
                //
                IAP_UPDATE_APP(); //Update the received pack package
                f_IAP_flashing = 0;
                f_receive_frame = 0; //Clear the receive frame flag
                if(f_final_frame == 1)
                {
                    f_final_frame = 0;
                    receive_app_done = 1; //Update is complete
                    app_flag_write(0x12345678 ,app_update_flag_addr);//Write IAP upgrade flag
                }
                TIM_Enable(TIM3, ENABLE);
                USART_Enable(DEBUG_USARTx, ENABLE);
            }
        }
        if(receive_app_done) //App has been updated
        {
            receive_app_done = 0;
            TIM_Enable(TIM3, DISABLE); //Turn off timer interrupt
            //
            printf("APP address:%x\r\n",(FLASH_START_ADDR));
            printf("Start to execute Flash user code!!\r\n");
			
			SCB->VTOR = FLASH_BASE | 0x4000;	//Set up interrupt vector table before jump
            iap_load_app(FLASH_START_ADDR); //Jump to the start address of the APP, during which it cannot be interrupted by other interrupts, otherwise the jump will fail
        }
    }
}

/**
 * @}
 */
