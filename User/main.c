#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"
#include "LED.h"
#include "Time.h"
#include "Menu.h"


int main(void)
{
	
	OLED_Init();
	Time_Init();
	Key_Init();
	//LED_Init();

	OLED_Clear();
	OLED_Update();
	

	while(1)
	{
		Main_Menu();
	}

}


// 中断服务函数模板
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET) {
		Key_Tick();
		//LED_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
	}
}
