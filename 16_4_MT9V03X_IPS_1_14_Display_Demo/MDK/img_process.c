#include "img_process.h"

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ͼ��δ���������任����ͼ�����ϲ�����Ϊ0�����²�Ϊimg_height-1
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
int16 valid_row = img_height - 1;		//��Ч������
int16 longest_col=img_width/2;
int16 shortest_col=img_width/2;

uint8 image[MT9V03X_H][MT9V03X_W];
uint8 Threshold=Default_Threshold;
uint8 threshold[img_height];
int16 weight_array[img_height];

int16 delta=1;//ÿ������ɨ�����
int16 EdgeL,EdgeR,lastEdgeL,lastEdgeR;//��¼���ұ߽�
int16 searsh_mid_line;;//��Чɨ���м���
int16 lostLeft_Sign,lostRight_Sign,leftLostLine_Cnt,rightLostLine_Cnt;//���߱�־
int8 stateRing_Sign=0,directRing_Sign=-1;//Բ����ر�־λ
int8 stateBranch_Sign=0,directBranch_Sign=2;//��·��ر�־λ
int16 branch_num=0;
int16 narrow_track_num;
int8 stateGarage_Sign=0,directGarage_Sign=Garage_Out;//������ر�־λ
int8 stateRelay_Sign=0;//������ر�־λ

int16 repair_Cnt;

int16 d_neg,d_pos;
int16 track_Width[img_height];//��¼ɨ��߽�õ����������
int16 min_Length,min_Length_Index;


track_Type_Enum track=Straight;

int16 Border[3][img_height];
int16 halftrack_Width[img_height];
uint8 islost_record[3][img_height];
int16 col_length[img_width];

int16 default_judge_line=40;
uint8 centre_Delta_Exceed_Cnt;//Բ��ʶ����λ
uint8 left_Delta_Exceed_Cnt;
uint8 right_Delta_Exceed_Cnt;

int16 inflection_A,inflection_B,inflection_C;//Բ���ɽ���Զ���������յ�
int16 inflection_G;//����յ�
uint8 stop_flag=0;
bool zebra_flag=0;
//const uint8 track_width[img_height] = { 0 ,0 ,0 ,2 ,4 ,6 ,9 ,13 ,19 ,25 ,
//	32 ,37 ,45 ,49 ,53 ,60 ,63 ,71 ,75 ,79 ,
//	87 ,91 ,97 ,101 ,106 ,112 ,116 ,120 ,126 ,130 ,
//	135 ,139 ,144 ,150 ,153 ,156 ,159 ,164 ,167 ,169 ,
//	171 ,173 ,175 ,178 ,179 ,181 ,183 ,183 ,187 ,187 };


//*******************************************************���Ʊ�ʶ*******************************************
void Show_Line()
{
	for (int16 i = img_height - 1; i >= valid_row; i--)
    {
        image[i][Border[0][i]] = 0;
        image[i][Border[1][i]] = 0;
		  	image[i][Border[2][i]]= 0;
			  //image[i][img_width/2]= 0;  
    }
		//ips114_showint16(188,1,stateRelay_Sign);
	  ips114_showint16(188,1,stateRing_Sign);		
	  ips114_showint16(188,0,valid_row);
		ips114_showint16(188,2,branch_num);
}
//*******************************************************���Ʊ�ʶ*******************************************

//�жϱ��߷�Χ���Ƿ�������
uint8 is_Track(uint8 Row_Index)
{
    if (image[Row_Index][(Border[RIGHT][Row_Index] + Border[LEFT][Row_Index]) / 2] < Dark_Gray_Scale)
    {
        return 0;
    }
    return 1;
}

