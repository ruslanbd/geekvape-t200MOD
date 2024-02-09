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
  * @brief  Initializes the pointer to the RAM for the run-time
  *   transparent functional test.
  * @param  : None
  * @retval : None
  */
void STL_TranspMarchInit(void)
{
  /* start address of the test has to be aligned to 16 address range */	  
   pRunTimeRamChk = (uint32_t *)((uint32_t)CLASS_B_START & 0xFFFFFFFCuL);
   pRunTimeRamChkInv = (uint32_t *)(uint32_t)(~(uint32_t)pRunTimeRamChk);
}

/* ---------------------------------------------------------------------------*/
/**
  * @brief  This function verifies that 6 words of RAM are functional,
  *   overlapping) using the March C- or March X algorithm.
  * @param  : None
  * @retval : ClassBTestStatus (TEST_RUNNING, CLASS_B_DATA_FAIL,
  *   TEST_FAILURE, TEST_OK)
  */
ClassBTestStatus STL_TranspMarch(void)
{
  ClassBTestStatus result = TEST_RUNNING;
  
  /* Check Class B var integrity */
  if ((((uint32_t)pRunTimeRamChk) ^ ((uint32_t)pRunTimeRamChkInv)) == 0xFFFFFFFFuL)
  {
    if (pRunTimeRamChk >= CLASS_B_END)
    {
      /*------------- March test applied on the RAM Buffer itself --------------- */
      if (STL_TranspRamMarchCXStep(&aRunTimeRamBuf[0], &aRunTimeRamBuf[0], BCKGRND) == MARCH_STEP_SUCCESS)         
      {
        /* All the RAM test is completed successfully */
        result = TEST_OK;
      }
      else  /* Buffer is not functional */
      {
        result = TEST_FAILURE;
      }
      /* Init next cycle of the transparent RAM test starting from the begin of the Class B area */
      pRunTimeRamChk = CLASS_B_START;
      pRunTimeRamChkInv = ((uint32_t *)~((uint32_t)CLASS_B_START));
    }
    else
    { /* ------------- March test applied on Class B data area ------------------ */
      /* !!! Application has to ensure that no write or read operation
               is performed within Class B RAM area during this test !!!      */
      if (STL_TranspRamMarchCXStep(pRunTimeRamChk, &aRunTimeRamBuf[1], BCKGRND) == MARCH_STEP_SUCCESS)         
      {
        /* Prepare next Row Transparent RAM test */
        pRunTimeRamChk += RT_RAM_BLOCKSIZE - (2u * RT_RAM_BLOCK_OVERLAP);
        pRunTimeRamChkInv = (uint32_t *)(uint32_t)(~(uint32_t)pRunTimeRamChk);
      }
      else
      {
        result = TEST_FAILURE;  /* Word line under test was not functional */
      }
    }
  }
  else  /* Class B error on pRunTimeRamChk */
  {
    result = CLASS_B_DATA_FAIL;
  }
  return (result);  
}

/**
  * @}
  */

/******************* (C)  *****END OF FILE****/
