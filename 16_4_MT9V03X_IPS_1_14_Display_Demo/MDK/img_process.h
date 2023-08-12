#ifndef _img_process_h
#define _img_process_h
#include "SEEKFREE_MT9V03X.h"
#include "headfile.h"
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ͼ��δ���������任����ͼ�����ϲ�����Ϊ0�����²�Ϊimg_height-1
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#define img_width       MT9V03X_W
#define img_height      MT9V03X_H

//extern const uint8 track_width[img_height];
extern uint8 image[MT9V03X_H][MT9V03X_W];
extern uint8 stop_flag;
extern uint8 Threshold;
extern int8 stateRelay_Sign;
extern int8 mode;
extern int curvature;

//*******************************************************�궨*******************************************
void Camera_Calibration();
void Get_Track_Width();
//*******************************************************�궨*******************************************

//*******************************************************���Ʊ�ʶ*******************************************
void Show_Line();
//*******************************************************���Ʊ�ʶ*******************************************

//***************************************************�ұ���****************************************************
extern int16 valid_row;						//��Ч���߷�Χ��0:valid_row)
#define Valid_Row_Width         6			//��Ч���ж�������������ȣ�
#define Gray_Scale_Difference   20			//�߽��жϻҶȲ�ֵ
#define Light_Gray_Scale		80			//ȫ�������ػҶ�ֵ
#define Dark_Gray_Scale			80			//ȫ�ְ����ػҶ�ֵ
void Get_Edge_Related();                                        //��ȡ���ұ߽缰����
uint8 is_Track(uint8 Row_Index);								//�жϱ��߷�Χ���Ƿ�������
//***************************************************�ұ���****************************************************

//***************************************************����Ԥ����****************************************************
typedef enum
{
	Right_Edge,
	Left_Edge
}EdgeSide;

void Smooth_Edge();
//***************************************************����Ԥ����****************************************************

//*************************************************my****************************************************
#define Default_Threshold 120    //��ֵ����ֵ
void threshold_init();
void weight_array_init();
void get_longest_col();
void get_shortest_col();
void mid_point_filter();
void erzhihua();
void refresh_current_element();
uint8 find_Zebra();
uint8 find_Zebra_y();
int16 count_Zebra_y();
void repair_Zebra();
uint8 relay_start();
void judge_relay();
void send_message(uint8 message);
uint8 receive_message();
void if_stop();
bool if_black_white(int16 start,int16 end);
//*************************************************chen****************************************************
extern int16 Border[3][img_height];
#define LEFT 0
#define MID 1
#define RIGHT 2
#define Default_Straight_Judge 40

#define Garage_Out 0
#define Garage_In 1
typedef enum{
    Straight=0,
    Corner,
    Crossing,
    Ring,
    Branch,
    Ramp,
    Garage,
    Stop
}track_Type_Enum;
extern track_Type_Enum current_type_element;
	
void trackBorder_Get();
void specialElem_Judge();
int centre_line_get();
uint8 OTSU(uint8 *pre_image);
uint8 judge_Ring();
void repair_Ring();
uint8 is_Branch();
uint8 judge_Branch();
void repair_Branch();
float cal_Curvature();
uint8 find_Inflection_A(int16 start,int16 end,int dir,int sign);
uint8 find_Inflection_B(int16 start,int16 end,int dir);
uint8 find_Inflection_C(int16 start,int16 end,int dir);
uint8 is_Straight(int16 start_index,int16 end_index,uint8 side);
#endif
