#include "stm32f10x.h"                  // Device header
#include "Motor.h"

/**
 * @brief 直流电机初始化
 * 
 */
void Motor_Init(void)
{

	//总线时钟初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
	//时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;		//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	

	//配置输出比较部分
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//为结构体赋值初始值后再修改
	TIM_OCStructInit(&TIM_OCInitStruct);
	
	
	
	//只列出我们需要的部分就好，部分高级定时器的功能用不上
	
	//配置输出比较部分的模式 PWN1模式
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//配置输出比较的极性 【配置有效电平为高电平】
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	//设置输出使能 【使能整个输出单元模块】
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	//设置CCR捕获比较寄存器
	TIM_OCInitStruct.TIM_Pulse = 0;		//CCR 
	
	//初始化定时器输出单元部分
	TIM_OC3Init(TIM3, &TIM_OCInitStruct);
	
	
	//配置GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//注意，此处必须查看引脚定义表，不是所有引脚都能输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	//注意，此处需要选择复用推挽输出将输出控制权交给片上外设【此处是TIM3】
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	//此处设置控制直流电机旋转方向的引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//开启定时器
	TIM_Cmd(TIM3,ENABLE);
}


/**
 * @brief 设置直流电机速度
 * 
 * @param Speed 设置范围为 -100 ~ 100
 */
void Motor_SetSpeed(int8_t Speed)
{
	if(Speed > 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		TIM_SetCompare3(TIM3, Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		TIM_SetCompare3(TIM3, -Speed);
	}
	
}

