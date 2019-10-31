// Control.h

#ifndef __Control_H
#define __Control_H

#include "includes.h"

typedef struct {
	float Kp;
	float KpOut;
	float Ki;
	float KiOut;
	float Kd;
	float KdOut;

	float Target; 					//设定目标值
	float Measured; 				//测量值
	float err; 						//本次偏差值
	float err_last; 				//上一次偏差
	float integral_err; 			//所有偏差之和
	
    float IntegralLimit;			//积分限幅 
	int32_t MaxOutput;				//输出限幅
	
	int32_t pwm_last; 				//上一次pwm输出
	int32_t pwm; 					//pwm输出
}PID;

extern PID		PID_1,PID_2,PID_3,PID_gy955;
extern float V1_speed, V2_speed, V3_speed; // 计算各个轮子的速度
extern uint8_t DuoJi_BiaoZhi;

extern float GY955_last,GY955_new;


void Car_ctrl(float speed,float angle,float angle_speed); // 车运动_处理
void Car_Speed(float speed,float angle,float angle_speed); // 计算各轮速度

void PID_set(PID *pid_t, float kp, float ki, float kd, float IntegralLimit, float MaxOutput);
void PID_PWM(PID *pid_t, float target, float measured);
void PID_GY955(PID *pid_t, float target, float measured);

void GY955_CTRL(void); // GY955 PID

void Update_Speed(void);
void DianJi_Speed(void); // 电机 PID 调速

void PS2_FangXiang(void);

void PS2_YaoGan_DianJi(void);
void YaoGan_Car_Speed(float Vx,float Vy); // 计算各轮速度 cm/s

void PS2_YaoGan_DuoJi(void);

void TuChuan_XY(int x,int y); //图传

void Motor_1(int32_t PWM); // 
void Motor_2(int32_t PWM); // 
void Motor_3(int32_t PWM); // 

#endif
