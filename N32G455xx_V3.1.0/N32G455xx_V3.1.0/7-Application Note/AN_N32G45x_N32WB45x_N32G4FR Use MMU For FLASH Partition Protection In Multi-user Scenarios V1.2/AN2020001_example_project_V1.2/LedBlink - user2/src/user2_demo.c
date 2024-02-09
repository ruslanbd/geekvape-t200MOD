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
 * @file user2_demo.c
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "user2_demo.h"
#include <stdio.h>

/** @addtogroup N32G45X_StdPeriph_Template
 * @{
 */

uint32_t test_data __attribute__((section(".ARM.__at_0x20012100")));

/**
 * @brief  FLASH program.
 */
void Test_ProgramFlashWord(uint32_t Address, uint32_t Data)
{
    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Address));

    /* Unlocks the FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Erase */
    if (FLASH_COMPL == FLASH_EraseOnePage(Address & 0xFFFFF800))
    {
    }

    /* Program */
    if (FLASH_COMPL != FLASH_ProgramWord(Address, Data))
    {

    }       

    /* Locks the FLASH Program Erase Controller */
    FLASH_Lock();

    /* Check */
    if (Data != (*(__IO uint32_t *)(Address)))
    {
        printf("Flash Program Test Failed\r\n");
        
    }   
}

/**
 * @brief  Initialize the global variable.
 */
void Test_InitData(void)
{
    test_data = 0x01234567;
}

void Test_User2(void) __attribute__((section(".ARM.__at_0x08041000")));

/**
 * @brief USER2_Demo
 * 
 */
void Test_User2(void)
{
    /* USART Configuration */
    // USART_Configuration(); 
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rHello! Here is USER2 Example!\n\r");

    /* LED(PA9) Blinks */
    Test_LedBlink(GPIOA, GPIO_PIN_9);
    
    /* Initialize the global variable of USER2 */
    Test_InitData();

    /* Program USER2 FLASH */
    Test_ProgramFlashWord(0x08042000, ~test_data);
    
    /* Read USER2 FLASH */
    printf("Get USER2 FLASH Data = 0x%X\r\n", *(__IO uint32_t*)(0x08042000));
}

/**
 * @}
 */
