#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include "Time.h"
#include "Motor.h"
#include "TIM_Encoder.h"
#include "PID.h"
#include "Delay.h"

PID_Struct pid;


//红外反射器测速,返回每秒的转数
//1ms触发一次
//每100ms返回一次结果，但是每圈有三次触发【有三片扇叶】
int16_t Motor_Speed;
void GetSpeed_Tick(void)
{
	static uint16_t Speed_Tick = 0;
	Speed_Tick++;
	if(Speed_Tick >= 100)
	{
		Speed_Tick = 0;
		Motor_Speed = Encoder_GetCounter() * 10 / 3;
	}
}


int main(void)
{

	OLED_Init();
	Serial_Init();
	Time_Init();
	Motor_Init();			//直流电机初始化
	Encoder_Init();			//编码器初始化


	// PID_Init(&pid, 1.0f, 0.0f, 0.0f, 10.0f, 50.0f);

	OLED_ShowString(1,1,"Speed",OLED_8X16);
	OLED_ShowString(1,32,"PWM",OLED_8X16);

	//直电机电流占空比 speed / 100
	uint8_t speed = 30;

	while(1)
	{
		OLED_ShowSignedNum(1, 16, Motor_Speed, 5, OLED_8X16);		//显示电机转速

		OLED_ShowNum(1, 48, speed, 3, OLED_8X16);					//显示电流占空比
		OLED_Update();
		Motor_SetSpeed(speed+=30);
		speed %= 100;
		Delay_ms(100);
	}


}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		GetSpeed_Tick();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

