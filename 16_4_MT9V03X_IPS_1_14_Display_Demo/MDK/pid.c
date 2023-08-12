#include "pid.h"

int PID_Increment(int Target,float Actual,PID_TypeDef* PID_Struct)
{
  int PID_Inc;
  PID_Struct->Error=Target-Actual;
  PID_Inc=(int)(PID_Struct->Kp*(PID_Struct->Error-PID_Struct->Error1)/1000+
    PID_Struct->Ki*PID_Struct->Error/1000+
      PID_Struct->Kd*(PID_Struct->Error-2*PID_Struct->Error1+PID_Struct->Error2)/1000);
  PID_Struct->Error2=PID_Struct->Error1;
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Inc;
}

int PID_Location1(int Target,float Actual,PID_TypeDef* PID_Struct)
{
   int PID_Loc;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	
	if(PID_Struct->Error_Sum>6000)  //�����޷�
		PID_Struct->Error_Sum=6000;
	if(PID_Struct->Error_Sum<-6000)  //�����޷�
		PID_Struct->Error_Sum=-6000;
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd*(PID_Struct->Error-PID_Struct->Error1)/1000+
      PID_Struct->Ki*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Loc;
}

int PID_Gyro(int Target,float Actual,PID_TypeDef* PID_Struct)
{
   int PID_Loc;
	int ki_n;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	
//	if(PID_Struct->Error_Sum>200000)  //�����޷�
//		PID_Struct->Error_Sum=200000;
//	else if(PID_Struct->Error_Sum<-200000)  //�����޷�
//		PID_Struct->Error_Sum=-200000;
	
			if(PID_Struct->Error>50||PID_Struct->Error<-50)
	{
		ki_n=PID_Struct->Ki;
	}
	else
	{
		ki_n=0;
	}
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd*(PID_Struct->Error-PID_Struct->Error1)/1000+
      ki_n*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Loc;
}

int PID_Angle(int Target,float Actual,PID_TypeDef* PID_Struct)
{
   int PID_Loc;
	int ki_n;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	
		if(PID_Struct->Error>30||PID_Struct->Error<-30)
	{
		ki_n=PID_Struct->Ki;
	}
	else
	{
		ki_n=8;
	}
	
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd*(PID_Struct->Error-PID_Struct->Error1)/1000+
      ki_n*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Loc;
}

int PID_Location(int Target,float Actual,PID_TypeDef* PID_Struct)
{
  int PID_Loc;
	int ki_n;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	

	if(PID_Struct->Error_Sum>6000)  //�����޷�
		PID_Struct->Error_Sum=6000;
	if(PID_Struct->Error_Sum<-6000)  //�����޷�
		PID_Struct->Error_Sum=-6000;
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd/1000*(PID_Struct->Error-PID_Struct->Error1)+
     PID_Struct->Ki*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
	
//	if(PID_Loc>2000)
//	{
//		PID_Loc=2000;
//	}
//	else if(PID_Loc<-2000)
//	{
//		PID_Loc=-2000;
//	}
  return PID_Loc;
}
int PID_DIFF(int Target,float Actual,PID_TypeDef* PID_Struct)
{
  int PID_Loc;
	int ki_n;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	
	if(PID_Struct->Error>30||PID_Struct->Error<-30)
	{
		ki_n=PID_Struct->Ki;
	}
	else
	{
		ki_n=0;
	}
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd*(PID_Struct->Error-PID_Struct->Error1)/1000+
     ki_n*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Loc;
}

int PID_MIDE(int Target,float Actual,PID_TypeDef* PID_Struct)
{
  int PID_Loc;
	int ki_n;
  PID_Struct->Error=Target-Actual;
  PID_Struct->Error_Sum+=PID_Struct->Error;
	
	if(PID_Struct->Error_Sum>20000)  //�����޷�
		PID_Struct->Error_Sum=20000;
	if(PID_Struct->Error_Sum<-20000)  //�����޷�
		PID_Struct->Error_Sum=-20000;
	
  PID_Loc=(int)(PID_Struct->Kp*PID_Struct->Error/1000+
    PID_Struct->Kd/1000*(PID_Struct->Error-PID_Struct->Error1)+
     PID_Struct->Ki*PID_Struct->Error_Sum/1000);
  PID_Struct->Error1=PID_Struct->Error;
  return PID_Loc;
}

int balance1(float Angle,float Gyro)  //ֱ����
{  
   float Bias,Balance_Kp=100,Balance_Kd=0;//PID����370  -1.9
	 int balance; 
	 Bias=377-Angle;       //===���ƽ��ĽǶ���ֵ �ͻ�е���  ZHONGZHI����е��ֵΪС�����ϵ�ƽ��ʱ�ĽǶ�
	 balance=Balance_Kp*Bias+Gyro*Balance_Kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}



