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
 * @file n32g45x_xfmc.c
 * @author Nations
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g45x_xfmc.h"
#include "n32g45x_rcc.h"

/** @addtogroup N32G45X_StdPeriph_Driver
 * @{
 */

/** @addtogroup XFMC
 * @brief XFMC driver modules
 * @{
 */

/** @addtogroup XFMC_Private_Functions
 * @{
 */

/**
 * @brief  Deinitializes the XFMC NOR/SRAM Banks registers to their default
 *         reset values.
 * @param Block specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK1_BLOCK1   XFMC Bank1 NOR/SRAM1
 *     @arg XFMC_BANK1_BLOCK2   XFMC Bank1 NOR/SRAM2
 * @retval None 
 */
void XFMC_DeInitNorSram(XFMC_Bank1_Block *Block)
{
    /* Check the parameter */
    assert_param(IS_XFMC_NOR_SRAM_BLOCK(Block));

    /* XFMC_BANK1_BLOCK1 */
    if (Block == XFMC_BANK1_BLOCK1)
    {
        Block->CRx = XFMC_NOR_SRAM_CR1_RESET;
    }
    /* XFMC_BANK1_BLOCK2 */
    else
    {
        Block->CRx = XFMC_NOR_SRAM_CR2_RESET;
    }
    
    Block->TRx   = XFMC_NOR_SRAM_TR_RESET;
    Block->WTRx  = XFMC_NOR_SRAM_WTR_RESET;
}

/**
 * @brief  Deinitializes the XFMC NAND Banks registers to their default reset values.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2_NAND XFMC Bank2 NAND
 *     @arg XFMC_BANK3_NAND XFMC Bank3 NAND
 * @retval None 
 */
void XFMC_DeInitNand(XFMC_Bank23_Module *Bank)
{
    /* Check the parameter */
    assert_param(IS_XFMC_NAND_BANK(Bank));

    Bank->CTRLx     = XFMC_NAND_CTRL_RESET;
    Bank->STSx      = XFMC_NAND_STS_RESET;
    Bank->CMEMTMx   = XFMC_NAND_CMEMTM_RESET;
    Bank->ATTMEMTMx = XFMC_NAND_ATTMEMTM_RESET;
}

/**
 * @brief  Initializes the XFMC NOR/SRAM Banks according to the specified
 *         parameters in the XFMC_NORSRAMInitStruct.
 * @param InitStruct pointer to a XFMC_NorSramInitTpye
 *         structure that contains the configuration information for
 *        the XFMC NOR/SRAM specified Banks.
 * @retval None 
 */
void XFMC_InitNorSram(XFMC_NorSramInitTpye* InitStruct)
{
    uint32_t NorAccess;
    
    /* Check the parameters */
    assert_param(IS_XFMC_NOR_SRAM_BLOCK(InitStruct->Block));
    assert_param(IS_XFMC_NOR_SRAM_MUX(InitStruct->DataAddrMux));
    assert_param(IS_XFMC_NOR_SRAM_MEMORY(InitStruct->MemType));
    assert_param(IS_XFMC_NOR_SRAM_MEMORY_WIDTH(InitStruct->MemDataWidth));
    assert_param(IS_XFMC_NOR_SRAM_BURSTMODE(InitStruct->BurstAccMode));
    assert_param(IS_XFMC_NOR_SRAM_WAIT_POLARITY(InitStruct->WaitSigPolarity));
    assert_param(IS_XFMC_NOR_SRAM_WRAP_MODE(InitStruct->WrapMode));
    assert_param(IS_XFMC_NOR_SRAM_WAIT_SIGNAL_ACTIVE(InitStruct->WaitSigConfig));
    assert_param(IS_XFMC_NOR_SRAM_WRITE_OPERATION(InitStruct->WriteEnable));
    assert_param(IS_XFMC_NOR_SRAM_WAITE_SIGNAL(InitStruct->WaitSigEnable));
    assert_param(IS_XFMC_NOR_SRAM_EXTENDED_MODE(InitStruct->ExtModeEnable));
    assert_param(IS_XFMC_NOR_SRAM_ASYNWAIT(InitStruct->AsynchroWait));
    assert_param(IS_XFMC_NOR_SRAM_PAGESIZE(InitStruct->PageSize));
    assert_param(IS_XFMC_NOR_SRAM_WRITE_BURST(InitStruct->WriteBurstEnable));
    
    if (InitStruct->MemType == XFMC_MEM_TYPE_NOR)
        NorAccess = XFMC_NOR_SRAM_ACC_ENABLE;
    else
        NorAccess = XFMC_NOR_SRAM_ACC_DISABLE;

    /* Bank1 NOR/SRAM control register configuration */
    InitStruct->Block->CRx =  InitStruct->DataAddrMux
                            | InitStruct->MemType
                            | InitStruct->MemDataWidth 
                            | NorAccess
                            | InitStruct->BurstAccMode
                            | InitStruct->WaitSigPolarity
                            | InitStruct->WrapMode 
                            | InitStruct->WaitSigConfig 
                            | InitStruct->WriteEnable
                            | InitStruct->WaitSigEnable 
                            | InitStruct->ExtModeEnable
                            | InitStruct->AsynchroWait 
                            | InitStruct->PageSize 
                            | InitStruct->WriteBurstEnable;
}

