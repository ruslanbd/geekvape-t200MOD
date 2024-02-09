1. ����˵��
    ������չʾ�� RT_Thread ϵͳ���� UART �豸

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QEL_EVB V1.1

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��LED:D17--PB15
                        USART3: TX--PB10  RX--PB11
                        ��־��USART1)��TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. ��������main()���洴�������̣߳�test0 �̺߳� test1 �̣߳�test0 �߳����ڿ��� D17 250ms��˸����test1�߳��в��� UART �豸���ͽ��չ��ܣ�
            ͨ�����ڹ��߽��� USART3 �� TX �� RX �ϣ��ϵ�����ͨ�� USART3 �� TX �������ݣ���λ�����ͨ�����ڹ��߷������ݵ� USART3 �� RX �ϣ�USART3 �Ὣ���յ�������ͨ�� TX �ٷ��ͳ�ȥ

4. ע������
    ��

1. Function description
    This routine shows how to create a UART device on the RT_Thread system

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QEL_EVB V1.1

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: LED:D17--PB15
                     USART3: TX--PB10  RX--PB11
                     Log(USART1): TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates two threads in main(), test0 thread and test1 thread, test0 thread is used to control D6 250ms flashing, test1 thread is used to test the sending and receiving function of UART device. 
            It is connected to TX and RX of USART3 through serial port tool. After power-on, data will be sent through TX of USART3 first. The upper computer software will send data to RX of USART3 through serial port tool, and USART3 will send the received data through TX again

4. Matters needing attention
    None