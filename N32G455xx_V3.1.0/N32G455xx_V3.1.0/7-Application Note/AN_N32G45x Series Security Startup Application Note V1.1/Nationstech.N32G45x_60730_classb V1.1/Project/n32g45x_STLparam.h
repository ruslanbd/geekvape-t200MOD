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
#ifndef __SELFTEST_PARAM_H
#define __SELFTEST_PARAM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Clock frequency test */
#include "n32g45x_STLclock.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* This is for having self-diagnostic messages reported on a PC via UART */
#define STL_VERBOSE_POR       /* During Power-on phase only */
#define STL_VERBOSE           /* During main program execution  */

/* uncoment next lines to configure watchdogs & resets for debug purposes */
#define USE_INDEPENDENT_WDOG
#define USE_WINDOW_WDOG


/* comment out next line to force micro reset at fail mode */
#define NO_RESET_AT_FAIL_MODE

/* comment out next line when CRC unit is not configurable */
#define CRC_UNIT_CONFIGURABLE

/* comment out next line when there is no window feature at IWDG */
#define IWDG_FEATURES_BY_WINDOW_OPTION

/* comment out next line when HSE clock is not used - clock test then runs based on HSI */
#define HSE_CLOCK_APPLIED

/* system clock & HSE when HSE is applied as PLL source */
#if !defined  (HSE_VALUE) 
#define HSE_VALUE    ((uint32_t)8000000) 
#endif /* HSE_VALUE */
#define SYSTCLK_AT_RUN_HSE (uint32_t)(144000000uL)
/* system clock when HSI is applied as PLL source (HSE is not used) */
#define SYSTCLK_AT_STARTUP (uint32_t)(144000000uL)
#define SYSTCLK_AT_RUN_HSI (uint32_t)(144000000uL)

/* Reserved area for RAM buffer, incl overlap for test purposes */
/* Don't change this parameter as it is related to physical technology used! */
#define RT_RAM_BLOCKSIZE   (uint32_t)6u 
/* Min overlap to cover coupling fault from one tested row to the other */
#define RT_RAM_BLOCK_OVERLAP  (uint32_t)1u
                                           
/* These are the direct and inverted data (pattern) used during the RAM
test, performed using March C- Algorithm */
#define BCKGRND     ((uint32_t)0x00000000uL)
#define INV_BCKGRND ((uint32_t)0xFFFFFFFFuL)
/* uncoment next line to use March-X test instead of March-C */
/* #define USE_MARCHX_RAM_TEST */
/* Define the time bases for clock frequency measure: RUN and START time bases
must be equal or multiple. RUN time base must take into account SysTick
prescaler and PLL setting (if enabled)*/
#define SYSTICK_2_ms_HSI ((uint32_t)16000uL) /* HSI = 8MHz */
#define SYSTICK_2_ms_HSE ((uint32_t)16000uL) /* HSE = 8MHz */

#define SYSTICK_2_ms_PLL ((uint32_t)288000uL) /* PLL = 144MHz, SysTick clock not prescaled */

#define SYSTICK_TB_START  ((uint32_t)SYSTICK_2_ms_HSI)
#define SYSTICK_TB_RUN  ((uint32_t)SYSTICK_2_ms_PLL)
/* time base for the main (to refresh watchdogs) */
#define SYSTICK_10ms_TB  ((uint32_t)10uL) /* 10*1ms */


/* Timeout required to avoid being stuck in while loops during clock circuitry
initialization, in case of problem. Watchdog is also active, but these timeouts
allow to react more quickly */
#define LSI_START_TIMEOUT ((uint32_t)1000uL) /* ~3.5ms (IAR, speed optimized) */

/* ~35ms to be adapted depending on crystal/resonator */
#define HSE_START_TIMEOUT ((uint32_t)10000uL)

/* ~35ms to be adapted depending on crystal/resonator */
#define SYSCLK_START_TIMEOUT ((uint32_t)10000uL)

/* For clock switch:*/
#define CLOCK_SWITCH_TIMEOUT ((uint32_t)1000uL)

/* PLL lock Timeout set to ~10ms (IAR, speed optimized) */
#define PLL_LOCK_TIMEOUT ((uint32_t)3000uL)

/* Value of the Internal oscillator in Hz*/
#define HSI_Freq    ((uint32_t)8000000uL)

/* Value of the Internal LSI oscillator in Hz */
#define LSI_Freq    ((uint32_t)40000uL)

/* HSE frequency above this limit considered as harmonics */
#define HSE_LimitHigh(fcy) ((uint32_t)(((fcy)/LSI_Freq)*8u*5u/2u)/4u) /* (HSEValue + 25%) -8:IcPrescaler,2:HSE/TIM5 CLOCK*/

/* HSE frequency below this limit considered as sub-harmonics*/
#define HSE_LimitLow(fcy) ((uint32_t)(((fcy)/LSI_Freq)*8u*3u/2u)/4u)  /* (HSEValue - 25%) */

