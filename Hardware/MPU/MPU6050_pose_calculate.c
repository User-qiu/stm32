#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"
#include "Math.h"
#include "Delay.h"

//融合互补计算的系数
//#define ALPHA 0.95238

//自己测试得到的勉强可用
#define ALPHA 0.001

//MPU6050原始数值
int16_t AX, AY, AZ;							//加速度原始数值
int16_t GX, GY, GZ;							//陀螺仪原始数值


/*由于使用的数值都是浮点数，需要对直接获取的数字进行处理*/
float AX_Handle, AY_Handle, AZ_Handle;		//处理后的加速度数值，单位g
float GX_Handle, GY_Handle, GZ_Handle;		//处理后的陀螺仪数值，单位°/s


//用于互补融合
float Yaw_g, Pitch_g, Roll_g;  				//计算出的陀螺仪角度
float Yaw_a, Pitch_a, Roll_a;				//计算出的加速度计角度



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
* @brief:此函数用来计算MPU6050的俯仰角
*/

void MPU6050_Pose_Calculate(void)
{
	MPU6050_Update();
	
	//通过陀螺仪数值计算欧拉角
	Yaw_g    =   Yaw_g   + GZ_Handle * 0.005;
	Pitch_g  =   Pitch_g + GY_Handle * 0.005;
	Roll_g   =   Roll_g  + GX_Handle * 0.005;
	
	
	//通过加速度计数值计算欧拉角
	Yaw_a   =   0.0f;
	Pitch_a =   atan2(AY_Handle , AZ_Handle) / 3.1415927f * 180.0f;
	Roll_a  =   atan2(AX_Handle , AZ_Handle) / 3.1415927f * 180.0f;
	
	
	//使用互补滤波器对陀螺仪和加速度计得计算结果进行融合
	Yaw     =   Yaw_g;
	Pitch   = 	ALPHA * Pitch_g + (1 - ALPHA) * Pitch_a;
	Roll  = 	ALPHA * Roll_g + (1 - ALPHA) * Roll_a;
	
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
