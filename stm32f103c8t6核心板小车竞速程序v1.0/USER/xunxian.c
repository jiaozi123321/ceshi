#include "xunxian.h"
#include "sys.h"
#include "delay.h"
#include "lun.h"

int sensor[8]={0,0,0,0,0,0,0,0};//���ڴ��ѭ��ģ�鷵�ص���ֵ
int error=0;//С����ƫ����
int a = 0 ;
int sign;//�ж�·�ڱ�־
float wt=10,ct=30;

void hongwai_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure ;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOA��ʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOB��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ�ܸ���ʱ��
	
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��PB0,PB1,PB3,PB4,PB5
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�JTAGD����PB3��PB4����Ϊ��ͨ����
}

void read_sensor(void)//���⴫����ʶ����ߵķ���ֵΪ0��δʶ��ķ���Ϊ1
{
				sensor[0]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
				sensor[1]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
				sensor[2]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	      sensor[3]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
				sensor[4]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
				sensor[5]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
				sensor[6]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
				sensor[7]=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}		
	void sign_crossing(){
//	  if( (sensor[0]==1&&sensor[1]==1&&sensor[2]==1&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)	//  1 1 1 0   0 0 0 0 
//			||(sensor[0]==1&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0) 	//  1 1 1 1   0 0 0 0
//			||(sensor[0]==1&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0))	//  1 1 1 1   1 0 0 0
//		{ sign=1; }// ��ת90��
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)	//   0 0 0 0  0 1 1 1 
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1) 	//   0 0 0 0  1 1 1 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1))	//   0 0 0 1  1 1 1 1
//		{	sign=2; }// ��ת90��
//		if( (sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)   //   1 0 0 1  0 0 0 0
//			||(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)   //   1 0 0 0  1 0 0 0
//			||(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0))  //   1 0 0 1  1 0 0 0
//		{ sign=3; }// ��ת���
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1)   //   0 0 0 0  1 0 0 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1)   //   0 0 0 1  0 0 0 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1))  //   0 0 0 1  1 0 0 1
//		{ sign=4; }// ��ת���
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==0&&sensor[7]==0)   //   0 0 1 1  1 1 0 0
//			||(sensor[0]==0&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==0)   //   0 1 1 1  1 1 1 0
//			||(sensor[0]==1&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)   //   1 1 1 1  1 1 1 1
//					 )
//		{ sign=2; }// ʮ��·��&T��·��
//		else if(sign ==5 )
//		{
//			
//			forward();
//			delay_ms(50);
//			if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//			{
//				sign=1;//0000 0000 δɨ�赽���ߣ�T��·��
//			}
//			else
//			{
//				sign=2;//ɨ�赽���ߣ�ʮ��·��
//			}
//		}
if(sensor[2]==1||sensor[3]==1||sensor[4]==1||sensor[5]==1)
{
	sign=9;
}
else {
	sign=0;
}
	
	}
		void m_speed_change()
		{
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//		{ error= 4*ct; }//0001 0000
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==1&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//		{ error= 3*ct; }//0010 0000
		 if(sensor[0]==0&&sensor[1]==1&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
		{ error=2*ct;}//0100 0000
		 if(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
		{ error=ct;}//1000 0000
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//		{ error= -4*ct; } //0000 1000
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==1&&sensor[6]==0&&sensor[7]==0)
//		{ error= -3*ct; } //0000 0100
		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==1&&sensor[7]==0)
		{ error=-2*ct;} //0000 0010
		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1)
		{ error=-ct;}//0000 0001
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//		{ error= -4*ct;  } //0001 1000
//		 if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//		{
//			error=0;
//			if(error == (3*ct))
//			{
//				error = (5*ct);
//			}
//			else if(error == (-3*ct))
//			{
//				error = (-5*ct);
//			}
		if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
		{error=0;}
		}
	