//***************************************************����Ԥ����****************************************************
//ƽ������
void Smooth_Edge()
{
    for (int16 i = img_height - 1; i > valid_row + 2; i--)
    {
        //�����쳣
        if (abs(Border[RIGHT][i] - Border[RIGHT][i - 2]) < 5
            && abs(Border[RIGHT][i] - Border[RIGHT][i - 1]) > 10)
        {
            Border[RIGHT][i - 1] = (Border[RIGHT][i] + Border[RIGHT][i - 2]) / 2;
        }
        if (abs(Border[LEFT][i] - Border[LEFT][i - 2]) < 5 && abs(Border[LEFT][i] - Border[LEFT][i - 1]) > 10)
        {
            Border[LEFT][i - 1] = (Border[LEFT][i] + Border[LEFT][i - 2]) / 2;
        }
        //˫���쳣
        if (abs(Border[RIGHT][i] - Border[RIGHT][i - 3]) < 5
            && abs(Border[RIGHT][i - 1] - Border[RIGHT][i - 2]) < 5
                && abs(Border[RIGHT][i] - Border[RIGHT][i - 1]) > 10)
        {
            Border[RIGHT][i - 1] = (Border[RIGHT][i] + Border[RIGHT][i - 3]) / 2;
            Border[RIGHT][i - 2] = (Border[RIGHT][i] + Border[RIGHT][i - 3]) / 2;
        }
        if (abs(Border[LEFT][i] - Border[LEFT][i - 3]) < 5
            && abs(Border[LEFT][i - 1] - Border[LEFT][i - 2]) < 5
                && abs(Border[LEFT][i] - Border[LEFT][i - 1]) > 10)
        {
            Border[LEFT][i - 1] = (Border[LEFT][i] + Border[LEFT][i - 3]) / 2;
            Border[LEFT][i - 2] = (Border[LEFT][i] + Border[LEFT][i - 3]) / 2;
        }
        //�����쳣
        if (abs(Border[RIGHT][i] - Border[RIGHT][i - 4]) < 5
            && abs(Border[RIGHT][i - 1] - Border[RIGHT][i - 3]) < 5
                && abs(Border[RIGHT][i] - Border[RIGHT][i - 1]) > 10)
        {
            Border[RIGHT][i - 1] = (Border[RIGHT][i] + Border[RIGHT][i - 4]) / 2;
            Border[RIGHT][i - 2] = (Border[RIGHT][i] + Border[RIGHT][i - 4]) / 2;
            Border[RIGHT][i - 3] = (Border[RIGHT][i] + Border[RIGHT][i - 4]) / 2;
        }
        if (abs(Border[LEFT][i] - Border[LEFT][i - 4]) < 5
            && abs(Border[LEFT][i - 1] - Border[LEFT][i - 3]) < 5
                && abs(Border[LEFT][i] - Border[LEFT][i - 1]) > 10)
        {
            Border[LEFT][i - 1] = (Border[LEFT][i] + Border[LEFT][i - 4]) / 2;
            Border[LEFT][i - 2] = (Border[LEFT][i] + Border[LEFT][i - 4]) / 2;
            Border[LEFT][i - 3] = (Border[LEFT][i] + Border[LEFT][i - 4]) / 2;
        }
    }
}

//***************************************************����Ԥ����****************************************************
track_Type_Enum current_element;
int8 m=0;
void refresh_current_element()
{
	if(narrow_track_num>8)
	{
		stateRing_Sign=0;
		directBranch_Sign=1;
		branch_num=1;
		zebra_flag=1;
		repair_Zebra();
	}
	else if(judge_Ring())
	{
		repair_Ring();
	}
	else if(judge_Branch())
	{
		repair_Branch();
	}
  if((stateBranch_Sign)&&!stop_flag)
		gpio_set(BUZZER_PIN, GPIO_HIGH);
	else if(stateRing_Sign)
	{
		if(m==2)
		{
		  gpio_set(BUZZER_PIN, GPIO_HIGH);
			m=0;
		}
		else
		{
			gpio_set(BUZZER_PIN, GPIO_LOW);
			m++;
		}
		
	}
	else
		gpio_set(BUZZER_PIN, GPIO_LOW);
	   
}
//*************************************************my****************************************************
void threshold_init()
{
	for (int16 i = img_height - 1; i >= 0; i--)
  {
		threshold[i]=Threshold-(i-img_height/2)*40/(img_height - 1);
	}
}
void get_longest_col()
{
	int16 max_length=0;
	int16 i,j,flag=0;
	memset(col_length,0,img_width*sizeof(int16));
	for (j = img_width - 51; j >= 50; j-=1)
	{
		for (i = img_height - 1; i >= 1; i-=1)
		{ 
			if(mt9v03x_image[i][j]<=threshold[i])
			{
				col_length[j]=img_height-1-i;
				if(max_length < img_height-1-i)
		    {
			    max_length=img_height-1-i;
			    longest_col=j;
			    valid_row=i;
					flag=1;
		    }
				break;
			}
		}
		if(i<=1)
			{
				max_length=img_height-1;
			  longest_col=j;
			  valid_row=i;
			  flag=1;
				break;
			}
	}
	if(!flag)
	{
		longest_col=94;
		valid_row=119;
	}
}

void get_shortest_col()
{
{
	int16 min_len=img_height;
	int16 shortest_col=0;
	int16 i;
	int16 left_jump=0,right_jump=0;
	int16 left=80,right=img_width-81;
	for(i= 51; i <= img_width-52; i++)
	{
		if(col_length[i]-col_length[i+1]>15)
			right_jump=i;
		else if(col_length[i]-col_length[i-1]>15)
      left_jump=i;			
	}
	if(left_jump&&right_jump-left_jump>30)
	{
		left=left_jump;
		right=right_jump;
	}
	for (i= left; i <= right; i++)
	{
		if(col_length[i]<min_len)
		{
			min_len=col_length[i];
			shortest_col=i;
		}
	}
	min_Length=img_height-1-min_len;
}
}

