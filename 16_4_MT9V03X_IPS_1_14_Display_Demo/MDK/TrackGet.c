/*
 * TrackGet.c
 *
 *  Created on: 2021��2��7��
 *      Author: 16917
 */
#include "headfile.h"
#include "TrackGet.h"

int16 delta=1;//ÿ������ɨ�����
int16 EdgeL,EdgeR,lastEdgeL,lastEdgeR;//��¼���ұ߽�
int16 validLine,searsh_mid_line;;//��Чɨ���м���
int16 lostLeft_Sign,lostRight_Sign,leftLostLine_Cnt,rightLostLine_Cnt;//���߱�־
int8 stateRing_Sign=0,directRing_Sign=-1;//Բ����ر�־λ
int8 stateBranch_Sign=0,directBranch_Sign=-1;//��·��ر�־λ
int8 stateGarage_Sign=0,directGarage_Sign=-1;//������ر�־λ
uint8 longest_col1;

int16 repair_Cnt;

int16 d_neg,d_pos;
int16 track_Width[High];//��¼ɨ��߽�õ����������
int16 min_Length,min_Length_Index;


track_Type_Enum track=Straight;

int16 Border[3][High];
int16 halftrack_Width[High];
uint8 islost_record[3][High];
int16 direct_Weight[High]=
{50,48,46,44,42,
 40,38,36,34,32,
 30,28,26,24,22,
 20,18,16,14,12,
 10,8,6,4,2,
 15,15,15,15,15,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0};
/*
 {10,9,8,5,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
 */
int16 default_judge_line=40;
uint8 centre_Delta_Exceed_Cnt;//Բ��ʶ����λ


uint8 absolute(uint8 a,uint8 b)//����ֵ����
{
    if(a>b)
        return a-b;
    else
        return b-a;
}
uint8 OTSU(uint8 *pre_image)//��̬������ֵ
{
    uint16 piexlCount[256];
    uint8 threshold_Max=200,threshold_Min=70;

    for(int i=0;i<256;i++)//��ʼ������
        piexlCount[i]=0;

    for(int i=0;i<High;i++)//ͳ��ÿ���Ҷ��ж��ٸ�����
        for(int j=0;j<Width;j++)
        {
            int tmp=*(pre_image+i*Width+j);
            piexlCount[tmp]++;
        }

    uint8 threshold;//��ֵ
    int32 deltaMax=0,deltaTmp;//��䷽��
    for(int i=threshold_Min;i<threshold_Max;i++)//Ѱ��ʹ��䷽��������ֵ
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
            threshold=i;
        }
    }

    return threshold;
}

uint8 trackBorder_Get_Cnt=0;
int16 last_start;
void trackBorder_Get(uint8 threshold)//Ԥ����߽�
{
    memset(Border[LEFT],0,High*sizeof(int));//���ݸ�λ
    memset(Border[CENTRE],Width/2,High*sizeof(int));
    memset(Border[RIGHT],Width-1,High*sizeof(int));

    centre_Delta_Exceed_Cnt=0;
    leftLostLine_Cnt=0;//����������ʼ��
    rightLostLine_Cnt=0;
    lostLeft_Sign=0;
    lostRight_Sign=0;
    validLine=High-1;

    d_neg=-15,d_pos=15;//-15 15
    min_Length=-1;

    for(int i=Width/2+d_neg;i<Width/2+d_pos;i++)//��ȡԤ����Զ��Ч�е��������䳤��
    {
        int j=High-1;
        while(mt9v03x_image[j][i]>threshold&&j>=0)
            j--;
        if(j-1<validLine)
        {
            validLine=j-1;
            searsh_mid_line=i;
        }
        if(j-1>min_Length)
        {
            min_Length=j-1;
            min_Length_Index=i;
        }
    }

    for(int i=High-1;i>=validLine;i--)//ɨ�����ұ��߲���������
    {
        for(int j=longest_col1-delta;j>=0;j=j-delta)//ɨ����߽�
        {
            if(mt9v03x_image[i][j]<threshold)
            {
                EdgeL=j;
                islost_record[LEFT][i]=0;
                break;
            }
            else if(j==0)//δ�ҵ������
            {
                EdgeL=0;
                lostLeft_Sign=1;
                islost_record[LEFT][i]=1;
                leftLostLine_Cnt++;
                break;
            }
        }
        for(int j=longest_col1+delta;j<Width-1;j=j+delta)//ɨ���ұ߽�
        {
            if(mt9v03x_image[i][j]<threshold)
            {
                EdgeR=j;
                islost_record[RIGHT][i]=0;
                break;
            }
            else if(j==Width-2)//δ�ҵ������
            {
                EdgeR=Width-2;
                lostRight_Sign=1;
                islost_record[RIGHT][i]=1;
                rightLostLine_Cnt++;
                break;
            }
        }

        Border[LEFT][i]=EdgeL;//��߽�
        Border[RIGHT][i]=EdgeR;//�ұ߽�
        Border[CENTRE][i]=(EdgeL+EdgeR)>>1;//��ʱ����

        track_Width[i]=Border[RIGHT][i]-Border[LEFT][i];

        if(i<High-1)//Ϊ���Բ����׼��
        {
            int16 tmp=Border[CENTRE][i]-Border[CENTRE][i+1];
            if(tmp>22||tmp<-22)
                centre_Delta_Exceed_Cnt++;
        }

    }
}



