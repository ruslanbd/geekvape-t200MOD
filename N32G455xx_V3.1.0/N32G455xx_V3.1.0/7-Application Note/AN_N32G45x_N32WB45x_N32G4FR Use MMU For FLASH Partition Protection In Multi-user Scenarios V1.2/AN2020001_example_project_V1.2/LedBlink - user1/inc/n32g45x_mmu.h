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
 * @file n32g45x_mmu.h
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#ifndef __N32G45X_MMU_H__
#define __N32G45X_MMU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32g45x.h"

/** @addtogroup N32G45X_StdPeriph_Driver
 * @{
 */

/** @addtogroup MMU
 * @{
 */

/** @addtogroup MMU_Exported_Types
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Exported_Constants
 * @{
 */

/**
 * @}
 */

/** @addtogroup MMU_Exported_Macros
 * @{
 */
typedef enum
{
    MMU_RST_EN = 1,
    MMU_INT_EN = !MMU_RST_EN,   
} MMU_ALARM_MODE;
#define IS_ALARM_MODE(MODE) (((MODE) == MMU_RST_EN) || ((MODE == MMU_INT_EN))

/**
 * @}
 */

/** @addtogroup MMU_Exported_Functions
 * @{
 */

void MMU_Init(MMU_ALARM_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* __N32G45X_MMU_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
