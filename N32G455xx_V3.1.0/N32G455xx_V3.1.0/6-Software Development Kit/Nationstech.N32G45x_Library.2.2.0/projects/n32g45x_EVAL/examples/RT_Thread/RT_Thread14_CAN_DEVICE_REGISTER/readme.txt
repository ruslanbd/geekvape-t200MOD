1. ����˵��
    ������չʾ�� RT_Thread ϵͳ���� CAN �豸

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QEL_EVB V1.1

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��LED��D6--PB10
                        CAN��RX--PD8��TX--PD9
                        ��־��USART1����TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. ��������main()���洴�������̣߳�test0 �̺߳� test1 �̣߳�test0 �߳����ڿ��� D6 250ms��˸����test1�̲߳���CAN�豸����CAN������RX��TX��
            ��test1�̷߳���CAN���ݣ�����CAN������λ���ϲ鿴���͵����ݣ��������Ժ������ǻ���ģʽ���������Ժ����յ��շ��͵����ݣ��жϽ��������Ƿ��������ô��ڴ�ӡ���Խ��

4. ע������
    ��

1. Function description
    This routine shows how to create a CAN device on the RT_Thread system

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: LED:D6--PB10
                     CAN: RX--PD8��TX--PD9
                     Log(USART1): TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates two threads in main(), test0 thread and test1 thread, test0 thread is used to control D6 250ms flashing, test1 thread is used to test the CAN device, 
            RX and TX are connected with the CAN card, and CAN data is sent in the test1 thread. CAN card can be used in the upper computer to check the data sent, after sending, 
            Because the loopback mode is used, the data just sent is received after the data is sent. Check whether the received data is normal and print the test result using the serial port

4. Matters needing attention
    None.