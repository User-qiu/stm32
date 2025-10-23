#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"
#include "LED.h"
#include "Time.h"


//按键值返回
uint8_t pKey_Num = 0;

//按键返回的次数
uint16_t pKey_Count = 0;


int main(void)
{
	
	OLED_Init();
	Time_Init();
	Key_Init();
	LED_Init();
	
	OLED_ShowString(1,1,"pKey_Count");
	while(1)
	{
		pKey_Num = Key_GetKey();
		if(pKey_Num){pKey_Count++;}
		OLED_ShowNum(2,1,pKey_Count,5);
	}
}


// 中断服务函数模板
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET) {
		Key_Tick();
		LED_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
	}
}