void mid_point_filter()
{
	for (int16 i = img_height - 1; i >= valid_row; i--)
  {
		Border[MID][i]= (Border[LEFT][i]+Border[RIGHT][i])/2;
    if(i<img_height - 1)
		{			
		if (Border[MID][i] - Border[MID][i+1] > 5)
        {
            Border[MID][i] = Border[MID][i+1] + 2;
        }
    else if (Border[MID][i+1] - Border[MID][i] > 5)
        {
            Border[MID][i] = Border[MID][i+1] - 2;
        }
		}
	}
}

void erzhihua()
{
	for (int16 i = img_height - 1; i >= 0; i--)
  {
		for (int j = img_width - 1; j >= 0; j--)
  {
		if(mt9v03x_image[i][j]<=threshold[i]
//			&&(mt9v03x_image[i+2][j]<=Threshold
//		||mt9v03x_image[i-2][j]<=Threshold
//		||mt9v03x_image[i][j+2]<=Threshold
//		||mt9v03x_image[i][j-2]<=Threshold)
		)
			image[i][j]=0;
		else
			image[i][j]=255;
		
	}
	}
}

uint8 find_Zebra_y()
{
	int16 a=0;
	int16 i,j,jump;
	for(j = 30; j <= img_width-31; j+=2)
	{
		jump=0;
		for(i = img_height-2; i >= valid_row; i--)
		{
			if(image[i][j]!=image[i+1][j])
			{
				jump++;
			}
		}
		if(jump>=10)
		{
			a++;
		}
		else
		{
			a=0;
		}
		if(a>=2)
		{
			return 1;
		}
	}
	return 0;
}

int16 count_Zebra_y()
{
	int16 i,j,jump,jump_max=0,start,flag=0;
	for(j = 30; j <= img_width-31; j+=2)
	{
		jump=0;
		flag=0;
		for(i = img_height-2; i >= valid_row+1; i--)
		{
			if(image[i][j]!=image[i-1][j])
			{
				if(!flag)
				start=i;
				flag=1;
				jump++;
			}
		}
		if(jump>=10)
		{
			return start;
		}
	}
		return 0;
}


void repair_Zebra()
{
	for(int16 i=img_height-2;i>valid_row;i--)
	{
		if(track_Width[i]<track_Width[i+1]-15)
		{
			Border[RIGHT][i]=Border[RIGHT][i+1]-1;
			Border[LEFT][i]=Border[LEFT][i+1]+1;
			track_Width[i]=track_Width[i+1]-2;
		}
	}
}

uint8 relay_start()
{
	if(receive_message()==5)
		return 1;
	else
		return 0;
}
void judge_relay()
{
	switch(stateRelay_Sign)
	{
		case 0:
			if(branch_num>=2)
			{	
				tim_interrupt_init(TIM_7,5,0,0);
	      tim_interrupt_enable(TIM_7);
			}
			break;
		case 1:
			if(relay_start())
			{
				stop_flag=0;
				stateRelay_Sign=0;
			}
			else
			{			
				stop_flag=2;
			}
			break;
		case 2:
			stop_flag=2;
			send_message(5);
			break;
	}
}

void send_message(uint8 message)
{
		uart_putchar(UART_4,message);
}
uint8 receive_message()
{
	uint8 receive_data=0;
	uart_getchar(UART_4,&receive_data);
	return receive_data;
}
void if_stop()
{
  if(stateRelay_Sign==1)
		stop_flag=2;
	else if(valid_row>115||stateRelay_Sign==2)
	{
		stop_flag=1;
	}
	else
	{
		stop_flag=0;
	}
	
}
void weight_array_init()
{
	memset(col_length,0,img_height*sizeof(int16));
	for(int16 i=img_height-1;i>=img_height/2;i--)
  {
		weight_array[i]=img_height-(img_height*3/4-i)*(img_height*3/4-i)*16/img_height;
	}
}

bool if_black_white(int16 start,int16 end)
{
	  int8 white_black,black_white;
	  for(int16 i=min_Length-2;i>=min_Length-3;i--)
	  {
		white_black=0,black_white=0;
		for(int16 j=start;j<=end-1;j++)
		{
			if(!white_black)
			{
			if(mt9v03x_image[i][j]>threshold[i]&&mt9v03x_image[i][j+1]<threshold[i])
				white_black++;
		  }
			else if(mt9v03x_image[i][j]<threshold[i]&&mt9v03x_image[i][j+1]>threshold[i])
			{
				black_white++;
				break;
			}
		}
		if(!white_black||!black_white)
				return 0;
	  }
		return 1;
}
//*************************************************chen****************************************************


