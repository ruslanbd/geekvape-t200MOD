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
 * @author Nations 
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "n32g45x.h"
#include "n32g45x_rtc.h"
#include "User_LED_Config.h"
#include "User_RTC_Config.h"
#include "log.h"

/** @addtogroup RTC_Calendar
 * @{
 */


/**
 * @brief  Delay function.
 *     @arg nCount
 */
void delay(vu32 nCount)
{
    vu32 index = 0;
    for (index = (34000 * nCount); index != 0; index--)
    {
    }
}


/**
 * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
 *         and select PLL as system clock source.
 */
void SYSCLKConfig_STOP(uint32_t RCC_PLLMULL)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    uint32_t Flash_Latency_Temp=0;
    uint32_t PLL_Temp=0;
    uint32_t System_Temp=0;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CTRL |= ((uint32_t)RCC_CTRL_HSEEN);
    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
       HSEStatus = RCC->CTRL & RCC_CTRL_HSERDF;
       StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTRL & RCC_CTRL_HSERDF) != RESET)
    {
       HSEStatus = (uint32_t)0x01;
    }
    else
    {
       HSEStatus = (uint32_t)0x00;
    }
    if (HSEStatus == (uint32_t)0x01)
    {
       /* Enable Prefetch Buffer */
       FLASH->AC |= FLASH_AC_PRFTBFEN;
       /* Flash 2 wait state */
       Flash_Latency_Temp = FLASH->AC;
       Flash_Latency_Temp &= (uint32_t)((uint32_t)~FLASH_AC_LATENCY);
       Flash_Latency_Temp |= (uint32_t)FLASH_AC_LATENCY_4;
       FLASH->AC = Flash_Latency_Temp;
       /* HCLK = SYSCLK */
       RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;
       /* PCLK2 = HCLK */
       RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV2; 
       /* PCLK1 = HCLK */
       RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV4; 
       /*  PLL configuration: PLLCLK = HSE * 18 = 144 MHz */
       PLL_Temp = RCC->CFG;
       PLL_Temp &= (uint32_t)((uint32_t) ~(RCC_CFG_PLLSRC | RCC_CFG_PLLHSEPRES | RCC_CFG_PLLMULFCT));
       PLL_Temp |= (uint32_t)(RCC_CFG_PLLSRC_HSE | RCC_PLLMULL);
       RCC->CFG = PLL_Temp;
       /* Enable PLL */
       RCC->CTRL |= RCC_CTRL_PLLEN;
       /* Wait till PLL is ready */
       while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0);
       /* Select PLL as system clock source */
       System_Temp = RCC->CFG;
       System_Temp &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
       System_Temp |= (uint32_t)RCC_CFG_SCLKSW_PLL;
       RCC->CFG = System_Temp;
       /* Wait till PLL is used as system clock source */
       while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x08);
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    }
}



/**
 * @brief  Main program.
 */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_n32g45x_xx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_n32g45x.c file
       */
    /* Initialize LEDs on n32g45x-EVAL board */
    LEDInit(LED1_PORT, LED1_PIN);
    LEDOff(LED1_PORT,LED1_PIN);
    /* Initialize USART,TX: PA9*/
    log_init();
    log_info("\r\n RTC Init \r\n");
    /* RTC date time alarm default value*/
    RTC_DateAndTimeDefaultVale();
    /* Enable the PWR clock */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR | RCC_APB1_PERIPH_BKP, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessEnable(ENABLE);
    if (USER_WRITE_BKP_DAT1_DATA != BKP_ReadBkpData(BKP_DAT1) )
    {
        /* RTC clock source select */
        if(SUCCESS==RTC_CLKSourceConfig(RTC_CLK_SRC_TYPE_LSE, true, false))
        {
            RTC_PrescalerConfig();
            /* Adjust time by values entered by the user on the hyperterminal */
            RTC_DateRegulate();
            RTC_TimeRegulate();
            /* wake up clock select */
            RTC_ConfigWakeUpClock(RTC_WKUPCLK_CK_SPRE_16BITS);
            /* wake up timer value */
            RTC_SetWakeUpCounter(4);
            BKP_WriteBkpData(BKP_DAT1, USER_WRITE_BKP_DAT1_DATA);
            log_info("\r\n RTC Init Success\r\n");
        }
        else
        {
            log_info("\r\n RTC Init Faile\r\n");
        }
    }
#if (RTC_ALARM_TEST_TYPE   ==  RTC_ALARM_TEST_TYPE_IRQ) // 1.Alarm interrupt test
    EXTI20_RTCWKUP_Configuration(ENABLE);
    /* Enable the RTC Wakeup Interrupt */
    RTC_ConfigInt(RTC_INT_WUT, ENABLE);
    RTC_EnableWakeUp(ENABLE);
    DBG_ConfigPeriph(DBG_STOP, ENABLE);
    while (1)
    {
        /* Insert a long delay */
        delay(20);
        /* Request to enter STOP2 mode*/
        PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI); 
        /* Configures system clock after wake-up from STOP: enable HSE, PLL and select
          PLL as system clock source (HSE and PLL are disabled in STOP mode) */
        SYSCLKConfig_STOP(RCC_CFG_PLLMULFCT18);
        log_init();
        /* Insert a delay */
        log_info("\r\n Stop2 Wakeup From RTC \r\n");
        RTC_TimeShow();
        LEDBlink(LED1_PORT,LED1_PIN);
    }
#elif(RTC_ALARM_TEST_TYPE   ==  RTC_ALARM_TEST_TYPE_OUTPUT) // 2.Alarm out test
    /* Set the output type (open-drain output + push-pull output).
       When setting OD, external  should be added pull-up resistor to PC13 */
    #if (RTC_ALARM_OUT_TYPE_OPEN_DRAIN == RTC_ALARM_OUT_TYPE)    
        RTC_ConfigOutputType(RTC_OUTPUT_OPENDRAIN);
    #elif(RTC_ALARM_OUT_TYPE_PULL_PUSH == RTC_ALARM_OUT_TYPE)   
        RTC_ConfigOutputType(RTC_OUTPUT_PUSHPULL);
    #endif
    RTC_ConfigOutput(RTC_OUTPUT_WKUP, RTC_OUTPOL_LOW);
    RTC_EnableWakeUp(ENABLE);
    while (1)
    {
       if (RTC_GetFlagStatus(RTC_FLAG_WTF) == SET)
       {
          RTC_ClrFlag(RTC_FLAG_WTF);
       }
    }
#endif
}
