#ifndef __MPU6050_KALMAN_FILTER_H
#define __MPU6050_KALMAN_FILTER_H

#include <stdint.h>

/**
 * @brief 卡尔曼滤波器结构体
 * 
 * 状态变量：
 * - angle: 估计的角度（度）
 * - bias: 估计的角速度偏差（度/秒）
 * - rate: 未经偏差校正的角速度（度/秒）
 * 
 * 协方差矩阵：
 * - P[2][2]: 误差协方差矩阵
 * 
 * 噪声参数：
 * - Q_angle: 角度过程噪声
 * - Q_bias: 角速度偏差过程噪声  
 * - R_measure: 测量噪声
 */
typedef struct {
    /* 状态变量 */
    float angle;    // 估计的角度
    float bias;     // 估计的角速度偏差
    float rate;     // 未经偏差校正的角速度
    
    /* 误差协方差矩阵 */
    float P[2][2];
    
    /* 噪声参数 */
    float Q_angle;  // 角度过程噪声
    float Q_bias;   // 角速度偏差过程噪声
    float R_measure;// 测量噪声
} KalmanFilter;

/* 函数声明 */
void Kalman_Init(KalmanFilter *kf, float q_angle, float q_bias, float r_measure);
float Kalman_Update(KalmanFilter *kf, float new_angle, float new_rate, float dt);
void Kalman_GetState(KalmanFilter *kf, float *angle, float *bias, float *rate);

#endif
