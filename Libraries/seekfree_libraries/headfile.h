/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				headfile
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/
 
#ifndef _headfile_h
#define _headfile_h


#include <stdint.h>

#include "board.h"
#include "SEEKFREE_PRINTF.h"

//------my headfile
#include "key.h"
#include "menu.h"
#include "control.h"
#include "motor.h"
#include "img_process.h"
#include "my_math.h"
#include "pid.h"
#include "shangweiji.h"
#include "FazzyControl.h"
#include  "Ultrasonic.h"
//------��ɿƼ���Ƭ����������ͷ�ļ�
#include "zf_adc.h"
#include "zf_camera.h"
#include "zf_exti.h"
#include "zf_flash.h"
#include "zf_gpio.h"
#include "zf_spi.h"
#include "zf_systick.h"
#include "zf_pit.h"
#include "zf_pwm.h"
#include "zf_tim.h"
#include "zf_uart.h"
#include "zf_fsmc.h"
#define BUZZER_PIN B9
extern uint8 ips_flag;
extern bool run_flag;
extern PID_TypeDef Angle_PID;
extern PID_TypeDef Gyro_PID;
//------��ɿƼ���Ʒ����ͷ�ļ�
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_UART_7725.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_MT9V03X.h"

#endif