uint8 is_Straight(uint8 start_Index,uint8 end_Index,uint8 side)//�ж�ĳһ�ߴ���㵽�յ��Ƿ�Ϊֱ��
{
    int white_Cnt=0;
	
		if(end_Index<validLine)
				return 0;
	
    if(start_Index==end_Index)
        return 1;
    else
    {
        uint8 start=start_Index<end_Index?start_Index:end_Index;
        uint8 end=start_Index>end_Index?start_Index:end_Index;
        int8 delta=Border[side][start]-Border[side][start+1];
        for(uint8 i=start+1;i<end;i++)
        {
            int8 tmp=Border[side][i]-Border[side][i+1];
            if(Border[side][i]==0||Border[side][i]==Width-1)
                white_Cnt++;
            if(white_Cnt>10)
                return 0;
            if(tmp-delta>1||tmp-delta<-1)
                return 0;
        }
        return 1;
    }
}

uint8 find_Zebra()
{

}

int16 inflection_A,inflection_B,inflection_C;//�ɽ���Զ���������յ�

uint8 find_Inflection_C(int16 start,int16 end,int dir)//Զ���յ�
{
    if(dir==LEFT)
    {
        for(int16 i=start+5;i<=end-3;i++)
        {
            if((Border[LEFT][i]-Border[LEFT][i+1]>5)
            &&(Border[LEFT][i]-Border[LEFT][i+2]>5)
            &&(Border[LEFT][i]-Border[LEFT][i+3]>5))
            {
                if(i-start<10&&islost_record[LEFT][i+3]==1)
                {
                    inflection_C=i;
                    return 1;
                }
                else
                {
                    inflection_C=i;
                    return 1;
                }
            }
        }
    }
    else
    {
        for(int16 i=start+5;i<=end-3;i++)
        {
            if((Border[RIGHT][i]-Border[RIGHT][i+1]<-5)
            &&(Border[RIGHT][i]-Border[RIGHT][i+2]<-5)
            &&(Border[RIGHT][i]-Border[RIGHT][i+3]<-5))
            {
                if(i-start<10&&islost_record[RIGHT][i+3]==1)
                {
                    inflection_C=i;
                    return 1;
                }
                else
                {
                    inflection_C=i;
                    return 1;
                }
            }
        }
    }
    inflection_C=255;
    return 0;
}

uint8 find_Inflection_B(int16 start,int16 end,int dir)//�м�յ�
{
    if(dir==LEFT)
    {
        for(int16 i=end-1;i>=start+1;i--)
            if(//(islost_record[LEFT][i-1]==0)
            //&&(islost_record[LEFT][i]==0)
            //&&(islost_record[LEFT][i+1]==0)
            /*&&*/(Border[LEFT][i]-Border[LEFT][i-2]>0)
            &&(Border[LEFT][i]-Border[LEFT][i+2]>0))
            {
                inflection_B=i;
                return 1;
            }
    }
    else
    {
        for(int16 i=end-2;i>=start+2;i--)
            if(/*(islost_record[RIGHT][i-1]==0)
            &&(islost_record[RIGHT][i]==0)
            &&(islost_record[RIGHT][i+1]==0)
            &&*/(Border[RIGHT][i]-Border[RIGHT][i-3]<0)
            &&(Border[RIGHT][i]-Border[RIGHT][i+3]<0))
            {
                inflection_B=i;
                return 1;
            }
    }
    inflection_B=255;
    return 0;
}

