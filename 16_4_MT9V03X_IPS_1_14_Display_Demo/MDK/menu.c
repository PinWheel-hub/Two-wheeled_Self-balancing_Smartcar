#include "headfile.h"
typedef struct
{
    uint8_t current;
    uint8_t next;
    uint8_t enter;
    void (*current_operation)(void);
} Menu_table;
int8 mode;
Menu_table  table[56] =
{
    {0,6,1,(*fun0)},
    {1,3,2,(*fun1)},
    {2,3,2,(*fun2)},
    {3,5,4,(*fun3)},
    {4,5,4,(*fun4)},
    {5,1,0,(*fun5)},
		{6,12,7,(*fun6)},
    {7,9,8,(*fun7)},
    {8,9,8,(*fun8)},
    {9,11,10,(*fun9)},
    {10,11,10,(*fun10)},
    {11,7,6,(*fun11)},
		{12,22,13,(*fun12)},
    {13,15,14,(*fun13)},
    {14,15,14,(*fun14)},
    {15,17,16,(*fun15)},
    {16,17,16,(*fun16)},
    {17,19,18,(*fun17)},
		{18,19,18,(*fun18)},
    {19,21,20,(*fun19)},
    {20,21,20,(*fun20)},
    {21,13,12,(*fun21)},
		{22,32,23,(*fun22)},
    {23,25,24,(*fun23)},
    {24,25,24,(*fun24)},
    {25,27,26,(*fun25)},
    {26,27,26,(*fun26)},
    {27,29,28,(*fun27)},
		{28,29,28,(*fun28)},
    {29,31,30,(*fun29)},
    {30,31,30,(*fun30)},
    {31,23,22,(*fun31)},
		{32,0,33,(*fun32)},
    {33,35,34,(*fun33)},
    {34,35,34,(*fun34)},
    {35,37,36,(*fun35)},
    {36,37,36,(*fun36)},
    {37,47,38,(*fun37)},
		{38,40,39,(*fun38)},
    {39,40,39,(*fun39)},
    {40,42,41,(*fun40)},
    {41,42,41,(*fun41)},
		{42,44,43,(*fun42)},
    {43,44,43,(*fun43)},
		{44,46,45,(*fun44)},
		{45,46,45,(*fun45)},
    {46,38,37,(*fun46)},
		{47,33,32,(*fun47)}
};
uint8  func_index = 0;
void  menu_key(uint8 Get_key)
{
  switch(Get_key)
  {
    case 2:func_index=table[func_index].next;
		       ips114_clear(WHITE);
           break; 
    case 3:func_index=table[func_index].enter;
		       ips114_clear(WHITE);
           break;
    default:break;
  }
	if(Get_key||func_index==0)
    (table[func_index].current_operation)();
}

