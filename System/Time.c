#include "stm32f10x.h"                  // Device header

/*定时器2初始化 1ms中断*/
void Time_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	/*外部时钟配置*/
	TIM_InternalClockConfig(TIM2);
	
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	
	TIM_InitStruct.TIM_Period = 1000 - 1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	
	
	//初始化定时器
	TIM_TimeBaseInit(TIM2,&TIM_InitStruct);
	
	//手动清除更新中断标志位，避免刚上电就进入中断状态
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	//使能定时器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//配置NVIC
	//配置中断嵌套分组 选择分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	//使能定时器2
	TIM_Cmd(TIM2,ENABLE);
}

