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
 * @file n32g45x_mmu.c
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g45x_mmu.h"

/** @addtogroup N32G45X_StdPeriph_Driver
 * @{
 */

/** @addtogroup MMU
 * @brief MMU driver modules
 * @{
 */

/** @addtogroup MMU_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Private_Defines
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Private_Macros
 * @{
 */
#define MMU_CTRL_OFFSET 0x00
#define MMU_STS_OFFSET 0x04

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

/**
 * @}
 */

/** @addtogroup MMU_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Private_Functions
 * @{
 */

/**
 * @brief  Configures mode of MMU alarm(only for USER1).
 * @param mode mode of of MMU alarm.
 *   This parameter can be: MMU_RST_EN or MMU_INT_EN.
 */
void MMU_Init(MMU_ALARM_MODE mode)
{
    /* Check the parameters */
    assert_param(IS_ALARM_MODE(mode));

    NVIC->ICPR[2] |= (uint32_t)(1UL << ((uint32_t)16));    
    *((volatile unsigned long  *)(MMU_BASE + MMU_STS_OFFSET)) = 0;
    while(*((volatile unsigned long  *)(MMU_BASE + MMU_STS_OFFSET)))
    {

    }

    BIT_ADDR((MMU_BASE + MMU_CTRL_OFFSET), 6)  = (uint32_t)mode;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
