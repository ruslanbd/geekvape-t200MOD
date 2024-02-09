1. ����˵��
    ������չʾ�� RT_Thread ϵͳ���� ADC �豸

2. ʹ�û���
    Ӳ�����������̶�Ӧ�Ŀ���Ӳ��ƽ̨ 
    �����壺   N32G457QE_EVB V1.0

3. ʹ��˵��
    �������ģ�����÷���������:ʱ�ӣ�I/O�� 
         1. SystemClock��144MHz
         2. GPIO��LED:D17--PB15
                        ADC:
                               PC0ѡ��Ϊģ�⹦��ADC1ת��ͨ��
                               PC1ѡ��Ϊģ�⹦��ADC1ת��ͨ��
                        ��־��TX--PA9  RX--PA10�������ʣ�115200

    ����Demo�Ĳ��Բ�������� 
         1. ��������س���λ����
         2. ��������main()���洴�������̣߳�test0 �̺߳� test1 �̣߳�test0 �߳����ڿ��� D17 500ms��˸����test1�߳��в���ADC�豸,����PC0��PC1���ŵ�ѹ��ͨ�����ڴ�ӡ���

4. ע������
    ��

1. Function description
    This routine shows how to create an ADC device on the RT_Thread system

2. Use environment
    Hardware environment: development hardware platform corresponding to the project 
    Development board:      N32G457QE_EVB V1.0

3. Instructions for use
    Describe the configuration method of related modules; for example: clock, I/O, etc. 
        1. SystemClock: 144MHz
        2. GPIO: LED:D17--PB15
                     ADC:
                            PC0 is selected as the analog function ADC1 conversion channel
                            PC1 is selected as the analog function ADC1 conversion channel
                     Log: TX--PA9 RX--PA10 Baud rate: 115200

    Describe the test steps and phenomena of Demo 
        1. After compiling, download the program to reset and run;
        2. This routine creates two threads in main(), test0 thread and test1 thread, test0 thread is used to control D17 500ms flashing, 
            test the ADC device in test1 thread, measure the pin voltage of PC0 and PC1, and print the result through serial port

4. Matters needing attention
    None.