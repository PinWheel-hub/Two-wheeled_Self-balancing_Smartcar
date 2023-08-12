#include "headfile.h"
#define Tope_Car_Speed                  3000     //最高车速
#define Default_Car_Speed               2400     //默认车速
extern int car_speed;
extern int sp_1_kp;
extern int sp_1_kd;
int Speed_Control();     //电机转速控制
void Different_Speed();                         //总差速
void Different_Speed_Fazzy();

int Angle_control(int speed);   //直立环控制
int Speed_Control_test();
void Turn_Control();
void Gyro_control(int Angle_speed,int diffe);
void Speed_Control_stop();
extern int different;
extern uint8 Breakaway_flag;