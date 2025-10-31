#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "Serial.h"
#include "Serial_Buffer.h"
#include "Command.h"



//串口接收到的数据
uint8_t RxData;


//串口出队数据
uint8_t DataOut;


//处理命令的最长长度
#define COMMAND_LENGTH 8

//待处理命令
uint8_t Wait_Handle[COMMAND_LENGTH + 1];



/**
 * @brief 初始化
 * 
 */
void Command_Init(void)
{
    Serial_Init();
    Serial_SendString("Command is ready");
}


/**
 * @brief 接收命令
 * 
 */
void Command_Receive(void)
{
    while(1)
    {
        if(Serial_GetRxFlag())
        {
            RxData = Serial_GetRxData();
            EnterQueue(RxData);
        }
        
        if(RxData == '#')
        {
            break;
        }
    }
}


/**
 * @brief 处理命令
 */
void Command_Handle(void)
{
    uint8_t  Wait_Handle_Index = 0;
    while (1)
    {
        OutQueue(&RxData);
        if(RxData == '@')
        {
            break;
        }
    }

    while(1)
    {
        OutQueue(&RxData);

        if(RxData == '#')
        {
            Wait_Handle[Wait_Handle_Index] = '\0';
            break;
        }

        Wait_Handle[Wait_Handle_Index] = RxData;
        Wait_Handle_Index++;

        if(Wait_Handle_Index > COMMAND_LENGTH){return;}
    }
    
    if(strcmp((char*)Wait_Handle,"good") == 0)
    {
        Serial_Printf("goooooooooooooood\n");
    }
    else if(strcmp((char*)Wait_Handle,"abc") == 0)
    {
        Serial_Printf("yes i am abc\n");
    }
    else
    {
        Serial_Printf("命令输入错误\n");
    }

    RxData = '!';

}
