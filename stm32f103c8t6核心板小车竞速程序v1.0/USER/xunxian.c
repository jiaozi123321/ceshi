#include "xunxian.h"
#include "sys.h"
#include "delay.h"
#include "lun.h"

int sensor[8]={0,0,0,0,0,0,0,0};//用于存放循迹模块返回的数值
int error=0;//小车的偏移量
int a = 0 ;
int sign;//判断路口标志
float wt=10,ct=30;

void hongwai_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure ;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA的时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能复用时钟
	
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化PB0,PB1,PB3,PB4,PB5
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭JTAGD，将PB3，PB4配置为普通引脚
}

void read_sensor(void)//红外传感器识别黑线的返回值为0，未识别的返回为1
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
//		{ sign=1; }// 左转90度
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)	//   0 0 0 0  0 1 1 1 
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1) 	//   0 0 0 0  1 1 1 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1))	//   0 0 0 1  1 1 1 1
//		{	sign=2; }// 右转90度
//		if( (sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)   //   1 0 0 1  0 0 0 0
//			||(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)   //   1 0 0 0  1 0 0 0
//			||(sensor[0]==1&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0))  //   1 0 0 1  1 0 0 0
//		{ sign=3; }// 左转锐角
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1)   //   0 0 0 0  1 0 0 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1)   //   0 0 0 1  0 0 0 1
//			||(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==1&&sensor[4]==1&&sensor[5]==0&&sensor[6]==0&&sensor[7]==1))  //   0 0 0 1  1 0 0 1
//		{ sign=4; }// 右转锐角
//		if( (sensor[0]==0&&sensor[1]==0&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==0&&sensor[7]==0)   //   0 0 1 1  1 1 0 0
//			||(sensor[0]==0&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==0)   //   0 1 1 1  1 1 1 0
//			||(sensor[0]==1&&sensor[1]==1&&sensor[2]==1&&sensor[3]==1&&sensor[4]==1&&sensor[5]==1&&sensor[6]==1&&sensor[7]==1)   //   1 1 1 1  1 1 1 1
//					 )
//		{ sign=2; }// 十字路口&T字路口
//		else if(sign ==5 )
//		{
//			
//			forward();
//			delay_ms(50);
//			if(sensor[0]==0&&sensor[1]==0&&sensor[2]==0&&sensor[3]==0&&sensor[4]==0&&sensor[5]==0&&sensor[6]==0&&sensor[7]==0)
//			{
//				sign=1;//0000 0000 未扫描到黑线，T字路口
//			}
//			else
//			{
//				sign=2;//扫描到黑线，十字路口
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
	



