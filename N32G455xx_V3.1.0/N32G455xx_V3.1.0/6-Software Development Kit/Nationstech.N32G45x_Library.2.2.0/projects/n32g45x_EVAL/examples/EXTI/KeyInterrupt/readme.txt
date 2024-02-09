1、功能说明

    1、此例程展示通过外部触发中断，控制 LED 闪烁


2、使用环境

    /* 硬件环境：工程对应的开发硬件平台 */
    开发板：N32G4XR-STB V1.1

3、使用说明
    
    /* 描述相关模块配置方法；例如:时钟，I/O等 */
    SystemClock：144MHz
    GPIO：PA0 选择作为外部中断入口，PB15 控制 LED(D17) 闪烁


    /* 描述Demo的测试步骤和现象 */
    1.编译后下载程序复位运行；
    2.将 PA0 接入 3.3V 再断开，LED 闪烁；


4、注意事项
    无
	
	
1. Function description
    1. this example shows an external trigger interrupt to control the LED flashing.
	
2. Use environment
    /* Hardware environment: the development hardware platform corresponding to the project */
     Development board: N32G4XR-STB V1.1
	
3. Instructions for use
	/* Describe the related module configuration methods; For example: clock, I/O, etc. */
	1. SystemClock: 144 MHZ
	2. GPIO: PA0 selected as external interrupt entry, PB15 controls LED(D17) flashing
	
	/* Describe the test steps and phenomena of the Demo */
    1. After compiling, download the program to reset and run;
    2. Connect PA0 to 3.3V and then disconnect it, the LED flashes;.
		
4. Matters needing attention
    None.

