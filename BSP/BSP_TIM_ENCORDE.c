/*
*********************************************************************************************************
*
*	模块名称 : 编码器驱动模块
*	文件名称 : BSP_TIM_ENCORDE.c
*	版    本 : V1.0
*	说    明 : 
*
*********************************************************************************************************
*/
#include "includes.h"

/* 编码器1使用定时器2，GPIO为 A_PA15  B_PB3  ,用于电机1测速 *///(测试通过)
/* 编码器2使用定时器4，GPIO为 A_PD12  B_PD13 ,用于电机2测速 *///(测试通过)
/* 编码器3使用定时器3，GPIO为 A_PB4   B_PB5  ,用于电机3测速 *///(测试通过)

/*
*********************************************************************************************************
*	函 数 名: TIM2_EncoderInit
*	功能说明: 把TIM2初始化为编码器接口模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_EncoderInit(void)
{
	/* 初始化相关结构体 */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//配置时基结构体 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//配置输入捕获结构体 
	GPIO_InitTypeDef	GPIO_InitStructure;			//配置GPIO初始化结构体
	NVIC_InitTypeDef 	NVIC_InitStructure;			//配置NVIC初始化结构体
	/* 使能相应的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE) ;//使能定时器2的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//使能GPIOA的时钟,PA15 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能GPIOB的时钟,PB3
	/* 配置编码器的A/B相对应的GPIO */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);			//PA15 复用为TIM2_CH1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);			//PB3  复用为TIM2_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//预分频器,不分频
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//计数器自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//选择时钟分频，不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//这个参数仅对TIM1和TIM8有用
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);				//运行更新中断
	
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
*	函 数 名: TIM4_EncoderInit
*	功能说明: 把TIM4初始化为编码器接口模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM4_EncoderInit(void)
{
	/* 初始化相关结构体 */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//配置时基结构体 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//配置输入捕获结构体 
	GPIO_InitTypeDef	GPIO_InitStructure;			//配置GPIO初始化结构体
	NVIC_InitTypeDef 	NVIC_InitStructure;			//配置NVIC初始化结构体
	/* 使能相应的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE) ;//使能定时器2的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//使能GPIOD的时钟,PD12 ,PD13
	/* 配置编码器的A/B相对应的GPIO */
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);			//PD12  复用为TIM4_CH1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);			//PD13  复用为TIM4_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//预分频器,不分频
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//计数器自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//选择时钟分频，不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//这个参数仅对TIM1和TIM8有用
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);				//运行更新中断
	
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
*	函 数 名: TIM3_EncoderInit
*	功能说明: 把TIM3初始化为编码器接口模式（PB4 PB5）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM3_EncoderInit(void)
{
	/* 初始化相关结构体 */
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//配置时基结构体 
	TIM_ICInitTypeDef	TIM_ICInitStructure;		//配置输入捕获结构体 
	GPIO_InitTypeDef	GPIO_InitStructure;			//配置GPIO初始化结构体
	NVIC_InitTypeDef 	NVIC_InitStructure;			//配置NVIC初始化结构体
	/* 使能相应的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE) ;//使能定时器2的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能GPIOE的时钟,PE5 ,PE6
	/* 配置编码器的A/B相对应的GPIO */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);			//PB4  复用为TIM3_CH1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);			//PB5  复用为TIM3_CH2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period 	= 0XFFFF;				//预分频器,不分频
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;			//计数器自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//选择时钟分频，不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0X00;		//这个参数仅对TIM1和TIM8有用
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);					//将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);					//清除TIM的更新标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);				//运行更新中断
	
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
*	函 数 名: TIM2_IRQHandler
*	功能说明: TIM2中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/*
*********************************************************************************************************
*	函 数 名: TIM4_IRQHandler
*	功能说明: TIM4中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}

/*
*********************************************************************************************************
*	函 数 名: TIM9_IRQHandler
*	功能说明: TIM9中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{    				   				     	    	
	}		
	
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