uint8 find_Inflection_A(int16 start,int16 end,int dir,int sign)//�����յ�
{
    if(dir==LEFT)
    {
        if(sign==0)//������A��
        {
            for(int16 i=end-1;i>=start;i--)
                if((islost_record[LEFT][i]==0)
                &&(islost_record[LEFT][i-1]==1)
                &&(Border[LEFT][i]-Border[LEFT][i-1]>10))
                {
                    inflection_A=i;
                    return 1;
                }
        }
        else if(sign==1)//������A��
        {
            for(int16 i=end-3;i>=start;i--)
                if((Border[RIGHT][i]>Border[RIGHT][i+1]+5)
                &&(Border[RIGHT][i]>Border[RIGHT][i+2]+5)
                &&(Border[RIGHT][i]>Border[RIGHT][i+3]+5))
                {
                    inflection_A=i;
                    return 1;
                }
        }
    }
    else
    {
        if(sign==0)//������A��
        {
            for(int16 i=end-1;i>=start;i--)
                if((islost_record[RIGHT][i]==0)
                &&(islost_record[RIGHT][i-1]==1)
                &&(Border[RIGHT][i]-Border[RIGHT][i-1]<-10))
                {
                    inflection_A=i;
                    return 1;
                }
        }
        else if(sign==1)//������A��
        {
            for(int16 i=end-3;i>=start;i--)
                if((Border[LEFT][i]<Border[LEFT][i+1]-5)
                &&(Border[LEFT][i]<Border[LEFT][i+2]-5)
                &&(Border[LEFT][i]<Border[LEFT][i+3]-5))
                {
                    inflection_A=i;
                    return 1;
                }
        }
    }
    inflection_A=255;
    return 0;
}

int centre_line_get()//��ȡ����ƫ��ֵ
{
    int sum_A=0,sum_B=0;
    int16 cnt=1;
    float k=0.8;

    Border[CENTRE][High-1]=(Border[LEFT][High-1]+Border[RIGHT][High-1])>>1;

    for(int i=High-2;i>=validLine;i--)
    {
        Border[CENTRE][i]=(Border[LEFT][i]+Border[RIGHT][i])>>1;

        if(stateRing_Sign==2)
        {
            if(directRing_Sign==LEFT&&Border[CENTRE][i]-Border[CENTRE][i+1]>5)
            {
                int16 tmp=Border[CENTRE][i+cnt]-k*cnt;
                if(tmp<0)
                    tmp=0;
                Border[CENTRE][i]=tmp;
                cnt++;
            }
            if(directRing_Sign==RIGHT&&Border[CENTRE][i]-Border[CENTRE][i+1]<-20)
            {
                int16 tmp=Border[CENTRE][i+cnt]+k*cnt;
                if(tmp>Width-1)
                    tmp=Width-1;
                Border[CENTRE][i]=tmp;
                cnt++;
            }
        }

        sum_A=sum_A+i*(Border[CENTRE][i]-Width/2);
        sum_B=sum_B+i;
    }
    return sum_A/sum_B;
}

