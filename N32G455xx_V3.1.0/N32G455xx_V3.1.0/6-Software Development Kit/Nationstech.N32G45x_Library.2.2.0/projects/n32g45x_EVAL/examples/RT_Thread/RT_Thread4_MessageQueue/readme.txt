1. ����˵��
    ������չʾ�� RT_Thread ��Ϣ���еĴ��������ͺͽ���

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QEL_EVB V1.1

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��LED��D6--PB10��D17--PB15
                        KEY��S4--PC6��KEY0����S5--PC7��KEY1��, S6--PB6��KEY2��
                        ��־��TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. �Ӻô��ڴ�ӡ����
         3. �����̴��������̣߳�key �̺߳� test �̣߳�key �̼߳�ⰴ��������Ϣ���У�test �߳̽�����Ϣ���У����°�����S4��S5��S6��������Ϣ���У��鿴��ӡ��־

4. ע������
    ��

1. Function description
    This routine shows the creation, sending, and receiving of a message queue in RT_Thread

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: LED: D6--PB10��D17--PB15
                     KEY: S4--PC6��KEY0����S5--PC7��KEY1��, S6--PB6��KEY2��
                     Log: TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. Connect the serial port printing tool;
        3. This routine creates two threads, the key thread and the test thread. The key thread detects the key sending message queue, and the test thread receives the message queue. 
            Press the key (S4, S5, S6) to send the message queue and view the print log

4. Matters needing attention
    None.