uint8 absolute(uint8 a,uint8 b)//����ֵ����
{
    if(a>b)
        return a-b;
    else
        return b-a;
}
uint8 Threshold;
uint8 OTSU(uint8 *pre_image)//��̬������ֵ
{
    uint16 piexlCount[256];
    uint8 Threshold_Max=200,Threshold_Min=70;

    for(int i=0;i<256;i++)//��ʼ������
        piexlCount[i]=0;

    for(int i=0;i<img_height;i++)//ͳ��ÿ���Ҷ��ж��ٸ�����
        for(int j=0;j<img_width;j++)
        {
            int tmp=*(pre_image+i*img_width+j);
            piexlCount[tmp]++;
        }
    int32 deltaMax=0,deltaTmp;//��䷽��
    for(int i=Threshold_Min;i<Threshold_Max;i++)//Ѱ��ʹ��䷽��������ֵ
    {
        int32 N0,N1,U0,U1,U0tmp,U1tmp;
        N0=N1=U0=U1=U0tmp=U1tmp=0;
        for(int j=0;j<256;j++)
        {
            if(j<=i)//�ϰ�����
            {
                N0=N0+piexlCount[j];
                U0tmp=U0tmp+j*piexlCount[j];
            }
            else//��������
            {
                N1=N1+piexlCount[j];
                U1tmp=U1tmp+j*piexlCount[j];
            }
        }
        U0=U0tmp/N0;
        U1=U1tmp/N1;
        deltaTmp=N0*N1*(U0-U1)*(U0-U1);
        if(deltaTmp>deltaMax)
        {
            deltaMax=deltaTmp;
            Threshold=i;
        }
    }

    return Threshold;
}

uint8 trackBorder_Get_Cnt=0;
int16 last_start;
void trackBorder_Get()//Ԥ����߽�
{
    memset(Border[LEFT],0,img_height*sizeof(int));//���ݸ�λ
    memset(Border[MID],img_width/2,img_height*sizeof(int));
    memset(Border[RIGHT],img_width-1,img_height*sizeof(int));

    left_Delta_Exceed_Cnt=0;
	  right_Delta_Exceed_Cnt=0;
    leftLostLine_Cnt=0;//����������ʼ��
    rightLostLine_Cnt=0;
    lostLeft_Sign=0;
    lostRight_Sign=0;
	  
	  int16 narrow=0;
    narrow_track_num=0;
    min_Length=-1;
    for(int16 i=img_height-1;i>=valid_row;i--)//ɨ�����ұ��߲���������
    {
			  for(int j=longest_col-delta;j>=0;j=j-delta)//ɨ����߽�
        {
            if(mt9v03x_image[i][j]<threshold[i])
            {
                EdgeL=j;
                //islost_record[LEFT][i]=0;
                break;
            }
            else if(j==0)//δ�ҵ������
            {
                EdgeL=0;
                lostLeft_Sign=1;
                //islost_record[LEFT][i]=1;
							  if(i<img_height-30)
                  leftLostLine_Cnt++;
                break;
            }
        }
        for(int j=longest_col+delta;j<img_width-1;j=j+delta)//ɨ���ұ߽�
        {
            if(mt9v03x_image[i][j]<threshold[i])
            {
                EdgeR=j;
                //islost_record[RIGHT][i]=0;
                break;
            }
            else if(j==img_width-2)//δ�ҵ������
            {
                EdgeR=img_width-2;
                lostRight_Sign=1;
                //islost_record[RIGHT][i]=1;
							  if(i<img_height-30)
                  rightLostLine_Cnt++;
                break;
            }
        }

        Border[LEFT][i]=EdgeL;//��߽�
        Border[RIGHT][i]=EdgeR;//�ұ߽�
        Border[MID][i]=(EdgeL+EdgeR)>>1;//��ʱ����

        track_Width[i]=Border[RIGHT][i]-Border[LEFT][i];

        if(i<img_height-1)//Ϊ���Բ����׼��
        {
					if(Border[LEFT][i]-Border[LEFT][i+1]>15||Border[LEFT][i]-Border[LEFT][i+1]<-15)
						left_Delta_Exceed_Cnt++;
					if(Border[RIGHT][i]-Border[RIGHT][i+1]>15||Border[RIGHT][i]-Border[RIGHT][i+1]<-15)
						right_Delta_Exceed_Cnt++;
        }
				if(i>valid_row+10)
				{
				  if(track_Width[i]<15)
				  	{
				  		narrow++;
				  		if(narrow>narrow_track_num)
			  				narrow_track_num=narrow;
			  		}
			  		else
			  			narrow=0;
				}
    }
}





