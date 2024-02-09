﻿1. 功能说明
    USB  Joystick Mouse 设备

2. 使用环境
    软件开发环境：KEIL MDK-ARM 5.26.2.0
  
    硬件环境：工程对应的开发硬件平台 
    开发板：    N32G4XM_STB V1.1

3. 使用说明
    描述相关模块配置方法；例如:时钟，I/O等 
         1. SystemClock：144MHz
         2. USBClock: 48MHz
         3. GPIO：右KEY1（PA4）、左KEY2（PA5）、上KEY3（PA6）、下KEY4（PA7）。

    描述Demo的测试步骤和现象 
         1. 编译后下载程序复位运行；
         2. 通过 USB 线连接 J3 USB 口，按下 KEY1、KEY2、KEY3、KEY4 鼠标会上下左右移动。

4. 注意事项
    开发板上没有KEY4按键，KEY4对应的引脚是PA7，使用时需要对PA7进行跳线或另接按键

1. Function description
    USB Joystick Mouse device

2. Use environment
    Software development environment: KEIL MDK-ARM 5.26.2.0

    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G4XV-STB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. USBClock: 48MHz
        3. GPIO: right KEY1 (PA4), left KEY2 (PA5), upper KEY3 (PA6), lower KEY4 (PA7)
                  
    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. Connect the J3 USB port through a USB cable, press KEY1, KEY2, KEY3, KEY4, the mouse will move up and down, left and right.
 
4. Matters needing attention
    There is no KEY4 button on the development board. The corresponding pin of KEY4 is PA7, which needs to be jumped or connected with another key.