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
#include "n32g45x_STLparam.h"
#define ALLOC_GLOBALS
#include "n32g45x_STLlib.h"
#ifdef __CC_ARM            /* KEIL Compiler */
#include "CRC32.h"
#endif
/** @addtogroup N32G45xSelfTestLib_src
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  
 


/******************************************************************************/
/**
  * @brief  This routine is executed in case of failure is detected by one of
  *    self-test routines. The routine is empty and it has to be filled by end
  *    user to keep application safe while define proper recovery operation
  * @param  : None
  * @retval : None
  */
void FailSafePOR(void)
{
   /* SysTick could be disabled here */
  SysTick->CTRL &= 0xFFFFFFFD;

  #ifdef STL_VERBOSE_POR
    printf(" >>>>>>>>>> POR FailSafe Mode <<<<<<<<<<\n\r");
  #endif  /* STL_VERBOSE_POR */
  
  while(1)
  {
    /* Generate system reset */
	#ifdef  GENERATE_RESET_AT_FAIL_SAFE
    NVIC_SystemReset();
	#else
		#ifdef USE_INDEPENDENT_WDOG
			IWDG_ReloadKey();
		#endif /* USE_INDEPENDENT_WDOG */
            
        #ifdef USE_WINDOW_WDOG
            WWDG_SetCnt(0x7F);
        #endif  /* USE_WWDOG */
            
	#endif  /* GENERATE_RESET_AT_FAIL_SAFE */
  }
}


#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
#pragma optimize = none
#endif

#ifdef __CC_ARM             /* KEIL Compiler */
/******************************************************************************/
/**
  * @brief  Switch between startup and main code
  * @param  : None
  * @retval : None
  */
  void $Sub$$main(void)
  {
    if ( CRC_FLAG != 0xAAu )
    {
      STL_StartUp();		/* trick to call StartUp before main entry */
    }
    CRC_FLAG = 0u;
    $Super$$main(); 
  }
#endif /* __CC_ARM */
	
/******************************************************************************/
/**
  * @brief  Contains the very first test routines executed right after
  *   the reset
  * @param  : None
  *   Flash interface initialized, Systick timer ON (2ms timebase)
  * @retval : None
  */
void STL_StartUp(void)
{
  uint32_t crc_result;
  ClockStatus clk_sts;
  
 
  #ifdef STL_VERBOSE_POR
  /* Update the SystemCoreClock global variable as USART Baud rate setting depends on it */
  STL_VerbosePORInit();
  printf("\r\n*******  Self Test Library Init  *******\r\n");
  #endif
  
  /* Initialization of counters for control flow monitoring */
  init_control_flow();
 
  /*--------------------------------------------------------------------------*/
  /*------------------- CPU registers and flags self test --------------------*/
  /*--------------------------------------------------------------------------*/ 
  /* WARNING: all registers destroyed when exiting this function (including
  preserved registers R4 to R11) while excluding stack pointer R13) */
  
  control_flow_call(CPU_TEST_CALLER);
  
  if (STL_StartUpCPUTest() != CPUTEST_SUCCESS)
  {    
    #ifdef STL_VERBOSE_POR
      printf("Start-up CPU Test Failure\n\r");
    #endif /* STL_VERBOSE_POR */
    FailSafePOR();
  }
  else  /* Test OK */
  {
    /* If else statement is not executed, it will be detected by control flow monitoring */
    control_flow_resume(CPU_TEST_CALLER);
    #ifdef STL_VERBOSE_POR
      printf("Start-up CPU Test OK\n\r");
    #endif /* STL_VERBOSE_POR */
  }
   /*--------------------------------------------------------------------------*/
   /*------------------------- Watch dogs Self Test ---------------------------*/
   /*--------------------------------------------------------------------------*/
  CtrlFlowCnt += WDG_TEST_CALLER;
  STL_WDGSelfTest();
  CtrlFlowCntInv -= WDG_TEST_CALLER;

  /*--------------------------------------------------------------------------*/
  /*--------------------- Invariable memory CRC check ------------------------*/
  /*--------------------------------------------------------------------------*/
  
  control_flow_call(CRC32_TEST_CALLER);
  /* Compute the 32-bit crc of the whole Flash by CRC unit except the checksum
     pattern stored at top of FLASH */
  RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_CRC, ENABLE);
  
  
  #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
    /* ==============================================================================*/
    /* MISRA violation of rule 11.4, 17.4 - pointer arithmetic is used for
       CRC calculation */
    #pragma diag_suppress=Pm088,Pm141
  #endif  /* __IAR_SYSTEMS_ICC__ */