/**
 * @brief  Initializes the XFMC NOR/SRAM read/write timing in normal mode,
           or only read timing in extended mode.
 * @param Block specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK1_BLOCK1   XFMC Bank1 NOR/SRAM1
 *     @arg XFMC_BANK1_BLOCK2   XFMC Bank1 NOR/SRAM2
 * @param Timing pointer to a XFMC_NorSramTimingInitType structure that contain 
               read write timing infomation.
 * @retval None 
 */
void XFMC_InitNorSramTiming(XFMC_Bank1_Block *Block, XFMC_NorSramTimingInitType* Timing)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NOR_SRAM_ADDR_SETUP_TIME(Timing->AddrSetTime));
    assert_param(IS_XFMC_NOR_SRAM_ADDR_HOLD_TIME(Timing->AddrHoldTime));
    assert_param(IS_XFMC_NOR_SRAM_DATASETUP_TIME(Timing->DataSetTime));
    assert_param(IS_XFMC_NOR_SRAM_BUSRECOVERY_TIME(Timing->BusRecoveryCycle));
    assert_param(IS_XFMC_NOR_SRAM_CLK_DIV(Timing->ClkDiv));
    assert_param(IS_XFMC_NOR_SRAM_DATA_LATENCY(Timing->DataLatency));
    assert_param(IS_XFMC_NOR_SRAM_ACCESS_MODE(Timing->AccMode));

    /* Bank1 NOR/SRAM timing register configuration */
    Block->TRx =  Timing->AddrSetTime
                | Timing->AddrHoldTime
                | Timing->DataSetTime
                | Timing->BusRecoveryCycle
                | Timing->ClkDiv
                | Timing->DataLatency
                | Timing->AccMode;
}

/**
 * @brief  Initializes the XFMC NOR/SRAM write timing in extended mode.
 *         parameters in the XFMC_NORSRAMInitStruct.
 * @param Block specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK1_BLOCK1   XFMC Bank1 NOR/SRAM1
 *     @arg XFMC_BANK1_BLOCK2   XFMC Bank1 NOR/SRAM2
 * @param ExtTiming pointer to a XFMC_NorSramTimingInitType structure that contain 
               read write timing infomation.
 * @retval None 
 */
void XFMC_InitNorSramExtendTiming(XFMC_Bank1_Block *Block, XFMC_NorSramTimingInitType* ExtTiming)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NOR_SRAM_ADDR_SETUP_TIME(ExtTiming->AddrSetTime));
    assert_param(IS_XFMC_NOR_SRAM_ADDR_HOLD_TIME(ExtTiming->AddrHoldTime));
    assert_param(IS_XFMC_NOR_SRAM_DATASETUP_TIME(ExtTiming->DataSetTime));
    assert_param(IS_XFMC_NOR_SRAM_CLK_DIV(ExtTiming->ClkDiv));
    assert_param(IS_XFMC_NOR_SRAM_DATA_LATENCY(ExtTiming->DataLatency));
    assert_param(IS_XFMC_NOR_SRAM_ACCESS_MODE(ExtTiming->AccMode));

    /* Bank1 NOR/SRAM timing register configuration */
    Block->WTRx = ExtTiming->AddrSetTime
                | ExtTiming->AddrHoldTime
                | ExtTiming->DataSetTime
                | ExtTiming->ClkDiv
                | ExtTiming->DataLatency
                | ExtTiming->AccMode;
}

/**
 * @brief  Initializes the XFMC NAND Banks according to the specified
 *         parameters in the XFMC_NANDInitStruct.
 * @param InitStruct pointer to a XFMC_NandInitType
 *         structure that contains the configuration information for the XFMC
 *         NAND specified Banks.
 * @retval None 
 */
