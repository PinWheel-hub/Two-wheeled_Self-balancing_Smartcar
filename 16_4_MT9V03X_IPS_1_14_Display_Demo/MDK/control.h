#include "headfile.h"
#define Tope_Car_Speed                  3000     //��߳���
#define Default_Car_Speed               2400     //Ĭ�ϳ���
extern int car_speed;
extern int sp_1_kp;
extern int sp_1_kd;
int Speed_Control();     //���ת�ٿ���
void Different_Speed();                         //�ܲ���
void Different_Speed_Fazzy();

int Angle_control(int speed);   //ֱ��������
int Speed_Control_test();
void Turn_Control();
void Gyro_control(int Angle_speed,int diffe);
void Speed_Control_stop();
extern int different;
extern uint8 Breakaway_flag;