/* the next lines replaces the standard HAL function call
   crc_result = HAL_CRC_Calculate(&CrcHandle, (uint32_t *)ROM_START, (uint32_t)ROM_SIZEinWORDS); 
  due to bug at IAR linker - check sum computation can't support both big & little endian  */
  #ifdef __CC_ARM            /* KEIL Compiler */
  crc_result = crc32_fsl(0,(void*)ROM_START,ROM_SIZE);
  #endif  /* __CC_ARM */
  #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  CRC32_ResetCrc();  
  for(uint32_t index = 0; index < (uint32_t)ROM_SIZEinWORDS; index++)
  {
    CRC->CRC32DAT = __REV(*((uint32_t *)ROM_START + index));
  }
  crc_result = CRC->CRC32DAT;
  #endif  /* __IAR_SYSTEMS_ICC__ */

  #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
    #pragma diag_default=Pm088,Pm141
    /* ==============================================================================*/
  #endif  /* __IAR_SYSTEMS_ICC__ */
          
  control_flow_resume(CRC32_TEST_CALLER);

  
  
  /* Store pattern for regular 32-bit crc computation */
  control_flow_call(CRC_TEST_CALLER);
  /* ==============================================================================*/
  /* MISRA violation of rule 10.1, 11.3 and 11.4: integral casting and pointer arithmetic 
     is used here to manage the crc computation and Check Class B var integrity */
  
  #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
    #pragma diag_suppress=Pm129,Pm140,Pm141

    if(crc_result != *(uint32_t *)(&REF_CRC32))

    #pragma diag_default=Pm129,Pm140,Pm141
  #endif  /* __IAR_SYSTEMS_ICC__ */
  /* ==============================================================================*/
      
  #if defined(__CC_ARM) || defined(__GNUC__)             /* KEIL or GCC Compiler */
    /* Computed 32-bit crc check is temporary stored at crc_result. This value must be copied
       into __Check_Sum address placed at the end of Flash area 
       Condition here can be reversed for debugging */

    if(crc_result != *(uint32_t *)(&REF_CRC32))
  #endif  /* __CC_ARM */   
    {
      #ifdef STL_VERBOSE_POR
        printf("FLASH 32-bit CRC Error at Start-up\n\r");
      #endif  /* STL_VERBOSE_POR */
      FailSafePOR();
    }
    else
    { /* Test OK */
      /* add just check sum pattern to be included at overall FLASH calculation at crc unit */
     #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
			//CRC32_CalcCrc(*(uint32_t *)(&REF_CRC32));
      #endif  /* __IAR_SYSTEMS_ICC__ */
      //CRC32_SetIDat(~((uint8_t)(CRC32_GetCrc())));
      #ifdef STL_VERBOSE_POR
        printf(" Start-up FLASH 32-bit CRC OK\n\r");
      #endif  /* STL_VERBOSE_POR */

      /* If else statement is not executed, it will be detected by control flow monitoring */
      control_flow_resume(CRC_TEST_CALLER);  
    }
    
  CRC32_ResetCrc(); 
  CRC32_SetIDat(0x0);

   /*--------------------------------------------------------------------------*/
  /*   Verify Control flow before RAM init (which clears Ctrl flow counters)  */
  /*--------------------------------------------------------------------------*/
  if (control_flow_check_point(CHECKPOINT1) == ERROR)
  {
     #ifdef STL_VERBOSE_POR
     printf("Control Flow Error Checkpoint 1\r\n");
     #endif  /* STL_VERBOSE_POR */
     FailSafePOR();
  }
  else
  {
   #ifdef STL_VERBOSE_POR
   printf("Control Flow Checkpoint 1 OK\r\n");
   #endif  /* STL_VERBOSE_POR */
  }
 
  /*--------------------------------------------------------------------------*/
  /* --------------------- Variable memory functional test -------------------*/
  /*--------------------------------------------------------------------------*/
 
  /* no stack operation can be performed during the test */  
   __disable_irq();
  
  /* WARNING: Stack is zero-initialized when exiting from this routine */
  if (STL_FullRamMarchC(RAM_START, RAM_END, BCKGRND) != SUCCESS)
  {
    #ifdef STL_VERBOSE_POR
      printf("RAM Test Failure\n\r");
    #endif  /* STL_VERBOSE_POR */
    FailSafePOR();
  }
   else
  {
   #ifdef STL_VERBOSE_POR
   printf("Full RAM Test OK\r\n");
   #endif  /* STL_VERBOSE_POR */
  }


  /* restore interrupt capability */
  __enable_irq();
  
