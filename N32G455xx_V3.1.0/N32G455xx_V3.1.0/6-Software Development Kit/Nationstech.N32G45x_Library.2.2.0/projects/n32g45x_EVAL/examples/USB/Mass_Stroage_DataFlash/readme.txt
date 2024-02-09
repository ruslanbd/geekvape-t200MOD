﻿1. 功能说明
    USB 使用内部 FLASH 模拟 U 盘

2. 使用环境
    硬件环境：工程对应的开发硬件平台 
    开发板：    N32G4XM_STB V1.1

3. 使用说明
    描述相关模块配置方法；例如:时钟，I/O等 
         1. SystemClock：144MHz
         2. USBClock: 48MHz

    描述Demo的测试步骤和现象 
         1. 编译后下载程序复位运行；
         2. 通过 USB 线连接 J3 USB 口，USB 挂载完成后，识别成 U 盘设备

4. 注意事项
    首次挂载 U 盘需要格式化，格式化完成后即可当成 U 盘使用

1. Function description
    USB uses internal FLASH to simulate U disk

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G4XM_STB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. USBClock: 48MHz
                  
    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. Connect the J3 USB port through a USB cable, and after the USB is mounted, it will be recognized as a U disk device.
 
4. Matters needing attention
    The first mount U disk needs to be formatted, and it can be used as a U disk after formatting.