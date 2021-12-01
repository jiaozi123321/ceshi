#include "trail.h"
#include "usart.h"
#include "xunxian.h"
#include "lun.h"
#include "delay.h"

int OUTPUT1,OUTPUT2,stopL,stopR;//控制电机速度的pwm值
int PID_sd=0 ;//小车差速PID的pwm输出值
int PID_jc=7200 ;//小车基础速度PID的PWM输出值
extern int error ,sign;//引用xunxian中定义的error ,a,sign
int motor_speed=7200;



void TIM3_IRQHandler(void)//TIM3中断服务函数
{
	if(TIM3->SR&0x0001)//定时器每10ms定时中断一次
	{
	TIM3->SR&=~(1<<0);      //清除定时器三的计时标志，执行完TIM3中断服务函数里的操作后定时器会重新计时
	PID_sd=Position_PIDS(error,0);//将差速PID控制器输出的PWM值赋值给PID_sd
//	PID_jc=Position_PIDJ(a,0);//将基础速度PID控制器输出的PWM值赋值给PID_jc
//	Xianfu_PID();
//	Suchu();//PWM输出函数
		if(sign==0)
		{
		straight_s_line();
			
		}
		else 
		{
			turn();
		}
	}
}

//int Position_PIDJ(int Encoder,int Target)//基础速度PD控制器Encoder=a，Target=0
//{
//	float Position_KP=160,Position_KD=90;  //调试取KP，KD 无KI
//	static float Bias,Pwm,Last_Bias;
//	Bias=Encoder-Target;                   //偏差=实际值-目标值
//	Pwm=7200-Position_KP*Bias+Position_KD*(Bias-Last_Bias);//这里7200是小车电机PWM的最大值，也就是主函数TIM2的传参传入的数，当error为零时，表示小车处于黑线的中央，没有偏移，所以这时a=0，使得最后返回的PWM值为7200，使电机达到最大速度
//	Last_Bias=Bias;
//	return Pwm;
//}
//int Position_PIDS(int Encoder,int Target)
//{
//	float Position_KP=1000,Position_KI=0,Position_KD=0;
//	static float Bias,Pwm,Integral_bias,Last_bias;
//	Bias=Encoder-Target;
//	Integral_bias +=Bias;
//	Pwm =Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_bias);
//	Last_bias=Bias;
//	return Pwm;
//}

//void Suchu(void)
//{

//	TIM_SetCompare3(TIM2,OUTPUT1);//电机控制
//	TIM_SetCompare4(TIM2,OUTPUT2);//电机控制
//}
//void Xianfu_PID(void)
//{
//	OUTPUT1=PID_jc+PID_sd;//基础速度+速度偏差量 //基础速度也可以恒定为某个值
//	OUTPUT2=PID_jc-PID_sd;//基础速度-速度偏差量
//	   /*控制小车电机的PWM有效值范围为0-7200*/
//	  if(OUTPUT1>7200) OUTPUT1=7200;
//    if(OUTPUT1<0) OUTPUT1=0;
//	  if(OUTPUT2>7200) OUTPUT2=7200;
//    if(OUTPUT2<0) OUTPUT2=0;
//	
// 
//}
void turn()
{
	if(sign==1)//左转90度
	{
	zuozhuan();
	TIM_SetCompare3(TIM2,7200);//电机控制
	TIM_SetCompare4(TIM2,7200);//电机控制
	delay_ms(200);
	sign=0;//跳出转向
	}
	if(sign==2)//右转90度
	{
		youzhuan();
		TIM_SetCompare3(TIM2,7200);//电机控制
    TIM_SetCompare4(TIM2,7200);//电机控制
		delay_ms(200);
		sign=0;
	}
	if(sign==3)
	{
		zuozhuan();
		TIM_SetCompare3(TIM2,7200);//电机控制
    TIM_SetCompare4(TIM2,7200);//电机控制
		delay_ms(200);
		sign=0;//跳出转向
	}
	if(sign==4)
	{
		youzhuan();
		TIM_SetCompare3(TIM2,7200);//电机控制
    TIM_SetCompare4(TIM2,7200);//电机控制
		delay_ms(200);
		sign=0;
	}
	if(sign==6)
	{
		forward();
		TIM_SetCompare3(TIM2,7200);//电机控制
    TIM_SetCompare4(TIM2,7200);//电机控制
		delay_ms(200);
		sign=0;
	}
	if(sign==9)
	{
		stop();
		TIM_SetCompare3(TIM2,7200);//电机控制
    TIM_SetCompare4(TIM2,7200);//电机控制
		delay_ms(500);
		sign=0;
		error=10;
	}
}

int Position_PIDS(int Encoder,int Target)
{
	float Position_KP=100,Position_KI=0,Position_KD=0;
	static float Bias,Pwm,Integral_bias,Last_bias;
	Bias=Encoder-Target;
	Integral_bias +=Bias;
	Pwm =Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_bias);
	Last_bias=Bias;
	return Pwm;
}
//int Position_PIDJ(int Encoder,int Target)//基础速度PD控制器Encoder=a，Target=0
//{
//	float Position_KP=160,Position_KD=90;  //调试取KP，KD 无KI
//	static float Bias,Pwm,Last_Bias;
//	Bias=Encoder-Target;                   //偏差=实际值-目标值
//	Pwm=6000-Position_KP*Bias+Position_KD*(Bias-Last_Bias);//这里7200是小车电机PWM的最大值，也就是主函数TIM2的传参传入的数，当error为零时，表示小车处于黑线的中央，没有偏移，所以这时a=0，使得最后返回的PWM值为7200，使电机达到最大速度
//	Last_Bias=Bias;
//	return Pwm;
//}
void straight_s_line()
{
	PID_sd=Position_PIDS(error,0);
	OUTPUT1=PID_jc+PID_sd;//基础速度+速度偏差量 //基础速度也可以恒定为某个值
	OUTPUT2=PID_jc-PID_sd;//基础速度-速度偏差量
	if(OUTPUT1>7200) OUTPUT1=7200;
    if(OUTPUT1<0) OUTPUT1=0;
	  if(OUTPUT2>7200) OUTPUT2=7200;
    if(OUTPUT2<0) OUTPUT2=0;
	TIM_SetCompare3(TIM2,OUTPUT1);//电机控制
	TIM_SetCompare4(TIM2,OUTPUT2);//电机控制
}







