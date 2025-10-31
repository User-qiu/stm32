#include "stm32f10x.h"                  // Device header
#include "PID.h"


/**
 * @brief 用于初始化PID参数
 * 
 * @param PID       PID结构体
 * @param p         比例系数
 * @param i         积分系数
 * @param d         微分系数
 * @param maxI      最大积分值
 * @param maxOut    最大输出值
 */
void PID_Init(PID_Struct *PID, float p, float i, float d, float maxI, float maxOut)
{
    PID->kp = p;
    PID->ki = i;
    PID->ki = d;
    PID->maxInteral = maxI;
    PID->maxOutput = maxOut;
}


/**
 * @brief 
 * 
 * @param PID            PID结构体
 * @param reference     目标值
 * @param feekback      反馈值
 */
void PID_Calculate(PID_Struct *pid, float reference, float feekback)
{
    //存储上次的误差
    pid->lastError = pid->error;
    
    //计算当前误差
    pid->error = reference - feekback;
    

    //更新数据
    float pout = pid->kp * pid->error;                     //计算比例值
    pid->interal += pid->ki * pid->error;             //计算积分值
    float dout = (pid->lastError - pid->error) * pid->kd;  //计算微分值


    //积分限幅
    if(pid->interal > pid->maxInteral){pid->interal = pid->maxInteral;}
    else if(pid->interal < -pid->maxInteral){pid->interal = -pid->maxInteral;}


    //计算输出值
    pid->output = pout + pid->interal + dout;


    //输出值限幅
    if(pid->output > pid->maxOutput){pid->output = pid->maxOutput;}
    else if(pid->output < -pid->maxOutput){pid->output = -pid->maxOutput;}
}


/**
 * @brief 串级PID的计算函数
 * 
 * @param pid                   //串级PID结构体
 * @param outerRef              //外环目标值
 * @param outerFdb              //外环反馈值
 * @param innerFdb              //内环反馈值
 */
void PID_CascadeCalc(Cascade_PID_Struct *pid, float outerRef, float outerFdb, float innerFdb)
{
    PID_Calculate(&pid->outer, outerRef, outerFdb);             //计算外环
    PID_Calculate(&pid->inner, pid->outer.output,innerFdb);     //计算内环

    pid->output = pid->inner.output;                            //内环输出就是串级PID的输出
}

