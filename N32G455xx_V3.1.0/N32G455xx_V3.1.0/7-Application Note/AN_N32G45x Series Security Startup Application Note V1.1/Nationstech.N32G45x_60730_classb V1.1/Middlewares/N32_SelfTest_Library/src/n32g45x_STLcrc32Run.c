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
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes CRC peripheral (enable its clock and reset CRC)
  * @param :  None
  * @retval : None
  */  
void CRC_Init(void)
{  
  /* This is for control flow monitoring */
  CtrlFlowCnt += CRC32_INIT_CALLER;

  /* Enable CRC module clock */
  RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_CRC, ENABLE);

  /* Reset CRC generator */
  CRC32_ResetCrc(); 

  CtrlFlowCntInv -= CRC32_INIT_CALLER;
}


/**
  * @brief  Computes the 16-bit CRC of a given part of the memory (words
  *   by words).
  * @param p: points to the first memory byte to be taken into account
  * @param len: length (in bytes) of the memory to be computed					
  * @retval : 32-bit CRC using the 0x4C11DB7 polynomial
  */
uint32_t CRC_CalcBlockCrc(const uint32_t *p, uint32_t len)
{
  /* This is for control flow monitoring */
  CtrlFlowCnt += CRC32_TEST_CALLEE;

   /* Load memory content into the CRC generator data register */
  do
  {
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  /* ==============================================================================*/
  /* MISRA violation of rule 17.4 - pointer arithmetic is used for CRC calculation */
		#pragma diag_suppress=Pm088
	#endif   /* IAR Compiler */
    CRC->CRC32DAT = *p++;
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
		#pragma diag_default=Pm088
  /* ==============================================================================*/
	#endif   /* IAR Compiler */
  }
  while (--len);

  CtrlFlowCntInv -= CRC32_TEST_CALLEE;

  return (CRC->CRC32DAT);
}
  /**
  * @brief  Inializes the pointers to the Flash memory required during
  *   run-time
  * @param :  None
  * @retval : None
  */
void STL_FlashCrc32Init(void)
{
  /* MISRA violation of rules 11.3, 11.4: pointer casting is used to check the 
     memory area and keep class B variable integrity */
#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  #pragma diag_suppress=Pm140, Pm141
#endif /* IAR Compiler */
  p_RunCrc32Chk = (uint32_t*)ROM_START;
  p_RunCrc32ChkInv = ((uint32_t *)(uint32_t)(~(uint32_t)(ROM_START)));
#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  #pragma diag_default=Pm140, Pm141
#endif /* IAR Compiler */

  CRC_Init(); /* Reset the CRC generator */
}

/* ---------------------------------------------------------------------------*/
/**
  * @brief  Computes the crc in multiple steps and compare it with the
  *   ref value when the whole memory has been tested
  * @param :  None
  * @retval : ClassBTestStatus = (TEST_RUNNING, CLASS_B_DATA_FAIL,
  *   TEST_FAILURE, TEST_OK)
  */
ClassBTestStatus STL_crc32Run(void)
{
  ClassBTestStatus Result = CTRL_FLW_ERROR; /* In case of abnormal func exit*/
    

  CtrlFlowCnt += CRC32_RUN_TEST_CALLEE;

  /* MISRA violation of rules 10.1, 10.3, 11.3, 11.4 and 17.4: integral casting and pointer arithmetic 
    is used here to manage the crc compuation and Check Class B var integrity */
#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  #pragma diag_suppress=Pm088,Pm129,Pm136,Pm140,Pm141
#endif /* IAR Compiler */
  
  if ((((uint32_t)p_RunCrc32Chk) ^ ((uint32_t)p_RunCrc32ChkInv)) == 0xFFFFFFFFuL)
  {
    if (p_RunCrc32Chk < (uint32_t *)ROM_END)
    {
      CtrlFlowCnt += CRC32_TEST_CALLEE;
      //CRC_CalcBlockCrc((const uint32_t *)p_RunCrc32Chk, (uint32_t)FLASH_BLOCK_WORDS);
        
        #ifdef __CC_ARM            /* KEIL Compiler */
          if(p_RunCrc32Chk==(uint32_t*)ROM_START)
          {
            crc32_fsl_continuous(0, (void *) p_RunCrc32Chk, FLASH_BLOCK_WORDS*4, 0);
          }else
          {
            crc32_fsl_continuous(0, (void *) p_RunCrc32Chk, FLASH_BLOCK_WORDS*4, 1);
          }
          Result = TEST_RUNNING;

      #endif  /* __CC_ARM */
      
      #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */    
      for(uint32_t index = 0; index < (uint32_t)FLASH_BLOCK_WORDS; index++)
      {
        CRC->CRC32DAT = __REV(*(p_RunCrc32Chk + index));
      }
      Result = TEST_RUNNING;
      #endif  /* __IAR_SYSTEMS_ICC__ */
      CtrlFlowCntInv -= CRC32_TEST_CALLEE;
      
      p_RunCrc32Chk += FLASH_BLOCK_WORDS;     /* Increment pointer to next block */
      p_RunCrc32ChkInv = ((uint32_t *)~(uint32_t)((uint32_t)p_RunCrc32Chk));
#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
  #pragma diag_default=Pm088,Pm129,Pm136,Pm140,Pm141
#endif /* IAR Compiler */
      
      
    }
    else
    {
      if ((RefCrc32 ^ RefCrc32Inv) == 0xFFFFFFFFuL)
      {
          
         #ifdef __CC_ARM            /* KEIL Compiler */
            if(crc32_fsl_continuous(0, (void *) p_RunCrc32Chk, FLASH_BLOCK_WORDS*4, 2) == REF_CRC32)
            {
              Result = TEST_OK;
            }
            else
            {
              Result = TEST_FAILURE;
            }
        #endif  /* __CC_ARM */
            
        #ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
            /* The reference CRC was stored in a Class B variable */
            if (CRC->CRC32DAT == *(uint32_t *)(&REF_CRC32))
            {
              Result = TEST_OK;
            }
            else
            {
              Result = TEST_FAILURE;
            }
        #endif  /* __IAR_SYSTEMS_ICC__ */
            
        STL_FlashCrc32Init(); /* Prepare next test (or redo it if this one failed) */
      }
      else /* Class B error on RefCrc32 */
      {
        Result = CLASS_B_DATA_FAIL;
      }
    }
  }
  else  /* Class B error p_RunCrc32Chk */
  {
    Result = CLASS_B_DATA_FAIL;
  }

  CtrlFlowCntInv -= CRC32_RUN_TEST_CALLEE;

  return (Result);
}
/**
  * @}
  */
/******************* (C)  *****END OF FILE****/
