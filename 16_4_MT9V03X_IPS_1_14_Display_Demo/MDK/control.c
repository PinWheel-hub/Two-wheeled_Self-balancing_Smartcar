#include "headfile.h"
#include "shangweiji.h"
CAR_SPEED current_speed={0,0};
int car_speed=0;
int angle;
bool element_flag=0;
//*****************************************************总控制******************************************************

float direction_error=0;
float average_error=0;
int mideview=0;
int different=0;
int different1=0;


int my_abs(int x)
{
	if(x<0)
	{
		return (-x);
	}
	else
	{
		return x;
	}
}

int gyro_zz=1;  
//摄像头转向环
void Turn_Control()
{
	 get_longest_col();
	 trackBorder_Get();
	 if(angle>200)
		 element_flag=1;
	 if(element_flag)
	  refresh_current_element();
	 average_error=centre_line_get();
	 if_stop();
	 //gyro_zz=my_abs(get_Gyro_Balance2());
   
	 if(ips_flag==1)
	 {
		 erzhihua();
		 Show_Line();
  	 ips114_displayimage032(image[0], MT9V03X_W, MT9V03X_H);
		 //ips114_showint16(188,5,average_error);
	 }
}

//*****************************************************总控制******************************************************

//*****************************************************车速控制******************************************************
//总差速

int sp_1_kp=1700;
int sp_1_kd=200;

PID_TypeDef DifferentSpeed_PID={1000,0,300,0,0,0,0}; 
PID_TypeDef Mide_PID={3000,0,120000,0,0,0}; 
void Different_Speed()
{
	 

	int sp=(current_speed.left_speed+current_speed.right_speed)/2;
	
//	
//   Mide_PID.Kp=sp/2; 
//   Mide_PID.Kd=sp*10;
//	  if(sp<1500)
//	{
//		  Mide_PID.Kp=3000; 
//      Mide_PID.Kd=50000;//*gyro_zz; 
//	}
//	else
//	{
//      Mide_PID.Kp=5000; 
//      Mide_PID.Kd=80000;
//	}



	
	mideview=PID_DIFF(0,sp/100,&DifferentSpeed_PID);

	different=PID_MIDE(0,my_abs(mideview)*average_error,&Mide_PID);
}

//电机转速控制

PID_TypeDef Speed_Left_PID={600,0,500,0,0,0,0};
PID_TypeDef Speed_Right_PID={600,0,500,0,0,0,0};   //速度环pid对输出进行限幅
int temp_length = 0;
int8 length_count = 0;

int round_length = 0;


int Speed_Control()
{
    CAR_SPEED target_speed={0,0};
		int left_speed=0,right_speed=0;
		int speed;
    current_speed = Get_Speed(Speed_Circuit_T);      //获取速度
		
		 
    int current_sp_left=current_speed.left_speed;
		int current_sp_right=current_speed.right_speed;
		
		if(car_speed<2000)
		{
			car_speed+=40;
		}
    left_speed=PID_Location(car_speed,current_sp_left,&Speed_Left_PID);
		right_speed=PID_Location(car_speed,current_sp_right,&Speed_Right_PID);
		
		//OutPut_Data(current_speed.left_speed,current_speed.right_speed,target_speed.left_speed,0);
		speed=(left_speed+right_speed)/2;
		return speed;
		


}
//*****************************************************角度控制******************************************************

uint8 Breakaway_flag=0;
PID_TypeDef Angle_PID={-40000,0,-80000,0,0,0,0};  
int Angle_control(int speed)   //直立环控制
{
	float tan;
  int	Gyro_Balance;
	int Balance_Pwm_left;
	int Balance_Pwm;
	tan=get_Angle_Balance1();
	Gyro_Balance=get_Gyro_Balance1();
	tan=-tan*1800/3.1415926;
	angle=angle_calc(tan,Gyro_Balance);
	
//	if(angle>550)
//	{
//		speed=0;
//	}


		

	Balance_Pwm=PID_Angle(430,angle+speed,&Angle_PID);
	//ips114_showint16(188,5,angle);

	
	return Balance_Pwm;
	
}



PID_TypeDef Gyro_PID={1000,10,0,0,0,0,0};
void Gyro_control(int Angle_speed,int diffe) 
{
	int Gyro_Balance;
	int Gyro_PWM_left,Gyro_PWM_right;
	Gyro_Balance=get_Gyro_Balance1();
	

	  Gyro_PWM_left=PID_Gyro(0,Gyro_Balance+Angle_speed-diffe,&Gyro_PID);
  	Gyro_PWM_right=PID_Gyro(0,Gyro_Balance+Angle_speed+diffe,&Gyro_PID);

	 
	
	 	CAR_SPEED speed_set={Gyro_PWM_left,Gyro_PWM_right};
      set_Speed(speed_set);
 

}


PID_TypeDef Speed_Left_Stop_PID={4000,500,0,0,0,0,0};
PID_TypeDef Speed_Right_Stop_PID={4000,500,0,0,0,0,0};
void Speed_Control_stop()
{
    CAR_SPEED target_speed={0,0};
		int left_speed,right_speed;
    current_speed = Get_Speed(Speed_Circuit_T2);      //获取速度
		

    int current_sp_left=current_speed.left_speed;
		int current_sp_right=current_speed.right_speed;
    left_speed=PID_Location1(0,current_sp_left,&Speed_Left_Stop_PID);
		right_speed=PID_Location1(0,current_sp_right,&Speed_Right_Stop_PID);
		
   CAR_SPEED speed_set={left_speed,right_speed};
	 set_Speed(speed_set);

}
	

//*****************************************************元素分别控制******************************************************


//*****************************************************元素分别控制******************************************************