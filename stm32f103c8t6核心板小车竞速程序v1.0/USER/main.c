#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include "trail.h"
#include "pwm.h"
#include "lun.h"
#include "xunxian.h"
#include "stdio.h"
extern int sign;
int main(void)
{

	SystemInit();
	delay_init();
	lunzi_Init();
	hongwai_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ��ж�
	TIM2_PWM_Init(7199,0);   //pwm����ʼ��
	TIM3_Init_Init(9,7199); //�жϺ�����ʼ��������Ϊ10ms
	while(1)
	{
		read_sensor();//����������ֵ��ʼ��
		m_speed_change();
	  sign_crossing();
		forward();


		
		}}




