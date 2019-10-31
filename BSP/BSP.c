
#include "BSP.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为180MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
	*/
	/* 优先级分组设置为4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SystemCoreClockUpdate();	/* 根据PLL配置更新系统时钟频率变量 SystemCoreClock */
	bsp_InitDWT();
	TIM2_EncoderInit();
	TIM4_EncoderInit();
	TIM3_EncoderInit();
	bsp_InitSPIBus();
	BSP_TIM1_PWM(10000,5000);
	temperature_ADC_init();
	
}
