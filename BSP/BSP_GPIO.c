#include "BSP.h"

/**
* @brief  封装GPIO初始化的相关参数
* @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
*			GPIOx: where x can be (A..G) to select the GPIO peripheral.	
* 		@ref GPIO_pins_define
*			@ref GPIOMode_TypeDef
* @retval  None
*/
void GPIO_Config(uint32_t RCC_AHB1Periph ,
				GPIO_TypeDef* GPIOx,
				uint32_t GPIO_Pin,
				GPIOMode_TypeDef GPIO_Mode,
				GPIOOType_TypeDef GPIO_OType,
				GPIOPuPd_TypeDef GPIO_PuPd)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode ;
	GPIO_InitStructure.GPIO_OType = GPIO_OType;		//推挽/开漏
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd ;		//上下拉
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOx,GPIO_Pin);  //PC13输出低电平
}

