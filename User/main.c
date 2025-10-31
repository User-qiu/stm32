#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Command.h"


int main(void)
{
	
	OLED_Init();
	Command_Init();
	
	
	OLED_Clear();
	OLED_ShowString(1,1,"Ready",OLED_8X16);
	OLED_Update();
	
	

	while(1)
	{
		Command_Receive();
		Command_Handle();
	}
	
}

