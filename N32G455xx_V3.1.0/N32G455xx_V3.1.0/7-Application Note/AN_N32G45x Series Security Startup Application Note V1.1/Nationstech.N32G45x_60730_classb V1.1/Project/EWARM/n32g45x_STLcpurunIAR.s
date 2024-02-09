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

  SECTION .text:CODE(2)

  ; Reference to the FailSafe routine to be executed in case of non-recoverable
  ; failure
  EXTERN FailSafePOR

    ; C variables for control flow monitoring
  EXTERN CtrlFlowCnt
  EXTERN CtrlFlowCntInv
  EXTERN conAA
  EXTERN con55

;*******************************************************************************
; Function Name  : STL_RunTimeCPUTest
; Description    : Cortex-M0 CPU test during run-time
;                  Note: when possible, BRANCH are 16-bit only (depending on
;                  relative offset to final BL instruction)
; Input          : None.
; Output         : Branch directly to a Fail Safe routine in case of failure
; Return         : CPUTEST_SUCCESS (=1) if test is ok
;*******************************************************************************/

STL_RunTimeCPUTest:
    EXPORT STL_RunTimeCPUTest

    PUSH {R4-R7}              ; Safe registers

    ; This is for control flow test (ENTRY point)
    LDR R0,=CtrlFlowCnt
    ; Assumes R1 OK; If not, error will be detected by R1 test and Ctrl flow test later on
    LDR R1,[R0]
    ADDS R1,R1,#0x3	 ; CtrlFlowCnt += OxO3
    STR R1,[R0]

    ; Register R1
    LDR R0, =conAA
    LDR R1,[R0]
    LDR R0,[R0]
    CMP R0,R1
    BNE CPUTestFail
    LDR R0, =con55
    LDR R1,[R0]
    LDR R0,[R0]
    CMP R0,R1
    BNE CPUTestFail
    MOVS R1, #0x1             ; For ramp test

    ; Register R2
    LDR R0, =conAA
    LDR R2,[R0]
    LDR R0,[R0]
    CMP R0,R2
    BNE CPUTestFail
    LDR R0, =con55
    LDR R2,[R0]
    LDR R0,[R0]
    CMP R0,R2
    BNE CPUTestFail
    MOVS R2, #0x2             ; For ramp test

    ; Register R3
    LDR R0, =conAA
    LDR R3,[R0]
    LDR R0,[R0]
    CMP R0,R3
    BNE CPUTestFail
    LDR R0, =con55
    LDR R3,[R0]
    LDR R0,[R0]
    CMP R0,R3
    BNE CPUTestFail
    MOVS R3, #0x3             ; For ramp test

    ; Register R4
    LDR R0, =conAA
    LDR R4,[R0]
    LDR R0,[R0]
    CMP R0,R4
    BNE CPUTestFail
    LDR R0, =con55
    LDR R4,[R0]
    LDR R0,[R0]
    CMP R0,R4
    BNE CPUTestFail
    MOVS R4, #0x4             ; For ramp test

    ; Register R5
    LDR R0, =conAA
    LDR R5,[R0]
    LDR R0,[R0]
    CMP R0,R5
    BNE CPUTestFail
    LDR R0, =con55
    LDR R5,[R0]
    LDR R0,[R0]
    CMP R0,R5
    BNE CPUTestFail
    MOVS R5, #0x5             ; For ramp test

    ; Register R6
    LDR R0, =conAA
    LDR R6,[R0]
    LDR R0,[R0]
    CMP R0,R6
    BNE CPUTestFail
    LDR R0, =con55
    LDR R6,[R0]
    LDR R0,[R0]
    CMP R0,R6
    BNE CPUTestFail
    MOVS R6, #0x6             ; For ramp test

    ; Register R7
    LDR R0, =conAA
    LDR R7,[R0]
    LDR R0,[R0]
    CMP R0,R7
    BNE CPUTestFail
    LDR R0, =con55
    LDR R7,[R0]
    LDR R0,[R0]
    CMP R0,R7
    BNE CPUTestFail
    MOVS R7, #0x7             ; For ramp test

    ; Register R8
    LDR R0, =conAA
    LDR R0,[R0]
    MOV R8,R0
    CMP R0,R8
    BNE CPUTestFail
    LDR R0, =con55
    LDR R0,[R0]
    MOV R8,R0
    CMP R0,R8
    BNE CPUTestFail
    MOVS R0, #0x08            ; For ramp test
    MOV	R8,R0

    BAL CPUTstCont

CPUTestFail
    BL FailSafePOR

CPUTstCont
    ; Register R9
    LDR R0, =conAA
    LDR R0,[R0]
    MOV R9,R0
    CMP R0,R9
    BNE CPUTestFail
    LDR R0, =con55
    LDR R0,[R0]
    MOV R9,R0
    CMP R0,R9
    BNE CPUTestFail
    MOVS R0, #0x09            ; For ramp test
    MOV	R9,R0

    ; Register R10
    LDR R0, =conAA
    LDR R0,[R0]
    MOV R10,R0
    CMP R0,R10
    BNE CPUTestFail
    LDR R0, =con55
    LDR R0,[R0]
    MOV R10,R0
    CMP R0,R10
    BNE CPUTestFail
    MOVS R0, #0x0A            ; For ramp test
    MOV	R10,R0

    ; Register R11
    LDR R0, =conAA
    LDR R0,[R0]
    MOV R11,R0
    CMP R0,R11
    BNE CPUTestFail
    LDR R0, =con55
    LDR R0,[R0]
    MOV R11,R0
    CMP R0,R11
    BNE CPUTestFail
    MOVS R0, #0x0B            ; For ramp test
    MOV	R11,R0

    ; Register R12
    LDR R0, =conAA
    LDR R0,[R0]
    MOV R12,R0
    CMP R0,R12
    BNE CPUTestFail
    LDR R0, =con55
    LDR R0,[R0]
    MOV R12,R0
    CMP R0,R12
    BNE CPUTestFail
    MOVS R0, #0x0C            ; For ramp test
    MOV	R12,R0
    LDR R0, =CPUTstCont

    ; Link register R14	cannot be tested an error should be detected by	Ctrl flow test later

    ; Ramp pattern verification	(R0 is not tested)
    CMP R1, #0x01
    BNE CPUTestFail
    CMP R2, #0x02
    BNE CPUTestFail
    CMP R3, #0x03
    BNE CPUTestFail
    CMP R4, #0x04
    BNE CPUTestFail
    CMP R5, #0x05
    BNE CPUTestFail
    CMP R6, #0x06
    BNE CPUTestFail
    CMP R7, #0x07
    BNE CPUTestFail
    MOVS R0, #0x08
    CMP R0,R8
    BNE CPUTestFail
    MOVS R0, #0x09
    CMP R0,R9
    BNE CPUTestFail
    MOVS R0, #0x0A
    CMP R0,R10
    BNE CPUTestFail
    MOVS R0, #0x0B
    CMP R0,R11
    BNE CPUTestFail
    MOVS R0, #0x0C
    CMP R0,R12
    BNE CPUTestFail

    ; Control flow test (EXIT point)
    LDR R0,=CtrlFlowCntInv
    LDR R1,[R0]
    SUBS R1,R1,#0x3	 ; CtrlFlowCntInv -= OxO3
    STR R1,[R0]

    POP {R4-R7}              ; Restore registers

    MOVS R0, #0x1       ; CPUTEST_SUCCESS
    BX LR               ; return to the caller

  END

;******************* (C) COPYRIGHT  *****END OF FILE*****
