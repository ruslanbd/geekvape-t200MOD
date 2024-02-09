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
 * @file user2_demo.h
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __USER2_DEMO_H__
#define __USER2_DEMO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"

typedef enum
{
    DEINIT_STS = 0,
    INIT_STS = !DEINIT_STS,   
} INIT_STATUS;
#define IS_INIT_STATUS(STATUS) (((STATUS) == DEINIT_STS) || ((STATUS == INIT_STS ))

typedef void (*pFunction)(void); 

#define USER2_FUNC_ADDR (0x08041001)
#define API_FuncEntry2 ((pFunction)(USER2_FUNC_ADDR))

void Test_LedBlink(GPIO_Module* GPIOx, uint32_t Pin);

void USART_Configuration(void);

#ifdef __cplusplus
}
#endif

#endif /* __USER2_DEMO_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
