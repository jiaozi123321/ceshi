#ifndef __TRAIL_H
#define __TRAIL_H
#include "sys.h"



void TIM_3IRQHandler(void);//TIM3�жϷ�����
void Xianfu_PID(void);
void Suchu(void);
int Position_PIDJ (int Encoder,int Target);//�����ٶ�PID������
int Position_PIDS (int Encoder,int Target);//����PID������
void turn(void);
void straight_s_line(void);
#endif