uint8 is_Straight(int16 start_index,int16 end_index,uint8 side)//�ж�ĳһ�ߴ���㵽�յ��Ƿ�Ϊֱ��
{
        int16 white_cnt=0;
			  float k = (float)(Border[side][start_index] - Border[side][end_index]) / (start_index - end_index);
            for (int i = (start_index < end_index ? start_index : end_index);
                 i <= (start_index < end_index ? end_index : start_index); i++)
            {
                if (abs(Border[side][i] - (k * (i - start_index) + Border[side][start_index])) > 1.2)
                {
                    return 0;
                }   
								if(Border[side][i]<=1||Border[side][i]>=img_width-2)
								{
									white_cnt++;
									if (white_cnt>=10)
										return 0;
								}
								
                if (i == (start_index < end_index ? end_index : start_index))
                {
                    return 1;
                }
            }
        return 0;
}

uint8 find_Zebra()
{
	int16 a=0;
	int16 i,j,jump;
	for(i = img_height - 1; i >= valid_row+30; i--)
	{
		jump=0;
		for(j = Border[0][119]; j <= Border[2][119]; j++)
		{
			if((mt9v03x_image[i][j]>Threshold&&mt9v03x_image[i][j+1]<Threshold)
				||(mt9v03x_image[i][j]<Threshold&&mt9v03x_image[i][j+1]>Threshold))
			{
				jump++;
			}
		}
		if(jump>=10)
		{
			a++;
		}
		else
		{
			a=0;
		}
		if(a>=2)
		{
			return 1;
		}
	}
	return 0;
}

int curvature=0;
int centre_line_get()//��ȡ����ƫ��ֵ
{
	  static int cur=0,cur_last=0;
    int sum_A=0,sum_B=0;
    int16 cnt=1;
	  curvature=0;

    Border[MID][img_height-1]=(Border[LEFT][img_height-1]+Border[RIGHT][img_height-1])>>1;

    for(int16 i=img_height-1;i>=valid_row;i--)//&&i>=img_height/2
    {
        Border[MID][i]=(Border[LEFT][i]+Border[RIGHT][i])>>1;
			  if(stateRing_Sign==2)
		    {
	      if(i<=inflection_C)		
				{
					Border[MID][i]=Border[MID][inflection_C+1]+1*(inflection_C+1-i)*(directRing_Sign-1);
				}
			  }

				
//				sum_A=sum_A+weight_array[i]*(img_width/2-Border[MID][i]);
//        sum_B=sum_B+weight_array[i];
				
				sum_A=sum_A+i*i*(img_width/2-Border[MID][i]);
        sum_B=sum_B+i*i;
				

				
    }
    return sum_A/sum_B;
}

uint8 find_Inflection_C(int16 start,int16 end,int dir)//Զ���յ�
{
    if(dir==LEFT)
    {
        for(int16 i=end-1;i>=start+1;i--)
        {
            if(Border[LEFT][i]-Border[LEFT][i+1]>6)
            {
                    if((Border[LEFT][i]+Border[RIGHT][i])/2>80
										&&(Border[LEFT][i]-Border[LEFT][i-1]>=-5)
										&&track_Width[i]<105)
										{
							        inflection_C=i;
                      return 1;
										}
                
            }
        }
    }
    else
    {
        for(int16 i=end-1;i>=start+1;i--)
        {
            if(Border[RIGHT][i]-Border[RIGHT][i+1]<-6)
            {
                    if((Border[LEFT][i]+Border[RIGHT][i])/2<img_width-81
										&&(Border[RIGHT][i]-Border[RIGHT][i-1]<=5)
										&&track_Width[i]<105)
										{
							        inflection_C=i;
                      return 1;
										}
            }
        }
    }
    inflection_C=0;
    return 0;
}

uint8 find_Inflection_B(int16 start,int16 end,int dir)//�м�յ�
{
    if(dir==LEFT)
    {
        for(int16 i=end-1;i>=start+1;i--)
            if(Border[LEFT][i]-Border[LEFT][i-1]>0)
            {
              if(Border[LEFT][i]-Border[LEFT][i+1]>=0)
						  {
							  inflection_B=i;
                return 1;
						  }
            }
    }
    else
    {
        for(int16 i=end-1;i>=start+1;i--)
            if(Border[RIGHT][i]-Border[RIGHT][i-1]<0)
            {
              if(Border[RIGHT][i]-Border[RIGHT][i+1]<=0) 
							{
							  inflection_B=i;
                return 1;
							}
            }
    }
    inflection_B=0;
    return 0;
}

