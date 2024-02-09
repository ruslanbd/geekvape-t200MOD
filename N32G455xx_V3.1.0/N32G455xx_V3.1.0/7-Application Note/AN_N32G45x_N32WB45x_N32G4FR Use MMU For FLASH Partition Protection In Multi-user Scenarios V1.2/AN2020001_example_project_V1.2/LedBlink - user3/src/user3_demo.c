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
 * @file user3_demo.c
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "user3_demo.h"
#include <stdio.h>

/** @addtogroup N32G45X_StdPeriph_Template
 * @{
 */

uint32_t data[10];

void Test_User3(void) __attribute__((section(".ARM.__at_0x08061000")));

/**
 * @brief USER3_Demo
 * 
 */
void Test_User3(void)
{
    uint32_t i;   

    /* USART Configuration */
    // USART_Configuration(); 
    /* Output a message on Hyperterminal using printf function */
    printf("\n\rHello! Here is USER3 Example!\n\r");

    /* LED(PA10) Blinks */
    Test_LedBlink(GPIOA, GPIO_PIN_10);

    /* Read USER2 SRAM */
    printf("Get USER2 SRAM Data = 0x%x\r\n", *(__IO uint32_t*)(0x20012100));

    /* Read USER2 FLASH (at address 0x08042000) */
    for(i = 0; i < 20; i++)
    {
        data[i] = *(__IO uint32_t*)(0x08042000 + i);       
    }    
}
/**
 * @}
 */
