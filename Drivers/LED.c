#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名: IO_InitHard
*	功能说明: LED配置、枪配置、按键配置
*	形    参：无
*	返 回 值: 无		                   			  
*********************************************************************************************************
*/
void IO_InitHard(void)
{
	/* 初始化LED */
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_2,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);		
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* 初始化枪 */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_9,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);
	/* 初始化激光 */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_10,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* 初始化蜂鸣器 */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_11,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* 初始化按键*/
	GPIO_Config(RCC_AHB1Periph_GPIOB ,GPIOB,GPIO_Pin_6,GPIO_Mode_IN,GPIO_OType_PP,GPIO_PuPd_UP);	
	GPIO_Config(RCC_AHB1Periph_GPIOB ,GPIOB,GPIO_Pin_6,GPIO_Mode_IN,GPIO_OType_PP,GPIO_PuPd_UP);	
}

/*
*********************************************************************************************************
*	函 数 名: KEY_Scan
*	功能说明: 按键扫描
*	形    参：无
*	返 回 值: 按键状态 0：无动作 1：单击 	                   			  
*********************************************************************************************************
*/
uint8_t KEY_Scan(void)
{
	static u8 flag_key1=1;//按键按松开标志
	static u8 flag_key2=1;//按键按松开标志
	//static u8 flag_key3=1;//按键按松开标志
	/* KEY1 */
	if(flag_key1&&KEY1==0)
	{
	flag_key1=0;
	return 1;	// 按键按下
	}
	else if(1==KEY1)			
		flag_key1=1;
	/* KEY2 */
	if(flag_key2&&KEY2==0)
	{
	flag_key2=0;
	return 2;	// 按键按下
	}
	else if(1==KEY2)			
		flag_key2=1;
	
	return 0;//无按键按下
}