uint8 find_Inflection_A(int16 start,int16 end,int dir,int sign)//�����յ�
{
    if(dir==LEFT)
    {
        if(sign==0)//������A��
        {
            for(int16 i=end-1;i>=start+2;i--)
                if(Border[LEFT][i]-Border[LEFT][i-1]>15&&Border[LEFT][i]-Border[LEFT][i-2]>15)
								{
                  if(Border[LEFT][i]-Border[LEFT][i+1]>=0&&Border[LEFT][i]-Border[LEFT][i+1]<5||1)
									{
									  inflection_A=i;
                    return 1;
									}
                }
        }
        else if(sign==1)//������A��
        {
            for(int16 i=end-1;i>=start+2;i--)
                if(Border[RIGHT][i]<Border[RIGHT][i-1]&&Border[RIGHT][i]<Border[RIGHT][i-2]-30)
                {
									if(Border[RIGHT][i]<=Border[RIGHT][i+1])
									{
                    inflection_A=i;
                    return 1;
									}
                }
        }
    }
    else
    {
        if(sign==0)//������A��
        {
            for(int16 i=end-1;i>=start+2;i--)
                if(Border[RIGHT][i]-Border[RIGHT][i-1]<-15&&Border[RIGHT][i]-Border[RIGHT][i-2]<-15)
                {
                  if(Border[RIGHT][i]-Border[RIGHT][i+1]<=0&&Border[RIGHT][i]-Border[RIGHT][i+1]>-5||1) 
                  {										
									  inflection_A=i;
                    return 1;
									}
                }
        }
        else if(sign==1)//������A��
        {
            for(int16 i=end-1;i>=start+2;i--)
                if(Border[LEFT][i]>Border[LEFT][i-1]&&Border[LEFT][i]>Border[LEFT][i-2]+30)
                {
									if(Border[LEFT][i]>=Border[LEFT][i+1])
									{
                    inflection_A=i;
                    return 1;
									}
                }
        }
    }
    inflection_A=0;
    return 0;
}



uint8 judge_Ring()//ʶ��Բ�����ж�����״̬
{
    switch(stateRing_Sign)
    {
    case 0://��ʼ״̬������Ƿ�����Բ��
        if(right_Delta_Exceed_Cnt>=2&&left_Delta_Exceed_Cnt<=0)//������Բ��
        {
            //ips114_showint16(188,3,11);
					  if(find_Inflection_A(valid_row+20, img_height-1, RIGHT,0))//���һ���
            {
							//ips114_showint16(188,4,22);
							if(is_Straight(valid_row+5, img_height-16, LEFT))//
							{
								//ips114_showint16(188,5,33);
								if(find_Inflection_B(valid_row+5, inflection_A-5, RIGHT))
							  {
									//ips114_showint16(188,6,44);
                  stateRing_Sign=1;
                  track=Ring;
                  directRing_Sign=RIGHT;
							    return 1;
								}
							}
            }
				}
				else if(left_Delta_Exceed_Cnt>=2&&right_Delta_Exceed_Cnt<=0)
				{
            if(find_Inflection_A(valid_row+20, img_height-1, LEFT,0))//���󻷣�
            {
							if(is_Straight(valid_row+5, img_height-16, RIGHT))//
							{
								if(find_Inflection_B(valid_row+5, inflection_A-5, LEFT))
								{
                  stateRing_Sign=1;
                  track=Ring;
                  directRing_Sign=LEFT;
							    return 1;
								}
							}
            }
        }
				//ips114_showint16(188,7,00);
        return 0;
    case 1://����1����״̬����ⲻ��A�յ�ʱ������һ�׶�
        if(!find_Inflection_A(40, img_height-1, directRing_Sign,0)&&find_Inflection_C(valid_row, img_height-21, directRing_Sign))
            stateRing_Sign=2;
        return 1;
    case 2://����2����״̬����ⲻ��C�յ�ʱ������һ״̬
        if(!find_Inflection_C(valid_row, img_height-1, directRing_Sign))
            stateRing_Sign=3;
        return 1;
    case 3://����״̬����⵽A�յ�ʱ����ó�������
        if(find_Inflection_A(valid_row, img_height-1, directRing_Sign,1))
            stateRing_Sign=4;
        return 1;
    case 4://����3����״̬���ܼ�⵽C�յ�����ѳ���
        if((valid_row<25)&&find_Inflection_C(valid_row, img_height-21, directRing_Sign)&&!find_Inflection_A(valid_row, img_height-1, directRing_Sign,1))
            stateRing_Sign=5;
        return 1;
    case 5://����4����״̬����������������ж������ߴ����뿪Բ��
        if(!find_Inflection_C(valid_row, img_height-1, directRing_Sign))
        {
                stateRing_Sign=0;
                track=Straight;
					      return 0;
        }
        return 1;
    }
    return 0;
}