void XFMC_InitNand(XFMC_NandInitType* InitStruct)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NAND_BANK(InitStruct->Bank));
    assert_param(IS_XFMC_NAND_WAIT_FEATURE(InitStruct->WaitFeatureEnable));
    assert_param(IS_XFMC_NAND_BUS_WIDTH(InitStruct->MemDataWidth));
    assert_param(IS_XFMC_ECC_STATE(InitStruct->EccEnable));
    assert_param(IS_XFMC_NAND_ECC_PAGE_SIZE(InitStruct->EccPageSize));
    assert_param(IS_XFMC_NAND_ALE_DELAY(InitStruct->TARSetTime));
    assert_param(IS_XFMC_NAND_CLE_DELAY(InitStruct->TCLRSetTime));

    InitStruct->Bank->CTRLx =   XFMC_BANK23_MEM_TYPE_NAND
                            |   InitStruct->WaitFeatureEnable 
                            |   InitStruct->MemDataWidth
                            |   InitStruct->EccEnable
                            |   InitStruct->EccPageSize
                            |   InitStruct->TCLRSetTime
                            |   InitStruct->TARSetTime;
}

/**
 * @brief  Initializes the XFMC NAND Banks according to the specified
 *         parameters in the XFMC_NANDInitStruct.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2_NAND XFMC Bank2 NAND
 *     @arg XFMC_BANK3_NAND XFMC Bank3 NAND
 * @param CommonTiming pointer to a XFMC_NandTimingInitType structure that 
 *         contains the read write timing information for the nand common sapce.
 * @retval None 
 */
void XFMC_InitNandCommonTiming(XFMC_Bank23_Module   * Bank, XFMC_NandTimingInitType* CommonTiming)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NAND_SETUP_TIME(CommonTiming->SetTime));
    assert_param(IS_XFMC_NAND_WAIT_TIME(CommonTiming->WaitSetTime));
    assert_param(IS_XFMC_NAND_HOLD_TIME(CommonTiming->HoldSetTime));
    assert_param(IS_XFMC_NAND_HIZ_TIME(CommonTiming->HiZSetTime));

    Bank->CMEMTMx =   (CommonTiming->SetTime    << XFMC_CMEMTM_SET_SHIFT)
                    | (CommonTiming->WaitSetTime<< XFMC_CMEMTM_WAIT_SHIFT)
                    | (CommonTiming->HoldSetTime<< XFMC_CMEMTM_HLD_SHIFT)
                    | (CommonTiming->HiZSetTime << XFMC_CMEMTM_HIZ_SHIFT);
}

/**
 * @brief  Initializes the XFMC NAND Banks according to the specified
 *         parameters in the XFMC_NANDInitStruct.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2_NAND XFMC Bank2 NAND
 *     @arg XFMC_BANK3_NAND XFMC Bank3 NAND
 * @param AttributeTiming pointer to a XFMC_NandTimingInitType structure that 
 *         contains the read write timing information for the nand attribute sapce.
 * @retval None 
 */
void XFMC_InitNandAttributeTiming(XFMC_Bank23_Module   * Bank, XFMC_NandTimingInitType* AttributeTiming)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NAND_SETUP_TIME(AttributeTiming->SetTime));
    assert_param(IS_XFMC_NAND_WAIT_TIME(AttributeTiming->WaitSetTime));
    assert_param(IS_XFMC_NAND_HOLD_TIME(AttributeTiming->HoldSetTime));
    assert_param(IS_XFMC_NAND_HIZ_TIME(AttributeTiming->HiZSetTime));

    Bank->ATTMEMTMx = (AttributeTiming->SetTime     << XFMC_ATTMEMTM_SET_SHIFT)
                    | (AttributeTiming->WaitSetTime << XFMC_ATTMEMTM_WAIT_SHIFT)
                    | (AttributeTiming->HoldSetTime << XFMC_ATTMEMTM_HLD_SHIFT)
                    | (AttributeTiming->HiZSetTime  << XFMC_ATTMEMTM_HIZ_SHIFT);
}

/**
 * @brief  Enables or disables the specified NOR/SRAM Memory Bank.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK1_BLOCK1 XFMC Bank1 NOR/SRAM block1
 *     @arg XFMC_BANK1_BLOCK2 XFMC Bank1 NOR/SRAM block2
 * @param Cmd new state of the Bank. This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void XFMC_EnableNorSram(XFMC_Bank1_Block *Block, FunctionalState Cmd)
{
    assert_param(IS_XFMC_NOR_SRAM_BLOCK(Block));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        /* Enable the selected NOR/SRAM Bank by setting the PBKEN bit in the BCRx register */
        Block->CRx |= XFMC_NOR_SRAM_ENABLE;
    }
    else
    {
        /* Disable the selected NOR/SRAM Bank by clearing the PBKEN bit in the BCRx register */
        Block->CRx &= ~XFMC_NOR_SRAM_ENABLE;
    }
}

