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

	float Target; 					//�趨Ŀ��ֵ
	float Measured; 				//����ֵ
	float err; 						//����ƫ��ֵ
	float err_last; 				//��һ��ƫ��
	float integral_err; 			//����ƫ��֮��
	
    float IntegralLimit;			//�����޷� 
	int32_t MaxOutput;				//����޷�
	
	int32_t pwm_last; 				//��һ��pwm���
	int32_t pwm; 					//pwm���
}PID;

extern PID		PID_1,PID_2,PID_3,PID_gy955;
extern float V1_speed, V2_speed, V3_speed; // ����������ӵ��ٶ�
extern uint8_t DuoJi_BiaoZhi;

extern float GY955_last,GY955_new;


void Car_ctrl(float speed,float angle,float angle_speed); // ���˶�_����
void Car_Speed(float speed,float angle,float angle_speed); // ��������ٶ�

void PID_set(PID *pid_t, float kp, float ki, float kd, float IntegralLimit, float MaxOutput);
void PID_PWM(PID *pid_t, float target, float measured);
void PID_GY955(PID *pid_t, float target, float measured);

void GY955_CTRL(void); // GY955 PID

void Update_Speed(void);
void DianJi_Speed(void); // ��� PID ����

void PS2_FangXiang(void);

void PS2_YaoGan_DianJi(void);
void YaoGan_Car_Speed(float Vx,float Vy); // ��������ٶ� cm/s

void PS2_YaoGan_DuoJi(void);

void TuChuan_XY(int x,int y); //ͼ��

void Motor_1(int32_t PWM); // 
void Motor_2(int32_t PWM); // 
void Motor_3(int32_t PWM); // 

#endif
