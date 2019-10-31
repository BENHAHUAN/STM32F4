/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_CONFIG.C
 *      Purpose: Configuration of RTX Kernel for Cortex-M
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "includes.h"
/*----------------------------------------------------------------------------
 *      RTX User configuration part BEGIN
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>Task Configuration
// =====================
//
//   <o>Number of concurrent running tasks <0-250>
//   <i> Define max. number of tasks that will run at the same time.
//   <i> Default: 6
#ifndef OS_TASKCNT
 #define OS_TASKCNT    8
#endif

//   <o>Number of tasks with user-provided stack <0-250>
//   <i> Define the number of tasks that will use a bigger stack.
//   <i> The memory space for the stack is provided by the user.
//   <i> Default: 0
#ifndef OS_PRIVCNT
 #define OS_PRIVCNT     6
#endif

//   <o>Task stack size [bytes] <20-4096:8><#/4>
//   <i> Set the stack size for tasks which is assigned by the system.
//   <i> Default: 512
#ifndef OS_STKSIZE
 #define OS_STKSIZE     512
#endif

// <q>Check for the stack overflow
// ===============================
// <i> Include the stack checking code for a stack overflow.
// <i> Note that additional code reduces the Kernel performance.
#ifndef OS_STKCHECK
 #define OS_STKCHECK    1
#endif

// <q>Run in privileged mode
// =========================
// <i> Run all Tasks in privileged mode.
// <i> Default: Unprivileged
#ifndef OS_RUNPRIV
 #define OS_RUNPRIV     1
#endif

// </h>
// <h>Tick Timer Configuration
// =============================
//   <o>Hardware timer <0=> Core SysTick <1=> Peripheral Timer
//   <i> Define the on-chip timer used as a time-base for RTX.
//   <i> Default: Core SysTick
#ifndef OS_TIMER
 #define OS_TIMER       0
#endif

//   <o>Timer clock value [Hz] <1-1000000000>
//   <i> Set the timer clock value for selected timer.
//   <i> Default: 6000000  (6MHz)
#ifndef OS_CLOCK
 #define OS_CLOCK       180000000
#endif

//   <o>Timer tick value [us] <1-1000000>
//   <i> Set the timer tick value for selected timer.
//   <i> Default: 10000  (10ms)
#ifndef OS_TICK
 #define OS_TICK        1000
#endif

// </h>

// <h>System Configuration
// =======================
// <e>Round-Robin Task switching
// =============================
// <i> Enable Round-Robin Task switching.
#ifndef OS_ROBIN
 #define OS_ROBIN       0
#endif

//   <o>Round-Robin Timeout [ticks] <1-1000>
//   <i> Define how long a task will execute before a task switch.
//   <i> Default: 5
#ifndef OS_ROBINTOUT
 #define OS_ROBINTOUT   5
#endif

// </e>

//   <o>Number of user timers <0-250>
//   <i> Define max. number of user timers that will run at the same time.
//   <i> Default: 0  (User timers disabled)
#ifndef OS_TIMERCNT
 #define OS_TIMERCNT    5
#endif

//   <o>ISR FIFO Queue size<4=>   4 entries  <8=>   8 entries
//                         <12=> 12 entries  <16=> 16 entries
//                         <24=> 24 entries  <32=> 32 entries
//                         <48=> 48 entries  <64=> 64 entries
//                         <96=> 96 entries
//   <i> ISR functions store requests to this buffer,
//   <i> when they are called from the iterrupt handler.
//   <i> Default: 16 entries
#ifndef OS_FIFOSZ
 #define OS_FIFOSZ      16
#endif

// </h>

//------------- <<< end of configuration section >>> -----------------------

// Standard library system mutexes
// ===============================
//  Define max. number system mutexes that are used to protect 
//  the arm standard runtime library. For microlib they are not used.
#ifndef OS_MUTEXCNT
 #define OS_MUTEXCNT    8
#endif

/*----------------------------------------------------------------------------
 *      RTX User configuration part END
 *---------------------------------------------------------------------------*/

#define OS_TRV          ((U32)(((double)OS_CLOCK*(double)OS_TICK)/1E6)-1)

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/
extern volatile uint32_t  OSIdleCtr; 
/*--------------------------- os_idle_demon ---------------------------------*/

__task void os_idle_demon (void) 
{
	for (;;) {
		  __set_PRIMASK(1);
       OSIdleCtr++;
	  __set_PRIMASK(0);
	}
}
//__task void os_idle_demon (void) 
//{
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	uint32_t sleep;
//	
//	/* 使能PWR和BKP时钟 */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

//	/* 允许写入RTC和后备寄存器 */
//	PWR_BackupAccessCmd(ENABLE);

//	/* 复位后备寄存器 */
//	BKP_DeInit();

//	/* 使能LSI */
//	RCC_LSICmd(ENABLE);

//	/* 等待直到LSI就绪 */
//	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

