#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"


uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(0,(1-1)*16,"MPU6050",OLED_8X16);
	OLED_ShowString(0,(2-1)*16,"X",OLED_8X16);
	OLED_ShowString(0,(3-1)*16,"Y",OLED_8X16);
	OLED_ShowString(0,(4-1)*16,"Z",OLED_8X16);
	
	while (1)
	{
		//为前面提示字符添加16像素的空间
		
		//加速度计
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(0+16, (2-1)*16, AX, 5,OLED_8X16);
		OLED_ShowSignedNum(0+16, (3-1)*16, AY, 5,OLED_8X16);
		OLED_ShowSignedNum(0+16, (4-1)*16, AZ, 5,OLED_8X16);
		
		//陀螺仪
		OLED_ShowSignedNum(56+16, (2-1)*16, GX, 5,OLED_8X16);
		OLED_ShowSignedNum(56+16, (3-1)*16, GY, 5,OLED_8X16);
		OLED_ShowSignedNum(56+16, (4-1)*16, GZ, 5,OLED_8X16);
		OLED_Update();
	}
}

