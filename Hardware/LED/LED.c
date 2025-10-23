#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void LED_On(void)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
}

void LED_Off(void)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
}

void LED_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_11) == 1)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
	}
}

void LED_Tick(void)
{
	static uint16_t LED_Tick_Num = 0;
	LED_Tick_Num++;
	
	if(LED_Tick_Num >= 1000)
	{
		LED_Tick_Num = 0;
		
		LED_Turn();
	}
}

