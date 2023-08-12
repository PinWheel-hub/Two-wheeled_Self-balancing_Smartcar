#include "Ultrasonic.h"


// **************************** �������� ****************************
uint16 ranging_counter = 0;															// �������
uint8 ranging_flage = 0x00;															// ���״̬


uint8 dat[3];
uint8 num;
// **************************** �������� ****************************




void Ultrasonic_init()  //������ȥģ���ʼ��
{
	gpio_init(SEND_PORT_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);							// ��ʼ����ȥģ��ʹ������
	uart_init(SPLIT_ULTRASONIC_UART, SPLIT_ULTRASONIC_BAUD, SPLIT_ULTRASONIC_TX, SPLIT_ULTRASONIC_RX);
	//uart_rx_irq(SPLIT_ULTRASONIC_UART, 1);
}

void Ultrasonic_send()  //��ȥģ�鷢��
{
	gpio_set(SEND_PORT_PIN, GPIO_HIGH);												// ����ʹ�� ��ȥģ�鷢�� ʵ�ʽ��ϵ������Ϳ�ʼ���� ������Բ��ÿ���
}

int Ultrasonic_receive() //���ؾ�����Ϣ
{
	uint8 uart_data;
	uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
	if(uart_data==0xa5)
	{
		uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
		dat[1]=uart_data;
		uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
		dat[2]=uart_data;
		ranging_counter = dat[1]<<8 | dat[2];										//�õ�������ģ�����ľ���
		ranging_flage = 0x01;
	}
	return 	ranging_counter;		// ��������Ϣ
}
	