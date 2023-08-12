#include "headfile.h"

// **************************** �궨�� ****************************
#define SPLIT_ULTRASONIC_UART		UART_3
#define SPLIT_ULTRASONIC_BAUD		115200
#define SPLIT_ULTRASONIC_TX			UART3_TX_B10
#define SPLIT_ULTRASONIC_RX			UART3_RX_B11

#define SEND_PORT_PIN				B9												// ���ö�Ӧ�˿� ��ȥģ�鷢�� ������ʵ���Բ��ÿ���
// **************************** �궨�� ****************************

// **************************** �������� ****************************


void uart_handler (void);
void Ultrasonic_init();  //������ȥģ���ʼ��
void Ultrasonic_send();  //��ȥģ�鷢��
int Ultrasonic_receive(); //���ؾ�����Ϣ