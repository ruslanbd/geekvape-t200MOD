1. ����˵��
    ������չʾ�� RT_Thread ϵͳ���� WATCHDOG �豸

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QE_EVB V1.0

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��LED:D6--PB10
                        ��־��USART1����TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. ��������main()���洴�������̣߳�test0 �̺߳� test1 �̣߳�test0 �߳����ڿ��� D6 500ms��˸����test1�߳��в��� IWDG �豸������ IWDG �豸�����ó�ʱʱ�䣬���� IWDG���ڿ���ʱι��

4. ע������
    ��

1. Function description
    This routine shows how to create a WATCHDOG device on the RT_Thread system

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QE_EVB V1.0

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: LED:D6--PB10
                     Log(USART1): TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates two threads in main(), test0 thread and test1 thread, test0 thread is used to control D6 500ms flashing, test1 thread is used to test the IWDG device, 
            find the IWDG device, set the timeout, start the IWDG, and feed the dog when idle

4. Matters needing attention
    None.