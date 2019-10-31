/*
	10.2�޸ļ�¼��
		1������ע��
		2��PS2�����Ϊswitch�ж�

*/
#include "Control.h"

#define Radius 15 // ���İ뾶 15 cm 

/*
**********************************************************************************************************
											 ����
**********************************************************************************************************
*/

PID		PID_1,
		PID_2,
		PID_3,
		PID_gy955;

int		FTM_Get_1 = 0,
		FTM_Get_2 = 0,
		FTM_Get_3 = 0; 		// ��ȡ����

float	FTM_Speed_1 = 0,
		FTM_Speed_2 = 0,
		FTM_Speed_3 = 0; 	// ���ٸ������ӵ��ٶ�

float	V1_speed = 0, 
		V2_speed = 0,
		V3_speed = 0; 		// ����������ӵ��ٶ�

float 	duoji_jiaodu_x = 0,
		duoji_jiaodu_y = 0;

float	GY955_last = 0,
		GY955_new = 0;

uint8_t DuoJi_BiaoZhi = 0;

/**
  * @brief  ���˶�_����
  * @param  speed��
  * @param  angle��
  * @param  angle_speed��
  * @retval  None
*/
void Car_ctrl(float speed,float angle,float angle_speed) 
{
	angle = (int)angle % 360;
	
	Car_Speed(speed,angle,angle_speed); // ���˶�
}


/**
  * @brief  ��������ٶ� mm/s
  * @param  speed���ٶ� mm/s
  * @param  angle���ٶȷ��� 0-360
  * @param  angle_speed�����ٶ� ��/s
  * @retval  None
*/
	float test_x,test_y;
	float Vx,Vy;     //���������ֽ�֮����ٶ�
void Car_Speed(float speed,float angle,float angle_speed) 
{		
	//float V1,V2,V3;  //����������ӵ��ٶ�
	
	angle = angle - duoji_jiaodu_x;
	angle_speed = angle_speed + PID_gy955.pwm;
	
	test_x = cos((double)angle/180.0*3.1415926);
	test_y = sin((double)angle/180.0*3.1415926);

	Vx = speed * cos((double)angle/180.0*3.1415926);  //����X�����ٶ�
	Vy = speed * sin((double)angle/180.0*3.1415926);  //����Y�����ٶ�
	
	V1_speed = 1.0/3.0*angle_speed*Radius + 2.0/3.0*Vx;         
	V2_speed = 1.0/3.0*angle_speed*Radius - 1.0/3.0*Vx - sqrt(3.0)*Vy/3.0;
	V3_speed = 1.0/3.0*angle_speed*Radius - 1.0/3.0*Vx + sqrt(3.0)*Vy/3.0;  //������������ٶ�
}

/**
  * @brief  ��ȡ��������
  * @param  TIMx: TIM2��TIM3��TIM4
  * @retval ��ʱ����������
*/
short FTM_AB_Get(TIM_TypeDef* TIMx)  //��ȡ��������
{
    short val;

    val = (short)TIMx->CNT;

    TIMx->CNT = 0;

    return val;
}


/**
  * @brief  �����̲��� mm/s
  * @retval None
*/
void Update_Speed(void) // �����̲��� mm/s
{
	FTM_Get_1 = -FTM_AB_Get(TIM4);  //��ȡ��������
	FTM_Get_2 = -FTM_AB_Get(TIM2);  //��ȡ��������
	FTM_Get_3 = -FTM_AB_Get(TIM3);  //��ȡ��������
	
    //  һȦ�� 3.14*78 mm  ��ʱ���� 10ms*100=1S    ת��Ϊm/s   MC /2048 * L * T = MC / 2048 * 3.14*78 * 100 = 11.96505
    FTM_Speed_1 = FTM_Get_1 * 11.96505; //
    FTM_Speed_2 = FTM_Get_2 * 11.96505; //
    FTM_Speed_3 = FTM_Get_3 * 11.96505; //
	
//	printf("FTM_Speed_1 %f \t",FTM_Speed_1);
//	printf("FTM_Speed_2 %f \t",FTM_Speed_2);
//	printf("FTM_Speed_3 %f \r\n",FTM_Speed_3);
	
}