/* here you can define HSI frequency limits  */
#define HSI_LimitHigh(fcy) ((uint32_t)(((fcy)/LSI_Freq)*8u*6u/2u)/5u) /* (HSIValue + 20%) */
#define HSI_LimitLow(fcy) ((uint32_t)(((fcy)/LSI_Freq)*8u*4u/2u)/5u)  /* (HSIValue - 20%) */

/* define the maximum U32 */
#define U32_MAX    ((uint32_t)4294967295uL)

/* -------------------------------------------------------------------------- */
/* ------------------ CONTROL FLOW TAGS and CHECKPOINTS --------------------- */
/* -------------------------------------------------------------------------- */
/* RAM_TEST_CALLEE is only needed for CtrlFlowCntInv when exiting routine */
/* This is because the RAM test routines destroys the control flow counters */
#define RAM_TEST_CALLEE         ((uint32_t)0xFFFFFFFFuL)

#define CPU_TEST_CALLER         ((uint32_t)2)
#define CPU_TEST_CALLEE         ((uint32_t)3) /* Do not modify: hard coded in assembly file */
#define WDG_TEST_CALLER         ((uint32_t)5)
#define CRC32_INIT_CALLER       ((uint32_t)7)
#define CRC32_TEST_CALLER       ((uint32_t)11)
#define CRC32_TEST_CALLEE       ((uint32_t)13)
#define CRC_TEST_CALLER         ((uint32_t)17)
#define CRC_STORE_CALLER        ((uint32_t)19)
#define CLOCK_TEST_CALLER       ((uint32_t)23)
#define CLOCK_TEST_CALLEE       ((uint32_t)29)
#define LSI_INIT_CALLEE         ((uint32_t)31)
#define HSE_INIT_CALLEE         ((uint32_t)37)
#define RTC_INIT_CALLEE         ((uint32_t)41)
#define SYSTICK_INIT_CALLEE     ((uint32_t)43)
#define CLOCK_SWITCH_CALLEE     ((uint32_t)47)
#define STACK_OVERFLOW_TEST     ((uint32_t)53)
#define STACK_OVERFLOW_CALLEE   ((uint32_t)59)
#define CLOCKPERIOD_TEST_CALLEE ((uint32_t)61)
#define FLASH_TEST_CALLER       ((uint32_t)63)
#define CRC32_RUN_TEST_CALLEE   ((uint32_t)63)

#define CHECKPOINT1 ((uint32_t)CPU_TEST_CALLER + \
                          CPU_TEST_CALLEE + \
                          WDG_TEST_CALLER + \
                          CRC32_TEST_CALLER + \
                          CRC_TEST_CALLER)

#define CHECKPOINT2 ((uint32_t)CLOCK_TEST_CALLER + \
                          CLOCK_TEST_CALLEE + \
                          STACK_OVERFLOW_TEST)

/* This is for run-time tests with 32-bit CRC */
#define DELTA_MAIN  ((uint32_t)CPU_TEST_CALLER + \
                          CPU_TEST_CALLEE + \
                          STACK_OVERFLOW_TEST + \
                          STACK_OVERFLOW_CALLEE + \
                          CLOCK_TEST_CALLER + \
                          CLOCKPERIOD_TEST_CALLEE + \
                          FLASH_TEST_CALLER + \
                          CRC32_RUN_TEST_CALLEE + \
                          CRC32_TEST_CALLEE)

#define LAST_DELTA_MAIN ((uint32_t) DELTA_MAIN - CRC32_TEST_CALLEE + CRC32_INIT_CALLER)
#define FULL_FLASH_CHECKED ((uint32_t)DELTA_MAIN * STEPS_NUMBER + LAST_DELTA_MAIN)

#define MEASPERIOD_ISR_CALLER   ((uint32_t)2)
#define MEASPERIOD_ISR_CALLEE   ((uint32_t)3)
#define CLOCKPERIOD_ISR_CALLEE  ((uint32_t)5)
#define RAM_MARCHC_ISR_CALLER   ((uint32_t)7)
#define RAM_MARCHC_ISR_CALLEE   ((uint32_t)11)

/* This is for March C tests */
#define DELTA_ISR  (uint32_t)(RAM_MARCHC_ISR_CALLER + \
                           RAM_MARCHC_ISR_CALLEE)
//#define DELTA_ISR  (uint32_t)(RAM_MARCHC_ISR_CALLER)

#define CLASS_B_ROWS (((uint32_t)CLASS_B_END - (uint32_t)CLASS_B_START) / (RT_RAM_BLOCKSIZE - 2u*RT_RAM_BLOCK_OVERLAP))
/* +2 below is for last block & buffer self-test itself */
#define RAM_TEST_COMPLETED ((uint32_t)(DELTA_ISR * (uint32_t)(CLASS_B_ROWS/4u + 1u)))

