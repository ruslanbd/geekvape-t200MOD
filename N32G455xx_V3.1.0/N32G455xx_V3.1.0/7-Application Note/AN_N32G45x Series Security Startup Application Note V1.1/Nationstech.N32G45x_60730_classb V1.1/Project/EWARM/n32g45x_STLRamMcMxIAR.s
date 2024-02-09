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

	SECTION mydata:CONST(2)

; tables with offsets of physical order of address in RAM
__STANDARD_RAM_ORDER
        DCD    -4
        DCD     0
        DCD     4
        DCD     8
        DCD     12
        DCD     16
        DCD     20
        DCD     24
        DCD     28
__ARTISAN_RAM_ORDER
        DCD    -8
        DCD     0
        DCD     4
        DCD     12
        DCD     8
        DCD     16
        DCD     20
        DCD     28
        DCD     24

	SECTION text:CODE(2)
 
  EXTERN ISRCtrlFlowCnt
  EXTERN ISRCtrlFlowCntInv
  
  EXPORT STL_FullRamMarchC
  EXPORT STL_TranspRamMarchCXStep

;*******************************************************************************
; Function Name  : STL_FullRamMarchC
; Description    : Full RAM MarchC test for start-up
; Input          : R0 .. RAM begin (first address to check), 
;                  R1 .. RAM end (last address to check)
;                  R2 .. Background pattern
; Local          : R3 .. Inverted background pattern
;                  R4 .. keeps test result status
;                  R5  .. pointer to RAM
;                  R6  .. content RAM to compare
; Return         : TEST_SUCCESSFULL (=1)
; WARNING        : All the RAM area including stack is destroyed during this test
;*******************************************************************************/
STL_FullRamMarchC:  // R0 = RAM_START, R1 = RAM_END, R2 = BCKGRND = 0
  MOVS  R4, #0x1       ; Test success status by default
  
  MOVS  R3,R2          ; setup inverted background pattern
  RSBS  R3, R3, #0
  SUBS  R3,R3, #1  // R3 = 0xFFFF FFFF
  
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5,R0
??FULL1_LOOP:
  CMP   R5,R1
  BHI   ??FULLSTEP_2
  STR   R2,[R5, #+0]
  ADDS  R5,R5,#+4
  B     ??FULL1_LOOP
    
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
??FULLSTEP_2:
  MOVS  R5,R0
??FULL2_LOOP:
  CMP   R5,R1
  BHI   ??FULLSTEP_3
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+0]
  LDR   R6,[R5,#+4]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+4]
#ifdef ARTISAN
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+8]
#else
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+12]
#endif
  ADDS  R5,R5,#+16
  B     ??FULL2_LOOP
  
; *** Step 3 ***
; Verify inverted background and write background with addresses increasing  
??FULLSTEP_3:
  MOVS  R5,R0
??FULL3_LOOP:
  CMP   R5,R1
  BHI   ??FULLSTEP_4  
  LDR   R6,[R5,#+0]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+0]
  LDR   R6,[R5,#+4]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+4]
#ifdef ARTISAN
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+8]
#else
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+12]
#endif
  ADDS  R5,R5,#+16
  B     ??FULL3_LOOP

; *** Step 4 ***
; Verify background and write inverted background with addresses decreasing  
??FULLSTEP_4:
  MOVS  R5,R1
  SUBS  R5,R5,#+15
??FULL4_LOOP:
  CMP   R5,R0
  BLO   ??FULLSTEP_5
