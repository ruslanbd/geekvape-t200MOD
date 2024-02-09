1. ����˵��
    ������չʾ�� RT_Thread ϵͳ���� PIN �豸

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QEL_EVB V1.1

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��PB10 ���� LED(D6) ��˸��PB15 ���� LED(D17) ��˸
                        KEY0(S4) -- PC6, KEY1(S5) -- PC7, KEY2(S6) -- PB6
                        ��־��TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. �����̴��������̣߳�led0 �̡߳�led1 �߳� key �̣߳�led0 �߳����ڿ��� D6 500ms��˸��led1 �߳����ڿ��� D17 250ms��˸��
            key �̼߳�� KEY0��KEY1��KEY2������KEY2�����жϹ��ܣ���KEY2����ʱ�������жϻص��������а�������ʱ��ӡ��Ӧ��־

4. ע������
    ��

1. Function description
    This routine shows to create PIN devices on the RT_Thread system

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: PB10 controls LED(D6) flashing; The PB15 controls LED(D17) flashing
                     KEY0(S4) -- PC6, KEY1(S5) -- PC7, KEY2(S6) -- PB6
                     Log(USART1): TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates three threads, led0 thread, led1 thread key thread, led0 thread is used to control D6 500ms flashing, led1 thread is used to control D17 250ms flashing, 
            key thread detects KEY0, KEY1 and KEY2, and the KEY2 opens the interrupt function, When KEY2 is pressed, the interrupt callback function is called, and the corresponding log is printed when a key is pressed

4. Matters needing attention
    None.