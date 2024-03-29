/**
  ****************************************************************************
  Copyright (c) 2019, Nations Technologies Inc.

  All rights reserved.
  ****************************************************************************

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice,
  this list of conditions and the disclaimer below.

  Nations' name may not be used to endorse or promote products derived from
  this software without specific prior written permission.

  DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
  DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  ****************************************************************************
 **/
    
  .syntax unified
  .cpu cortex-m4
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4
    
LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss

/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  
  /* External Interrupts */
  .word     WWDG_IRQHandler                   /* Window WatchDog              */                                        
  .word     PVD_IRQHandler                    /* PVD through EXTI Line detection */                        
  .word     TAMPER_IRQHandler             /* Tamper  */            
  .word     RTC_WKUP_IRQHandler               /* RTC Wakeup  */                      
  .word     FLASH_IRQHandler                  /* FLASH                        */                                          
  .word     RCC_IRQHandler                    /* RCC                          */                                            
  .word     EXTI0_IRQHandler                  /* EXTI Line0                   */                        
  .word     EXTI1_IRQHandler                  /* EXTI Line1                   */                          
  .word     EXTI2_IRQHandler                  /* EXTI Line2                   */                          
  .word     EXTI3_IRQHandler                  /* EXTI Line3                   */                          
  .word     EXTI4_IRQHandler                  /* EXTI Line4                   */                          
  .word     DMA1_Channel1_IRQHandler           /* DMA1 Channel 1                */                  
  .word     DMA1_Channel2_IRQHandler           /* DMA1 Channel 2                */                   
  .word     DMA1_Channel3_IRQHandler           /* DMA1 Channel 3                */                   
  .word     DMA1_Channel4_IRQHandler           /* DMA1 Channel 4                */                   
  .word     DMA1_Channel5_IRQHandler           /* DMA1 Channel 5                */                   
  .word     DMA1_Channel6_IRQHandler           /* DMA1 Channel 6                */                   
  .word     DMA1_Channel7_IRQHandler           /* DMA1 Channel 7                */                   
  .word     ADC1_2_IRQHandler                    /* ADC1, ADC2          */                   
  .word     USB_HP_CAN1_TX_IRQHandler                /* USB High Priority or CAN1 TX                      */                         
  .word     USB_LP_CAN1_RX0_IRQHandler               /* USB Low  Priority or CAN1 RX0                     */                          
  .word     CAN1_RX1_IRQHandler               /* CAN1 RX1                     */                          
  .word     CAN1_SCE_IRQHandler               /* CAN1 SCE                     */                          
  .word     EXTI9_5_IRQHandler                /* EXTI Line 9..5          */                          
  .word     TIM1_BRK_IRQHandler          /* TIM1 Break          */         
  .word     TIM1_UP_IRQHandler          /* TIM1 Update       */         
  .word     TIM1_TRG_COM_IRQHandler     /* TIM1 Trigger and Commutation */
  .word     TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */                          
  .word     TIM2_IRQHandler                   /* TIM2                         */                   
  .word     TIM3_IRQHandler                   /* TIM3                         */                   
  .word     TIM4_IRQHandler                   /* TIM4                         */                   
  .word     I2C1_EV_IRQHandler                /* I2C1 Event                   */                          
  .word     I2C1_ER_IRQHandler                /* I2C1 Error                   */                          
  .word     I2C2_EV_IRQHandler                /* I2C2 Event                   */                          
  .word     I2C2_ER_IRQHandler                /* I2C2 Error                   */                            
  .word     SPI1_IRQHandler                   /* SPI1                         */                   
  .word     SPI2_IRQHandler                   /* SPI2                         */                   
  .word     USART1_IRQHandler                 /* USART1                       */                   
  .word     USART2_IRQHandler                 /* USART2                       */                   
  .word     USART3_IRQHandler                 /* USART3                       */                   
  .word     EXTI15_10_IRQHandler              /* EXTI Line 15..10        */                          
  .word     RTCAlarm_IRQHandler              /* RTC Alarm through EXTI Line */                 
  .word     USBWakeUp_IRQHandler            /* USB Wakeup from suspend */                       
  .word     TIM8_BRK_IRQHandler         /* TIM8 Break         */         
  .word     TIM8_UP_IRQHandler          /* TIM8 Update        */         
  .word     TIM8_TRG_COM_IRQHandler     /* TIM8 Trigger and Commutation */
  .word     TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */                          
  .word     ADC3_4_IRQHandler           /* ADC3 & ADC4                 */                          
  .word     XFMC_IRQHandler                   /* XFMC                         */                   
  .word     SDIO_IRQHandler                   /* SDIO                         */                   
  .word     TIM5_IRQHandler                   /* TIM5                         */                   
  .word     SPI3_IRQHandler                   /* SPI3                         */                   
  .word     UART4_IRQHandler                  /* UART4                        */                   
  .word     UART5_IRQHandler                  /* UART5                        */                   
  .word     TIM6_IRQHandler               /* TIM6  */                   
  .word     TIM7_IRQHandler                   /* TIM7                         */
  .word     DMA2_Channel1_IRQHandler           /* DMA2 Channel1                */                   
  .word     DMA2_Channel2_IRQHandler           /* DMA2 Channel2                */                   
  .word     DMA2_Channel3_IRQHandler           /* DMA2 Channel3                */                   
  .word     DMA2_Channel4_IRQHandler           /* DMA2 Channel4               */                   
  .word     DMA2_Channel5_IRQHandler           /* DMA2 Channel5                */                   
  .word     ETH_IRQHandler                    /* Ethernet  global interrupt                     */                   
  .word     ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line interrupt */                     
  .word     CAN2_TX_IRQHandler                /* CAN2 TX                      */                          
  .word     CAN2_RX0_IRQHandler               /* CAN2 RX0                     */                          
  .word     CAN2_RX1_IRQHandler               /* CAN2 RX1                     */                          
  .word     CAN2_SCE_IRQHandler               /* CAN2 SCE                     */                          
  .word     QSPI_IRQHandler                 /* QSPI                   */                   
  .word     DMA2_Channel6_IRQHandler           /* DMA2 Channel6                */                   
  .word     DMA2_Channel7_IRQHandler           /* DMA2 Channel7                */                   
  .word     I2C3_EV_IRQHandler           /* I2C3 event                */                   
  .word     I2C3_ER_IRQHandler                 /* I2C3 error                       */                    
  .word     I2C4_EV_IRQHandler                /* I2C4 event                   */                          
  .word     I2C4_ER_IRQHandler                /* I2C4 error                   */                          
  .word     UART6_IRQHandler         /* UART6   */                   
  .word     UART7_IRQHandler          /* UART7    */                   
  .word     DMA1_Channel8_IRQHandler            /* DMA1 Channel8 */                         
  .word     DMA2_Channel8_IRQHandler                 /* DMA2 Channel8                   */                   
  .word     DVP_IRQHandler                   /* DVP                         */                   
  .word     SAC_IRQHandler                   /* SAC                  */                   
  .word     MMU_IRQHandler               /* MMU                 */
  .word     TSC_IRQHandler                    /* TSC                          */
                        
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler
  
   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler
  
   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler
  
   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler              
  
   .weak      WWDG_IRQHandler                   
   .thumb_set WWDG_IRQHandler,Default_Handler      
                  
   .weak      PVD_IRQHandler      
   .thumb_set PVD_IRQHandler,Default_Handler
               
   .weak      TAMPER_IRQHandler            
   .thumb_set TAMPER_IRQHandler,Default_Handler
            
   .weak      RTC_WKUP_IRQHandler                  
   .thumb_set RTC_WKUP_IRQHandler,Default_Handler
            
   .weak      FLASH_IRQHandler         
   .thumb_set FLASH_IRQHandler,Default_Handler
                  
   .weak      RCC_IRQHandler      
   .thumb_set RCC_IRQHandler,Default_Handler
                  
   .weak      EXTI0_IRQHandler         
   .thumb_set EXTI0_IRQHandler,Default_Handler
                  
   .weak      EXTI1_IRQHandler         
   .thumb_set EXTI1_IRQHandler,Default_Handler
                     
   .weak      EXTI2_IRQHandler         
   .thumb_set EXTI2_IRQHandler,Default_Handler 
                 
   .weak      EXTI3_IRQHandler         
   .thumb_set EXTI3_IRQHandler,Default_Handler
                        
   .weak      EXTI4_IRQHandler         
   .thumb_set EXTI4_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel1_IRQHandler               
   .thumb_set DMA1_Channel1_IRQHandler,Default_Handler
         
   .weak      DMA1_Channel2_IRQHandler               
   .thumb_set DMA1_Channel2_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel3_IRQHandler               
   .thumb_set DMA1_Channel3_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel4_IRQHandler               
   .thumb_set DMA1_Channel4_IRQHandler,Default_Handler 
                 
   .weak      DMA1_Channel5_IRQHandler              
   .thumb_set DMA1_Channel5_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel6_IRQHandler               
   .thumb_set DMA1_Channel6_IRQHandler,Default_Handler
                  
   .weak      DMA1_Channel7_IRQHandler               
   .thumb_set DMA1_Channel7_IRQHandler,Default_Handler
                  
   .weak      ADC1_2_IRQHandler      
   .thumb_set ADC1_2_IRQHandler,Default_Handler
               
   .weak      USB_HP_CAN1_TX_IRQHandler   
   .thumb_set USB_HP_CAN1_TX_IRQHandler,Default_Handler
            
   .weak      USB_LP_CAN1_RX0_IRQHandler                  
   .thumb_set USB_LP_CAN1_RX0_IRQHandler,Default_Handler
                           
   .weak      CAN1_RX1_IRQHandler                  
   .thumb_set CAN1_RX1_IRQHandler,Default_Handler
            
   .weak      CAN1_SCE_IRQHandler                  
   .thumb_set CAN1_SCE_IRQHandler,Default_Handler
            
   .weak      EXTI9_5_IRQHandler   
   .thumb_set EXTI9_5_IRQHandler,Default_Handler
            
   .weak      TIM1_BRK_IRQHandler            
   .thumb_set TIM1_BRK_IRQHandler,Default_Handler
            
   .weak      TIM1_UP_IRQHandler            
   .thumb_set TIM1_UP_IRQHandler,Default_Handler
      
   .weak      TIM1_TRG_COM_IRQHandler      
   .thumb_set TIM1_TRG_COM_IRQHandler,Default_Handler
      
   .weak      TIM1_CC_IRQHandler   
   .thumb_set TIM1_CC_IRQHandler,Default_Handler
                  
   .weak      TIM2_IRQHandler            
   .thumb_set TIM2_IRQHandler,Default_Handler
                  
   .weak      TIM3_IRQHandler            
   .thumb_set TIM3_IRQHandler,Default_Handler
                  
   .weak      TIM4_IRQHandler            
   .thumb_set TIM4_IRQHandler,Default_Handler
                  
   .weak      I2C1_EV_IRQHandler   
   .thumb_set I2C1_EV_IRQHandler,Default_Handler
                     
   .weak      I2C1_ER_IRQHandler   
   .thumb_set I2C1_ER_IRQHandler,Default_Handler
                     
   .weak      I2C2_EV_IRQHandler   
   .thumb_set I2C2_EV_IRQHandler,Default_Handler
                  
   .weak      I2C2_ER_IRQHandler   
   .thumb_set I2C2_ER_IRQHandler,Default_Handler
                           
   .weak      SPI1_IRQHandler            
   .thumb_set SPI1_IRQHandler,Default_Handler
                        
   .weak      SPI2_IRQHandler            
   .thumb_set SPI2_IRQHandler,Default_Handler
                  
   .weak      USART1_IRQHandler      
   .thumb_set USART1_IRQHandler,Default_Handler
                     
   .weak      USART2_IRQHandler      
   .thumb_set USART2_IRQHandler,Default_Handler
                     
   .weak      USART3_IRQHandler      
   .thumb_set USART3_IRQHandler,Default_Handler
                  
   .weak      EXTI15_10_IRQHandler               
   .thumb_set EXTI15_10_IRQHandler,Default_Handler
               
   .weak      RTCAlarm_IRQHandler               
   .thumb_set RTCAlarm_IRQHandler,Default_Handler
            
   .weak      USBWakeUp_IRQHandler         
   .thumb_set USBWakeUp_IRQHandler,Default_Handler
            
   .weak      TIM8_BRK_IRQHandler         
   .thumb_set TIM8_BRK_IRQHandler,Default_Handler
         
   .weak      TIM8_UP_IRQHandler            
   .thumb_set TIM8_UP_IRQHandler,Default_Handler
         
   .weak      TIM8_TRG_COM_IRQHandler      
   .thumb_set TIM8_TRG_COM_IRQHandler,Default_Handler
      
   .weak      TIM8_CC_IRQHandler   
   .thumb_set TIM8_CC_IRQHandler,Default_Handler
                  
   .weak      ADC3_4_IRQHandler               
   .thumb_set ADC3_4_IRQHandler,Default_Handler
                     
   .weak      XFMC_IRQHandler            
   .thumb_set XFMC_IRQHandler,Default_Handler
                     
   .weak      SDIO_IRQHandler            
   .thumb_set SDIO_IRQHandler,Default_Handler
                     
   .weak      TIM5_IRQHandler            
   .thumb_set TIM5_IRQHandler,Default_Handler
                     
   .weak      SPI3_IRQHandler            
   .thumb_set SPI3_IRQHandler,Default_Handler
                     
   .weak      UART4_IRQHandler         
   .thumb_set UART4_IRQHandler,Default_Handler
                  
   .weak      UART5_IRQHandler         
   .thumb_set UART5_IRQHandler,Default_Handler
                  
   .weak      TIM6_IRQHandler                  
   .thumb_set TIM6_IRQHandler,Default_Handler
               
   .weak      TIM7_IRQHandler            
   .thumb_set TIM7_IRQHandler,Default_Handler
         
   .weak      DMA2_Channel1_IRQHandler               
   .thumb_set DMA2_Channel1_IRQHandler,Default_Handler
               
   .weak      DMA2_Channel2_IRQHandler               
   .thumb_set DMA2_Channel2_IRQHandler,Default_Handler
                  
   .weak      DMA2_Channel3_IRQHandler               
   .thumb_set DMA2_Channel3_IRQHandler,Default_Handler
            
   .weak      DMA2_Channel4_IRQHandler               
   .thumb_set DMA2_Channel4_IRQHandler,Default_Handler
            
   .weak      DMA2_Channel5_IRQHandler               
   .thumb_set DMA2_Channel5_IRQHandler,Default_Handler
            
   .weak      ETH_IRQHandler      
   .thumb_set ETH_IRQHandler,Default_Handler
                  
   .weak      ETH_WKUP_IRQHandler                  
   .thumb_set ETH_WKUP_IRQHandler,Default_Handler
            
   .weak      CAN2_TX_IRQHandler   
   .thumb_set CAN2_TX_IRQHandler,Default_Handler
                           
   .weak      CAN2_RX0_IRQHandler                  
   .thumb_set CAN2_RX0_IRQHandler,Default_Handler
                           
   .weak      CAN2_RX1_IRQHandler                  
   .thumb_set CAN2_RX1_IRQHandler,Default_Handler
                           
   .weak      CAN2_SCE_IRQHandler                  
   .thumb_set CAN2_SCE_IRQHandler,Default_Handler
                           
   .weak      QSPI_IRQHandler      
   .thumb_set QSPI_IRQHandler,Default_Handler
                     
   .weak      DMA2_Channel6_IRQHandler               
   .thumb_set DMA2_Channel6_IRQHandler,Default_Handler
                  
   .weak      DMA2_Channel7_IRQHandler               
   .thumb_set DMA2_Channel7_IRQHandler,Default_Handler
                  
   .weak      I2C3_EV_IRQHandler               
   .thumb_set I2C3_EV_IRQHandler,Default_Handler
                  
   .weak      I2C3_ER_IRQHandler      
   .thumb_set I2C3_ER_IRQHandler,Default_Handler
                        
   .weak      I2C4_EV_IRQHandler   
   .thumb_set I2C4_EV_IRQHandler,Default_Handler
                        
   .weak      I2C4_ER_IRQHandler   
   .thumb_set I2C4_ER_IRQHandler,Default_Handler
                        
   .weak      UART6_IRQHandler         
   .thumb_set UART6_IRQHandler,Default_Handler
               
   .weak      UART7_IRQHandler            
   .thumb_set UART7_IRQHandler,Default_Handler
               
   .weak      DMA1_Channel8_IRQHandler         
   .thumb_set DMA1_Channel8_IRQHandler,Default_Handler
            
   .weak      DMA2_Channel8_IRQHandler      
   .thumb_set DMA2_Channel8_IRQHandler,Default_Handler
                  
   .weak      DVP_IRQHandler            
   .thumb_set DVP_IRQHandler,Default_Handler
                     
   .weak      SAC_IRQHandler            
   .thumb_set SAC_IRQHandler,Default_Handler
               
   .weak      MMU_IRQHandler                  
   .thumb_set MMU_IRQHandler,Default_Handler   

   .weak      TSC_IRQHandler                  
   .thumb_set TSC_IRQHandler,Default_Handler  
