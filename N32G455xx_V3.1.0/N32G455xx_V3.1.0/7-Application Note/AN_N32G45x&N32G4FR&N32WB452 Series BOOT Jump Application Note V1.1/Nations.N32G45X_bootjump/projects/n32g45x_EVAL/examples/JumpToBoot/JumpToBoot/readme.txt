1.  Function description

     Used to demonstrate how to jump to SYSTEM BOOT from the user program (FLASH).

2. Use environment

     Software development environment: KEIL MDK-ARM V5.26
    
3. Instructions for use
     System Configuration:
         1. Button: S1--PA4
         2. USB port: DM--PA11, DP--PA12
         3. Debug serial products: TX--PA9, RX--PA10
        
     Instructions:
         1. Open the download tool (Nations MCU Download Tool) on the PC side
         2. After compiling under KEIL, burn it to the development board, connect the PC through the USB cable, and turn on the power
         3. At this time, the development board is working in a normal state. You can see the prompt information through the debugging serial port. The development board cannot be connected through the download tool.
         4. Press the button S1, the system jumps to SYSTEM BOOT, at this time you can connect the development board through the download tool and burn the program
        
4.  Matters needing attention
     1. If the USB cable is connected to J3 (USB COMM) of the full-featured board, select the "USB(DFU)" interface on the download tool (the driver needs to be installed)
     2. If the USB cable is connected to J4 (DEBUG USB) of the full-featured board, select the "USART" interface on the download tool