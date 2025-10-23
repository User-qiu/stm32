#include "stm32f10x.h"                  // Device header
#include "Key.h"

uint8_t Key_Num;


uint8_t Key_GetKey(void)
{
	uint8_t temp;
	if(Key_Num != 0)
	{
		temp = Key_Num;
		Key_Num = 0;
		return temp;
	}
	return 0;

}


void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Key_GetStatus(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0){return 1;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0){return 2;}
	return 0;
}


void Key_Tick(void)
{
	static uint8_t Key_Tick_Num;
	static uint8_t CurrentNum,PreventNum;
	
	Key_Tick_Num++;
	
	if(Key_Tick_Num >= 50)
	{
		Key_Tick_Num = 0;
		
		PreventNum = CurrentNum;
		CurrentNum = Key_GetStatus();
		
		if(PreventNum != 0 && CurrentNum == 0)
		{
			Key_Num = PreventNum;
		}
		
	}
}
