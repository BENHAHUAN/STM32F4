/*
	10.2修改记录：
		1、增加注释
		2、PS2处理改为switch判断

*/
#include "Control.h"

#define Radius 15 // 车的半径 15 cm 

/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/

PID		PID_1,
		PID_2,
		PID_3,
		PID_gy955;

int		FTM_Get_1 = 0,
		FTM_Get_2 = 0,
		FTM_Get_3 = 0; 		// 获取脉冲

float	FTM_Speed_1 = 0,
		FTM_Speed_2 = 0,
		FTM_Speed_3 = 0; 	// 测速各个轮子的速度

float	V1_speed = 0, 
		V2_speed = 0,
		V3_speed = 0; 		// 计算各个轮子的速度

float 	duoji_jiaodu_x = 0,
		duoji_jiaodu_y = 0;

float	GY955_last = 0,
		GY955_new = 0;

uint8_t DuoJi_BiaoZhi = 0;

/**
  * @brief  车运动_处理
  * @param  speed：
  * @param  angle：
  * @param  angle_speed：
  * @retval  None
*/
void Car_ctrl(float speed,float angle,float angle_speed) 
{
	angle = (int)angle % 360;
	
	Car_Speed(speed,angle,angle_speed); // 车运动
}


/**
  * @brief  计算各轮速度 mm/s
  * @param  speed：速度 mm/s
  * @param  angle：速度方向 0-360
  * @param  angle_speed：角速度 °/s
  * @retval  None
*/
	float test_x,test_y;
	float Vx,Vy;     //定义正交分解之后的速度
void Car_Speed(float speed,float angle,float angle_speed) 
{		
	//float V1,V2,V3;  //定义各个轮子的速度
	
	angle = angle - duoji_jiaodu_x;
	angle_speed = angle_speed + PID_gy955.pwm;
	
	test_x = cos((double)angle/180.0*3.1415926);
	test_y = sin((double)angle/180.0*3.1415926);

	Vx = speed * cos((double)angle/180.0*3.1415926);  //计算X方向速度
	Vy = speed * sin((double)angle/180.0*3.1415926);  //计算Y方向速度
	
	V1_speed = 1.0/3.0*angle_speed*Radius + 2.0/3.0*Vx;         
	V2_speed = 1.0/3.0*angle_speed*Radius - 1.0/3.0*Vx - sqrt(3.0)*Vy/3.0;
	V3_speed = 1.0/3.0*angle_speed*Radius - 1.0/3.0*Vx + sqrt(3.0)*Vy/3.0;  //解算各个轮子速度
}

/**
  * @brief  获取脉冲和清除
  * @param  TIMx: TIM2、TIM3、TIM4
  * @retval 定时器的脉冲数
*/
short FTM_AB_Get(TIM_TypeDef* TIMx)  //获取脉冲和清除
{
    short val;

    val = (short)TIMx->CNT;

    TIMx->CNT = 0;

    return val;
}


/**
  * @brief  编码盘测速 mm/s
  * @retval None
*/
void Update_Speed(void) // 编码盘测速 mm/s
{
	FTM_Get_1 = -FTM_AB_Get(TIM4);  //获取脉冲和清除
	FTM_Get_2 = -FTM_AB_Get(TIM2);  //获取脉冲和清除
	FTM_Get_3 = -FTM_AB_Get(TIM3);  //获取脉冲和清除
	
    //  一圈轮 3.14*78 mm  定时周期 10ms*100=1S    转换为m/s   MC /2048 * L * T = MC / 2048 * 3.14*78 * 100 = 11.96505
    FTM_Speed_1 = FTM_Get_1 * 11.96505; //
    FTM_Speed_2 = FTM_Get_2 * 11.96505; //
    FTM_Speed_3 = FTM_Get_3 * 11.96505; //
	
//	printf("FTM_Speed_1 %f \t",FTM_Speed_1);
//	printf("FTM_Speed_2 %f \t",FTM_Speed_2);
//	printf("FTM_Speed_3 %f \r\n",FTM_Speed_3);
	
}


/**
  * @brief  电机 PID 调速 
  * @retval None
*/
void DianJi_Speed(void) 
{	
	/*       PID   目标值    测量值	*/
	PID_PWM(&PID_1,V1_speed,FTM_Speed_1);
	PID_PWM(&PID_2,V2_speed,FTM_Speed_2);
	PID_PWM(&PID_3,V3_speed,FTM_Speed_3);
		
	Motor_1(PID_1.pwm); //
	Motor_2(PID_2.pwm); //
	Motor_3(PID_3.pwm); //
}

