1、功能说明
    1、TIM2周期门控TIM3，TIM3周期门控TIM4
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32G4XVL-STB V1.1开发
3、使用说明
    系统配置；
        1、时钟源：
                    HSE=8M,PLL=144M,AHB=144M,APB1=36M,APB2=72M,TIM2 CLK=72M,TIM3 CLK=72M,TIM4 CLK=72M
        2、端口配置：
                    PA0选择为TIM2 CH1输出
                    PA6选择为TIM3 CH1输出
                    PB6选择为TIM4 CH1输出
        3、TIM：
                    TIM2 的周期门控TIM3，TIM3的周期门控TIM4
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM2 CH1，TIM3 CH1，TIM4 CH1波形
        2、TIM3 4倍周期TIM2，TIM4 4倍周期TIM3
4、注意事项
    无
    
1. Function description
     1. TIM2 cycle gate TIM3, TIM3 cycle gate TIM4
2. Use environment
     Software development environment: KEIL MDK-ARM V5.26.2.0
     Hardware environment: Developed based on N32G4XVL-STB V1.1
3. Instructions for use
     System Configuration;
         1. Clock source:
                     HSE=8M, PLL=144M, AHB=144M, APB1=36M, APB2=72M, TIM2 CLK=72M, TIM3 CLK=72M, TIM4 CLK=72M
         2. Port configuration:
                     PA0 is selected as TIM2 CH1 output
                     PA6 is selected as TIM3 CH1 output
                     PB6 is selected as TIM4 CH1 output
         3. TIM:
                     TIM2 cycle gating TIM3, TIM3 cycle gating TIM4
     Instructions:
         1. After compiling, turn on the debug mode, and use an oscilloscope or logic analyzer to observe the waveforms of TIM2 CH1, TIM3 CH1, and TIM4 CH1
         2. TIM3 4 times cycle TIM2, TIM4 4 times cycle TIM3
4. Matters needing attention
     without