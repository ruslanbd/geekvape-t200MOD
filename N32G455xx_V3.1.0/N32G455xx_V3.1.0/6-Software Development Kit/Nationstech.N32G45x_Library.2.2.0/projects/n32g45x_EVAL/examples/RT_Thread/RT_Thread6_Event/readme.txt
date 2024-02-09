1. ����˵��
    ������չʾ�� RT_Thread �¼��Ĵ��������ͺͽ���

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QE_EVB V1.0

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��KEY��S4--PC6��KEY0����S5--PC7��KEY1��, S6--PB6��KEY2��
                        ��־��USART1����TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. �Ӻô��ڴ�ӡ����
         3. �����̴��������̣߳�key �̺߳� test �̣߳�key �̼߳�ⰴ�������¼���test �߳̽����¼������°�����S4��S5��S6�������¼���S4��S5 �����£������¼� AND �Żᱻ������S6 ���£������¼� OR ���������鿴��ӡ��־

4. ע������
    ��

1. Function description
    This routine shows the creation, sending, and receiving of events in RT_Thread

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QE_EVB V1.0

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: KEY: S4--PC6��KEY0����S5--PC7��KEY1��, S6--PB6��KEY2��
                     Log(USART1): TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. Connect the serial port printing tool;
        3. This example creates two threads, key thread and test thread, key thread detects the key to send event, test thread receives event, press the key (S4, S5, S6) to send event, 
            S4, S5 press, receive event AND will be triggered, S6 press, receive event OR will be triggered. Viewing Print Logs

4. Matters needing attention
    None.