void repair_Ring()//Բ������
{
    switch(stateRing_Sign)
    {
    case 1://����1����
        if(directRing_Sign==LEFT)//�󻷲���
        {
            int16 i=inflection_A-1;
            repair_Cnt=1;
            int16 tmp=Border[LEFT][inflection_A]+1.1*repair_Cnt;
            while(i>=0&&tmp>Border[LEFT][i])
            {
                Border[LEFT][i]=tmp;
                repair_Cnt++;
                i--;
                tmp=Border[LEFT][inflection_A]+1.1*repair_Cnt;
            }
        }
        else //�һ�����
        {
            int16 i=inflection_A-1;
            repair_Cnt=1;
            int16 tmp=Border[RIGHT][inflection_A]-1.1*repair_Cnt;
            while(i>=0&&tmp<Border[RIGHT][i])
            {
                Border[RIGHT][i]=tmp;
                repair_Cnt++;
                i--;
                tmp=Border[RIGHT][inflection_A]-1.1*repair_Cnt;
            }
        }
        break;
    case 2://����2����
        if(directRing_Sign==LEFT)//�󻷲���
        {
            int16 i=inflection_C+1;
            repair_Cnt=1;
            float k;
            k=3;//1.8
            while(i<img_height&&Border[LEFT][inflection_C]+k*repair_Cnt<Border[RIGHT][i])//��C�յ����
            {
               int16 tmp=Border[LEFT][inflection_C]+k*repair_Cnt;//ϵ������
               if(tmp>img_width-1)
                   tmp=img_width-1;
               Border[RIGHT][i]=tmp;
               repair_Cnt++;
               i++;
            }

            if(find_Inflection_B(inflection_C+2, img_height-1, LEFT))//���ҵ�B�յ㣬��B�յ����
            {
               repair_Cnt=1;
               for(i=inflection_B+1;i<img_height;i++)
               {
                   int16 tmp=Border[LEFT][inflection_B]-1.1*repair_Cnt;//ϵ������
                   if(tmp<0)
                       tmp=0;
                   Border[LEFT][i]=tmp;
                   repair_Cnt++;
               }
            }
        }
        else//�һ�����
        {
            int16 i=inflection_C+1;
            repair_Cnt=1;
            float k;
            k=3;//2

            while(i<img_height&&Border[RIGHT][inflection_C]-k*repair_Cnt>Border[LEFT][i])//��C�յ����
            {
               int16 tmp=Border[RIGHT][inflection_C]-k*repair_Cnt;//ϵ������
               if(tmp<0)
                   tmp=0;
               Border[LEFT][i]=tmp;
               repair_Cnt++;
               i++;
            }

            if(find_Inflection_B(inflection_C+2, img_height-1, RIGHT))//���ҵ�B�յ㣬��B�յ����
            {
               repair_Cnt=1;
               for(i=inflection_B+1;i<img_height;i++)
               {
                   int16 tmp=Border[RIGHT][inflection_B]+1.1*repair_Cnt;//ϵ������
                   if(tmp>img_width-1)
                       tmp=img_width-1;
                   Border[RIGHT][i]=tmp;
                   repair_Cnt++;
               }
            }

        }
        break;
		case 4://����3����
			  float k;
		    if(directRing_Sign==LEFT)
        {		  
            int16 i=img_height-1;
					  int16 inflection=img_height-1;
            repair_Cnt=1;
					  k=1.2;
					  if(find_Inflection_A(valid_row, img_height-1, LEFT,1)==1)
            {
							 i=inflection_A-1;
							 inflection=inflection_A-1;
						}							
							 while(i>valid_row&&Border[RIGHT][inflection]-k*repair_Cnt>Border[LEFT][i])//��A�յ����
            {
                   int16 tmp=Border[RIGHT][inflection]-k*repair_Cnt;//ϵ������
                   if(tmp<0)
                   tmp=0;
                   Border[RIGHT][i]=tmp;
                   repair_Cnt++;
                   i--;
            }
						valid_row=i;
        }
        else
        {
            int16 i=img_height-1;
					  int16 inflection=img_height-1;
            repair_Cnt=1;
					  k=1.2;
					  if(find_Inflection_A(valid_row, img_height-1, RIGHT,1)==1)
            {
							  i=inflection_A-1;
							  inflection=inflection_A-1;
						}
							  while(i>valid_row&&Border[LEFT][inflection]+k*repair_Cnt<Border[RIGHT][i])//��A�յ����
            {
                   int16 tmp=Border[LEFT][inflection]+k*repair_Cnt;//ϵ������
                  if(tmp>img_width-1)
                       tmp=img_width-1;
                   Border[LEFT][i]=tmp;
                   repair_Cnt++;
                   i--;
            }
						valid_row=i;
				}
			  break;
    case 5://����4����
        if(directRing_Sign==LEFT)
        {
            if(find_Inflection_C(valid_row, img_height-1, LEFT)==1)
            {
                repair_Cnt=1;
                for(int i=inflection_C+1;i<img_height;i++)
                {
                    int16 tmp=Border[LEFT][inflection_C]-1.1*repair_Cnt;
                    if(tmp<0)
                        tmp=0;
                    Border[LEFT][i]=tmp;
                    repair_Cnt++;
                }
            }
        }
        else
        {
            if(find_Inflection_C(valid_row, img_height-1, RIGHT)==1)
            {
                repair_Cnt=1;
                for(int i=inflection_C+1;i<img_height;i++)
                {
                    int16 tmp=Border[RIGHT][inflection_C]+1.1*repair_Cnt;
                    if(tmp>img_width-1)
                        tmp=img_width-1;
                    Border[RIGHT][i]=tmp;
                    repair_Cnt++;
                }
            }
        }
        break;
    }
}