void GY955_CTRL(void) // GY955 PID
{
	GY955_new = PTZ_Pose.YAW;
	PID_GY955(&PID_gy955,GY955_last,GY955_new);
}

void PID_set(PID *pid_t, float kp, float ki, float kd, float IntegralLimit, float MaxOutput)
{
	pid_t->Kp = kp;
	pid_t->KpOut = 0;
	pid_t->Ki = ki;
	pid_t->KiOut = 0;
	pid_t->Kd = kd;
	pid_t->KdOut = 0;

	pid_t->Target = 0; 					//设定目标值
	pid_t->Measured = 0; 				//测量值
	pid_t->err = 0; 						//本次偏差值
	pid_t->err_last = 0; 				//上一次偏差
	pid_t->integral_err = 0; 			//所有偏差之和
	
    pid_t->IntegralLimit = IntegralLimit;			//积分限幅 
	pid_t->MaxOutput = MaxOutput;				//输出限幅
	
	pid_t->pwm_last = 0; 					//上一次pwm输出
	pid_t->pwm = 0; 						//pwm输出
	
}

void abs_i(float *a, float ABS_MAX) //积分限幅 
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

void abs_pwm(int32_t *a, int32_t ABS_MAX) //输出限幅
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

void PID_PWM(PID *pid_t, float target, float measured)
{	
	pid_t->Target = target;
	pid_t->Measured = measured;
	pid_t->err = pid_t->Target - pid_t->Measured;

	pid_t->integral_err += pid_t->err;

	pid_t->KpOut = pid_t->Kp*pid_t->err;
	pid_t->KiOut = pid_t->Ki*pid_t->integral_err;
	pid_t->KdOut = pid_t->Kd*(pid_t->err - pid_t->err_last);

	//积分限幅
	abs_i(&pid_t->KiOut, pid_t->IntegralLimit);

	pid_t->pwm = (pid_t->KpOut + pid_t->KiOut + pid_t->KdOut);

	//输出限幅
	abs_pwm(&pid_t->pwm, pid_t->MaxOutput);

	pid_t->pwm  = pid_t->pwm_last * 0.95 + pid_t->pwm * 0.05;
	
	pid_t->err_last = pid_t->err;
	pid_t->pwm_last = pid_t->pwm;
}

void PID_GY955(PID *pid_t, float target, float measured)
{	
	pid_t->Target = target;
	pid_t->Measured = measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	if( pid_t->err < -180 || pid_t->err > 180)
	{
		pid_t->err = -pid_t->err;
	}
	
	pid_t->integral_err += pid_t->err;

	pid_t->KpOut = pid_t->Kp*pid_t->err;
	pid_t->KiOut = pid_t->Ki*pid_t->integral_err;
	pid_t->KdOut = pid_t->Kd*(pid_t->err - pid_t->err_last);

	//积分限幅
	abs_i(&pid_t->KiOut, pid_t->IntegralLimit);

	pid_t->pwm = (pid_t->KpOut + pid_t->KiOut + pid_t->KdOut);

	//输出限幅
	abs_pwm(&pid_t->pwm, pid_t->MaxOutput);

	pid_t->pwm  = pid_t->pwm_last * 0.7 + pid_t->pwm * 0.3;
	
	pid_t->err_last = pid_t->err;
	pid_t->pwm_last = pid_t->pwm;
}

