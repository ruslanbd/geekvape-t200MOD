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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Includes ------------------------------------------------------------------*/
#include "n32g45x.h"
#include <stdio.h>

#define TICK_INT_PRIORITY   ((uint32_t)0)    /*!< tick interrupt priority (lowest by default)  */ 

/* SysTick counter state */
#define SysTick_Counter_Disable        ((uint32_t)0xFFFFFFFE)
#define SysTick_Counter_Enable         ((uint32_t)0x00000001)
#define SysTick_Counter_Clear          ((uint32_t)0x00000000)

/* SysTick Flag */
#define SysTick_FLAG_COUNT             ((uint8_t)0x30)
#define SysTick_FLAG_SKEW              ((uint8_t)0x5E)
#define SysTick_FLAG_NOREF             ((uint8_t)0x5F)

#define CLOCKSWITCH_TIMEOUT_VALUE      5000    /* 5 s    */
#define HSE_TIMEOUT_VALUE              HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE              3U      /* 3 ms */
#define LSI_TIMEOUT_VALUE              3U      /* 3 ms */
#define PLL_TIMEOUT_VALUE              3U      /* 3 ms */      

/* Exported types ------------------------------------------------------------*/
/* Exported variableses ------------------------------------------------------*/
extern uint32_t MyRAMCounter;
extern uint32_t MyFLASHCounter;
/* Exported variableses ------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define ADDR_FLASH_PAGE_31    ((uint32_t)0x08007C00) 
#define FLASH_EN
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void STL_StartUp(void);
void SystemInit (void);
void StartUpClock_Config(void);
void User_signal_Init(uint16_t gpio_pin);

/* STLstartup.c - Exported functions ------------------------------------------------------- */
void STL_VerbosePORInit(void);
void STL_WDGSelfTest(void);
//ErrorStatus STL_FullRamMarchC(void);
void USART_ReConfigurationClk(void);
//void ComputeInitialFlash32CRC(void);
ErrorStatus RCC_SwitchOffPLL(void);
//ClockStatus STL_ClockStartUpTest(void);
void IncTick(void);
uint32_t GetTick(void);

#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  void __iar_data_init3(void);
#endif /* __IAR_SYSTEMS_ICC__ */
#ifdef __CC_ARM             /* KEIL Compiler */
extern void $Super$$main(void);  
#endif /* __CC_ARM */
#ifdef __GNUC__
int16_t __io_putchar(int16_t ch);
#endif /* __GNUC__ */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C)  *****END OF FILE****/
