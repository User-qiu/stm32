#include "stm32f10x.h"                  // Device header
#include "Serial_Buffer.h"
#include "Serial.h"

/*<--此缓冲区使用环形队伍设计-->*/



//缓冲区数组
uint8_t Serial_Buffer[MAXSIZE];


//当前解析的数据开头
uint16_t front;


//当前解析数据的结尾
uint16_t rear;


/**
 * @brief 清空队伍
 * 
 */

void Enpty_Buffer(void)
{
	front = rear;
}



/**
 * @brief：数据入队 
 * 
 * @param Data 
 */
uint8_t EnterQueue(uint8_t Data)
{
	if((rear+1) % MAXSIZE == front)
	{
		return 0; 
	}

	Serial_Buffer[rear] = Data;
	
	rear++;
	rear %= MAXSIZE;
	

	return 1;
}


/**
 * @brief 数据出队
 * 
 * @param Data 
 */
uint8_t OutQueue(uint8_t *Data)
{
	if(front % MAXSIZE == rear)
	{
		return 0;
	}

	*Data = Serial_Buffer[front];

	front++;
	front %= MAXSIZE;
	return 1;
}