/**
  * @brief  ��� PID ���� 
  * @retval None
*/
void DianJi_Speed(void) 
{	
	/*       PID   Ŀ��ֵ    ����ֵ	*/
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

	pid_t->Target = 0; 					//�趨Ŀ��ֵ
	pid_t->Measured = 0; 				//����ֵ
	pid_t->err = 0; 						//����ƫ��ֵ
	pid_t->err_last = 0; 				//��һ��ƫ��
	pid_t->integral_err = 0; 			//����ƫ��֮��
	
    pid_t->IntegralLimit = IntegralLimit;			//�����޷� 
	pid_t->MaxOutput = MaxOutput;				//����޷�
	
	pid_t->pwm_last = 0; 					//��һ��pwm���
	pid_t->pwm = 0; 						//pwm���
	
}

void abs_i(float *a, float ABS_MAX) //�����޷� 
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

void abs_pwm(int32_t *a, int32_t ABS_MAX) //����޷�
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

	//�����޷�
	abs_i(&pid_t->KiOut, pid_t->IntegralLimit);

	pid_t->pwm = (pid_t->KpOut + pid_t->KiOut + pid_t->KdOut);

	//����޷�
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

	//�����޷�
	abs_i(&pid_t->KiOut, pid_t->IntegralLimit);

	pid_t->pwm = (pid_t->KpOut + pid_t->KiOut + pid_t->KdOut);

	//����޷�
	abs_pwm(&pid_t->pwm, pid_t->MaxOutput);

	pid_t->pwm  = pid_t->pwm_last * 0.7 + pid_t->pwm * 0.3;
	
	pid_t->err_last = pid_t->err;
	pid_t->pwm_last = pid_t->pwm;
}

