#ifndef __PID_H_
#define __PID_H_

//PID结构体
typedef struct
{
    float kp, ki, kd;           //比例、积分、微分
    float error, lastError;     //误差、上次误差
    float interal, maxInteral;  //积分，积分幅度
    float output, maxOutput;    //输出，最大输出

}PID_Struct;


//串级PID结构体
typedef struct
{
    PID_Struct inner;                  //PID内环
    PID_Struct outer;                  //PID外环
    float output;                      //传级输出，相当于inner.output

}Cascade_PID_Struct;


void PID_Init(PID_Struct *PID, float p, float i, float d, float maxI, float maxOut);
void PID_Calculate(PID_Struct *pid, float reference, float feekback);


#endif