void re_trackBorder_Get()
{

    for(int i=img_height-1;i>=min_Length;i--)//ɨ�����ұ��߲���������
    {
        for(int j=min_Length_Index-delta;j>=0;j=j-delta)//ɨ����߽�
        {
            if(mt9v03x_image[i][j]<Threshold)
            {
                EdgeL=j;
                break;
            }
            else if(j==0)//δ�ҵ������
            {
                EdgeL=0;
                break;
            }
        }
        for(int j=min_Length_Index+delta;j<img_width-1;j=j+delta)//ɨ���ұ߽�
        {
            if(mt9v03x_image[i][j]<Threshold)
            {
                EdgeR=j;
                break;
            }
            else if(j==img_width-2)//δ�ҵ������
            {
                EdgeR=img_width-2;
                break;
            }
        }
        Border[LEFT][i]=EdgeL;//��߽�
        Border[RIGHT][i]=EdgeR;//�ұ߽�
    }
}

uint8 is_Branch()
{
	get_shortest_col();
	if(shortest_col>85&&shortest_col<img_width-86&&col_length[shortest_col]<110&&rightLostLine_Cnt>1&&leftLostLine_Cnt>1)
	{
    int16 i;
		bool track_flag=0;
    for(i=img_height-1;i>=min_Length+2;i--)
    {
			if(Border[RIGHT][i]-Border[RIGHT][i-1]<0			
      &&Border[LEFT][i]-Border[LEFT][i-1]>0
			&&Border[RIGHT][i-1]-Border[RIGHT][i-2]<0			
      &&Border[LEFT][i-1]-Border[LEFT][i-2]>0
//			&&Border[RIGHT][i-1]-Border[RIGHT][i-2]>-10			
//      &&Border[LEFT][i-1]-Border[LEFT][i-2]<10
			)
			{
				track_flag=1;
				break;
			}
		}
		if(!track_flag)
			return 0;
		int16 track_row=i;
		return if_black_white(Border[LEFT][track_row],Border[RIGHT][track_row]);
	}
	return 0;
}


uint8 judge_Branch()
{
    switch(stateBranch_Sign)
    {
    case 0:
        if(is_Branch())//&&(branch_num==0||zebra_flag))
        {
					  stateBranch_Sign=1;
					  branch_num++;				  				  
					  return 1;
        }
        return 0;
    case 1:
			  get_shortest_col();
        if(shortest_col<85||shortest_col>img_width-86||col_length[shortest_col]>110||rightLostLine_Cnt<1||leftLostLine_Cnt<1)
				{
					 stateBranch_Sign=0;
					 directBranch_Sign=1;
           return 0;
				}
				return 1;
    }
		return 0;
}

void repair_Branch()//��·����
{
    switch(stateBranch_Sign)
    {
    case 1:
        if(directBranch_Sign==2)//��ת����·
        {
            repair_Cnt=1;
            float k=2;
            for(int16 i=min_Length+1;i<=img_height-1;i++)
            {
                int16 tmp=shortest_col+k*repair_Cnt;
                if(tmp>img_width-1)
                    tmp=img_width-1;
                Border[RIGHT][i]=tmp;
                repair_Cnt++;
            }
        }
        else if(directBranch_Sign==1)//��ת����·
        {
            repair_Cnt=1;
            float k=2;
            for(int16 i=min_Length+1;i<=img_height-1;i++)
            {
                int16 tmp=shortest_col-k*repair_Cnt;
                if(tmp<0)
                    tmp=0;
                Border[LEFT][i]=tmp;
                repair_Cnt++;
            }
        }
        break;
		}
}

float cal_Curvature()
{
    float a_x=img_height-1,a_y=Border[MID][img_height-1];
    float b_x=(img_height-1+valid_row)>>1,b_y=Border[MID][(img_height-1+valid_row)>>1];
    float c_x=valid_row,c_y=Border[MID][valid_row];

    float tmp,s,a,b,c;
    tmp=a_x*b_y+b_x*c_y+c_x*a_y-a_y*b_x-b_y*c_x-c_y*a_x;
    if(tmp<0)
        tmp=-tmp;
    s=tmp/2;
    a=sqrtf((b_x-c_x)*(b_x-c_x)+(b_y-c_y)*(b_y-c_y));
    b=sqrtf((a_x-c_x)*(a_x-c_x)+(a_y-c_y)*(a_y-c_y));
    c=sqrtf((a_x-b_x)*(a_x-b_x)+(a_y-b_y)*(a_y-b_y));
    return (a*b*c)/(4*s);
}