#ifdef ARTISAN
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+12]
#else
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+8]
#endif
  LDR   R6,[R5,#+4]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+4]
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   ??FULL_ERR
  STR   R3,[R5,#+0]
  SUBS  R5,R5,#+16
  B     ??FULL4_LOOP
  
; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
??FULLSTEP_5:
  MOVS  R5,R1
  SUBS  R5,R5,#+15
??FULL5_LOOP:
  CMP   R5,R0
  BLO   ??FULLSTEP_6
#ifdef ARTISAN
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+12]
#else
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+8]
#endif
  LDR   R6,[R5,#+4]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+4]
  LDR   R6,[R5,#+0]
  CMP   R6,R3
  BNE   ??FULL_ERR
  STR   R2,[R5,#+0]
  SUBS  R5,R5,#+16
  B     ??FULL5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
??FULLSTEP_6:
  MOVS  R5,R0
??FULL6_LOOP:
  CMP   R5,R1
  BHI   ??FULL_RET
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   ??FULL_ERR
  ADDS  R5,R5,#+4
  B     ??FULL6_LOOP

??FULL_ERR:
  MOVS  R4,#0       ; error result

??FULL_RET:
  MOVS  R0,R4
  BX    LR          ; return to the caller
  
;*******************************************************************************
; Function Name  : STL_TranspRamMarchCXStep
; Description    : Transparent RAM MarchC-/March X test for run time
; Input          : R0 .. RAM begin (first address to test), 
;                  R1 .. Buffer begin (First address of backup buffer)
;                  R2 .. Background pattern
; Return         : TEST_SUCCESSFULL (=1)
; WARNING -  The RAM area under test is out of original content during this test!
;            Neighbour addresses (first-1 or -2 and last+1) are tested, too.
; Compilation paramaters : ARTISAN - changes order of the sequence of tested
;                                    addresses to respect their physical order
;                  USE_MARCHX_TEST - Skip step 3 and 4 of March C- to make the test
;                                    shorter and faster overall
;*******************************************************************************/
STL_TranspRamMarchCXStep:
  PUSH  {R4-R7}

  LDR   R5,=ISRCtrlFlowCnt  ; Control flow control
  LDR   R6,[R5]
  ADDS  R6,R6,#11
  STR   R6,[R5]
  
  MOVS  R3,R2          ; setup inverted background pattern (R3)
  RSBS  R3, R3, #0
  SUBS  R3,R3, #1  

#ifdef ARTISAN
  LDR   R4, =__ARTISAN_RAM_ORDER ;setup pointer to physical order of the addresses (R4)
#else
  LDR   R4, =__STANDARD_RAM_ORDER
#endif

  MOVS  R5,R0       ; backup buffer to be tested?
  CMP   R5,R1
  BEQ   ??BUFF_TEST
  
; ***************** test of the RAM slice *********************
  MOVS  R5, #0       ; NO - save content of the RAM slice into the backup buffer
??SAVE_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; load data from RAM
  ADDS  R5,R5,#4     ; original data are stored starting from second item of the buffer
  STR   R7,[R1, R5]  ; (first and last items are used for testing purpose exclusively)
  CMP   R5, #20
  BLE   ??SAVE_LOOP
  
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5, #0
??STEP1_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  STR   R2,[R0, R6]  ; store background pattern
  ADDS  R5,R5,#4
  CMP   R5, #20
  BLE   ??STEP1_LOOP
  
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
  MOVS  R5, #0
??STEP2_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattern
  ADDS  R5,R5,#4
  CMP   R5, #20
  BLE   ??STEP2_LOOP

#ifndef USE_MARCHX_TEST   
; *** Step 3 *** (not used at March-X test)
; Verify inverted background and write background with addresses increasing  
  MOVS  R5, #0
??STEP3_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   ??STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  ADDS  R5,R5,#4
  CMP   R5, #20
  BLE   ??STEP3_LOOP
  
; *** Step 4 *** (not used at March-X test)
; Verify background and write inverted background with addresses decreasing  
  MOVS  R5, #24
??STEP4_LOOP:
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattrern
  CMP   R5, #0
  BHI   ??STEP4_LOOP
#endif                ; March-X
  
; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
  MOVS  R5, #24
??STEP5_LOOP:
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   ??STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  CMP   R5, #0
  BHI   ??STEP5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
  MOVS  R5, #0
??STEP6_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  ADDS  R5,R5,#4
  CMP   R5, #20
  BLE   ??STEP6_LOOP

  MOVS  R5, #24      ; restore content of the RAM slice back from the backup buffer
??RESTORE_LOOP:
  LDR   R7,[R1, R5]  ; (first and last items are used for testing purpose exclusively)
  SUBS  R5,R5,#4     ; original data are stored starting from second item of the buffer
  LDR   R6,[R4, R5]  ; load data offset
  STR   R7,[R0, R6]  ; load data from RAM
  CMP   R5, #0
  BHI   ??RESTORE_LOOP
  
  B     ??MARCH_RET

; ************** test of the buffer itself ********************
??BUFF_TEST:
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5, #4
??BUFF1_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  STR   R2,[R0, R6]  ; store background pattern
  ADDS  R5,R5,#4
  CMP   R5, #32
  BLE   ??BUFF1_LOOP
  
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
  MOVS  R5, #4
??BUFF2_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattern
  ADDS  R5,R5,#4
  CMP   R5, #32
  BLE   ??BUFF2_LOOP
  
#ifndef USE_MARCHX_TEST   
; *** Step 3 *** (not used at March-X test)
; Verify inverted background and write background with addresses increasing  
  MOVS  R5, #4
??BUFF3_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   ??STEP_ERR
  STR   R2,[R0, R6]  ; store  background pattern
  ADDS  R5,R5,#4
  CMP   R5, #32
  BLE   ??BUFF3_LOOP

; *** Step 4 *** (not used at March-X test)
; Verify background and write inverted background with addresses decreasing  
  MOVS  R5, #36
??BUFF4_LOOP:
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattrern
  CMP   R5, #4
  BHI   ??BUFF4_LOOP
#endif                ; March-X

; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
  MOVS  R5, #36
??BUFF5_LOOP:
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   ??STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  CMP   R5, #4
  BHI   ??BUFF5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
  MOVS  R5, #4
??BUFF6_LOOP:
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   ??STEP_ERR
  ADDS  R5,R5,#4
  CMP   R5, #32
  BLE   ??BUFF6_LOOP

??MARCH_RET:
  LDR   R4,=ISRCtrlFlowCntInv  ; Control flow control
  LDR   R5,[R4]
  SUBS  R5,R5,#11
  STR   R5,[R4]
  
  MOVS  R0, #1       ; Correct return
  B     ??STEP_RET
  
??STEP_ERR:
  MOVS  R0, #0       ; error result
  
??STEP_RET:
  POP   {R4-R7}
  BX    LR           ; return to the caller
  
  END

;******************* (C) COPYRIGHT  *****END OF FILE*****
