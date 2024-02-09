1. 功能说明
    USB模拟串口

2. 使用环境
    硬件环境：工程对应的开发硬件平台 
    开发板：   N32G45XV-STB V1.1

3. 使用说明
    描述相关模块配置方法；例如:时钟，I/O等 
         1. SystemClock：144MHz
         2. USBClock: 48MHz
         3. GPIO：USART1（TX--PA9，RX--PA10）波特率 115200

    描述Demo的测试步骤和现象 
         1. 编译后下载程序复位运行；
         2. 通过 USB 线连接 J3 USB 口，USB 挂载完成后，在电脑设备管理中可以看到新增的串口，将 USART1 接到串口工具上，打开 USB 虚拟串口，USB 虚拟串口发送数据，用串口工具接收数据，然后用串口工具发送数据， USB 虚拟串口接收数据

4. 注意事项
    MCU_TX 和 MCU_RX 的跳线帽需取下

1. Function description
    USB virtual serial port

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G45XV-STB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. USBClock: 48MHz
        3. GPIO: USART1 (TX--PA9, RX--PA10) baud rate 115200
                  
    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. Connect THE USB cable to THE J3 USB port. After the USB is mounted, the newly added serial port can be seen in computer device management. 
           Connect USART1 to the serial port tool, open the USB virtual serial port, send data through the USB virtual serial port, receive data with the serial port tool, 
           and then send data with the serial port tool, USB virtual serial port receives data.
 
4. Matters needing attention
    The jumper caps of MCU_TX and MCU_RX need to be removed.