/* Initialization of counters for control flow monitoring 
     (destroyed during RAM test) */
  init_control_flow();
   
  /*------------- Store reference 32-bit CRC in RAM after RAM test -----------*/
    
   RefCrc32 = 0;  
   RefCrc32Inv = ~RefCrc32;

   /* startup test completed successfully - restart the application */
  


  //--------------------------------------------------------------------------*/
  //----------------------- Clock Frequency Self Test ------------------------*/
  //--------------------------------------------------------------------------*/
  control_flow_call(CLOCK_TEST_CALLER);
  
  /* test LSI & HSE clock systems */
  clk_sts = SelfTest_ClockStartUpTest();
  
  /* Re-init USART with modified clock setting */
  #ifdef STL_VERBOSE_POR
    USART_ReConfigurationClk();
  #endif  /* STL_VERBOSE_POR */
    
  switch(clk_sts)
  {
    case LSI_START_FAIL:     
    #ifdef STL_VERBOSE_POR
        printf("LSI start-up failure\r\n");
    #endif  /* STL_VERBOSE_POR */
        
      break;

    case HSE_START_FAIL:
      #ifdef STL_VERBOSE_POR
        printf("HSE start-up failure\r\n");
      #endif  /* STL_VERBOSE_POR */
      break;

    case HSI_HSE_SWITCH_FAIL:
    case HSE_HSI_SWITCH_FAIL:
    case PLL_OFF_FAIL:
      #ifdef STL_VERBOSE_POR
      printf("Clock switch failure\r\n");
      #endif  /* STL_VERBOSE_POR */
      break;

    case XCROSS_CONFIG_FAIL:
      #ifdef STL_VERBOSE_POR
      printf("Clock Xcross measurement failure\r\n");
      #endif  /* STL_VERBOSE_POR */
      break;

    case EXT_SOURCE_FAIL:
      #ifdef STL_VERBOSE_POR
      printf("HSE clock found out of range\r\n");
      #endif  /* STL_VERBOSE_POR */
      break;
      
    case FREQ_OK:
      #ifdef STL_VERBOSE_POR
      printf("Clock frequency OK \r\n");
      #endif  /* STL_VERBOSE_POR */
      break;
      
    default:
      #ifdef STL_VERBOSE_POR
      printf("Abnormal exit from clock test\r\n");
      #endif  /* STL_VERBOSE_POR */
  }
  
  if(clk_sts != FREQ_OK)
  {
    FailSafePOR();
  }
    
  /* Either switch back to HSI or start PLL on HSE asap */
  control_flow_resume(CLOCK_TEST_CALLER); 
 
  
 
  /* -----  Store verify pattern to stack bottom for its later testing  ----- */
   CtrlFlowCnt += STACK_OVERFLOW_TEST;

   aStackOverFlowPtrn[0] = 0xAAAAAAAAuL;
   aStackOverFlowPtrn[1] = 0xBBBBBBBBuL;
   aStackOverFlowPtrn[2] = 0xCCCCCCCCuL;
   aStackOverFlowPtrn[3] = 0xDDDDDDDDuL;

   CtrlFlowCntInv -= STACK_OVERFLOW_TEST;
   /*--------------------------------------------------------------------------*/
  /* -----  Verify Control flow before Starting main program execution ------ */
  /*--------------------------------------------------------------------------*/ 
    if (control_flow_check_point(CHECKPOINT2) == ERROR)
   {
     #ifdef STL_VERBOSE_POR
       printf("Control Flow Error Checkpoint 2 \n\r");
     #endif  /* STL_VERBOSE_POR */
     FailSafePOR();
   }
   #ifdef STL_VERBOSE_POR
    printf(" Control Flow Checkpoint 2 OK \n\r");
   #endif  /* STL_VERBOSE_POR */

   GotoCompilerStartUp();
}


    
    
