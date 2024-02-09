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
#include "n32g45x.h"

/* USER CODE BEGIN Includes */
#include "n32g45x_STLparam.h"
#include "n32g45x_STLlib.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwTick;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void IncTick(void)
{
  uwTick++;
}
uint32_t GetTick(void)
{
  return uwTick;
}

int main(void)
{
  
    SystemInit();
      
    #ifdef STL_VERBOSE
    STL_VerbosePORInit();
    printf("\n\r %s\n\r", " N32G45x CortexM4 ");
    printf(" %s\n\r", "   IEC60730 test    ");
    printf(" ... main routine starts ...\r\n");
    #endif /* STL_VERBOSE */
    
    STL_InitRunTimeChecks();
    while (1)
    {

        /* Add your application tasks here  */
        if(TimeBaseFlag == 0xAAAAAAAA)
        {
          /* run time tests */
          STL_DoRunTimeChecks();         
          TimeBaseFlag= 0;
          TimeBaseFlagInv= ~TimeBaseFlag;
        }

    }
 

}


/* -------------------------------------------------------------------------*/
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param :  None
  * @retval None
  */
static int is_lr_sent = 0;
int fputc(int ch, FILE* f)
{
    if (ch == '\r')
    {
        is_lr_sent = 1;
    }
    else if (ch == '\n')
    {
        if (!is_lr_sent)
        {
            USART_SendData(USART1, (uint8_t)'\r');
            /* Loop until the end of transmission */
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXC) == RESET)
            {
            }
        }
        is_lr_sent = 0;
    }
    else
    {
        is_lr_sent = 0;
    }
    USART_SendData(USART1, (uint8_t)ch);
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXC) == RESET)
    {
    }
    return ch;
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) *****END OF FILE****/
