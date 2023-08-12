#include "shangweiji.h"

void CRC16(uint8 *Array,uint8 *Rcvbuf,unsigned int Len )  //����У����
{
	uint16 IX,IY,CRC;
	CRC=0xFFFF;
	
	if(Len<=0)
		CRC=0;
	else
	{
		Len--;
		for(IX=0;IX<=Len;IX++)
		{
 			CRC=CRC^Array[IX];
			for(IY=0;IY<=7;IY++)
			{
			if((CRC&1)!=0) CRC=(CRC>>1)^0xA001;
			else CRC=CRC>>1;
			}
		}
	}
	Rcvbuf[1]=(CRC&0xFF00)>>8; //��λ��
	Rcvbuf[0]=(CRC&0x00FF); //��λ��
}

void OutPut_Data(uint16 ch1,uint16 ch2,uint16 ch3,uint16 ch4)
{
	uint8 Array[8],Rcvbuf[2];
	Array[0]=(uint8)(ch1&0x00FF);
	Array[1]=(uint8)((ch1>>8)&0x00FF);
	Array[2]=(uint8)(ch2&0x00FF);
	Array[3]=(uint8)((ch2>>8)&0x00FF);
	Array[4]=(uint8)(ch3&0x00FF);
	Array[5]=(uint8)((ch3>>8)&0x00FF);
	Array[6]=(uint8)(ch4&0x00FF);
	Array[7]=(uint8)((ch4>>8)&0x00FF);
	CRC16(Array,Rcvbuf,8);
	/*printf("Array[0]:%d\r\n",Array[0]);
	printf("Array[1]:%d\r\n",Array[1]);
	printf("Array[2]:%d\r\n",Array[2]);
	printf("Array[3]:%d\r\n",Array[3]);
	printf("Array[4]:%d\r\n",Array[4]);
	printf("Array[5]:%d\r\n",Array[5]);
	printf("Array[6]:%d\r\n",Array[6]);
	printf("Array[7]:%d\r\n",Array[7]);
	printf("Rcvbuf[0]:%d\r\n",Rcvbuf[0]);
	printf("Rcvbuf[1]:%d\r\n",Rcvbuf[1]);*/
	uart_putbuff(UART_4, Array, 8); //����4��ͨ��������
	uart_putbuff(UART_4, Rcvbuf, 2);// ����У����
}