/******************************************************************************/
/**
  * @brief  Verifies the consistency and value of control flow counters
  * @param  : check value of the positive counter
  * @retval : ErrorStatus (SUCCESS, ERROR)
  */
ErrorStatus control_flow_check_point(uint32_t chck)
{
  ErrorStatus Result= SUCCESS;
  
  if ((CtrlFlowCnt != (chck)) || ((CtrlFlowCnt ^ CtrlFlowCntInv) != 0xFFFFFFFFuL))
  {
    Result= ERROR;
  }
  return(Result);
}

/* ------------------------------------------------------------ */
/**
  * @brief  Initializes the USART1 and few I/Os for test purposes
  * @param :  None
  * @retval : None
  */
void STL_VerbosePORInit(void)
{
  GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1 | RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOA, ENABLE);

    GPIO_InitStructure.Pin            = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin             = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    // init uart
    USART_Init(USART1, &USART_InitStructure);

    // enable uart
    USART_Enable(USART1, ENABLE);
}

/* ------------------------------------------------------------ */
/**
  * @brief  Reinitializes the USART1 with new clock frequency
  * @param  : None
  * @retval : None
  */
void USART_ReConfigurationClk()
{
    USART_InitType USART_InitStructure;

    USART_Enable(USART1, DISABLE);
    
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
    // init uart
    USART_Init(USART1, &USART_InitStructure);

    // enable uart
    USART_Enable(USART1, ENABLE);
}
/* ------------------------------------------------------------ */
/**
  * @brief  Switch off the PLL to be able to measure external clock
  *   source frequency
  * @param :  None
  * @retval : ErrorStatus = (ERROR, SUCCESS)
  */
ErrorStatus RCC_SwitchOffPLL(void)
{
    ErrorStatus Result = ERROR;
    uint32_t TimeOut = CLOCK_SWITCH_TIMEOUT;
    /* Even if product runs at 64MHz, timeout is still much higher than the
    few clock cycles required for clock switch */

  /* Select HSI as system clock source */
  RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSI);
  /* Wait till HSI is used again as a system clock source */
  do
  {
    TimeOut--;
  }
  while ((RCC_GetSysclkSrc() != RCC_CFG_SCLKSTS_HSI) && (TimeOut != 0u));

  if (TimeOut == 0u)
  {
    Result = ERROR;     /* Clock switch failure */
  }
  else
  {
    /* We can safely switch off the PLL */
    RCC_EnablePll(DISABLE);
    Result = SUCCESS;
  }
  return (Result);
}

