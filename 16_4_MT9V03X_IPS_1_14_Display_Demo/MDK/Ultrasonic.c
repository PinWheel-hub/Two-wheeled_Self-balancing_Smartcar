#include "Ultrasonic.h"


// **************************** 变量定义 ****************************
uint16 ranging_counter = 0;															// 测距数据
uint8 ranging_flage = 0x00;															// 测距状态


uint8 dat[3];
uint8 num;
// **************************** 代码区域 ****************************




void Ultrasonic_init()  //有来有去模块初始化
{
	gpio_init(SEND_PORT_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);							// 初始化有去模块使能引脚
	uart_init(SPLIT_ULTRASONIC_UART, SPLIT_ULTRASONIC_BAUD, SPLIT_ULTRASONIC_TX, SPLIT_ULTRASONIC_RX);
	//uart_rx_irq(SPLIT_ULTRASONIC_UART, 1);
}

void Ultrasonic_send()  //有去模块发送
{
	gpio_set(SEND_PORT_PIN, GPIO_HIGH);												// 拉高使能 有去模块发送 实际接上电上拉就开始发送 程序可以不用控制
}

int Ultrasonic_receive() //返回距离信息
{
	uint8 uart_data;
	uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
	if(uart_data==0xa5)
	{
		uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
		dat[1]=uart_data;
		uart_getchar(SPLIT_ULTRASONIC_UART, &uart_data);
		dat[2]=uart_data;
		ranging_counter = dat[1]<<8 | dat[2];										//得到超声波模块测出的距离
		ranging_flage = 0x01;
	}
	return 	ranging_counter;		// 输出测距信息
}
	