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
 * @file i2c_eeprom.c
 * @author Nations
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32g45x.h"
#include "n32g45x_dma.h"
#include "i2c_eeprom.h"
#include "string.h"
#include "stdbool.h"

/** @addtogroup I2C_EEPROM
 * @{
 */

/* when EEPROM is writing data inside,it won't response the request from the master.check the ack,
if EEPROM response,make clear that EEPROM finished the last data-writing,allow the next operation */
u32 sEETimeout = sEE_LONG_TIMEOUT;
u16 I2C_NumByteToWrite   = 0;
u8 I2C_NumByteWritingNow = 0;
u8* I2C_pBuffer          = 0;
u16 I2C_WriteAddr        = 0;
/* global state variable i2c_comm_state */
volatile I2C_STATE i2c_comm_state;

/**
 * @brief  Timeout callback used by the I2C EEPROM driver.
 */
u8 sEE_TIMEOUT_UserCallback(void)
{
    printf("error!!!\r\n");
    /* Block communication and all processes */
    while (1)
    {
    }
}

/**
 * @brief  Initializes peripherals used by the I2C EEPROM driver.
 */
void I2C_EE_Init(void)
{
    /** GPIO configuration and clock enable */
    GPIO_InitType GPIO_InitStructure;
    I2C_InitType I2C_InitStructure;
    /** enable peripheral clk*/
    I2Cx_peripheral_clk_en();
    I2C_DeInit(I2Cx);

    I2Cx_scl_clk_en();
    I2Cx_sda_clk_en();

#ifdef I2C1_REMAP
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    GPIO_ConfigPinRemap(GPIO_RMP_I2C1, ENABLE);
#endif
    GPIO_InitStructure.Pin        = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);

    /** I2C periphral configuration */
    I2C_DeInit(I2Cx);
    I2C_InitStructure.BusMode     = I2C_BUSMODE_I2C;
    I2C_InitStructure.FmDutyCycle = I2C_FMDUTYCYCLE_2;
    I2C_InitStructure.OwnAddr1    = 0xff;
    I2C_InitStructure.AckEnable   = I2C_ACKEN;
    I2C_InitStructure.AddrMode    = I2C_ADDR_MODE_7BIT;
    I2C_InitStructure.ClkSpeed    = I2C_Speed;
    I2C_Init(I2Cx, &I2C_InitStructure);
}

/**
 * @brief  Writes buffer of data to the I2C EEPROM.
 * @param pBuffer pointer to the buffer  containing the data to be
 *                  written to the EEPROM.
 * @param WriteAddr EEPROM's internal address to write to.
 * @param NumByteToWrite number of bytes to write to the EEPROM.
 */
void I2C_EE_WriteBuffer(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
    if (I2C_GetFlag(I2Cx, I2C_FLAG_BUSY))
    {
        return;
    }
    I2C_pBuffer        = pBuffer;
    I2C_WriteAddr      = WriteAddr;
    I2C_NumByteToWrite = NumByteToWrite;

    while (I2C_NumByteToWrite > 0)
    {
        I2C_EE_WriteOnePage(I2C_pBuffer, I2C_WriteAddr, I2C_NumByteToWrite);
    }
}

/**
 * @brief  Writes a page of data to the I2C EEPROM, general called by
 *         I2C_EE_WriteBuffer.
 * @param pBuffer pointer to the buffer  containing the data to be
 *                  written to the EEPROM.
 * @param WriteAddr EEPROM's internal address to write to.
 * @param NumByteToWrite number of bytes to write to the EEPROM.
 */
void I2C_EE_WriteOnePage(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

    Addr        = WriteAddr % I2C_PageSize;
    count       = I2C_PageSize - Addr;
    NumOfPage   = NumByteToWrite / I2C_PageSize;
    NumOfSingle = NumByteToWrite % I2C_PageSize;

    I2C_NumByteWritingNow = 0;
    /** If WriteAddr is I2C_PageSize aligned */
    if (Addr == 0)
    {
        /** If NumByteToWrite < I2C_PageSize */
        if (NumOfPage == 0)
        {
            I2C_NumByteWritingNow = NumOfSingle;
            I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
        /** If NumByteToWrite > I2C_PageSize */
        else
        {
            I2C_NumByteWritingNow = I2C_PageSize;
            I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        }
    }
    /** If WriteAddr is not I2C_PageSize aligned */
    else
    {
        /* If NumByteToWrite < I2C_PageSize */
        if (NumOfPage == 0)
        {
            I2C_NumByteWritingNow = NumOfSingle;
            I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
        /* If NumByteToWrite > I2C_PageSize */
        else
        {
            if (count != 0)
            {
                I2C_NumByteWritingNow = count;
                I2C_EE_PageWrite(pBuffer, WriteAddr, count);
            }
        }
    }
}

/**
 * @brief  Writes more than one byte to the EEPROM with a single WRITE
 *         cycle. The number of byte can't exceed the EEPROM page size.
 * @param pBuffer pointer to the buffer containing the data to be
 *                  written to the EEPROM.
 * @param WriteAddr EEPROM's internal address to write to (1-16).
 * @param NumByteToWrite number of bytes to write to the EEPROM.
 */
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
    sEETimeout = sEE_LONG_TIMEOUT;
    while (I2C_GetFlag(I2Cx, I2C_FLAG_BUSY))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send START condition */
    I2C_GenerateStart(I2Cx, ENABLE);
    /** Test on EV5 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_MODE_FLAG))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send EEPROM address for write */
    I2C_SendAddr7bit(I2Cx, EEPROM_ADDRESS, I2C_DIRECTION_SEND);
    /** Test on EV6 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_TXMODE_FLAG))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send the EEPROM's internal address to write to */
    I2C_SendData(I2Cx, WriteAddr);
    /** Test on EV8 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_DATA_SENDED))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** While there is data to be written */
    while (NumByteToWrite--)
    {
        /** Send the current byte */
        I2C_SendData(I2Cx, *pBuffer);
        /** Point to the next byte to be written */
        pBuffer++;
        /** Test on EV8 and clear it */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_DATA_SENDED))
        {
            if ((sEETimeout--) == 0)
                sEE_TIMEOUT_UserCallback();
        }
    }
    /** Send STOP condition */
    I2C_GenerateStop(I2Cx, ENABLE);
    I2C_EE_WaitEepromStandbyState();
    I2C_EE_WriteOnePageCompleted();
}

