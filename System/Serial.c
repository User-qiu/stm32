#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"


//串口接收到的数据
uint8_t Serial_RxData;
//串口接收标志位
uint8_t Serial_RxFlag;


/**
  * 函数：串口USART1初始化
  */
void Serial_Init(void)
{
	/*开启总线时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*初始化GPIO口 USART1 PA9 Tx引脚*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*初始化GPIO口 USART1 PA10 Rx引脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*初始化USART*/
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;											//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;					//模式【发送/接收/收发一起】
	USART_InitStruct.USART_Parity = USART_Parity_No;								//校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;								//停止位选择
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;						//数据位位宽
	USART_Init(USART1, &USART_InitStruct);
	
	/*开启USART到NVIC的输出*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	/*配置NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	//使能USART
	USART_Cmd(USART1,ENABLE);
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);									//发送字节数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待数据转运出TDR
																	//再次写入数据时TXE标志位自动清零，无需手动清零
}


/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t* Array,uint16_t Size)
{
	for(int i = 0; i < Size;i++)
	{
		Serial_SendByte(Array[i]);
	}
}


/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(char* String)
{
	char* ch = String;
	while(*ch != '\0')
	{
		Serial_SendByte(*ch);
		ch++;
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于Num的Pow次方
  */
uint32_t Number_Pow(uint32_t Num,uint32_t Pow)
{
	uint32_t Number = 1;
	while(Pow--)
	{
		Number *= Num;
	}
	return Number;
}


/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNum(uint32_t Number, uint8_t Length)
{
	uint8_t ch;
	for(int i = Length - 1;i >= 0; i--)
	{
		ch = (Number / Number_Pow(10,i)) % 10;
		Serial_SendByte(ch + '0');
	}
}

/*将fputc重定向到串口*/
int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}


/*--------串口数据包接收部分--------------*/


//是否使用串口数据包接受模式判断
#ifdef IF_DataPacket

/*HEX数据包接收缓冲*/
uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_PacketFlag;	//数据包接收标志位

uint8_t Serial_GetPacketFlag(void)
{
	if(Serial_PacketFlag == 1)
	{
		Serial_PacketFlag = 0;
		return 1;
	}
	return 0;
}

//发送数据包
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);						//发送包头
	Serial_SendArray(Serial_TxPacket,4);		//发送数据载荷
	Serial_SendByte(0xFE);						//发送包尾
}


/*收发HEX数据包 固定包长 载荷数据固定4字节，包尾为FE 包头为FF*/
//接收数据包【使用状态机思维编程】
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;									//数据包接收标志位，0等待包头，1接收数据，2等待包尾
	static uint8_t pRxData = 0;									//指示收到的是数据包内的第几个数据
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		if(RxState == 0)										//等待数据包包头
		{
			if(RxData == 0xFF)									//确认收到包头
			{
				RxState = 1;
				pRxData = 0;
			}
		}
		else if(RxState == 1)									//接收数据
		{
			Serial_RxPacket[pRxData] = RxData;
			pRxData++;
			if(pRxData >= 4)									//接收到完整的四个数据
			{
				RxState = 2;
				pRxData = 0;
			}
		}
		else if(RxState == 2)									//等待包尾
		{
			if(RxData == 0xFE)									//收到包尾
			{
				RxState = 0;
				Serial_PacketFlag = 1;							//置接收完成标志位
			}
		}
		USART_ClearFlag(USART1,USART_IT_RXNE);					//读取数据寄存器会自动清除此标志位														//如果已经读取了数据寄存器，也可以不执行此代码
	}
}


#endif


//判断是否使用USART中断服务函数
#ifdef IS_USART_INT_FUNC



/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}


/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}


/**
  * 函    数：USART1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
		Serial_RxFlag = 1;										//置接收标志位变量为1
		USART_ClearFlag(USART1,USART_IT_RXNE);					//读取数据寄存器会自动清除此标志位
																//如果已经读取了数据寄存器，也可以不执行此代码
	}
}

#endif

