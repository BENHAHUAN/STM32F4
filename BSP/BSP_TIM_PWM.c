/*
*********************************************************************************************************
*
*	模块名称 : 电机PWM驱动模块
*	文件名称 : BSP_TIM_PWM.c
*	版    本 : V1.0
*	说    明 : 
*
*
*********************************************************************************************************
*/
#include "includes.h"
/*
	使用定时器1产生互补PWM
	TIM1_CH1:PE9 /TIM1_CH1N:PB13
	TIM1_CH2:PE11/TIM1_CH2N:PB0
	TIM1_CH3:PE13/TIM1_CH3N:PB1
*/

/*
取值范围 1 -- 8999
TIM1->CCR1 = ;
TIM1->CCR2 = ;
TIM1->CCR3 = ;
*/
void BSP_TIM1_PWM(uint32_t freq,uint32_t _ulDutyCycle)
{
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE);
	
	if(freq == 0)
	{
		TIM_Cmd(TIM1,DISABLE);	//关闭定时器输出
		return;
	}
	uiTIMxCLK = SystemCoreClock;
	usPrescaler = 0;			//不分频
	usPeriod = uiTIMxCLK/freq - 1;//自动重装载的值
	
	/* 配置定时器的通道1 2 3相对应的GPIO */
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1);//TIM1_CH1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9 ,GPIO_AF_TIM1);//TIM1_CH1N
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0 ,GPIO_AF_TIM1);//TIM1_CH2
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);//TIM1_CH2N
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1 ,GPIO_AF_TIM1);//TIM1_CH3
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);//TIM1_CH3N
	
	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period    		= usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler 		= usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision		= 0;
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X0000;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (_ulDutyCycle * usPeriod) / 10000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;		/* only for TIM1 and TIM8. */	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;			/* only for TIM1 and TIM8. */		
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;		/* only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;		/* only for TIM1 and TIM8. */
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	 
}



