#include "trail.h"
#include "usart.h"
#include "xunxian.h"
#include "lun.h"
#include "delay.h"

int OUTPUT1,OUTPUT2,stopL,stopR;//���Ƶ���ٶȵ�pwmֵ
int PID_sd=0 ;//С������PID��pwm���ֵ
int PID_jc=6000 ;//С�������ٶ�PID��PWM���ֵ
extern int error ,sign;//����xunxian�ж����error ,a,sign
int motor_speed=7200;



void TIM3_IRQHandler(void)//TIM3�жϷ�����
{
	if(TIM3->SR&0x0001)//��ʱ��ÿ10ms��ʱ�ж�һ��
	{
	TIM3->SR&=~(1<<0);      //�����ʱ�����ļ�ʱ��־��ִ����TIM3�жϷ�������Ĳ�����ʱ�������¼�ʱ
	PID_sd=Position_PIDS(error,0);//������PID�����������PWMֵ��ֵ��PID_sd
//	PID_jc=Position_PIDJ(a,0);//�������ٶ�PID�����������PWMֵ��ֵ��PID_jc
//	Xianfu_PID();
//	Suchu();//PWM�������
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

//int Position_PIDJ(int Encoder,int Target)//�����ٶ�PD������Encoder=a��Target=0
//{
//	float Position_KP=160,Position_KD=90;  //����ȡKP��KD ��KI
//	static float Bias,Pwm,Last_Bias;
//	Bias=Encoder-Target;                   //ƫ��=ʵ��ֵ-Ŀ��ֵ
//	Pwm=7200-Position_KP*Bias+Position_KD*(Bias-Last_Bias);//����7200��С�����PWM�����ֵ��Ҳ����������TIM2�Ĵ��δ����������errorΪ��ʱ����ʾС�����ں��ߵ����룬û��ƫ�ƣ�������ʱa=0��ʹ����󷵻ص�PWMֵΪ7200��ʹ����ﵽ����ٶ�
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

//	TIM_SetCompare3(TIM2,OUTPUT1);//�������
//	TIM_SetCompare4(TIM2,OUTPUT2);//�������
//}
//void Xianfu_PID(void)
//{
//	OUTPUT1=PID_jc+PID_sd;//�����ٶ�+�ٶ�ƫ���� //�����ٶ�Ҳ���Ժ㶨Ϊĳ��ֵ
//	OUTPUT2=PID_jc-PID_sd;//�����ٶ�-�ٶ�ƫ����
//	   /*����С�������PWM��Чֵ��ΧΪ0-7200*/
//	  if(OUTPUT1>7200) OUTPUT1=7200;
//    if(OUTPUT1<0) OUTPUT1=0;
//	  if(OUTPUT2>7200) OUTPUT2=7200;
//    if(OUTPUT2<0) OUTPUT2=0;
//	
// 
//}
void turn()
{
	if(sign==1)//��ת90��
	{
	youzhuan();
	TIM_SetCompare3(TIM2,5000);//�������
	TIM_SetCompare4(TIM2,5000);//�������
	delay_ms(300);
	sign=0;//����ת��
	}
	if(sign==2)//��ת90��
	{
		zuozhuan ();
		TIM_SetCompare3(TIM2,5000);//�������
    TIM_SetCompare4(TIM2,5000);//�������
		delay_ms(300);
		sign=0;
	}
	if(sign==3)
	{
		youzhuan ();
		TIM_SetCompare3(TIM2,5000);//�������
    TIM_SetCompare4(TIM2,5000);//�������
		delay_ms(500);
		sign=0;//����ת��
	}
	if(sign==4)
	{
		zuozhuan ();
		TIM_SetCompare3(TIM2,5000);//�������
    TIM_SetCompare4(TIM2,5000);//�������
		delay_ms(500);
		sign=0;
	}
	if(sign==6)
	{
		zuozhuan();
		TIM_SetCompare3(TIM2,5000);//�������
    TIM_SetCompare4(TIM2,5000);//�������
		delay_ms(500);
		sign=0;
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
//int Position_PIDJ(int Encoder,int Target)//�����ٶ�PD������Encoder=a��Target=0
//{
//	float Position_KP=160,Position_KD=90;  //����ȡKP��KD ��KI
//	static float Bias,Pwm,Last_Bias;
//	Bias=Encoder-Target;                   //ƫ��=ʵ��ֵ-Ŀ��ֵ
//	Pwm=6000-Position_KP*Bias+Position_KD*(Bias-Last_Bias);//����7200��С�����PWM�����ֵ��Ҳ����������TIM2�Ĵ��δ����������errorΪ��ʱ����ʾС�����ں��ߵ����룬û��ƫ�ƣ�������ʱa=0��ʹ����󷵻ص�PWMֵΪ7200��ʹ����ﵽ����ٶ�
//	Last_Bias=Bias;
//	return Pwm;
//}
void straight_s_line()
{
	PID_sd=Position_PIDS(error,0);
	OUTPUT1=PID_jc+PID_sd;//�����ٶ�+�ٶ�ƫ���� //�����ٶ�Ҳ���Ժ㶨Ϊĳ��ֵ
	OUTPUT2=PID_jc-PID_sd;//�����ٶ�-�ٶ�ƫ����
	if(OUTPUT1>7200) OUTPUT1=7200;
    if(OUTPUT1<0) OUTPUT1=0;
	  if(OUTPUT2>7200) OUTPUT2=7200;
    if(OUTPUT2<0) OUTPUT2=0;
	TIM_SetCompare3(TIM2,OUTPUT1);//�������
	TIM_SetCompare4(TIM2,OUTPUT2);//�������
}