/**
 * @brief  Process Write one page completed.
 */
void I2C_EE_WriteOnePageCompleted(void)
{
    I2C_pBuffer += I2C_NumByteWritingNow;
    I2C_WriteAddr += I2C_NumByteWritingNow;
    I2C_NumByteToWrite -= I2C_NumByteWritingNow;
}

/**
 * @brief  Reads a block of data from the EEPROM.
 * @param pBuffer pointer to the buffer that receives the data read
 *                  from the EEPROM.
 * @param ReadAddr EEPROM's internal address to read from.
 * @param NumByteToRead number of bytes to read from the EEPROM.
 */
void I2C_EE_ReadBuffer(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)
{
    sEETimeout = sEE_LONG_TIMEOUT;
    while (I2C_GetFlag(I2Cx, I2C_FLAG_BUSY))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    I2Cx->CTRL1 &= ~0x0800; // clear POSEN
    I2C_ConfigAck(I2Cx, ENABLE);
    /** Send START condition */
    I2C_GenerateStart(I2Cx, ENABLE);

    /** Test on EV5 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_MODE_FLAG))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send EEPROM address for write */
    I2C_SendAddr7bit(I2Cx, EEPROM_ADDRESS, I2C_DIRECTION_SEND);
    /** Test on EV6 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_TXMODE_FLAG))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Clear EV6 by setting again the PE bit */
    I2C_Enable(I2Cx, ENABLE);
    /** Send the EEPROM's internal address to write to */
    I2C_SendData(I2Cx, ReadAddr);
    /** Test on EV8 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_DATA_SENDED))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send STRAT condition a second time */
    I2C_GenerateStart(I2Cx, ENABLE);
    /** Test on EV5 and clear it */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_MODE_FLAG))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
    /** Send EEPROM address for read */
    I2C_SendAddr7bit(I2Cx, EEPROM_ADDRESS, I2C_DIRECTION_RECV);
    sEETimeout = sEE_LONG_TIMEOUT;
    while (!I2C_GetFlag(I2Cx, I2C_FLAG_ADDRF))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }

    /** While there is data to be read */
    if (NumByteToRead == 1)
    {
        /** Disable Acknowledgement */
        I2C_ConfigAck(I2Cx, DISABLE);
        (void)(I2Cx->STS1); /// clear ADDR
        (void)(I2Cx->STS2);
        I2C_GenerateStop(I2Cx, ENABLE);
    }
    else if (NumByteToRead == 2)
    {
        I2C1->CTRL1 |= I2C_NACK_POS_NEXT; /// set ACKPOS
        (void)(I2Cx->STS1);
        (void)(I2Cx->STS2);
        I2C_ConfigAck(I2Cx, DISABLE);
    }
    else
    {
        I2C_ConfigAck(I2Cx, ENABLE);
        (void)(I2Cx->STS1);
        (void)(I2Cx->STS2);
    }
    while (NumByteToRead)
    {
        if (NumByteToRead <= 3)
        {
            /** One byte */
            if (NumByteToRead == 1)
            {
                /** Wait until RXNE flag is set */
                sEETimeout = sEE_LONG_TIMEOUT;
                while (!I2C_GetFlag(I2Cx, I2C_FLAG_RXDATNE))
                {
                    if ((sEETimeout--) == 0)
                        sEE_TIMEOUT_UserCallback();
                }
                /** Read data from DAT */
                /** Read a byte from the EEPROM */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;
            }
            /** Two bytes */
            else if (NumByteToRead == 2)
            {
                /** Wait until BTF flag is set */
                sEETimeout = sEE_LONG_TIMEOUT;
                while (!I2C_GetFlag(I2Cx, I2C_FLAG_BYTEF))
                {
                    if ((sEETimeout--) == 0)
                        sEE_TIMEOUT_UserCallback();
                }
                /** Send STOP Condition */
                I2C_GenerateStop(I2Cx, ENABLE);
                I2C1->CTRL1 &= I2C_NACK_POS_CURRENT; /// reset ACKPOS

                /** Read data from DAT */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;
                /** Read data from DAT */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;
            }
            /** 3 Last bytes */
            else
            {
                sEETimeout = sEE_LONG_TIMEOUT;
                while (!I2C_GetFlag(I2Cx, I2C_FLAG_BYTEF))
                {
                    if ((sEETimeout--) == 0)
                        sEE_TIMEOUT_UserCallback();
                }
                /** Disable Acknowledgement */
                I2C_ConfigAck(I2Cx, DISABLE);
                /** Read data from DAT */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;

                /** Wait until BTF flag is set */
                sEETimeout = sEE_LONG_TIMEOUT;
                while (!I2C_GetFlag(I2Cx, I2C_FLAG_BYTEF))
                {
                    if ((sEETimeout--) == 0)
                        sEE_TIMEOUT_UserCallback();
                }
                /** Send STOP Condition */
                I2C_GenerateStop(I2Cx, ENABLE);

                /** Read data from DAT */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;

                /** Read data from DAT */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;
            }
        }
        else
        {
            /** Test on EV7 and clear it */
            sEETimeout = sEE_LONG_TIMEOUT;
            while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_DATA_RECVD_FLAG))
            {
                if ((sEETimeout--) == 0)
                    sEE_TIMEOUT_UserCallback();
            }
            /** Read a byte from the EEPROM */
            *pBuffer = I2C_RecvData(I2Cx);
            /** Point to the next location where the byte read will be saved */
            pBuffer++;
            /** Decrement the read bytes counter */
            NumByteToRead--;
            if (I2C_GetFlag(I2Cx, I2C_FLAG_BYTEF))
            {
                /** Read a byte from the EEPROM */
                *pBuffer = I2C_RecvData(I2Cx);
                /** Point to the next location where the byte read will be saved */
                pBuffer++;
                /** Decrement the read bytes counter */
                NumByteToRead--;
            }
        }
    }
}

