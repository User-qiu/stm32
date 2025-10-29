#include "MPU6050_Kalman_Filter.h"
#include "math.h"
#include "stdio.h"

/**
 * @brief 卡尔曼滤波器初始化
 * @param kf: 卡尔曼滤波器结构体指针
 * @param q_angle: 过程噪声协方差（角度）
 * @param q_bias: 过程噪声协方差（角速度偏差）
 * @param r_measure: 测量噪声协方差
 */
void Kalman_Init(KalmanFilter *kf, float q_angle, float q_bias, float r_measure)
{
    /* 重置卡尔曼滤波器的状态 */
    kf->Q_angle = q_angle;    // 角度过程噪声，影响滤波器对角度变化的响应速度
    kf->Q_bias = q_bias;      // 角速度偏差过程噪声，影响偏差估计的稳定性
    kf->R_measure = r_measure; // 测量噪声，影响对传感器测量值的信任程度
    
    kf->angle = 0.0f;         // 当前估计的角度
    kf->bias = 0.0f;          // 当前估计的角速度偏差（陀螺仪零漂）
    kf->rate = 0.0f;          // 未经偏差校正的角速度
    
    /* 初始化误差协方差矩阵
     * 较大的初始值表示滤波器初始时更信任测量值 */
    kf->P[0][0] = 0.0f;       // 角度估计的方差
    kf->P[0][1] = 0.0f;       // 角度与偏差的协方差
    kf->P[1][0] = 0.0f;       // 偏差与角度的协方差
    kf->P[1][1] = 0.0f;       // 偏差估计的方差
}

/**
 * @brief 卡尔曼滤波器更新步骤
 * @param kf: 卡尔曼滤波器结构体指针
 * @param new_angle: 加速度计测量的角度（度）
 * @param new_rate: 陀螺仪测量的角速度（度/秒）
 * @param dt: 采样时间（秒）
 * @return 滤波后的角度估计值（度）
 * 
 * 卡尔曼滤波原理：
 * 1. 预测步骤：使用陀螺仪数据预测当前角度
 * 2. 更新步骤：使用加速度计数据修正预测值
 * 
 * 该方法能有效融合陀螺仪的高频特性和加速度计的低频特性，
 * 得到稳定、准确的角度估计。
 */
float Kalman_Update(KalmanFilter *kf, float new_angle, float new_rate, float dt)
{
    /* 第一步：预测步骤（时间更新） */
    
    // 1. 状态预测：使用陀螺仪数据预测角度（减去偏差校正）
    kf->rate = new_rate - kf->bias;  // 角速度 = 测量值 - 估计偏差
    kf->angle += dt * kf->rate;      // 角度 = 上一时刻角度 + 角速度×时间
    
    // 2. 误差协方差预测
    // P = F * P * F^T + Q
    // 其中F是状态转移矩阵：[1, -dt; 0, 1]
    kf->P[0][0] += dt * (dt * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q_angle);
    kf->P[0][1] -= dt * kf->P[1][1];
    kf->P[1][0] -= dt * kf->P[1][1];
    kf->P[1][1] += kf->Q_bias * dt;
    
    /* 第二步：更新步骤（测量更新） */
    
    // 3. 计算卡尔曼增益
    // K = P * H^T / (H * P * H^T + R)
    // 其中H是观测矩阵：[1, 0]
    float S = kf->P[0][0] + kf->R_measure;  // 创新协方差
    float K[2];  // 卡尔曼增益
    K[0] = kf->P[0][0] / S;  // 角度增益
    K[1] = kf->P[1][0] / S;  // 偏差增益
    
    // 4. 计算测量残差（实际测量值与预测值的差）
    float y = new_angle - kf->angle;
    
    // 5. 状态更新
    kf->angle += K[0] * y;  // 用卡尔曼增益修正角度估计
    kf->bias += K[1] * y;   // 用卡尔曼增益修正偏差估计
    
    // 6. 误差协方差更新
    // P = (I - K * H) * P
    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];
    
    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;
    
    return kf->angle;
}

/**
 * @brief 获取卡尔曼滤波器当前状态
 * @param kf: 卡尔曼滤波器结构体指针
 * @param angle: 角度输出指针
 * @param bias: 偏差输出指针
 * @param rate: 角速度输出指针
 */
void Kalman_GetState(KalmanFilter *kf, float *angle, float *bias, float *rate)
{
    if(angle != NULL) *angle = kf->angle;
    if(bias != NULL) *bias = kf->bias;
    if(rate != NULL) *rate = kf->rate;
}