void judge_Ring()//ʶ��Բ�����ж�����״̬
{
    switch(stateRing_Sign)
    {
    case 0://��ʼ״̬������Ƿ�����Բ��
        if(centre_Delta_Exceed_Cnt>=2)//������Բ��
        {
            if((is_Straight(90, Default_Straight_Judge, LEFT))&&
							(find_Inflection_A(validLine, High-1, RIGHT,0)))//���һ���
            {
                stateRing_Sign=1;
                track=Ring;
                directRing_Sign=RIGHT;
            }
            else if((is_Straight(90, Default_Straight_Judge, RIGHT))&&
							(find_Inflection_A(validLine, High-1, LEFT,0)))//���󻷣�
            {
                stateRing_Sign=1;
                track=Ring;
                directRing_Sign=LEFT;
            }
        }
        break;
    case 1://����1����״̬����ⲻ��A�յ�ʱ������һ�׶�
        if(find_Inflection_A(40, High-1, directRing_Sign,0)==0)
            stateRing_Sign=2;
        break;
    case 2://����2����״̬����ⲻ��C�յ�ʱ������һ״̬
        if(find_Inflection_C(validLine, High-1, directRing_Sign)==0)
            stateRing_Sign=3;
        break;
    case 3://����״̬����⵽A�յ�ʱ����ó�������
        if(directRing_Sign==LEFT
        &&(find_Inflection_A(validLine, High-1, LEFT,1)==1))
            stateRing_Sign=4;
        else if(directRing_Sign==RIGHT
        &&(find_Inflection_A(validLine, High-1, RIGHT,1)==1))
            stateRing_Sign=4;
        break;
    case 4://����3����״̬���ܼ�⵽C�յ�����ѳ���
        if((validLine<30)&&(directRing_Sign==LEFT)&&(find_Inflection_C(validLine, High-1, LEFT)==1))
            stateRing_Sign=5;
        else if((validLine<30)&&(directRing_Sign==RIGHT)&&(find_Inflection_C(validLine, High-1, RIGHT)==1))
            stateRing_Sign=5;
        break;
    case 5://����4����״̬����������������ж������ߴ����뿪Բ��
        if(islost_record[LEFT][105]==0&&islost_record[LEFT][100]==0&&islost_record[RIGHT][105]==0&&islost_record[RIGHT][100]==0)
        {
                stateRing_Sign=0;
                track=Straight;
        }
        break;
    }
    return;
}

