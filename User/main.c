#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


int main(void)
{
	
	OLED_Init();


	OLED_Clear();
	OLED_Update();
	

	while(1)
	{
		OLED_Clear();
		OLED_DrawCircle(20, 20, 10, 1);
		OLED_Update();
		Delay_ms(500);
		
		OLED_Clear();
		OLED_DrawCircle(40, 40, 10, 0);
		OLED_Update();
		Delay_ms(500);
	}

}

