#include "stm32f10x.h"                  // Device header
#include "Menu.h"
#include "Key.h"


//目录值
uint8_t pMenu_Num = 1;


//主函数目录入口函数
void Main_Menu(void)
{
	pMenu_Num = Menu_1(pMenu_Num);
	switch(pMenu_Num)
	{
		case 1:pMenu_Num = Menu_1_1(1);break;
		case 2:pMenu_Num = Menu_1_2(1);break;
		default:break;
	}
}

//主函数目录
uint8_t Menu_1(uint8_t selected)
{
	uint8_t Now_select = 0;
	while(1)
	{
		switch(selected)
		{
			case 1:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-4",OLED_8X16);
				OLED_ReverseArea(1,1,40,16);
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-4",OLED_8X16);
				OLED_ReverseArea(1,16,40,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-4",OLED_8X16);
				OLED_ReverseArea(1,32,40,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-4",OLED_8X16);
				OLED_ReverseArea(1,48,40,16);
				OLED_Update();	
				break;
			default:
				break;
		}
		
		
		//存储当前选取的项
		if(selected != 0){Now_select = selected;}
		
		//获取键位值，在目录项之前切换
		selected = Key_GetKey();

		//按下5为确认键，返回当前选取的项
		if(selected == 5){return Now_select;}
		
	}
}


//主函数目录-1【选择第一个项后的目录，即1-1】
uint8_t Menu_1_1(uint8_t selected)
{
	uint8_t Now_select = 0;
	while(1)
	{
		switch(selected)
		{
			case 1:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-1-4",OLED_8X16);
				OLED_ReverseArea(1,1,40,16);
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-1-4",OLED_8X16);
				OLED_ReverseArea(1,16,40,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();	
				OLED_ShowString(1,1,"the 1-1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-1-4",OLED_8X16);
				OLED_ReverseArea(1,32,40,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-1-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-1-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-1-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-1-4",OLED_8X16);
				OLED_ReverseArea(1,48,40,16);
				OLED_Update();	
				break;
			default:
				break;
		}
		
		//存储当前选取的项
		if(selected != 0){Now_select = selected;}
		
		//获取键位值，在目录项之前切换
		selected = Key_GetKey();

		//按下5为确认键，返回当前选取的项
		if(selected == 5){return Now_select;}
		
	}
}

//主函数目录-2【选择第一个项后的目录，即1-2】
uint8_t Menu_1_2(uint8_t selected)
{
	uint8_t Now_select = 0;
	while(1)
	{
		switch(selected)
		{
			case 1:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-2-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-2-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-2-4",OLED_8X16);
				OLED_ReverseArea(1,1,40,16);
				OLED_Update();
				break;
			case 2:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-2-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-2-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-2-4",OLED_8X16);
				OLED_ReverseArea(1,16,40,16);
				OLED_Update();
				break;
			case 3:
				OLED_Clear();	
				OLED_ShowString(1,1,"the 1-2-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-2-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-2-4",OLED_8X16);
				OLED_ReverseArea(1,32,40,16);
				OLED_Update();
				break;
			case 4:
				OLED_Clear();
				OLED_ShowString(1,1,"the 1-2-1",OLED_8X16);
				OLED_ShowString(1,16,"the 1-2-2",OLED_8X16);
				OLED_ShowString(1,32,"the 1-2-3",OLED_8X16);
				OLED_ShowString(1,48,"the 1-2-4",OLED_8X16);
				OLED_ReverseArea(1,48,40,16);
				OLED_Update();	
				break;
			default:
				break;
		}
		
		//存储当前选取的项
		if(selected != 0){Now_select = selected;}
		
		//获取键位值，在目录项之前切换
		selected = Key_GetKey();

		//按下5为确认键，返回当前选取的项
		if(selected == 5){return Now_select;}
		
	}
}