void repair_Ring(uint8 threshold)//Բ������
{
    switch(stateRing_Sign)
    {
    case 1://����1����
        if(directRing_Sign==LEFT)//�󻷲���
        {
            int16 i=inflection_A-1;
            repair_Cnt=1;
            int16 tmp=Border[LEFT][inflection_A]+0.6*repair_Cnt;
            while(i>=0&&tmp>Border[LEFT][i])
            {
                Border[LEFT][i]=tmp;
                repair_Cnt++;
                i--;
                tmp=Border[LEFT][inflection_A]+0.6*repair_Cnt;
            }
        }
        else //�һ�����
        {
            int16 i=inflection_A-1;
            repair_Cnt=1;
            int16 tmp=Border[RIGHT][inflection_A]-0.6*repair_Cnt;
            while(i>=0&&tmp<Border[RIGHT][i])
            {
                Border[RIGHT][i]=tmp;
                repair_Cnt++;
                i--;
                tmp=Border[RIGHT][inflection_A]-0.6*repair_Cnt;
            }
        }
        break;
    case 2://����2����
        if(directRing_Sign==LEFT)//�󻷲���
        {
            int16 i=inflection_C+1;
            repair_Cnt=1;
            float k;
            k=1.6;//1.8
            while(i<High&&Border[LEFT][inflection_C]+k*repair_Cnt<Border[RIGHT][i])//��C�յ����
            {
               int16 tmp=Border[LEFT][inflection_C]+k*repair_Cnt;//ϵ������
               if(tmp>Width-1)
                   tmp=Width-1;
               Border[RIGHT][i]=tmp;
               repair_Cnt++;
               i++;
            }

            if(find_Inflection_B(inflection_C+2, High-1, LEFT)==1)//���ҵ�B�յ㣬��B�յ����
            {
               repair_Cnt=1;
               for(i=inflection_B+1;i<High;i++)
               {
                   int16 tmp=Border[LEFT][inflection_B]-1.2*repair_Cnt;//ϵ������
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
            k=1.6;//2

            while(i<High&&Border[RIGHT][inflection_C]-k*repair_Cnt>Border[LEFT][i])//��C�յ����
            {
               int16 tmp=Border[RIGHT][inflection_C]-k*repair_Cnt;//ϵ������
               if(tmp<0)
                   tmp=0;
               Border[LEFT][i]=tmp;
               repair_Cnt++;
               i++;
            }

            if(find_Inflection_B(inflection_C+2, High-1, RIGHT)==1)//���ҵ�B�յ㣬��B�յ����
            {
               repair_Cnt=1;
               for(i=inflection_B+1;i<High;i++)
               {
                   int16 tmp=Border[RIGHT][inflection_B]+1.2*repair_Cnt;//ϵ������
                   if(tmp>Width-1)
                       tmp=Width-1;
                   Border[RIGHT][i]=tmp;
                   repair_Cnt++;
               }
            }

        }
        break;
		case 3://����3����
		    if(directRing_Sign==LEFT)
        {
					  int16 i=inflection_A-1;
            repair_Cnt=1;
		        float k;
		        k=1;
            if(find_Inflection_A(validLine, High-1, LEFT,1)==1)
            {
							  while(i<High&&Border[RIGHT][inflection_A]-k*repair_Cnt>Border[LEFT][i])//��A�յ����
            {
                   int16 tmp=Border[RIGHT][inflection_A]-k*repair_Cnt;//ϵ������
                   if(tmp<0)
                   tmp=0;
                   Border[RIGHT][i]=tmp;
                   repair_Cnt++;
                   i++;
            }
            }
        }
        else
        {
            int16 i=inflection_A-1;
            repair_Cnt=1;
		        float k;
		        k=1;
            if(find_Inflection_A(validLine, High-1, RIGHT,1)==1)
            {
							  while(i<High&&Border[LEFT][inflection_A]+k*repair_Cnt<Border[RIGHT][i])//��A�յ����
            {
                   int16 tmp=Border[LEFT][inflection_A]+k*repair_Cnt;//ϵ������
                  if(tmp>Width-1)
                       tmp=Width-1;
                   Border[RIGHT][i]=tmp;
                   repair_Cnt++;
                   i++;
            }
            }
				}
			  break;
    case 5://����4����
        if(directRing_Sign==LEFT)
        {
            if(find_Inflection_C(validLine, High-1, LEFT)==1)
            {
                repair_Cnt=1;
                for(int i=inflection_C+1;i<High;i++)
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
            if(find_Inflection_C(validLine, High-1, RIGHT)==1)
            {
                repair_Cnt=1;
                for(int i=inflection_C+1;i<High;i++)
                {
                    int16 tmp=Border[RIGHT][inflection_C]+1.1*repair_Cnt;
                    if(tmp>Width-1)
                        tmp=Width-1;
                    Border[RIGHT][i]=tmp;
                    repair_Cnt++;
                }
            }
        }
        break;
    }
}

uint8 is_Branch()
{
    for(int16 i=High-1;i>=validLine+4;i--)
        if((track_Width[i]-track_Width[i-1]<0)
        &&(track_Width[i]-track_Width[i-2]<0)
        &&(track_Width[i]-track_Width[i-3]<0)
        &&(track_Width[i]-track_Width[i-4]<0))
            return 1;
    return 0;
}

void re_trackBorder_Get(uint8 threshold)
{

    for(int i=High-1;i>=min_Length;i--)//ɨ�����ұ��߲���������
    {
        for(int j=min_Length_Index-delta;j>=0;j=j-delta)//ɨ����߽�
        {
            if(mt9v03x_image[i][j]<threshold)
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
        for(int j=min_Length_Index+delta;j<Width-1;j=j+delta)//ɨ���ұ߽�
        {
            if(mt9v03x_image[i][j]<threshold)
            {
                EdgeR=j;
                break;
            }
            else if(j==Width-2)//δ�ҵ������
            {
                EdgeR=Width-2;
                break;
            }
        }
        Border[LEFT][i]=EdgeL;//��߽�
        Border[RIGHT][i]=EdgeR;//�ұ߽�
    }
}

void judge_Branch()
{
    switch(stateBranch_Sign)
    {
    case 0:
        if(is_Branch()==1)
        {
            stateBranch_Sign=1;
            directBranch_Sign=2;
        }
        break;
    case 1:
        if(is_Branch()==0)
            stateBranch_Sign=2;
        break;
    case 2:
        if(is_Branch()==1)
            stateBranch_Sign=3;
        break;
    case 3:
        if (is_Branch()==0)
            stateBranch_Sign=0;
        break;
    }
}

void repair_Branch(uint8 threshold)//��·����
{
    switch(stateBranch_Sign)
    {
    case 1:
        re_trackBorder_Get(threshold);
        if(directBranch_Sign==1)//��ת����·
        {
            repair_Cnt=1;
            float k=1.3;
            for(int16 i=min_Length+1;i<High;i++)
            {
                int16 tmp=min_Length_Index+k*repair_Cnt;
                if(tmp>Width-1)
                    tmp=Width-1;
                Border[RIGHT][i]=tmp;
                repair_Cnt++;
            }
        }
        else if(directBranch_Sign==2)//��ת����·
        {
            repair_Cnt=1;
            float k=1.3;
            for(int16 i=min_Length+1;i<High;i++)
            {
                int16 tmp=min_Length_Index-k*repair_Cnt;
                if(tmp<0)
                    tmp=0;
                Border[LEFT][i]=tmp;
                repair_Cnt++;
            }
        }
        break;
    case 3:
        re_trackBorder_Get(threshold);
        if(directBranch_Sign==1)//��ת����·
        {
            repair_Cnt=1;
            float k=1.3;
            for(int16 i=min_Length+1;i<High;i++)
            {
                int16 tmp=min_Length_Index+k*repair_Cnt;
                if(tmp>Width-1)
                    tmp=Width-1;
                Border[RIGHT][i]=tmp;
                repair_Cnt++;
            }
        }
        else if(directBranch_Sign==2)//��ת����·
        {
            repair_Cnt=1;
            float k=1.3;
            for(int16 i=min_Length+1;i<High;i++)
            {
                int16 tmp=min_Length_Index-k*repair_Cnt;
                if(tmp<0)
                    tmp=0;
                Border[LEFT][i]=tmp;
                repair_Cnt++;
            }
        }
        break;
    }
}

void judge_Garage()//ʶ�𳵿�
{
    if(directGarage_Sign==Garage_Out)//��д�ҳ���
    {
        switch(stateGarage_Sign)
        {
        case 1://ֱ�߽׶�
            if(Border[LEFT][30]==0)//ֱ�ߵ�һ���̶ȿ�ʼת��
                stateGarage_Sign=2;
            break;
        case 2://�����߽׶�
            if(find_Zebra()==0)
                stateGarage_Sign=0;
            break;
        }
    }
    else if(directGarage_Sign==Garage_In) {

    }
}

void repair_Garage()//���ⲹ��
{
    if(directGarage_Sign==Garage_Out)
    {
        int16 inf;
        switch(stateGarage_Sign)
        {
        case 1://��һ��ֱ��
            for(int16 i=High-1;i>=validLine;i--)
            {
                Border[LEFT][i]=0;
                Border[LEFT][i]=Width-1;
            }
            break;
        case 2:
            for(int16 i=High-1;i>validLine;i--)//������յ�
                if(Border[LEFT][i]-Border[LEFT][i-1]>5)
                {
                    inf=i;
                    break;
                }

        }
    }
    else if(directGarage_Sign==Garage_In) {

    }
}

float cal_Curvature()
{
    float a_x=High-1,a_y=Border[CENTRE][High-1];
    float b_x=(High-1+validLine)>>1,b_y=Border[CENTRE][(High-1+validLine)>>1];
    float c_x=validLine,c_y=Border[CENTRE][validLine];

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

void get_longest_col1()
{
	int length=0;
	int i,j,flag=0;
	for (j = img_width - 1; j >= 0; j-=8)
	{
		for (i = img_height - 1; i >= 4; i-=4)
		{
			if(image[i][j]<=120)
				break;
			if(image[i][j]-image[i-4][j]>50)
			{
				if(length < img_height-1-i)
		    {
			    length=img_height-1-i;
			    longest_col1=j;
			    validLine=i;
					flag=1;
		    }
				break;
			}
		}
		if(i<=4)
			{
				length=img_height-1;
			  longest_col1=j;
			  validLine=0;
			  flag=1;
				break;
			}
	}
	if(!flag)
	{
		longest_col1=img_width/2;
		validLine=img_height-1;
	}
}
void Show_Line1()
{
	for (int16 i = img_height - 1; i >= validLine; i--)
    {
        image[i][Border[0][i]] = 0;
        image[i][Border[1][i]] = 0;
		  	image[i][Border[2][i]]= 0;
    }
	  //ips114_showint16(188,0,inflection_A);
		//ips114_showint16(188,1,inflection_B);
		//ips114_showint16(188,2,inflection_C);
	  ips114_showint16(188,3,stateRing_Sign);
		ips114_showint16(188,4,directRing_Sign);
	  ips114_showint16(188,5,validLine);
}