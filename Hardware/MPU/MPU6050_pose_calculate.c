#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"
#include "Math.h"
#include "MPU6050_pose_calculate.h"
#include "MPU6050_Kalman_Filter.h"
#include "Delay.h"


//MPU6050原始数值
int16_t AX, AY, AZ;							//加速度原始数值
int16_t GX, GY, GZ;							//陀螺仪原始数值


/*由于使用的数值都是浮点数，需要对直接获取的数字进行处理*/
float AX_Handle, AY_Handle, AZ_Handle;		//处理后的加速度数值，单位g
float GX_Handle, GY_Handle, GZ_Handle;		//处理后的陀螺仪数值，单位°/s


//用于互补融合
float Yaw_g, Pitch_g, Roll_g;  				//计算出的陀螺仪角度
float Yaw_a, Pitch_a, Roll_a;				//计算出的加速度计角度

// 定义卡尔曼滤波器实例
KalmanFilter kalman_pitch, kalman_roll;


//最终求得的横滚角、俯仰角、偏航角
static float Roll,Pitch,Yaw;

//校准零点漂移的部分
typedef struct {
    float offset_x;
    float offset_y;
    float offset_z;
} SensorCalibration;

static SensorCalibration gyro_calib = {0};
static SensorCalibration accel_calib = {0};
static uint8_t is_calibrated = 0;


void MPU6050_GetHandledData(float *GetYaw, float *GetPitch, float *GetRoll);
void MPU6050_Pose_Calculate(void);
void MPU6050_Update(void);
void MPU6050_CalibrateSensors(void);
void MPU6050_Kalman_Init(void);


/**
 * @brief MPU6050姿态解算初始化 
 * 
 */

 void MPU6050_pose_claculate_Init(void)
 {
    MPU6050_Kalman_Init();
    MPU6050_CalibrateSensors();
 }


/**
 * @brief 初始化卡尔曼滤波器
 */
void MPU6050_Kalman_Init(void)
{
    // 初始化俯仰角和横滚角的卡尔曼滤波器
    // 参数说明：
    // Q_angle: 角度过程噪声 (0.001-0.1)，值越小滤波器越平滑但响应变慢
    // Q_bias: 角速度偏差过程噪声 (0.001-0.01)，值越小偏差估计越稳定
    // R_measure: 测量噪声 (0.01-1.0)，值越大越信任陀螺仪，值越小越信任加速度计
    
    Kalman_Init(&kalman_pitch, 0.001f, 0.003f, 0.03f);
    Kalman_Init(&kalman_roll, 0.001f, 0.003f, 0.03f);
}

//陀螺仪零漂校准
void MPU6050_CalibrateSensors(void)
{
    int32_t gx_sum = 0, gy_sum = 0, gz_sum = 0;
    int32_t ax_sum = 0, ay_sum = 0, az_sum = 0;
    const uint16_t sample_count = 1000;
    
    // 采集样本进行校准
    for(uint16_t i = 0; i < sample_count; i++) {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        
        gx_sum += GX;
        gy_sum += GY;
        gz_sum += GZ;
        ax_sum += AX;
        ay_sum += AY;
        az_sum += AZ;
        
        Delay_ms(1);
    }
    
    // 计算平均值作为零偏
    gyro_calib.offset_x = (float)gx_sum / sample_count;
    gyro_calib.offset_y = (float)gy_sum / sample_count;
    gyro_calib.offset_z = (float)gz_sum / sample_count;
    
    // 加速度计校准（假设Z轴指向重力方向）
    accel_calib.offset_x = (float)ax_sum / sample_count;
    accel_calib.offset_y = (float)ay_sum / sample_count;
    accel_calib.offset_z = (float)az_sum / sample_count - 16384; // 1g对应的值
    
    is_calibrated = 1;
}

/**
* @brief:此函数用来获取MPU6050原始数据，并将其转化为可用的浮点型数据
*/
void MPU6050_Update(void)
{
	//MPU6050获取原始数值
	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
	
	
    // 应用陀螺仪零漂校准
    if(is_calibrated) {
        GX -= (int16_t)gyro_calib.offset_x;
        GY -= (int16_t)gyro_calib.offset_y;
        GZ -= (int16_t)gyro_calib.offset_z;
        
        AX -= (int16_t)accel_calib.offset_x;
        AY -= (int16_t)accel_calib.offset_y;
        AZ -= (int16_t)accel_calib.offset_z;
    }
	
	//处理获取的数值【2g/32768 = 6.1035e*10-5 * g】
	AX_Handle = AX * 6.1035e-5f;
	AY_Handle = AY * 6.1035e-5f;
	AZ_Handle = AZ * 6.1035e-5f;
	
	//处理获取的数值【2000°/s * 32768 = 6.1035e*10-5 * 2000】
	GX_Handle = GX * 6.1035e-2f;
	GY_Handle = GY * 6.1035e-2f;
	GZ_Handle = GZ * 6.1035e-2f;
}



/**
 * @brief 使用卡尔曼滤波计算姿态角
 */
void MPU6050_Pose_Calculate(void)
{
    float dt = 0.005f;      //陀螺仪采样间隔
    
    MPU6050_Update();

    
    // 通过加速度计计算角度
    float pitch_accel = atan2(AY_Handle, AZ_Handle) * 180.0f / 3.1415927f;
    float roll_accel = atan2(AX_Handle, AZ_Handle) * 180.0f / 3.1415927f;
    
    // 使用卡尔曼滤波融合数据
    Pitch = Kalman_Update(&kalman_pitch, pitch_accel, GY_Handle, dt);
    Roll = Kalman_Update(&kalman_roll, roll_accel, GX_Handle, dt);
    
    // 偏航角仍使用陀螺仪积分（无磁场传感器）
    Yaw += GZ_Handle * dt;
	
}


/**
* @brief:此函数用来返回处理完成的欧拉角数据
*/
void MPU6050_GetHandledData(float *GetYaw, float *GetPitch, float *GetRoll)
{
	*GetYaw   =  Yaw;
	*GetPitch =  Pitch;
	*GetRoll  =  Roll;
}


/**
* @brief:此函数用来计算MPU6050的俯仰角,由定时器1ms调用一次
*/
void MPU6050_Tick(void)
{
	static uint16_t MPU6050_Count;
	MPU6050_Count++;
	
	if(MPU6050_Count >= 5)
	{
		MPU6050_Count = 0;
		
		//计算MPU6050欧拉角
		MPU6050_Pose_Calculate();
	}
}