void PS2_FangXiang(void)
{
	/******************************************
					���
	*******************************************/
	if(PS2_AnologData(1) == 0x41) // ���򰴼�
	{
		/* ʹ��switch ռ�ÿռ�󣬵���Ч�ʸ� */
		switch(PS2_key)				  	
		{
			/* * * * * * * * * ���� * * * * * * * * * * */
			case 0 :			 // ����
			{
				Car_ctrl(0,0,0); // ���˶�_����
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 5 :			 	 // ��
			{
				Car_Speed(2*1000,90,0); // ��������ٶ� cm/s
				//Motor_1(-4200); // A
				break;
			}
			case 6 :			  	 // ��
			{
				Car_Speed(2*1000,0,0);  // ��������ٶ� cm/s
				//Motor_2(-4200); // B
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 7 :			  	 // ��
			{
				Car_Speed(2*1000,270,0); // ��������ٶ� cm/s
				break;
			}
			case 8 :			  	 // ��
			{
				Car_Speed(2*1000,180,0); // ��������ٶ� cm/s
				//Motor_3(-4200); // C
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			/* * * * * * * * * ��̨ * * * * * * * * * * */
			case 13 :			  	 // ��
			{
				duoji_jiaodu_y = duoji_jiaodu_y + 1;
				duoji(1,duoji_jiaodu_y,50);
				break;
			}
			case 14 :			  	 // ��
			{
				duoji_jiaodu_x = duoji_jiaodu_x + 1;			
				duoji(0,duoji_jiaodu_x,50);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 15 :			  	 // ��
			{
				duoji_jiaodu_y = duoji_jiaodu_y - 1;
				duoji(1,duoji_jiaodu_y,50);
				break;
			}
			case 16 :			  	 // ��
			{
				duoji_jiaodu_x = duoji_jiaodu_x - 1;
				duoji(0,duoji_jiaodu_x,50);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			/* * * * * * * * * ���Ӽ� * * * * * * * * * * */
			case 9 :			  	 // ��ͣ
			{
				Car_ctrl(0,0,0); // ���˶�_����
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 11 :			  	 // ������
			{
				Car_Speed(0,0,-200); // ��������ٶ� cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 10 :			  	 // �����λ
			{
				duoji_jiaodu_x = 0;
				duoji_jiaodu_y = 0;
				duoji(0,0,50); // ��λ
				os_dly_wait(10);
				duoji(1,0,50); // ��λ
				os_dly_wait(10);
			
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 12 :			  	 // ������
			{
				Car_Speed(0,0,200); // ��������ٶ� cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
		}
	}
	else if(PS2_AnologData(1) == 0x73)
	{
		PS2_YaoGan_DianJi();
		PS2_YaoGan_DuoJi();
		
		/*     ���Ӽ�       */
		/* ʹ��switch ռ�ÿռ�󣬵���Ч�ʸ� */
		switch(PS2_key)				  	
		{
			/* * * * * * * * * ���� * * * * * * * * * * */
			case 9 :			 // ��ͣ
			{
				Car_ctrl(0,0,0); // ���˶�_����
				TIM1->CCR1 = 9000;
				TIM1->CCR2 = 9000;
				TIM1->CCR3 = 9000;
				break;
			}
			case 11 :			 	 // ������
			{
				Car_Speed(0,0,-300); // ��������ٶ� cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 10 :			 	 // �����λ
			{
				duoji_jiaodu_x = 0;
				duoji_jiaodu_y = 0;
				duoji(0,0,50); // ��λ
				os_dly_wait(10);
				duoji(1,0,50); // ��λ
				os_dly_wait(10);
				
				DuoJi_BiaoZhi = 1;
				break;
			}
			case 12 :			 	 // ������
			{
				Car_Speed(0,0,300); // ��������ٶ� cm/s
				
				DuoJi_BiaoZhi = 1;
				break;
			}
		}	
	}
}

	double V_X = 0.0,V_Y = 0.0;
	double V = 0.0;
	float X_jiaodu = 0.0;
void PS2_YaoGan_DianJi(void) // ң�� ���
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
void PS2_YaoGan_DuoJi(void) // PS2 ң�� ���
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

void TuChuan_XY(int x,int y) // ͼ�� X Y ��
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

	if(y > 15) // ��
	{
		duoji_jiaodu_y = duoji_jiaodu_y + 1 + y*0.005;
		duoji(1,duoji_jiaodu_y,80);
		os_dly_wait(10);
	}
	else if(y < -15) // ��
	{
		duoji_jiaodu_y = duoji_jiaodu_y - 1 + y*0.005;
		duoji(1,duoji_jiaodu_y,80);
		os_dly_wait(10); 
	}
	
	if(x > 15) // ��
	{
		duoji_jiaodu_x = duoji_jiaodu_x + 1 + x*0.005;			
		duoji(0,duoji_jiaodu_x,80);
		os_dly_wait(10);
		
		DuoJi_BiaoZhi = 1;
	}
	else if(x < -15) // ��
	{
		duoji_jiaodu_x = duoji_jiaodu_x - 1 + x*0.005;
		duoji(0,duoji_jiaodu_x,80);
		os_dly_wait(10);
		
		DuoJi_BiaoZhi = 1;
	}
	
#endif

}

/*************************************************************
          ��ڲ����ֱ��ǣ�X���ٶȡ�Y���ٶ�
*************************************************************/
void YaoGan_Car_Speed(float Vx,float Vy) // ��������ٶ� mm/s
{
	V1_speed =  + 2.0/3.0*Vx;         
	V2_speed =  - 1.0/3.0*Vx - sqrt(3.0)*Vy/3.0;
	V3_speed =  - 1.0/3.0*Vx + sqrt(3.0)*Vy/3.0;  //������������ٶ�
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
void Motor_gpio(void) // ����H�ſ���
{
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_2,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_4,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_5,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_6,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_7,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_NOPULL); 
}

/* PWM = -16800----16800 */
void Motor_Set1(int32_t PWM) // ���ǷϿ�����
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

