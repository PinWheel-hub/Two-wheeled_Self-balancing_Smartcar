/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				main
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#include "headfile.h"
// *************************** ����˵�� ***************************
// 
// ������Ҫ׼����ɿƼ� MM32F3277 ���İ�һ��
// 
// ����������Ҫ׼����ɿƼ� CMSIS-DAP ���������� �� ARM ���������� һ��
// 
// �����̳�ʼ���� �����ϵ�����ͷ�ӿ� �Լ� IPS1.14����Ļģ��
// 
// ��¼�����̺� ��ϵ����
// ��Ҫ��װ �����Ҷ���������ͷ UART�汾 ģ�� �� IPS1.14����Ļģ��
// ȷ���������� �ϵ� �����ʹ�õ�ع��� ��ȷ����ص�������
// ��ʱ IPS1.14����Ļģ�� ����ʾͼ����Ϣ
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************

// **************************** �궨�� ****************************

// **************************** �������� ****************************
uint8 ips_flag=0;
bool run_flag=0;
// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������
  mt9v03x_init();
  Encoder_init();
	motor_init();
	seekfree_wireless_init(); 		           // ��ʼ��ģ��
	ips114_init();
	icm20602_init_spi();
	pit_init();		
	key_init();
	Ultrasonic_init();
	weight_array_init();
	threshold_init();
  Ultrasonic_send();
  gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);  
	while(1)
	{
		key_ips();
		if (mt9v03x_finish_flag == 1)
		{
		  Turn_Control(); 		
			mt9v03x_finish_flag = 0;
		}
		judge_relay();
//			CAR_SPEED speed_set={1500,0};
//      set_Speed(speed_set);
//			CAR_SPEED current_speed = Get_Speed(Speed_Circuit_T);
//			ips114_showint16(188,5,current_speed.left_speed);
//			ips114_showint16(188,1,current_speed.right_speed);
	}
}
// **************************** �������� ****************************