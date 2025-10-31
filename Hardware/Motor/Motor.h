#ifndef _MOTOR_H_
#define _MOTOR_H_

/*
*   使用TIM3_CH3引脚输出比较 【GPIOB_Pin_0】
*   GPIOB_Pin_12、GPIOB_Pin_14控制直流电机转动方向
*   GPIOB_Pin_0负责PWM输出控制直流电机转速
*/

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);

#endif

