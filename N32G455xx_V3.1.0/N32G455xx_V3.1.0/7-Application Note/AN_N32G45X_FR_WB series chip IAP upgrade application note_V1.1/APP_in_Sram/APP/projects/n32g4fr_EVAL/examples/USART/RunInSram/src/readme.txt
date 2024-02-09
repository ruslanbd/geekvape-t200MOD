/**
  @page USART_Printf USART Printf example
  
  @verbatim
  ******************************************************************************
  * @file    USART/Printf/src/readme.txt 
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   Description of the USART Printf example.
  ******************************************************************************

   @endverbatim


@par Example Description 

This example provides a basic communication between USARTx and PC using flags.

The USARTx sends TxBuffer to PC. 
The USARTx receive data from PC.  

USARTy configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - Even parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  
USARTx PINs connection as follow:
  - USART1_Tx.PA9 
  - USART1_Rx.PA10
  or
  - USART2_Tx.PA2
  - USART2_Rx.PA3
  