//	/* 选择LSI作为RTC的时钟 */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

//	/* 使能RTC时钟 */ 
//	RCC_RTCCLKCmd(ENABLE);

//	/*
//	   在APB1总线复位或者停止后重新开启，RTC的任何读取前得等待RTC寄存器
//	   (RTC_CNT, RTC_ALR and RTC_PRL)跟RTC APB时钟同步。
//	*/
//	RTC_WaitForSynchro();

//	/* 等待RTC寄存器写操作完成 */
//	RTC_WaitForLastTask();

//	/* 
//	  1. LSI的频率典型值是40KHz(30KHz到60KHz)
//	  2. 这里按40KHz来计算
//	     RTC 周期 = RTCCLK / RTC_PR = (40 KHz)/(19 + 1) = 2KHz
//	*/
//	RTC_SetPrescaler(19); 

//	/* 等待RTC寄存器写操作完成 */
//	RTC_WaitForLastTask();

//	/* EXTI线17连接到RTC闹钟事件，使能中断 */
//	EXTI_ClearITPendingBit(EXTI_Line17); 
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

//	/* 设置闹钟中断的NVIC */
//	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	for (;;) 
//	{
//		/* 挂起OS, 并返回可以执行低功耗的时长 */
//		sleep = os_suspend (); 
//		
//		if (sleep) 
//		{
//			/* RTC计数设置 */
//			RTC_SetCounter(0);
//			RTC_WaitForLastTask();

//			/* 设置闹钟时间 */
//			RTC_SetAlarm(sleep*(OS_TICK/1000)*2);
//			RTC_WaitForLastTask();
//			
//			/* 使能闹钟中断 */
//			RTC_ITConfig(RTC_IT_ALR, ENABLE);
//			RTC_WaitForLastTask();
//			
//			/* 进入停机模式 */
//			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
//			
//			/* 
//			  1、当一个中断或唤醒事件导致退出停止模式时，HSI RC振荡器被选为系统时钟。
//			  2、退出低功耗的停机模式后，需要重新配置使用HSE和HSE 
//			*/
//			SystemInit();
//			
//			/*
//   			   在APB1总线复位或者停止后重新开启，RTC的任何读取前得等待RTC寄存器
//			   (RTC_CNT, RTC_ALR and RTC_PRL)跟RTC APB时钟同步。
//			*/
//			RTC_WaitForSynchro();
//			
//			/* 检查唤醒标志是否设置 */
//			if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//			{
//				/* 用户可以在这里加入相关串口打印等函数来检测是否进入停机模式 */
//				//printf("lowpower\r\n");
//				/* 清除唤醒标志 */
//				PWR_ClearFlag(PWR_FLAG_WU);
//			}
//			else
//			{
//				sleep = 0;
//			}
//			
//		}
//		
//		/* 恢复OS */
//		os_resume (sleep);            
//	}
//}

///*--------------------------- RTC闹钟中断 ----------------------------------*/
//void RTCAlarm_IRQHandler(void)
//{
//	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
//	{
//		/* 禁止RTC的闹钟中断 */
//		RTC_ITConfig(RTC_IT_ALR, DISABLE);
//		RTC_WaitForLastTask();
//		
//		/* 清除中断标志 */
//		EXTI_ClearITPendingBit(EXTI_Line17);
//		
//		/* 清除中断标志 */
//		RTC_ClearITPendingBit(RTC_IT_ALR);
//		RTC_WaitForLastTask();
//	}	
//}
/*--------------------------- os_tick_init ----------------------------------*/

#if (OS_TIMER != 0)
int os_tick_init (void) {
  /* Initialize hardware timer as system tick timer. */
  /* ... */
  return (-1);  /* Return IRQ number of timer (0..239) */
}
#endif

/*--------------------------- os_tick_irqack --------------------------------*/

#if (OS_TIMER != 0)
void os_tick_irqack (void) {
  /* Acknowledge timer interrupt. */
  /* ... */
}
#endif

/*--------------------------- os_tmr_call -----------------------------------*/
void os_tmr_call (U16 info) {
  /* This function is called when the user timer has expired. Parameter   */
  /* 'info' holds the value, defined when the timer was created.          */
	switch(info)
	{
		case 1: /* 参数为 1 */
			//TimerID1=1;
			break;
		case 2: /* 参数为 2 */
			//bsp_LedToggle(1);
			break;
		}
  /* HERE: include optional user code to be executed on timeout. */
}


/*--------------------------- os_error --------------------------------------*/

void os_error (U32 err_code) {
  /* This function is called when a runtime error is detected. Parameter */
  /* 'err_code' holds the runtime error code (defined in RTL.H).         */

  /* HERE: include optional code to be executed on runtime error. */
  for (;;);
}


/*----------------------------------------------------------------------------
 *      RTX Configuration Functions
 *---------------------------------------------------------------------------*/

#include "RTX_lib.c"

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