void fun0(void)
{
  ips114_showstr(0,0,"adjust speed <-");
	ips114_showstr(0,1,"adjust Threshold");
	ips114_showstr(0,2,"adjust angle_pid");
	ips114_showstr(0,3,"adjust gyro_pid");
	ips114_showstr(0,4,"adjust mode");
}
void fun1(void)
{
  ips114_showstr(0,0,"speed+100 <-");
	ips114_showstr(0,1,"speed-100");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_speed=");
	ips114_showint16(110,7,car_speed); 
}
void fun2(void)
{
	car_speed+=100;
  ips114_showstr(0,0,"speed+100 <-");
	ips114_showstr(0,1,"speed-100");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_speed=");
	ips114_showint16(110,7,car_speed);
}
void fun3(void)
{
  ips114_showstr(0,0,"speed+100");
	ips114_showstr(0,1,"speed-100 <-");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_speed=");
	ips114_showint16(110,7,car_speed);
}
void fun4(void)
{
	car_speed-=100;
  ips114_showstr(0,0,"speed+100");
	ips114_showstr(0,1,"speed-100 <-");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_speed=");
	ips114_showint16(110,7,car_speed);
}
void fun5(void)
{
  ips114_showstr(0,0,"speed+100");
	ips114_showstr(0,1,"speed-100");
	ips114_showstr(0,2,"back <-");
	ips114_showstr(0,7,"current_speed=");
	ips114_showint16(110,7,car_speed);
}
void fun6(void)
{
  ips114_showstr(0,0,"adjust speed");
	ips114_showstr(0,1,"adjust Threshold <-");
	ips114_showstr(0,2,"adjust angle_pid");
	ips114_showstr(0,3,"adjust gyro_pid");
	ips114_showstr(0,4,"adjust mode");
}
void fun7(void)
{
  ips114_showstr(0,0,"Threshold+5 <-");
	ips114_showstr(0,1,"Threshold-5");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_threshold=");
	ips114_showint16(150,7,Threshold); 
}
void fun8(void)
{
  Threshold+=5;
	ips114_showstr(0,0,"Threshold+5 <-");
	ips114_showstr(0,1,"Threshold-5");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_threshold=");
	ips114_showint16(150,7,Threshold); 
}
void fun9(void)
{
  ips114_showstr(0,0,"Threshold+5");
	ips114_showstr(0,1,"Threshold-5 <-");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_threshold=");
	ips114_showint16(150,7,Threshold); 
}
void fun10(void)
{
  Threshold-=5;
	ips114_showstr(0,0,"Threshold+5");
	ips114_showstr(0,1,"Threshold-5 <-");
	ips114_showstr(0,2,"back");
	ips114_showstr(0,7,"current_threshold=");
	ips114_showint16(150,7,Threshold); 
}
void fun11(void)
{
  ips114_showstr(0,0,"Threshold+5");
	ips114_showstr(0,1,"Threshold-5");
	ips114_showstr(0,2,"back <-");
	ips114_showstr(0,7,"current_threshold=");
	ips114_showint16(150,7,Threshold); 
}
void fun12(void)
{
  ips114_showstr(0,0,"adjust speed");
	ips114_showstr(0,1,"adjust Threshold");
	ips114_showstr(0,2,"adjust angle_pid <-");
	ips114_showstr(0,3,"adjust gyro_pid");
	ips114_showstr(0,4,"adjust mode");
}
void fun13(void)
{
  ips114_showstr(0,0,"kp+10000 <-");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun14(void)
{
  Angle_PID.Kp+=10000;
	ips114_showstr(0,0,"kp+10000 <-");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun15(void)
{
  ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000 <-");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun16(void)
{
  Angle_PID.Kp-=10000;
	ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000 <-");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun17(void)
{
  ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000 <-");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun18(void)
{
  Angle_PID.Kd+=5000;
	ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000 <-");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun19(void)
{
  ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun20(void)
{
	Angle_PID.Kd-=5000;
	ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun21(void)
{
	ips114_showstr(0,0,"kp+10000");
	ips114_showstr(0,1,"kp-10000");
	ips114_showstr(0,2,"kd+5000");
	ips114_showstr(0,3,"kd-5000");
	ips114_showstr(0,4,"back <-");
	ips114_showstr(0,6,"current_pd=");
	ips114_showint32(0,7,Angle_PID.Kp,8);
	ips114_showint32(100,7,Angle_PID.Kd,8);
}
void fun22(void)
{
  ips114_showstr(0,0,"adjust speed");
	ips114_showstr(0,1,"adjust Threshold");
	ips114_showstr(0,2,"adjust angle_pid");
	ips114_showstr(0,3,"adjust gyro_pid <-");
	ips114_showstr(0,4,"adjust mode");
}
void fun23(void)
{
  ips114_showstr(0,0,"kp+1000 <-");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun24(void)
{
  Gyro_PID.Kp+=1000;
	ips114_showstr(0,0,"kp+1000 <-");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun25(void)
{
  ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000 <-");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun26(void)
{
  Gyro_PID.Kp-=1000;
	ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000 <-");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun27(void)
{
  ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5 <-");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun28(void)
{
  Gyro_PID.Ki+=5;
	ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5 <-");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun29(void)
{
  ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun30(void)
{
	Gyro_PID.Ki-=5;
	ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun31(void)
{
	ips114_showstr(0,0,"kp+1000");
	ips114_showstr(0,1,"kp-1000");
	ips114_showstr(0,2,"ki+5");
	ips114_showstr(0,3,"ki-5");
	ips114_showstr(0,4,"back <-");
	ips114_showstr(0,6,"current_pi=");
	ips114_showint32(0,7,Gyro_PID.Kp,8);
	ips114_showint32(100,7,Gyro_PID.Ki,8);
}
void fun32(void)
{
  ips114_showstr(0,0,"adjust speed");
	ips114_showstr(0,1,"adjust Threshold");
	ips114_showstr(0,2,"adjust angle_pid");
	ips114_showstr(0,3,"adjust gyro_pid");
	ips114_showstr(0,4,"adjust mode <-");
}
void fun33(void)
{
  ips114_showstr(0,0,"choose mode0 <-");
	ips114_showstr(0,1,"choose mode1");
	ips114_showstr(0,2,"choose mode2");
	ips114_showstr(0,3,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}
void fun34(void)
{
	mode=0;
  ips114_showstr(0,0,"choose mode0 <-");
	ips114_showstr(0,1,"choose mode1");
	ips114_showstr(0,2,"choose mode2");
	ips114_showstr(0,3,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}
void fun35(void)
{
  ips114_showstr(0,0,"choose mode0");
	ips114_showstr(0,1,"choose mode1 <-");
	ips114_showstr(0,2,"choose mode2");
	ips114_showstr(0,3,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}
void fun36(void)
{
	mode=1;
  ips114_showstr(0,0,"choose mode0");
	ips114_showstr(0,1,"choose mode1 <-");
	ips114_showstr(0,2,"choose mode2");
	ips114_showstr(0,3,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}
void fun37(void)
{
  ips114_showstr(0,0,"choose mode0");
	ips114_showstr(0,1,"choose mode1");
	ips114_showstr(0,2,"choose mode2 <-");
	ips114_showstr(0,3,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}
void fun38(void)
{
  mode=2;
	ips114_showstr(0,0,"kp+100 <-");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun39(void)
{
  sp_1_kp+=100;
	ips114_showstr(0,0,"kp+100 <-");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun40(void)
{
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100 <-");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun41(void)
{
	sp_1_kp-=100;
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100 <-");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun42(void)
{
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100 <-");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun43(void)
{
	sp_1_kd+=100;
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100 <-");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun44(void)
{
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun45(void)
{
	sp_1_kd-=100;
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100 <-");
	ips114_showstr(0,4,"back");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun46(void)
{
	ips114_showstr(0,0,"kp+100");
	ips114_showstr(0,1,"kp-100");
	ips114_showstr(0,2,"kd+100");
	ips114_showstr(0,3,"kd-100");
	ips114_showstr(0,4,"back <-");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
	ips114_showstr(0,7,"mode_pid=");
	ips114_showint32(100,7,sp_1_kp,6);
	ips114_showint32(170,7,sp_1_kd,6);
}
void fun47(void)
{
  ips114_showstr(0,0,"choose mode0");
	ips114_showstr(0,1,"choose mode1");
	ips114_showstr(0,2,"choose mode2");
	ips114_showstr(0,3,"back <-");
	ips114_showstr(0,6,"current_mode=");
	ips114_showint32(110,6,mode,1);
}