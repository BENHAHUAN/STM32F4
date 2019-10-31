/*
*********************************************************************************************************
*
*	模块名称 : 底层驱动模块
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。
*	 	       应用程序只需 #include bsp.h 即可，不需要#include 每个模块的 h 文件
*
*	修改记录 :
*		版本号  日期         作者    说明
*		v1.0    2012-12-17  Eric2013  ST固件库V1.0.2版本。
*	
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif



/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "BSP_GPIO.h"
#include "BSP_USART_DMA.h"
#include "BSP_DWT.h"
#include "BSP_TIM_ENCORDE.h"
#include "BSP_TIM_PWM.h"
#include "BSP_SPI_HW.h"
#include "BSP_ADC.h"
void bsp_Idle(void);
void bsp_Init(void);
static void NVIC_Configuration(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