/**
 * @brief  wait operation is completed.
 */
void I2C_EE_WaitOperationIsCompleted(void)
{
    sEETimeout = sEE_LONG_TIMEOUT;
    while (i2c_comm_state != COMM_DONE)
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }
}

/**
 * @brief  Wait eeprom standby state.
 */
void I2C_EE_WaitEepromStandbyState(void)
{
    __IO uint16_t tmpSR1    = 0;
    __IO uint32_t sEETrials = 0;

    /** While the bus is busy */
    sEETimeout = sEE_LONG_TIMEOUT;
    while (I2C_GetFlag(I2Cx, I2C_FLAG_BUSY))
    {
        if ((sEETimeout--) == 0)
            sEE_TIMEOUT_UserCallback();
    }

    /** Keep looping till the slave acknowledge his address or maximum number
       of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER) */
    while (1)
    {
        /** Send START condition */
        I2C_GenerateStart(I2Cx, ENABLE);

        /** Test on EV5 and clear it */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (!I2C_CheckEvent(I2Cx, I2C_EVT_MASTER_MODE_FLAG))
        {
            if ((sEETimeout--) == 0)
                sEE_TIMEOUT_UserCallback();
        }

        /** Send EEPROM address for write */
        I2C_SendAddr7bit(I2Cx, EEPROM_ADDRESS, I2C_DIRECTION_SEND);
        /** Wait for ADDR flag to be set (Slave acknowledged his address) */
        sEETimeout = sEE_LONG_TIMEOUT;
        do
        {
            /** Get the current value of the STS1 register */
            tmpSR1 = I2Cx->STS1;

            /** Update the timeout value and exit if it reach 0 */
            if ((sEETimeout--) == 0)
                sEE_TIMEOUT_UserCallback();
        }
        /** Keep looping till the Address is acknowledged or the AF flag is
           set (address not acknowledged at time) */
        while ((tmpSR1 & (I2C_STS1_ADDRF | I2C_STS1_ACKFAIL)) == 0);

        /** Check if the ADDR flag has been set */
        if (tmpSR1 & I2C_STS1_ADDRF)
        {
            /** Clear ADDR Flag by reading STS1 then STS2 registers (STS1 have already
               been read) */
            (void)I2Cx->STS2;

            /** STOP condition */
            I2C_GenerateStop(I2Cx, ENABLE);

            /** Exit the function */
            return;
        }
        else
        {
            /** Clear AF flag */
            I2C_ClrFlag(I2Cx, I2C_FLAG_ACKFAIL);
        }

        /** Check if the maximum allowed numbe of trials has bee reached */
        if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
        {
            /** If the maximum number of trials has been reached, exit the function */
            sEE_TIMEOUT_UserCallback();
        }
    }
}

/**
 * @}
 */
