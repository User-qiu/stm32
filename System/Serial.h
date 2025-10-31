#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);



extern uint8_t Serial_TxPacket[];	//数据包发送缓存区
extern uint8_t Serial_RxPacket[];			//数据包接收缓冲区

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t* Array,uint16_t Size);
void Serial_SendString(char* String);
void Serial_SendNum(uint32_t Number, uint8_t Length);
void Serial_Printf(char* format,...);


void Serial_SendPacket(void);
//接收由中断自动完成，读取标志位和Serial_RxPacket[]即可
uint8_t Serial_GetPacketFlag(void);



/*宏定义部分*/

//是否使用串口数据包接收【不使用的话直接注释掉宏定义IF_DataPacket即可】
//#define IF_DataPacket

//是否使用USART中断服务函数【不影响上面的串口数据包接收函数】
#define IS_USART_INT_FUNC

#endif
