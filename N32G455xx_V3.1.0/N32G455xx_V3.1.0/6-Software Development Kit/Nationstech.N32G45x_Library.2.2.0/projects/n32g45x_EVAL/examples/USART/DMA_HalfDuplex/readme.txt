1、功能说明
    
    该测例演示了USARTy与USARTz间通过DMA实现半双工模式的基础通信。USARTy和
USARTz可以是USART1和USART2、USART3和UART4或UART6和UART7。
    首先，DMA（中断）传输TxBuffer1数据至USARTy发送数据寄存器，随后数据发送
至USARTz。USARTz接收从DMA（中断）传来的数据，存至RxBuffer2。比较收、发数据，
比较结果存入TransferStatus1变量。
    随后，DMA（查询）传输TxBuffer2数据至USARTz发送数据寄存器，随后数据发送
至USARTy。USARTy接收从DMA（查询）传来的数据，存至RxBuffer1。比较收、发数据，
比较结果存入TransferStatus2变量。   


2、使用环境

    软件开发环境：KEIL MDK-ARM Professional Version 5.26.2.0

        硬件环境：最小系统板N32G45XV-STB_V1.1


3、使用说明
    
    系统时钟配置如下：
    - 时钟源 = HSE + PLL
    - 系统时钟 = 144MHz
    
    USART配置如下：
    - 波特率 = 115200 baud
    - 字长 = 8数据位
    - 1停止位
    - 校验控制禁用
    - 硬件流控制禁用（RTS和CTS信号）
    - 发送器使能或接收器使能
    - 半双工模式使能
    - DMA发送模式和DMA接收模式使能
    
    
    USART引脚连接如下：
    - USART1_Tx.PA9    <------->   USART2_Tx.PA2
    或
    - USART3_Tx.PB10   <------->   UART4_Tx.PC10
    或
    - UART6_Tx.PE2    <------->    UART7_Tx.PC4

    
    测试步骤与现象：
    - Demo在KEIL环境下编译后，下载至MCU
    - 复位运行，依次查看变量TransferStatus1和TransferStatus2，其中，
      PASSED为测试通过，FAILED为测试异常


4、注意事项
    USART_Tx需外接上拉电阻。
    使用PA9，PA10需要把开发板NS-LINK的MCU_TX和MCU_RX跳线帽断开
    
1. Function description
    
    This test example demonstrates the basic communication between USARTy and USARTz in half-duplex mode through DMA. USARTy and
USARTz can be USART1 and USART2, USART3 and UART4 or UART6 and UART7.
    First, DMA (interrupt) transfers TxBuffer1 data to the USARTy send data register, and then the data is sent
To USARTz. USARTz receives data from DMA (interrupt) and stores it in RxBuffer2. Compare receiving and sending data,
The comparison result is stored in the TransferStatus1 variable.
    Subsequently, DMA (query) transfers the TxBuffer2 data to the USARTz send data register, and then the data is sent
To USARTy. USARTy receives the data from DMA (inquiry) and stores it in RxBuffer1. Compare receiving and sending data,
The comparison result is stored in the TransferStatus2 variable.


2. Use environment

    Software development environment: KEIL MDK-ARM Professional Version 5.26.2.0

    Hardware environment: minimum system board N32G45XV-STB_V1.1


3. Instructions for use
    
    The system clock configuration is as follows:
    -Clock source = HSE + PLL
    -System clock = 144MHz
    
    The USART configuration is as follows:
    -Baud rate = 115200 baud
    -Word length = 8 data bits
    -1 stop bit
    -Parity control disabled
    -Hardware flow control disabled (RTS and CTS signals)
    -Transmitter enable or receiver enable
    -Half-duplex mode enabled
    -DMA transmit mode and DMA receive mode enable
    
    
    The USART pin connections are as follows:
    -USART1_Tx.PA9 <-------> USART2_Tx.PA2
    or
    -USART3_Tx.PB10 <-------> UART4_Tx.PC10
    or
    -UART6_Tx.PE2 <-------> UART7_Tx.PC4

    
    Test steps and phenomena:
    -After the Demo is compiled in the KEIL environment, download it to the MCU
    -Reset operation, check the variables TransferStatus1 and TransferStatus2 in turn, among them,
      PASSED means the test passed, FAILED means the test is abnormal


4. Matters needing attention
    USART_Tx requires an external pull-up resistor.
    To use PA9/PA10, disconnect the jumper cap of MCU_TX/MCU_RX on NS-LINK