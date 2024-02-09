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
 * @file bsp_usart.c
 * @author Nations Solution Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
 
#include "bsp_usart.h"

#include "iap.h"
#include "n32g4fr_crc.h"
#include "string.h"


/**
 * @brief  Configure nested vector interrupt controller NVIC.
 */
void NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;

    /* Select nested vector interrupt controller group */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Configure USART as interrupt source */
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
    /*Set the priority*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /*Set the sub priority */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /*Enable interrupt */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* Initialize configuration NVIC */
    NVIC_Init(&NVIC_InitStructure);
	
	USART_ClrIntPendingBit(DEBUG_USARTx, USART_INT_RXDNE);  
}

/**
 * @brief  Configure parameters of usart port.
 */
void USART_Config(void)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;

    // Turn on the clock of usart port GPIO
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

    // Turn on the clock of usart peripheral
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

    // Configure GPIO of USART TX as push pull multiplexing mode
    GPIO_InitStructure.Pin        = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitPeripheral(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // Configure GPIO of USART RX as floating input mode
    GPIO_InitStructure.Pin       = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    // Configure parameters of usart port
    // Configure baud rate
    USART_InitStructure.BaudRate = DEBUG_USART_BAUDRATE;
    // Configure the length of frame data
    USART_InitStructure.WordLength = USART_WL_8B;
    // Configure stop bits
    USART_InitStructure.StopBits = USART_STPB_1;
    // Configure check bit
    USART_InitStructure.Parity = USART_PE_NO;
    // Configure hardware flow control
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    // Configure working mode, send and receive together
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    // Complete initialization configuration of usart port
    USART_Init(DEBUG_USARTx, &USART_InitStructure);

    // Configuration interrupt priority of the usart port
    NVIC_Configuration();

    // Enable usart port receive interrupt
    USART_ConfigInt(DEBUG_USARTx, USART_INT_RXDNE, ENABLE);

    // Enable usart
    USART_Enable(DEBUG_USARTx, ENABLE);
}

/*****************  Send one byte data **********************/
void Usart_SendByte(USART_Module* pUSARTx, uint8_t ch)
{
    /* Send one byte data to usart */
    USART_SendData(pUSARTx, ch);

    /* Waiting to send data register is empty */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXDE) == RESET)
        ;
}

/****************** Send an array of 8 bit************************/
void Usart_SendArray(USART_Module* pUSARTx, uint8_t* array, uint16_t num)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        /* Send one byte data to usart */
        Usart_SendByte(pUSARTx, array[i]);
    }
    /* Wait to send finished */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXC) == RESET)
        ;
}

/*****************  Send string **********************/
void Usart_SendString(USART_Module* pUSARTx, char* str)
{
    unsigned int k = 0;
    do
    {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (*(str + k) != '\0');

    /* Wait to send finished */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXC) == RESET)
    {
    }
}

/*****************  Send a 16 bits number **********************/
void Usart_SendHalfWord(USART_Module* pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* Take out the high byte */
    temp_h = (ch & 0XFF00) >> 8;
    /* Take out the loe byte */
    temp_l = ch & 0XFF;

    /* Send the high byte */
    USART_SendData(pUSARTx, temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXDE) == RESET)
        ;

    /* Send the low byte */
    USART_SendData(pUSARTx, temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXDE) == RESET)
        ;
}

// Redirect C library printf function to USART port.After rewriting, printf function can be used.
int fputc(int ch, FILE* f)
{
    /* Send one byte data to the usart port */
    USART_SendData(DEBUG_USARTx, (uint8_t)ch);

    /* Wait to send finished */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXDE) == RESET)
        ;

    return (ch);

    //    return ITM_SendChar(ch);
}

// Redirect C library scanf function to USART port.After rewriting, functions such as scanf, getchar can be used.
int fgetc(FILE* f)
{
    /* Waiting for usart port input data */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXDNE) == RESET)
        ;

    return (int)USART_ReceiveData(DEBUG_USARTx);
}

extern uint32_t receive_cnt;
extern uint8_t uart_rx_buf[];
extern uint8_t uart_receiveBIN_ok;

uint16_t crc16_value = 0;
uint16_t crc16_value_temp = 0;


uint8_t RX_buf[256] = {0};

uint16_t rx_nunber = 0;
uint8_t current_pack_length = 0;

uint8_t flash_buf[2048] = {0};

extern uint8_t f_IAP_flashing;
extern uint8_t f_IAP_start;
extern uint16_t slot_timer;
extern uint8_t f_receive_frame;
extern uint8_t f_final_frame;

void send_ack(void)
{
	uint8_t data_buf[6];
	//
	data_buf[0] = RX_buf[0];
	data_buf[1] = RX_buf[1];
	data_buf[2] = RX_buf[2];
	data_buf[3] = 0;
	data_buf[4] = 0x100 - (RX_buf[0] + RX_buf[1] + RX_buf[2]) ;
	
	Usart_SendArray(DEBUG_USARTx, data_buf, 5);
}
void USART1_IRQHandler(void)
{
    uint8_t i = 0;
    uint8_t buf_temp[256] = {0};
    uint8_t sum_check = 0;
    //
    if(USART_GetFlagStatus(DEBUG_USARTx, USART_INT_RXDNE) != RESET)
    {
        USART_ClrIntPendingBit(DEBUG_USARTx, USART_INT_RXDNE);
        slot_timer = 0;
        if(receive_cnt <= 134)
        {
            RX_buf[receive_cnt++] = USART_ReceiveData(DEBUG_USARTx);
            current_pack_length = RX_buf[3]+5; 													//Calculate the data length of the current pack
            if((RX_buf[0] == 0x01)&&(RX_buf[1] == 0x01)&& (receive_cnt== current_pack_length)) 	//Frame header is fixed to 0x01, 0x01
            {																					//pack length is fixed to uart_rx_buf[3] + 5 bytes
                receive_cnt = 0; 																//Maximum 128+5 bytes
                f_receive_frame = 1;
                memcpy(buf_temp,RX_buf,256);
                for(i = 0; i <current_pack_length -1; i++)
                {
                    sum_check = sum_check + buf_temp[i]; 										//Calculate SUM check
                }
                sum_check = ~sum_check + 1;
                if((sum_check == buf_temp[current_pack_length-1])&&(f_IAP_flashing==0)) 		//Compare SUM, if flash is being written, discard the current packet, and wait for the host computer to resend
                {
                    send_ack(); 																//Respond to the host computer
                    memcpy(&flash_buf[rx_nunber*128],&RX_buf[4],current_pack_length-5); 		//Dump data to flash_buf
                    rx_nunber ++;
                    if(rx_nunber >= 16) 														//After receiving 16 times for a total of 2K, write a flash
                    {
                        rx_nunber = 0;
                        f_IAP_flashing = 1;
                        f_IAP_start = 1;
                    }
                    else if((current_pack_length==5)&&(RX_buf[3]==0)) 							//After sending the last packet of bin content, the host computer will send a 5-byte frame end
                    {
                        rx_nunber = 0;
                        f_IAP_flashing = 1;
                        f_final_frame = 1;
                    }
                    current_pack_length = 0;
                }
                memset(RX_buf,0x00,sizeof(RX_buf));
            }
        }
        
    }
}