/**
 * @brief  Enables or disables the specified NAND Memory Bank.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2  XFMC Bank2 NAND
 *     @arg XFMC_BANK3  XFMC Bank3 NAND
 * @param Cmd new state of the Bank. This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void XFMC_EnableNand(XFMC_Bank23_Module *Bank, FunctionalState Cmd)
{
    assert_param(IS_XFMC_NAND_BANK(Bank));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        /* Enable the selected NAND Bank by setting the PBKEN bit in the PCRx register */
        Bank->CTRLx |= XFMC_NAND_BANK_ENABLE;
    }
    else
    {
        /* Disable the selected NAND Bank by clearing the PBKEN bit in the PCRx register */
        Bank->CTRLx &= ~XFMC_NAND_BANK_ENABLE;
    }
}

/**
 * @brief  Enables or disables the XFMC NAND ECC feature.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2  XFMC Bank2 NAND
 *     @arg XFMC_BANK3  XFMC Bank3 NAND
 * @param Cmd new state of the XFMC NAND ECC feature.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void XFMC_EnableNandEcc(XFMC_Bank23_Module *Bank, FunctionalState Cmd)
{
    assert_param(IS_XFMC_NAND_BANK(Bank));
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        /* Enable the selected NAND Bank ECC function by setting the ECCEN bit in the PCRx register */
        Bank->CTRLx |= XFMC_NAND_ECC_ENABLE;
    }
    else
    {
        /* Disable the selected NAND Bank ECC function by clearing the ECCEN bit in the PCRx register */
        Bank->CTRLx &= ~XFMC_NAND_ECC_ENABLE;
    }
}

/**
 * @brief  Clear ECC result and start a new ECC process.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2  XFMC Bank2 NAND
 *     @arg XFMC_BANK3  XFMC Bank3 NAND
 * @retval None
 */
void XFMC_RestartNandEcc(XFMC_Bank23_Module *Bank)
{
    Bank->CTRLx &= ~XFMC_NAND_ECC_ENABLE;
    Bank->CTRLx |= XFMC_NAND_ECC_ENABLE;
}

/**
 * @brief  Returns the error correction code register value.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2  XFMC Bank2 NAND
 *     @arg XFMC_BANK3  XFMC Bank3 NAND
 * @retval The Error Correction Code (ECC) value.
 */
uint32_t XFMC_GetEcc(XFMC_Bank23_Module *Bank)
{
    uint32_t tEccPageSize,tECC = 0;
    
    assert_param(IS_XFMC_NAND_BANK(Bank));

    tEccPageSize = Bank->CTRLx & XFMC_CTRL_ECCPGS_MASK;

    switch(tEccPageSize)
    {
        case XFMC_NAND_ECC_PAGE_256BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PAGE_256BYTE_MASK;
                break;
        case XFMC_NAND_ECC_PAGE_512BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PAGE_512BYTE_MASK;
                break;
        case XFMC_NAND_ECC_PAGE_1024BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PBAE_1024BYTE_MASK;
                break;
        case XFMC_NAND_ECC_PAGE_2048BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PBAE_2048BYTE_MASK;
                break;
        case XFMC_NAND_ECC_PAGE_4096BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PBAE_4096BYTE_MASK;
                break;
        case XFMC_NAND_ECC_PAGE_8192BYTES:
                tECC = Bank->ECCx & XFMC_ECC_PBAE_8192BYTE_MASK;
                break;
        default:
                break;
    }
    
    /* Return the error correction code value */
    return (tECC);
}

/**
 * @brief  Checks whether the specified XFMC flag is set or not.
 * @param Bank specifies the XFMC Bank to be used
 *   This parameter can be one of the following values:
 *     @arg XFMC_BANK2  XFMC Bank2 NAND
 *     @arg XFMC_BANK3  XFMC Bank3 NAND
 * @param XFMC_FLAG specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg XFMC_FLAG_FIFO_EMPTY Fifo empty Flag.
 * @retval The new state of XFMC_FLAG (SET or RESET).
 */
uint32_t XFMC_GetFlag(XFMC_Bank23_Module *Bank, uint32_t XFMC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_XFMC_NAND_BANK(Bank));
    assert_param(IS_XFMC_NAND_FLAG(XFMC_FLAG));

    /* Get the flag status */
    if ((Bank->STSx & XFMC_FLAG) == XFMC_FLAG)
        return SET;
    else
        return RESET;
}

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */
