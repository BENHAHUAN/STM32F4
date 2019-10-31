#ifndef _LED_H_
#define _LED_H_
#include "sys.h"
#define KEY1 PBin(6)
#define KEY2 PBin(7)

/* À¶µÆ */
#define LED_BLUE_ON()		 GPIOE->BSRRH = GPIO_Pin_2
#define LED_BLUE_OFF()	 	 GPIOE->BSRRL = GPIO_Pin_2
#define LED_BLUE_TOGGLE()	 GPIOE->ODR	 ^= GPIO_Pin_2
/* °×µÆ */
#define LED_WHITE_ON()		 GPIOE->BSRRH = GPIO_Pin_3
#define LED_WHITE_OFF()	 	 GPIOE->BSRRL = GPIO_Pin_3
#define LED_WHITE_TOGGLE()	 GPIOE->ODR	 ^= GPIO_Pin_3
/* Ç¹ */
#define QIANG_ON()			 GPIOD->BSRRL = GPIO_Pin_9
#define QIANG_OFF()	 	 	 GPIOD->BSRRH = GPIO_Pin_9
/* ¼¤¹â */
#define LWI_ON()			 GPIOD->BSRRL = GPIO_Pin_10
#define LWI_OFF()	 	 	 GPIOD->BSRRH = GPIO_Pin_10
/* ·äÃùÆ÷ */
#define BEEP_ON()			 GPIOD->BSRRL = GPIO_Pin_11
#define BEEP_OFF()	 	 	 GPIOD->BSRRH = GPIO_Pin_11

void IO_InitHard(void);
uint8_t KEY_Scan(void);
#endif
