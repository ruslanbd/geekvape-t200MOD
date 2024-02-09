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


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "n32g45x_STLlib.h"


/** @addtogroup N32G45xSelfTestLib_src
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Start up the internal and external oscillators and verifies
  *   that clock source is within the expected range
  * @param  : None
  * @retval : ClockStatus = (LSI_START_FAIL, HSE_START_FAIL,
  *   HSI_HSE_SWITCH_FAIL, XCROSS_CONFIG_FAIL, EXT_SOURCE_FAIL, FREQ_OK)
  */
ClockStatus SelfTest_ClockStartUpTest(void)
{

  ClockStatus clck_sts = TEST_ONGOING;
  uint32_t TimeOut = LSI_START_TIMEOUT;
  
  CtrlFlowCnt += CLOCK_TEST_CALLEE;

  /* Start low speed internal (LSI) oscillator */
  RCC_EnableLsi(ENABLE);

  /* Wait till LSI is ready */
  do
  {
    TimeOut--;
  }
  while((RCC_GetFlagStatus(RCC_FLAG_LSIRD) == RESET) && (TimeOut != 0uL));

  if (TimeOut == 0uL)
  {
    clck_sts = LSI_START_FAIL;     /* Internal low speed oscillator failure */
  }
  
  

#ifdef HSE_CLOCK_APPLIED  
  /* if LSI started OK - Start High-speed external oscillator (HSE) */
  if(clck_sts == TEST_ONGOING)
  {
    if((RCC_GetFlagStatus(RCC_FLAG_HSERD) == RESET))
    {  
           RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSI);
          /* Wait till Sysclk is ready */
            TimeOut = SYSCLK_START_TIMEOUT;
            do
            {
                TimeOut--;
            }
            while((RCC_GetSysclkSrc()!=0x00) && (TimeOut != 0uL));

            if (TimeOut == 0uL)
            {
                clck_sts = HSE_HSI_SWITCH_FAIL;     /* Internal low speed oscillator failure */
            }
          
          RCC_ConfigHse(RCC_HSE_ENABLE);

        /* Wait till HSE is ready */
          TimeOut = HSE_START_TIMEOUT;
          do
          {
            TimeOut--;
          }
          while((RCC_GetFlagStatus(RCC_FLAG_HSERD) == RESET) && (TimeOut != 0uL));

          if (TimeOut == 0uL)
          {
            clck_sts = HSE_START_FAIL;     /* Internal low speed oscillator failure */
          }
    }
  }
  
  /* if HSE started OK - enable CSS */
  if(clck_sts == TEST_ONGOING)
  {
    RCC_EnableClockSecuritySystem(ENABLE);
    /* Select HSE as system clock source */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSE);  
      
    /* Wait till Sysclk is ready */
      TimeOut = SYSCLK_START_TIMEOUT;
      do
      {
        TimeOut--;
      }
      while((RCC_GetSysclkSrc()!=0x04) && (TimeOut != 0uL));

      if (TimeOut == 0uL)
      {
        clck_sts = HSI_HSE_SWITCH_FAIL;     /* Internal low speed oscillator failure */
      }
  }
  
  /*-------------- Start Reference Measurement -------------------------------*/
  if(clck_sts == TEST_ONGOING)
  {  
    /* Configure dedicated timer to measure LSI period */
    if(SelfTest_InitClock_Xcross_Measurement() == ERROR)
    {
      clck_sts = XCROSS_CONFIG_FAIL;
    }
    else
    {   
      /* Wait for two subsequent LSI periods measurements */
      LSIPeriodFlag = 0u;
      while (LSIPeriodFlag == 0u){ }
      LSIPeriodFlag = 0u;
      while (LSIPeriodFlag == 0u){ }

      /*-------------------- HSE measurement check -------------------------*/
      if (PeriodValue < HSE_LimitLow(HSE_VALUE))
      {
        /* Sub-harmonics: HSE -25% below expected */
        clck_sts = EXT_SOURCE_FAIL;
      }
      else if (PeriodValue > HSE_LimitHigh(HSE_VALUE))
      {
        /* Harmonics: HSE +25% above expected */
        clck_sts = EXT_SOURCE_FAIL;
      }
    }
  }
#else
  if(clck_sts == TEST_ONGOING)
  {
    /* Configure dedicated timer to measure LSI period */
    if(SelfTest_InitClock_Xcross_Measurement() == ERROR)
    {
      clck_sts = XCROSS_CONFIG_FAIL;
    }
    else
    {   
      /* Wait for two subsequent LSI periods measurements */
      LSIPeriodFlag = 0u;
      while (LSIPeriodFlag == 0u){ }
      LSIPeriodFlag = 0u;
      while (LSIPeriodFlag == 0u){ }

      /*-------------------- HSI measurement check -------------------------*/
      if (PeriodValue < HSI_LimitLow(SYSTCLK_AT_STARTUP))
      {
        /* HSI -20% below expected */
        clck_sts = EXT_SOURCE_FAIL;
      }
      else if (PeriodValue > HSI_LimitHigh(SYSTCLK_AT_STARTUP))
      {
        /* HSI +20% above expected */
        clck_sts = EXT_SOURCE_FAIL;
      }
    }
  }
#endif
  
      /* Switch back HSI internal clock at any case */
      RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSI);
  
   /* Wait till Sysclk is ready */
      TimeOut = HSE_START_TIMEOUT;
      do
      {
        TimeOut--;
      }
      while((RCC_GetSysclkSrc()!=0x00) && (TimeOut != 0uL));

      if (TimeOut == 0uL)
      {
        clck_sts = HSI_HSE_SWITCH_FAIL;     /* Internal low speed oscillator failure */
      }
      
  if(clck_sts == TEST_ONGOING)
  {
    /* Switch off PLL */
    RCC_EnablePll(DISABLE);

    /* Wait till Sysclk is ready */
      TimeOut = HSE_START_TIMEOUT;
      do
      {
        TimeOut--;
      }
      while((RCC_GetFlagStatus(RCC_FLAG_PLLRD)!=RESET) && (TimeOut != 0uL));

      if (TimeOut == 0uL)
      {
        clck_sts = PLL_OFF_FAIL;     /* Internal low speed oscillator failure */
      }
  }
  if(clck_sts == TEST_ONGOING)
  {
    /* the test was finished correctly */
    clck_sts = FREQ_OK;
  }
  CtrlFlowCntInv -= CLOCK_TEST_CALLEE;
  
  return(clck_sts);
}
/**
  * @}
  */

/******************* (C)  *****END OF FILE****/
