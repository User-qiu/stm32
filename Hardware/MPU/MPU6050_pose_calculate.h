#ifndef __MPU6050_POSE_CALCULATE_H_
#define __MPU6050_POSE_CALCULATE_H_


void MPU6050_pose_claculate_Init(void);
void MPU6050_GetHandledData(float *GetYaw, float *GetPitch, float *GetRoll);
void MPU6050_Tick(void);

#endif
