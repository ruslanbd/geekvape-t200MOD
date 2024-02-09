;/**
  ;**************************************************************************
  ;* File   : n32g45x_STLRamMcMxKEIL.s
  ;* Version: V1.0.0
  ;* Date   : 2021-04-25
  ;* Brief  : This file contains procedures written in assembler for full
  ;           and partial transparent Marching RAM tests to be called during
  ;           start-up and run time
  ;**************************************************************************
  ;*/

  THUMB
;  REQUIRES
  PRESERVE8
  
  AREA |.text|, CODE, READONLY, ALIGN=2

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
STL_FullRamMarchC  ;// R0 = RAM_START, R1 = RAM_END, R2 = BCKGRND = 0
  MOVS  R4, #0x1       ; Test success status by default
  
  MOVS  R3,R2          ; setup inverted background pattern
  RSBS  R3, R3, #0
  SUBS  R3,R3, #1
  
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5,R0
__FULL1_LOOP
  CMP   R5,R1
  BHI   __FULLSTEP_2
  STR   R2,[R5, #+0]
  ADDS  R5,R5,#+4
  B     __FULL1_LOOP
    
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
__FULLSTEP_2
  MOVS  R5,R0
__FULL2_LOOP
  CMP   R5,R1
  BHI   __FULLSTEP_3
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+0]
  LDR   R6,[R5,#+4]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+4]
 IF :DEF:ARTISAN
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+8]
 ELSE
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+12]
 ENDIF
  ADDS  R5,R5,#+16
  B     __FULL2_LOOP
  
; *** Step 3 ***
; Verify inverted background and write background with addresses increasing  
__FULLSTEP_3
  MOVS  R5,R0
__FULL3_LOOP
  CMP   R5,R1
  BHI   __FULLSTEP_4  
  LDR   R6,[R5,#+0]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+0]
  LDR   R6,[R5,#+4]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+4]
 IF :DEF:ARTISAN
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+8]
 ELSE
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+12]
 ENDIF
  ADDS  R5,R5,#+16
  B     __FULL3_LOOP

; *** Step 4 ***
; Verify background and write inverted background with addresses decreasing  
__FULLSTEP_4
  MOVS  R5,R1
  SUBS  R5,R5,#+15
