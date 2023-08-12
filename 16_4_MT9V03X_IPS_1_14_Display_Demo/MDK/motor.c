#include "headfile.h"
//*****************************************Motor*****************************************
void motor_init(void)
{
	gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);								// GPIO ��ʼ��Ϊ��� Ĭ�����������
	gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);								// GPIO ��ʼ��Ϊ��� Ĭ�����������
	pwm_init(PWM_TIM, PWM_L, 10000, 0);												// PWM ͨ��2 ��ʼ��Ƶ��10KHz ռ�ձȳ�ʼΪ0
	pwm_init(PWM_TIM, PWM_R, 10000, 0);												// PWM ͨ��4 ��ʼ��Ƶ��10KHz ռ�ձȳ�ʼΪ0
}
//���õ���ٶ�
int8 duty_L = 0;
int8 duty_R = 0;
bool dir_L = true;
bool dir_R = true;

void set_Speed(CAR_SPEED speed)
{
    //����
    if(speed.left_speed>=0)
    {
        if(6.1*speed.left_speed<MAX_DUTY*500)
        {
            gpio_set(DIR_L,0);
					  pwm_duty_updata(PWM_TIM, PWM_L, 6.1*speed.left_speed);
        }
        else
        {
            gpio_set(DIR_L,0);
            pwm_duty_updata(PWM_TIM, PWM_L, MAX_DUTY*500);
        }
    }
    else
    {
        if(6.1*speed.left_speed>-MAX_DUTY*500)
        {
            gpio_set(DIR_L,1);
            pwm_duty_updata(PWM_TIM, PWM_L, -6.1*speed.left_speed);
        }
        else
        {
            gpio_set(DIR_L,1);
            pwm_duty_updata(PWM_TIM, PWM_L, MAX_DUTY*500);
        }
    }
    //����
    if(speed.right_speed>=0)
    {
        if(6.1*speed.right_speed<MAX_DUTY*500)
        {
            gpio_set(DIR_R,1);
            pwm_duty_updata(PWM_TIM,PWM_R,6.1*speed.right_speed);
        }
        else
        {
            gpio_set(DIR_R,1);
            pwm_duty_updata(PWM_TIM,PWM_R,MAX_DUTY*500);
        }
    }
    else
    {
        if(6.1*speed.right_speed>-MAX_DUTY*500)
        {
            gpio_set(DIR_R,0);
            pwm_duty_updata(PWM_TIM,PWM_R,-6.1*speed.right_speed);
        }
        else
        {
            gpio_set(DIR_R,0);
            pwm_duty_updata(PWM_TIM,PWM_R,MAX_DUTY*500);
        }
    }
}
//*****************************************Motor*****************************************

//*****************************************Encoder*****************************************
//��ʼ��������
void Encoder_init()
{
    tim_encoder_init(ENCODER1_TIM, ENCODER1_A, ENCODER1_B);
	  tim_encoder_init(ENCODER2_TIM, ENCODER2_A, ENCODER2_B);
}

//��ȡ������ֵ
CAR_SPEED Encoder_Get_Value()
{
    CAR_SPEED speed;
    speed.right_speed = -tim_encoder_get_count(ENCODER2_TIM);
    speed.left_speed = tim_encoder_get_count(ENCODER1_TIM);
    return speed;
}

//��ձ�����ֵ
void Encoder_Clean()
{
    tim_encoder_rst(ENCODER1_TIM);
    tim_encoder_rst(ENCODER2_TIM);
}
//*****************************************Encoder*****************************************

uint64 time=0;  //��ʱ��λ��100ms
//*****************************************************Car_Speed******************************************************
CAR_SPEED speed_1,speed_2={0,0};
CAR_SPEED Get_Speed(int T)//��������ȡ����ٶ�           T��������uint:ms        �����ٶ�uint:mm/s
{
    CAR_SPEED speed={0,0};
	  speed_1=speed_2;
    speed_2=Encoder_Get_Value();
    speed_2.left_speed*=(21.7/T);
    speed_2.right_speed*=(21.7/T);
	  speed.left_speed=0.7*speed_2.left_speed+0.3*speed_1.left_speed;//��ͨ�˲�
		speed.right_speed=0.7*speed_2.right_speed+0.3*speed_1.right_speed;
	  Encoder_Clean();
    return speed;
}
//*****************************************************Car_Speed******************************************************


//*****************************************pit_init*****************************************
void pit_init()
{
	tim_interrupt_init(TIM_6,1000,3,3);
	tim_interrupt_enable(TIM_6);
}
//*****************************************pit_init*****************************************