/*
*********************************************************************************************************
*
*	ģ������ : ����������ģ��
*	�ļ����� : BSP_TIM_ENCORDE.c
*	��    �� : V1.0
*	˵    �� : 
*
*********************************************************************************************************
*/
#include "includes.h"

/* ������1ʹ�ö�ʱ��2��GPIOΪ A_PA15  B_PB3  ,���ڵ��1���� *///(����ͨ��)
/* ������2ʹ�ö�ʱ��4��GPIOΪ A_PD12  B_PD13 ,���ڵ��2���� *///(����ͨ��)
/* ������3ʹ�ö�ʱ��3��GPIOΪ A_PB4   B_PB5  ,���ڵ��3���� *///(����ͨ��)

/*
*********************************************************************************************************
*	�� �� ��: TIM2_EncoderInit
*	����˵��: ��TIM2��ʼ��Ϊ�������ӿ�ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_EncoderInit(void)
{
	/* ��ʼ����ؽṹ�� */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//����ʱ���ṹ�� 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//�������벶��ṹ�� 
	GPIO_InitTypeDef	GPIO_InitStructure;			//����GPIO��ʼ���ṹ��
	NVIC_InitTypeDef 	NVIC_InitStructure;			//����NVIC��ʼ���ṹ��
	/* ʹ����Ӧ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE) ;//ʹ�ܶ�ʱ��2��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ��GPIOA��ʱ��,PA15 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��GPIOB��ʱ��,PB3
	/* ���ñ�������A/B���Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);			//PA15 ����ΪTIM2_CH1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);			//PB3  ����ΪTIM2_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//Ԥ��Ƶ��,����Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//�������Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//�����������TIM1��TIM8����
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);					//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);				//���и����ж�
	
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: TIM4_EncoderInit
*	����˵��: ��TIM4��ʼ��Ϊ�������ӿ�ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM4_EncoderInit(void)
{
	/* ��ʼ����ؽṹ�� */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//����ʱ���ṹ�� 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//�������벶��ṹ�� 
	GPIO_InitTypeDef	GPIO_InitStructure;			//����GPIO��ʼ���ṹ��
	NVIC_InitTypeDef 	NVIC_InitStructure;			//����NVIC��ʼ���ṹ��
	/* ʹ����Ӧ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE) ;//ʹ�ܶ�ʱ��2��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//ʹ��GPIOD��ʱ��,PD12 ,PD13
	/* ���ñ�������A/B���Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);			//PD12  ����ΪTIM4_CH1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);			//PD13  ����ΪTIM4_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//Ԥ��Ƶ��,����Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//�������Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//�����������TIM1��TIM8����
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);					//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);				//���и����ж�
	
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*
*********************************************************************************************************
*	�� �� ��: TIM3_EncoderInit
*	����˵��: ��TIM3��ʼ��Ϊ�������ӿ�ģʽ��PB4 PB5��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM3_EncoderInit(void)
{
	/* ��ʼ����ؽṹ�� */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//����ʱ���ṹ�� 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//�������벶��ṹ�� 
	GPIO_InitTypeDef	GPIO_InitStructure;			//����GPIO��ʼ���ṹ��
	NVIC_InitTypeDef 	NVIC_InitStructure;			//����NVIC��ʼ���ṹ��
	/* ʹ����Ӧ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE) ;//ʹ�ܶ�ʱ��2��ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ��GPIOE��ʱ��,PE5 ,PE6
	/* ���ñ�������A/B���Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);			//PB4  ����ΪTIM3_CH1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);			//PB5  ����ΪTIM3_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//Ԥ��Ƶ��,����Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//�������Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//�����������TIM1��TIM8����
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);					//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);				//���и����ж�
	
	TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: TIM2_IRQHandler
*	����˵��: TIM2�жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

/*
*********************************************************************************************************
*	�� �� ��: TIM4_IRQHandler
*	����˵��: TIM4�жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}

/*
*********************************************************************************************************
*	�� �� ��: TIM9_IRQHandler
*	����˵��: TIM9�жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{    				   				     	    	
	}		
	
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