__FULL4_LOOP
  CMP   R5,R0
  BLO   __FULLSTEP_5
 IF :DEF:ARTISAN
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+12]
 ELSE
  LDR   R6,[R5,#+12]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+8]
 ENDIF
  LDR   R6,[R5,#+4]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+4]
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   __FULL_ERR
  STR   R3,[R5,#+0]
  SUBS  R5,R5,#+16
  B     __FULL4_LOOP
  
; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
__FULLSTEP_5
  MOVS  R5,R1
  SUBS  R5,R5,#+15
__FULL5_LOOP
  CMP   R5,R0
  BLO   __FULLSTEP_6
 IF :DEF:ARTISAN
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+8]
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+12]
 ELSE
  LDR   R6,[R5,#+12]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+12]
  LDR   R6,[R5,#+8]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+8]
 ENDIF
  LDR   R6,[R5,#+4]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+4]
  LDR   R6,[R5,#+0]
  CMP   R6,R3
  BNE   __FULL_ERR
  STR   R2,[R5,#+0]
  SUBS  R5,R5,#+16
  B     __FULL5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
__FULLSTEP_6
  MOVS  R5,R0
__FULL6_LOOP
  CMP   R5,R1
  BHI   __FULL_RET
  LDR   R6,[R5,#+0]
  CMP   R6,R2
  BNE   __FULL_ERR
  ADDS  R5,R5,#+4
  B     __FULL6_LOOP

__FULL_ERR
  MOVS  R4,#0       ; error result

__FULL_RET
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
STL_TranspRamMarchCXStep
  PUSH  {R4-R7}

  LDR   R5,=ISRCtrlFlowCnt  ; Control flow control
  LDR   R6,[R5]
  ADDS  R6,R6,#11
  STR   R6,[R5]
  
  MOVS  R3,R2          ; setup inverted background pattern (R3)
  RSBS  R3, R3, #0
  SUBS  R3,R3, #1  

 IF :DEF:ARTISAN
  LDR   R4, =__ARTISAN_RAM_ORDER ;setup pointer to physical order of the addresses (R4)
 ELSE
  LDR   R4, =__STANDARD_RAM_ORDER
 ENDIF

  MOVS  R5,R0       ; backup buffer to be tested?
  CMP   R5,R1
  BEQ   __BUFF_TEST
  
; ***************** test of the RAM slice *********************
  MOVS  R5, #0       ; NO - save content of the RAM slice into the backup buffer
__SAVE_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; load data from RAM
  ADD   R5,R5,#4     ; original data are stored starting from second item of the buffer
  STR   R7,[R1, R5]  ; (first and last items are used for testing purpose exclusively)
  CMP   R5, #20
  BLE   __SAVE_LOOP
  
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5, #0
__STEP1_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  STR   R2,[R0, R6]  ; store background pattern
  ADD   R5,R5,#4
  CMP   R5, #20
  BLE   __STEP1_LOOP
  
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
  MOVS  R5, #0
__STEP2_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattern
  ADD   R5,R5,#4
  CMP   R5, #20
  BLE   __STEP2_LOOP

 IF :DEF:USE_MARCHX_TEST
 ELSE
; *** Step 3 *** (not used at March-X test)
; Verify inverted background and write background with addresses increasing  
  MOVS  R5, #0
__STEP3_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   __STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  ADD   R5,R5,#4
  CMP   R5, #20
  BLE   __STEP3_LOOP
  
; *** Step 4 *** (not used at March-X test)
; Verify background and write inverted background with addresses decreasing  
  MOVS  R5, #24
__STEP4_LOOP
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattrern
  CMP   R5, #0
  BHI   __STEP4_LOOP
 ENDIF                ; March-X
  
; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
  MOVS  R5, #24
__STEP5_LOOP
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   __STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  CMP   R5, #0
  BHI   __STEP5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
  MOVS  R5, #0
__STEP6_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  ADD   R5,R5,#4
  CMP   R5, #20
  BLE   __STEP6_LOOP

  MOVS  R5, #24      ; restore content of the RAM slice back from the backup buffer
__RESTORE_LOOP
  LDR   R7,[R1, R5]  ; (first and last items are used for testing purpose exclusively)
  SUB   R5,R5,#4     ; original data are stored starting from second item of the buffer
  LDR   R6,[R4, R5]  ; load data offset
  STR   R7,[R0, R6]  ; load data from RAM
  CMP   R5, #0
  BHI   __RESTORE_LOOP
  
  B     __MARCH_RET

; ************** test of the buffer itself ********************
__BUFF_TEST
; *** Step 1 *** 
; Write background pattern with addresses increasing
  MOVS  R5, #4
__BUFF1_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  STR   R2,[R0, R6]  ; store background pattern
  ADD   R5,R5,#4
  CMP   R5, #32
  BLE   __BUFF1_LOOP
  
; *** Step 2 ***
; Verify background and write inverted background with addresses increasing
  MOVS  R5, #4
__BUFF2_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattern
  ADD   R5,R5,#4
  CMP   R5, #32
  BLE   __BUFF2_LOOP
  
 IF :DEF:USE_MARCHX_TEST
 ELSE
; *** Step 3 *** (not used at March-X test)
; Verify inverted background and write background with addresses increasing  
  MOVS  R5, #4
__BUFF3_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   __STEP_ERR
  STR   R2,[R0, R6]  ; store  background pattern
  ADD   R5,R5,#4
  CMP   R5, #32
  BLE   __BUFF3_LOOP

; *** Step 4 *** (not used at March-X test)
; Verify background and write inverted background with addresses decreasing  
  MOVS  R5, #36
__BUFF4_LOOP
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  STR   R3,[R0, R6]  ; store inverted background pattrern
  CMP   R5, #4
  BHI   __BUFF4_LOOP
 ENDIF                ; March-X

; *** Step 5 ***
; Verify inverted background and write background with addresses decreasing 
  MOVS  R5, #36
__BUFF5_LOOP
  SUBS  R5,R5,#4
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify inverted background pattern
  CMP   R7, R3
  BNE   __STEP_ERR
  STR   R2,[R0, R6]  ; store background pattrern
  CMP   R5, #4
  BHI   __BUFF5_LOOP

; *** Step 6 ***
; Verify background with addresses increasing
  MOVS  R5, #4
__BUFF6_LOOP
  LDR   R6,[R4, R5]  ; load data offset
  LDR   R7,[R0, R6]  ; verify background pattern
  CMP   R7, R2
  BNE   __STEP_ERR
  ADD   R5,R5,#4
  CMP   R5, #32
  BLE   __BUFF6_LOOP

__MARCH_RET
  LDR   R4,=ISRCtrlFlowCntInv  ; Control flow control
  LDR   R5,[R4]
  SUBS  R5,R5,#11
  STR   R5,[R4]
  
  MOVS  R0, #1       ; Correct return
  B     __STEP_RET
  
__STEP_ERR
  MOVS  R0, #0       ; error result
  
__STEP_RET
  POP   {R4-R7}
  BX    LR           ; return to the caller
  NOP
  END


