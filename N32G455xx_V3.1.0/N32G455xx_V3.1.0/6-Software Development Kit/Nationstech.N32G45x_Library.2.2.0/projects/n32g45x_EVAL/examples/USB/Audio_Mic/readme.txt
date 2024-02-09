1. 功能说明
    USB Audio Mic 麦克风

2. 使用环境
    硬件环境：工程对应的开发硬件平台 
    开发板：   N32G457QEL_EVB V1.1

3. 使用说明
    描述相关模块配置方法；例如:时钟，I/O等 
         1. SystemClock：144MHz
         2. USBClock: 48MHz
         3. GPIO： VS1053B SPI3：SCK--PC3、MISO--PA0、MOSI--PA1；
             DREQ--PA4、RST--PB0、XCS--PC2、XDCS--PA3，
             J46、J47需要用跳线帽连接，与Flash连接的跳线帽需要断开。

    描述Demo的测试步骤和现象 
         1. 编译后下载程序复位运行；
         2. 通过 USB 线连接 J3 USB 口，电脑识别出麦克风设备，VS1053B通过麦克风进行录音，再通过USB将数据传到PC上，PC上插入耳机通过监听麦克风设备，可以听到开发板上麦克风传过来的声音。
             用 Bus Hound 查看 USB 传过去的数据，音频数据是 ISOC 同步传输类型。

4. 注意事项
    需要在PC上监听麦克风设备才能听到声音

1. Function description
    USB Audio Mic

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. USBClock: 48MHz
        3. GPIO: 
            VS1053B SPI3: SCK--PC3, MISO--PA0, MOSI--PA1; 
            DREQ--PA4, RST--PB0, XCS--PC2, XDCS--PA3, 
            J46, J47 require jumper caps To connect, the jumper cap connected to the Flash needs to be disconnected.
                  
    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. By connecting the USB cable to the J3 USB port, the computer can identify the microphone device, VS1053B can record through the microphone, 
           and then transfer the data to the PC through THE USB. The headset is inserted into the PC and the microphone device is monitored to hear the voice transmitted from the microphone on the development board.
           Use Bus Hound to view the data transmitted by USB. The audio data is ISOC synchronous transmission type.
 
4. Matters needing attention
    Need to monitor the microphone device on the PC to hear the sound