void PS2_FangXiang(void)
{
	/******************************************
					电机
	*******************************************/
	if(PS2_AnologData(1) == 0x41) // 方向按键
	{
		/* 使用switch 占用空间大，但是效率高 */
		switch(PS2_key)				  	
		{
			/* * * * * * * * * 底盘 * * * * * * * * * * */
			case 0 :			 // 待机
			{
				Car_ctrl(0,0,0); // 车运动_处理
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 5 :			 	 // 上
			{
				Car_Speed(2*1000,90,0); // 计算各轮速度 cm/s
				//Motor_1(-4200); // A
				break;
			}
			case 6 :			  	 // 右
			{
				Car_Speed(2*1000,0,0);  // 计算各轮速度 cm/s
				//Motor_2(-4200); // B
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 7 :			  	 // 下
			{
				Car_Speed(2*1000,270,0); // 计算各轮速度 cm/s
				break;
			}
			case 8 :			  	 // 左
			{
				Car_Speed(2*1000,180,0); // 计算各轮速度 cm/s
				//Motor_3(-4200); // C
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			/* * * * * * * * * 云台 * * * * * * * * * * */
			case 13 :			  	 // 上
			{
				duoji_jiaodu_y = duoji_jiaodu_y + 1;
				duoji(1,duoji_jiaodu_y,50);
				break;
			}
			case 14 :			  	 // 右
			{
				duoji_jiaodu_x = duoji_jiaodu_x + 1;			
				duoji(0,duoji_jiaodu_x,50);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 15 :			  	 // 下
			{
				duoji_jiaodu_y = duoji_jiaodu_y - 1;
				duoji(1,duoji_jiaodu_y,50);
				break;
			}
			case 16 :			  	 // 左
			{
				duoji_jiaodu_x = duoji_jiaodu_x - 1;
				duoji(0,duoji_jiaodu_x,50);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			/* * * * * * * * * 附加键 * * * * * * * * * * */
			case 9 :			  	 // 车停
			{
				Car_ctrl(0,0,0); // 车运动_处理
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 11 :			  	 // 左自旋
			{
				Car_Speed(0,0,-200); // 计算各轮速度 cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 10 :			  	 // 舵机复位
			{
				duoji_jiaodu_x = 0;
				duoji_jiaodu_y = 0;
				duoji(0,0,50); // 复位
				os_dly_wait(10);
				duoji(1,0,50); // 复位
				os_dly_wait(10);
			
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 12 :			  	 // 右自旋
			{
				Car_Speed(0,0,200); // 计算各轮速度 cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
		}
	}
	else if(PS2_AnologData(1) == 0x73)
	{
		PS2_YaoGan_DianJi();
		PS2_YaoGan_DuoJi();
		
		/*     附加键       */
		/* 使用switch 占用空间大，但是效率高 */
		switch(PS2_key)				  	
		{
			/* * * * * * * * * 底盘 * * * * * * * * * * */
			case 9 :			 // 车停
			{
				Car_ctrl(0,0,0); // 车运动_处理
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 11 :			 	 // 左自旋
			{
				Car_Speed(0,0,-300); // 计算各轮速度 cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 10 :			 	 // 舵机复位
			{
				duoji_jiaodu_x = 0;
				duoji_jiaodu_y = 0;
				duoji(0,0,50); // 复位
				os_dly_wait(10);
				duoji(1,0,50); // 复位
				os_dly_wait(10);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 12 :			 	 // 右自旋
			{
				Car_Speed(0,0,300); // 计算各轮速度 cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
		}	
	}
}

	double V_X = 0.0,V_Y = 0.0;
	double V = 0.0;
	float X_jiaodu = 0.0;
void PS2_YaoGan_DianJi(void) // 遥感 电机
{
	int Zuo_X,Zuo_Y;
	
	Zuo_X = PS2_AnologData(7);
	Zuo_Y = PS2_AnologData(8);

	V_X = Zuo_X-127;
	V_Y = 128-Zuo_Y;
	

	V = sqrt(V_X*V_X + V_Y*V_Y);
	
	X_jiaodu = acos(V_X/V)*180.0/3.1415926;
	
	if(V_Y < 0)
	{
		X_jiaodu = -X_jiaodu;
	}
	
	//X_jiaodu = asin(V_Y/V)*180.0/3.14159;
	
	//YaoGan_Car_Speed(V_X,V_Y);
	
	V = 30 * V;
	Car_Speed(V,X_jiaodu,0);
}

	 float You_X = 0,You_Y = 0;
void PS2_YaoGan_DuoJi(void) // PS2 遥感 舵机
{
	static int You_X_last = 0,You_Y_last = 0;
	
	You_X_last = You_X;
	You_Y_last = You_Y;
	
	You_X = PS2_AnologData(5);
	You_Y = PS2_AnologData(6);
	
	if(You_X != You_X_last)
	{
		duoji_jiaodu_x = You_X-127;
		duoji_jiaodu_x = (double)duoji_jiaodu_x/128.0*90.0;
		duoji(0,duoji_jiaodu_x,100);
		os_dly_wait(15);
		DuoJi_BiaoZhi = 1;
		
	}
	if(You_Y != You_Y_last)
	{
		duoji_jiaodu_y = 128-You_Y;
		duoji_jiaodu_y = (double)duoji_jiaodu_y/128.0*90.0;
		duoji(1,duoji_jiaodu_y,100);
		os_dly_wait(15);
	}
}

void TuChuan_XY(int x,int y) // 图传 X Y 轴
{
#if 0
	static int You_X = 0,You_Y = 0;
	static int You_X_last = 0,You_Y_last = 0;
	
	You_X_last = You_X;
	You_Y_last = You_Y;
	
	You_X = x;
	You_Y = y;
	
	#if 0
	
	#endif
	
	if(You_X != You_X_last)
	{
		duoji_jiaodu_x = You_X-128;
		duoji(0,(double)duoji_jiaodu_x/320.0*90.0,2200);
		os_dly_wait(15);
		DuoJi_BiaoZhi = 1;
	}
	if(You_Y != You_Y_last)
	{
		duoji_jiaodu_y = 127-You_Y;
		duoji(1,(double)duoji_jiaodu_y/240.0*90.0,2200);
		os_dly_wait(15);
	}
#endif

#if 1

	if(y > 15) // 上
	{
		duoji_jiaodu_y = duoji_jiaodu_y + 1 + y*0.005;
		duoji(1,duoji_jiaodu_y,80);
		os_dly_wait(10);
	}
	else if(y < -15) // 下
	{
		duoji_jiaodu_y = duoji_jiaodu_y - 1 + y*0.005;
		duoji(1,duoji_jiaodu_y,80);
		os_dly_wait(10); 
	}
	
	if(x > 15) // 右
	{
		duoji_jiaodu_x = duoji_jiaodu_x + 1 + x*0.005;			
		duoji(0,duoji_jiaodu_x,80);
		os_dly_wait(10);
		
		DuoJi_BiaoZhi = 1;
	}
	else if(x < -15) // 左
	{
		duoji_jiaodu_x = duoji_jiaodu_x - 1 + x*0.005;
		duoji(0,duoji_jiaodu_x,80);
		os_dly_wait(10);
		
		DuoJi_BiaoZhi = 1;
	}
	
#endif

}

/*************************************************************
          入口参数分别是，X轴速度、Y轴速度
*************************************************************/
void YaoGan_Car_Speed(float Vx,float Vy) // 计算各轮速度 mm/s
{
	V1_speed =  + 2.0/3.0*Vx;         
	V2_speed =  - 1.0/3.0*Vx - sqrt(3.0)*Vy/3.0;
	V3_speed =  - 1.0/3.0*Vx + sqrt(3.0)*Vy/3.0;  //解算各个轮子速度
}

void Motor_1(int32_t PWM) // 
{
	TIM1->CCR2 = -PWM + 9000;
}

void Motor_2(int32_t PWM) // 
{
	TIM1->CCR1 = -PWM + 9000;
}

void Motor_3(int32_t PWM) // 
{
	TIM1->CCR3 = -PWM + 9000;
}

#if 0
void Motor_gpio(void) // 不是H桥控制
{
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_2,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_4,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_5,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_6,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_7,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
}

/* PWM = -16800----16800 */
void Motor_Set1(int32_t PWM) // 不是废卡控制
{
	if(PWM > 0)
	{
		GPIOE->BSRRL = GPIO_Pin_2;
		GPIOE->BSRRH = GPIO_Pin_3;
		TIM1->CCR1 = PWM + 1000;
	}
	else if(PWM < 0)
	{
		GPIOE->BSRRL = GPIO_Pin_3;
		GPIOE->BSRRH = GPIO_Pin_2;
		TIM1->CCR1 = -PWM + 1000;
	}
}
void Motor_Set2(int32_t PWM)
{
	if(PWM > 0)
	{
		GPIOE->BSRRL = GPIO_Pin_4;
		GPIOE->BSRRH = GPIO_Pin_5;
		TIM1->CCR2 = PWM + 1000;
	}
	else if(PWM < 0)
	{
		GPIOE->BSRRL = GPIO_Pin_5;
		GPIOE->BSRRH = GPIO_Pin_4;
		TIM1->CCR2 = -PWM + 1000;
	}
}
void Motor_Set3(int32_t PWM)
{
	if(PWM > 0)
	{
		GPIOE->BSRRL = GPIO_Pin_6;
		GPIOE->BSRRH = GPIO_Pin_7;
		TIM1->CCR3 = PWM + 1000;
	}
	else if(PWM < 0)
	{
		GPIOE->BSRRL = GPIO_Pin_7;
		GPIOE->BSRRH = GPIO_Pin_6;
		TIM1->CCR3 = -PWM + 1000;
	}
}
#endif

