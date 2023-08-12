#include "common.h"
typedef struct
{
  int Kp;//����ϵ��
  int Ki;//����ϵ��
  int Kd;//΢��ϵ��
  float Error;//��ǰ���
  float Error1;//��һ�����
  float Error2;//���ϴ����
  float Error_Sum;//����ۻ�
}PID_TypeDef;

int PID_Increment(int Target,float Actual,PID_TypeDef* PID_Struct);
int PID_Location(int Target,float Actual,PID_TypeDef* PID_Struct);
int PID_Location1(int Target,float Actual,PID_TypeDef* PID_Struct);
int PID_Gyro(int Target,float Actual,PID_TypeDef* PID_Struct);
int balance1(float Angle,float Gyro);
int PID_Angle(int Target,float Actual,PID_TypeDef* PID_Struct);
int PID_DIFF(int Target,float Actual,PID_TypeDef* PID_Struct);
int PID_MIDE(int Target,float Actual,PID_TypeDef* PID_Struct);