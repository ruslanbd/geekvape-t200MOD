1. ����˵��
    ������չʾ�� RT_Thread �ź����Ĵ�������ȡ���ͷ�

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QEL_EVB V1.1

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��PB10 ���� LED(D6) ��˸��PB15 ���� LED(D17) ��˸
                        KEY0��S4--PC6��KEY0��
                        ��־:   TX--PA9  RX--PA10    �����ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ���У�
         2. ��������main()���������̣߳�LED0 �̡߳� LED1 �߳� �� KEY �̣߳�LED0 �߳����ڿ��� D6 ����LED1 �߳����ڿ��� D17 500ms��˸��
             KEY �߳�ɨ�谴������ KEY0 ��⵽����ʱ���ͷ��ź�����LED0 �̻߳�ȡ�ź�������ȡ�ɹ��󣬷�ת D6

4. ע������
    ��

1. Function description
    This example shows the creation, acquisition and release of the RT_Thread semaphore

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: 
                    PB10 controls the LED (D6) to blink; PB15 controls the LED (D17) to blink;
                    KEY0: S4--PC6��KEY0��
                    Log: TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates three threads in main(), LED0 thread, LED1 thread and KEY thread, LED0 thread is used to control D6 on and off, LED1 thread is used to control D17 500ms blink, 
            KEY thread scans the KEY, when KEY0 detects that it is pressed, release the semaphore, LED0 thread obtains the semaphore, and after obtaining it successfully, reverses D6

4. Matters needing attention
    None.