/* Exported macro ------------------------------------------------------------*/
#define init_control_flow() CtrlFlowCntInv = ~(CtrlFlowCnt = 0uL)
#define control_flow_call(a) CtrlFlowCnt += (a)
#define control_flow_resume(a) CtrlFlowCntInv -= (a)

/* Exported functions ------------------------------------------------------- */
#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
/* This is the IAR compiler entry point, usually executed right after reset */
  extern void __iar_program_start( void );
  extern uint16_t __checksum;
/* The 4 variables here above are set by IAR linker */
  extern uint32_t __ICFEDIT_region_ROM_start__;
  extern uint32_t __ICFEDIT_region_ROM_end__;
  extern uint32_t __ICFEDIT_region_RAM_start__;
  extern uint32_t __ICFEDIT_region_RAM_end__;
  extern uint32_t __ICFEDIT_region_CLASSB_start__;
  extern uint32_t __ICFEDIT_region_CLASSB_end__;  

  /* Constants necessary for Flash CRC calculation  (last block - 64 bytes - separated for CRC) */
  #define ROM_START (uint8_t *)&__ICFEDIT_region_ROM_start__ 
  #define ROM_END   ((uint8_t *)(&__checksum) - 1u) 
  #define ROM_SIZE  ((uint32_t)ROM_END - (uint32_t)ROM_START + 1u)

  #define STEPS_NUMBER          ((uint32_t)ROM_SIZE/64u)
  #define ROM_SIZEinWORDS       (uint32_t) (ROM_SIZE/4u)
  #define FLASH_BLOCK_WORDS     (uint32_t)((ROM_SIZEinWORDS) / STEPS_NUMBER)

  /* Constants necessary for Full March tests */
  #define RAM_START (uint32_t *)0x20000000
  #define RAM_END   (uint32_t *)&__ICFEDIT_region_RAM_end__  
  
  /* Constants necessary for Transparent March tests */
  #define CLASS_B_START ((uint32_t *)(&__ICFEDIT_region_CLASSB_start__))
  #define CLASS_B_END ((uint32_t *)(&__ICFEDIT_region_CLASSB_end__))
  
  #define GotoCompilerStartUp() { __iar_program_start(); }

  #define REF_CRC32 __checksum
#
#endif  /* __IAR_SYSTEMS_ICC__ */

#ifdef __CC_ARM   /* KEIL Compiler */

  /* This is the KEIL compiler entry point, usually executed right after reset */
  //extern void Reset_Handler( void ); 
  //extern const uint32_t __Check_Sum;
  extern void __main( void );

  /* Constants necessary for Flash CRC calculation (ROM_SIZE in byte) */
	/* byte-aligned addresses */
  #define ROM_START ((uint32_t *)0x08000000uL)	
  #define ROM_END   ((uint32_t *)0x0801FF00uL) /* Modify according to needs */
  #define ROM_SIZE  ((uint32_t)ROM_END - (uint32_t)ROM_START)
  
  #define STEPS_NUMBER          ((uint32_t)ROM_SIZE/64u)
  #define ROM_SIZEinWORDS (uint32_t) (ROM_SIZE/4u)
  #define FLASH_BLOCK_WORDS     (uint32_t)((ROM_SIZEinWORDS)/STEPS_NUMBER)

  /* Constants necessary for execution initial March test */
  #define RAM_START ((uint32_t *)0x20000000uL)
  #define RAM_END   ((uint32_t *)0x20013FFFuL)  /* Modify according to needs */

  /* Constants necessary for execution of transparent run time March tests */
  #define CLASS_B_START ((uint32_t *)0x20000040uL)
  #define CLASS_B_END   ((uint32_t *)0x20000100uL) 
  
  #define CRC_FLAG	CRC->CRC32IDAT
  
  #define GotoCompilerStartUp()  { CRC_FLAG = 0xAAu; __main(); } /* entry to init C before main() */
  
  #define REF_CRC32 (*(uint32_t *)ROM_END)

#endif  /* __CC_ARM */
  

/* Exported functions ------------------------------------------------------- */
void FailSafePOR(void);

#if defined(STL_VERBOSE) || defined(STL_VERBOSE_POR)
  void USART_Configuration(void);
#endif /* STL_VERBOSE */
  
ErrorStatus control_flow_check_point(uint32_t chck);
void STL_SysTickConfig(void);
void STL_FlashCrc32Init(void);
void CRC_Init(void);
void initialize_system_wdogs(void);

uint32_t STL_MeasurePeriod(void);
//ClassBTestStatus STL_TranspMarchC(void);

//void STL_InitRunTimeChecks(void);
//void STL_DoRunTimeChecks(void);
//ClassBTestStatus STL_crc32Run(void);
//CPUStatus STL_RunTimeCPUTest(void);
ClockStatus STL_CheckPeriod(uint32_t Period);
uint32_t CRC_CalcBlockCrc(const uint32_t *p, uint32_t len);

#endif /* __SELFTEST_PARAM_H */
/******************* (C) *****END OF FILE****/
