#ifndef __TRAIL_H
#define __TRAIL_H
#include "sys.h"



void TIM_3IRQHandler(void);//TIM3中断服务函数
void Xianfu_PID(void);
void Suchu(void);
int Position_PIDJ (int Encoder,int Target);//基础速度PID控制器
int Position_PIDS (int Encoder,int Target);//差速PID控制器
void turn(void);
void straight_s_line(void);
#endif