/* ============================================================================ */
/**
  * @brief  Verifies the watchdog by forcing watchdog resets
  * @param :  None
  * @retval : None
  */
void STL_WDGSelfTest(void)
{  
 
   #ifdef STL_VERBOSE_POR  
    if (RCC_GetFlagStatus(RCC_FLAG_PINRST)  != RESET) printf("Pin reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST)  != RESET) printf("POR reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST)  != RESET) printf("SW reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) printf("IWDG reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET) printf("WWDG reset \r\n");
    if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET) printf("LP reset \r\n");
  #endif /* STL_VERBOSE_POR */

  /* start watchdogs test if one of the 4 conditions below is valid */
  if ( (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)\
   ||  (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)\
   ||  (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)\
   || ((RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == RESET) && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == RESET)))
  {
    #ifdef STL_VERBOSE_POR
    printf("... Power-on or software reset, testing IWDG ... \r\n");
    #endif  /* STL_VERBOSE_POR */

    /* IWDG at debug mode */
    //RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
   // DBG_ConfigPeriph(DBG_IWDG_STOP, ENABLE);

    /* Clear all flags before resuming test */
    RCC_ClrFlag();
   
    /* Setup IWDG to minimum period */
    IWDG_WriteConfig(IWDG_WRITE_ENABLE);
    IWDG_SetPrescalerDiv(IWDG_PRESCALER_DIV4);
    IWDG_CntReload(1);
    IWDG_ReloadKey();
    IWDG_Enable();
    
    /* Wait for an independent watchdog reset */
    while(1)
    { }
  }
  else  /* Watchdog test or software reset triggered by application failure */
  {
    /* If WWDG only was set, re-start the complete test (indicates a reset triggered by safety routines */
    if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  != RESET) 
     && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == RESET) 
     && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET))
    {
      RCC_ClrFlag();
      #ifdef STL_VERBOSE_POR
      printf("... WWDG reset, re-start WDG test ... \r\n");
      #endif  /* STL_VERBOSE_POR */
      NVIC_SystemReset();
    }
    else  /* If IWDG only was set, continue the test with WWDG test*/
    {
      if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  != RESET) 
       && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) 
       && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == RESET))
      { /* If IWDG only was set, test WWDG*/
        #ifdef STL_VERBOSE_POR
        printf("... IWDG reset from test or application, testing WWDG\r\n");
        #endif  /* STL_VERBOSE_POR */
          
        /* WWDG at debug mode */
        //RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
        //DBG_ConfigPeriph(DBG_WWDG_STOP, ENABLE);
          
        /* Setup WWDG to minimum period */
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG, ENABLE);
        WWDG_SetPrescalerDiv(WWDG_PRESCALER_DIV1);
        WWDG_SetWValue(63);
        WWDG_Enable(64);
        /* Wait for an independent watchdog reset */
        while(1)
        { }
      }
      else  /* If both flags IWDG & WWDG flags are set, means that watchdog test is completed */
      {
        if ((RCC_GetFlagStatus(RCC_FLAG_PINRST)  != RESET) && (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) && (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET))
        {
          RCC_ClrFlag();
          #ifdef STL_VERBOSE_POR
          printf("... WWDG reset, WDG test completed ... \r\n");
          #endif  /* STL_VERBOSE_POR */
        }
        else  /* Unexpected Flag configuration, re-start WDG test */
        {
          RCC_ClrFlag();
          #ifdef STL_VERBOSE_POR
          printf("...Unexpected Flag configuration, re-start WDG test... \r\n");
          #endif  /* STL_VERBOSE_POR */
          NVIC_SystemReset();
        } /* End of Unexpected Flag configuration */
      } /* End of normal test sequence */
    } /* End of partial WDG test (IWDG test done) */
  } /* End of part where 1 or 2 Watchdog flags are set */
  
}
/**
  * @}
  */

/******************* (C)  *****END OF FILE****/
