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
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "main.h"
#include "bsp_usart.h"

/* Internal SRAM config fo MCU */
#define SRAM_BASE_ADDR          (0x20000000)
#define SRAM_SIZE               (0x20000)

/* Constant for BOOT */
#define SRAM_VECTOR_WORD_SIZE   (64)
#define SRAM_VECTOR_ADDR        (SRAM_BASE_ADDR+SRAM_SIZE-0x100)
#define BOOT_MARK1_ADDR         (0x1FFFF2D0)    /* BOOT NVIC */
#define BOOT_MARK2_ADDR         (0x1FFFF288)    /* BOOT Code */
#define BOOT_MARK3_ADDR         (0x40024C00)

/* The pointer of function */
typedef void (*pFunction)(void);

/**
* @brief  Function of how to jump to Boot
*/
void Jump_To_BOOT(void)
{
    uint32_t i,*pVec,*pMark;
    uint32_t BootAddr,SPAddr;
    
    /* Disable all interrupt */
    __disable_irq();
    
    /* Config IWDG */
    IWDG_ReloadKey();
    IWDG_WriteConfig(IWDG_WRITE_ENABLE);
    IWDG_SetPrescalerDiv(IWDG_PRESCALER_DIV256);
    
    /* Config MMU */
    pMark = (uint32_t *)(BOOT_MARK3_ADDR);
    *pMark = (uint32_t)0x00000011;
    
    /* Config system clock as 72M with HSI and PLL  */
    SetSysClock_HSI_PLL();
    
    /* Reset peripheral used by boot */
    USART_DeInit(USART1);
    GPIO_DeInit(GPIOA);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_USB, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_USB, DISABLE);
    
    /* Init vector */
    pVec = (uint32_t *)SRAM_VECTOR_ADDR;
    for(i=0;i<SRAM_VECTOR_WORD_SIZE;i++)
        pVec[i] = 0;
    
    /* Get SP addr */
    SPAddr = (*((uint32_t *)BOOT_MARK2_ADDR));
    
    /* Get usefull fuction addr */
    pMark = (uint32_t *)BOOT_MARK1_ADDR;
    if(*pMark != 0xFFFFFFFF)    /*BOOT V2.3 and above*/
    {
        BootAddr                        = pMark[0];
        pVec[SysTick_IRQn+16]           = pMark[1];
        pVec[USART1_IRQn+16]            = pMark[2];
        pVec[USB_LP_CAN1_RX0_IRQn+16]   = pMark[3];
        pVec[RTC_IRQn+16]               = pMark[4];
    }
    else
    {
        if(SPAddr != 0xFFFFFFFF)    /*BOOT V2.2*/
        {
            pVec[SysTick_IRQn+16]           = 0x1FFF0A67;
            pVec[USART1_IRQn+16]            = 0x1FFF0A9F;
            pVec[USB_LP_CAN1_RX0_IRQn+16]   = 0x1FFF0ACF;
            pVec[RTC_IRQn+16]               = 0x1FFF0AD3;
            BootAddr                        = 0x1FFF00D9;
        }
        else    /*BOOT V2.1*/
        {
            pVec[SysTick_IRQn+16]           = 0x1FFF10D7;
            pVec[USART1_IRQn+16]            = 0x1FFF115D;
            pVec[USB_LP_CAN1_RX0_IRQn+16]   = 0x1FFF117F;
            pVec[RTC_IRQn+16]               = 0x1FFF1183;
            pVec[EXTI15_10_IRQn+16]         = 0x1FFF10ED;
            BootAddr                        = 0x1FFF0101;
            SPAddr                          = 0x20008690;
        }
    }
    
    /* Enable interrupt */
    __enable_irq();
    
    /* Set JumpBoot addr */
    pFunction JumpBoot = (pFunction)BootAddr;
    
    /* Initalize Stack Pointer */
    __set_MSP(SPAddr);
    
    /* Initialize vector table */
    SCB->VTOR = SRAM_VECTOR_ADDR;
    
    /* Jump to BOOT */
    JumpBoot();
}

/**
* @brief  Main program.
*/
int main(void)
{
    /* Init debug usart */
    Debug_USART_Config();
    
    /* Init a button */
    IAP_Button_Config();
    
    /* Debug information */
    printf("Jump to Boot demo start with normal state!\r\n");
    printf("If you press S1(PA4),system will jump to BOOT!\r\n\r\n");
    
    /* Wait button to be pressed */
    while(IAP_Button_Read() == SET);
    printf("System jumped to BOOT!\r\n\r\n");
    
    /*Jump to BOOT */
    Jump_To_BOOT();
    
    /* Infinite loop */
    while(1);
}


/**
* @brief  Set system clock as 72M with HSI and PLL.
*/
void SetSysClock_HSI_PLL(void)
{
    /* It is necessary to initialize the RCC peripheral to the reset state.*/
    RCC_DeInit();
    
    /* Enable HSI   */
    RCC_EnableHsi(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRD) == RESET)
    {
        /*  If HSI failed to start-up,the clock configuration must be wrong.
            User can add some code here to dela with this problem   */
    }
    
    /* Enable ex mode */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR,ENABLE);
    PWR->CTRL3 |= (uint32_t)0x00000001;
    
    /* Enable ICACHE and Prefetch Buffer */
    FLASH_SetLatency(FLASH_LATENCY_2);
    FLASH_PrefetchBufSet(FLASH_PrefetchBuf_EN);
    FLASH_iCacheCmd(FLASH_iCache_EN);
    
    /* AHB prescaler factor set to 1,HCLK = SYSCLK = 72M    */
    RCC_ConfigHclk(RCC_SYSCLK_DIV1);
    /* APB2 prescaler factor set to 1,PCLK2 = HCLK/1 = 72M  */
    RCC_ConfigPclk2(RCC_HCLK_DIV1);
    /* APB1 prescaler factor set to 2,PCLK1 = HCLK/2 = 36M  */
    RCC_ConfigPclk1(RCC_HCLK_DIV2);
    
    /* Config PLL */
    RCC_ConfigPll(RCC_PLL_SRC_HSI_DIV2, RCC_PLL_MUL_18);
    
    /* Enable PLL */
    RCC_EnablePll(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRD) == RESET)
    {
    }
    
    /* Switch PLL clock to SYSCLK. */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);
    while (RCC_GetSysclkSrc() != RCC_CFG_SCLKSTS_PLL)
    {
    }
}

/**
* @brief  Init a button for test
*/
void IAP_Button_Config(void)
{  
    GPIO_InitType GPIO_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);

    GPIO_InitStructure.Pin          = GPIO_PIN_4;
    GPIO_InitStructure.GPIO_Speed   = GPIO_INPUT;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}

/**
* @brief  Get the state of button
*/
uint8_t IAP_Button_Read(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_4);
}
/******************************************************************/

/******************************************************************/

/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif // USE_FULL_ASSERT
/******************************************************************/


