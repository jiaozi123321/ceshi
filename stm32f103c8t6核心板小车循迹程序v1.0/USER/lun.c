#include "lun.h"
#include "stm32f10x.h"
#include "pwm.h"


void lunzi_Init(void)//对相应的IO口进行初始化,PA0-3
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);				//使能GPIOA的时钟
	
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;							//输出模式为推挽输出，电平可高可低
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PA0-3管脚IO口
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;							//管脚传输速度
	  GPIO_Init(GPIOA,&GPIO_InitStructure);												//管脚输出初始化
}

void stop(void)//小车两个电机反转，因为越过终点线时，单纯停止电机工作，小车会因为惯性停不下来，所以短时间的反转，能抵消惯性，从而使小车迅速停下来
{
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);	//PA0低电平   0
		GPIO_SetBits(GPIOA,GPIO_Pin_1);		//PA1高电平	  1
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);	//PA2低电平	  0
		GPIO_SetBits(GPIOA,GPIO_Pin_3);		//PA3高电平   1
}


void forward(void)//小车两个电机正转
{
	
	  GPIO_SetBits(GPIOA,GPIO_Pin_0);		//PA0高电平	     1
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//PA1低电平   0
		GPIO_SetBits(GPIOA,GPIO_Pin_2);		//PA2高电平	     1
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//PA3低电平   0

}
void zuozhuan(void)//左转
{
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);	//PA0低电平
		GPIO_SetBits(GPIOA,GPIO_Pin_1);		//PA1低电平
		GPIO_SetBits(GPIOA,GPIO_Pin_2);		//PA2高电平
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//PA3低电平
}
void youzhuan(void)//右转
{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);		//PA0高电平	     1
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);	//PA1低电平   0
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);	//PA2低电平
		GPIO_SetBits(GPIOA,GPIO_Pin_3);		//PA3高电平
}


