#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Time.h"
#include "MPU6050.h"
#include "MPU6050_pose_calculate.h"


//横滚角、俯仰角、偏航角
float Roll,Pitch,Yaw;

int main(void)
{
	OLED_Init();
	
	/*
	首先MPU6050初始化，
	接着是MPU6050校正【耗时5s左右，时间较长】，
	最后是启动定时器，顺序不可改变
	*/
	MPU6050_Init();
	
	OLED_Clear();
	OLED_ShowString(1, 1, "MPU校准中", OLED_8X16);
	OLED_Update();
	MPU6050_CalibrateSensors();
	OLED_Clear();
	
	
	Time_Init();
	
	OLED_ShowString(0,(1-1)*16,"MPU6050",OLED_8X16);
	OLED_ShowString(0,(2-1)*16,"Roll",OLED_8X16);
	OLED_ShowString(0,(3-1)*16,"Pitch",OLED_8X16);
	OLED_ShowString(0,(4-1)*16,"Yaw",OLED_8X16);
	
	OLED_Update();
	

	while (1)
	{
		
		MPU6050_GetHandledData(&Yaw, &Pitch, &Roll);
		OLED_ShowFloatNum(0+48, (2-1)*16, Roll,  3,3, OLED_8X16);
		OLED_ShowFloatNum(0+48, (3-1)*16, Pitch, 3,3,  OLED_8X16);
		OLED_ShowFloatNum(0+48, (4-1)*16, Yaw,   3,3,  OLED_8X16);
		
		OLED_Update();
		
	}
}

// 中断服务函数模板
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) == SET) {
		MPU6050_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
	}
}

