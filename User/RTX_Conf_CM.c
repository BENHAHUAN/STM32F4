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
//	/* ʹ��PWR��BKPʱ�� */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

//	/* ����д��RTC�ͺ󱸼Ĵ��� */
//	PWR_BackupAccessCmd(ENABLE);

//	/* ��λ�󱸼Ĵ��� */
//	BKP_DeInit();

//	/* ʹ��LSI */
//	RCC_LSICmd(ENABLE);

//	/* �ȴ�ֱ��LSI���� */
//	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

//	/* ѡ��LSI��ΪRTC��ʱ�� */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

//	/* ʹ��RTCʱ�� */ 
//	RCC_RTCCLKCmd(ENABLE);

//	/*
//	   ��APB1���߸�λ����ֹͣ�����¿�����RTC���κζ�ȡǰ�õȴ�RTC�Ĵ���
//	   (RTC_CNT, RTC_ALR and RTC_PRL)��RTC APBʱ��ͬ����
//	*/
//	RTC_WaitForSynchro();

//	/* �ȴ�RTC�Ĵ���д������� */
//	RTC_WaitForLastTask();

//	/* 
//	  1. LSI��Ƶ�ʵ���ֵ��40KHz(30KHz��60KHz)
//	  2. ���ﰴ40KHz������
//	     RTC ���� = RTCCLK / RTC_PR = (40 KHz)/(19 + 1) = 2KHz
//	*/
//	RTC_SetPrescaler(19); 

//	/* �ȴ�RTC�Ĵ���д������� */
//	RTC_WaitForLastTask();

//	/* EXTI��17���ӵ�RTC�����¼���ʹ���ж� */
//	EXTI_ClearITPendingBit(EXTI_Line17); 
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

//	/* ���������жϵ�NVIC */
//	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	for (;;) 
//	{
//		/* ����OS, �����ؿ���ִ�е͹��ĵ�ʱ�� */
//		sleep = os_suspend (); 
//		
//		if (sleep) 
//		{
//			/* RTC�������� */
//			RTC_SetCounter(0);
//			RTC_WaitForLastTask();

//			/* ��������ʱ�� */
//			RTC_SetAlarm(sleep*(OS_TICK/1000)*2);
//			RTC_WaitForLastTask();
//			
//			/* ʹ�������ж� */
//			RTC_ITConfig(RTC_IT_ALR, ENABLE);
//			RTC_WaitForLastTask();
//			
//			/* ����ͣ��ģʽ */
//			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
//			
//			/* 
//			  1����һ���жϻ����¼������˳�ֹͣģʽʱ��HSI RC������ѡΪϵͳʱ�ӡ�
//			  2���˳��͹��ĵ�ͣ��ģʽ����Ҫ��������ʹ��HSE��HSE 
//			*/
//			SystemInit();
//			
//			/*
//   			   ��APB1���߸�λ����ֹͣ�����¿�����RTC���κζ�ȡǰ�õȴ�RTC�Ĵ���
//			   (RTC_CNT, RTC_ALR and RTC_PRL)��RTC APBʱ��ͬ����
//			*/
//			RTC_WaitForSynchro();
//			
//			/* ��黽�ѱ�־�Ƿ����� */
//			if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//			{
//				/* �û����������������ش��ڴ�ӡ�Ⱥ���������Ƿ����ͣ��ģʽ */
//				//printf("lowpower\r\n");
//				/* ������ѱ�־ */
//				PWR_ClearFlag(PWR_FLAG_WU);
//			}
//			else
//			{
//				sleep = 0;
//			}
//			
//		}
//		
//		/* �ָ�OS */
//		os_resume (sleep);            
//	}
//}

///*--------------------------- RTC�����ж� ----------------------------------*/
//void RTCAlarm_IRQHandler(void)
//{
//	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
//	{
//		/* ��ֹRTC�������ж� */
//		RTC_ITConfig(RTC_IT_ALR, DISABLE);
//		RTC_WaitForLastTask();
//		
//		/* ����жϱ�־ */
//		EXTI_ClearITPendingBit(EXTI_Line17);
//		
//		/* ����жϱ�־ */
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
		case 1: /* ����Ϊ 1 */
			//TimerID1=1;
			break;
		case 2: /* ����Ϊ 2 */
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
