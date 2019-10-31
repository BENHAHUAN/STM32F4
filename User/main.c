#include "includes.h"
/* 版本，2019,10,04 */

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
__task void AppTaskStart(void);
__task void  AppTaskStatistic (void);
void  OSStatInit (void);

/*
**********************************************************************************************************
									  任务栈和任务句柄
**********************************************************************************************************
*/
static uint64_t AppTaskStatStk[1024/8]   ;         /* 任务栈 */
static uint64_t AppTaskStartStk[512/8];  		   /* 任务铡�
� */

/* 任务句柄 */
OS_TID HandleTaskStat = NULL;


/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/
RCC_ClocksTypeDef RCC_Structure;
unsigned char rece_buf[32];		//2401

/* 统计任务使用 */
__IO uint8_t   OSStatRdy;        /* 统计任务就绪标志 */
__IO uint32_t  OSIdleCtr;        /* 空闲任务计数 */
__IO float     OSCPUUsage;       /* CPU百分比 */
uint32_t       OSIdleCtrMax;     /* 1秒内最大的空闲计数 */
uint32_t       OSIdleCtrRun;     /* 1秒内空闲任务当前计数 */

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
	/* 硬件初始化 */
	bsp_Init();
	bsp_DelayMS(2000);
	PS2_Init();			 //驱动端口初始化
	PS2_SetInit();
	nRF24L01_InitHard(); /* nRF24L01初始化 */
	bsp_DelayMS(100);
	NRF24L01_RT_Init();
	IO_InitHard();
	
	RCC_GetClocksFreq(&RCC_Structure);		//获取系统时钟
	
	/* 创建启动任务 */
	os_sys_init_user(AppTaskStart,			 	  /* 任务函数   */
					 10,						  /* 任务优先级，越大优先级越小 */
					 &AppTaskStartStk,		  	  /* 任务栈 	*/
					 sizeof(AppTaskStartStk));	  /* 任务栈大小，单位字节数     */
	
	while(1);
	
}


u8 PS2_yao = 0;
u8 buttons = 0;
int 	NFR_X = 0,
		NFR_Y = 0;
float 	test_NFR_X = 0.0,
		test_NFR_Y = 0.0;

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务，也是最高优先级任务，用于信号处理。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 10  
*********************************************************************************************************
*/

__task void AppTaskStart(void)
{
	/* 优先创建统计任务-----------------*/
	HandleTaskStat = os_tsk_create_user(AppTaskStatistic,        /* 任务函数 */ 
	                                    1,                       /* 任务优先级 */ 
					                    &AppTaskStatStk,         /* 任务栈 */
					                    sizeof(AppTaskStatStk)); /* 任务栈大小，单位字节数 */
	
	OSStatInit();
	UsartTaskCreate();
	ControlTaskCreate();
	
	duoji(0,0,1000); // 下舵机复位
	bsp_DelayMS(10);
	duoji(1,0,1000); // 上舵机复位
	bsp_DelayMS(10);
	
	DuoJi_BiaoZhi = 1;
	
	BEEP_OFF();
	
	while(1)
	{
		buttons = KEY_Scan();
		
		/* NRF2401数据接收 */
		if((GPIOC->IDR & GPIO_Pin_5) == 0)
		{
			if(nRF24L01_RxData(rece_buf)==0)
			{
				#if 0
				printf("%s",(rece_buf+sizeof(uint8_t)));
				
				printf("%d,%d\r\n",NFR_X,NFR_Y);
				#endif
				
				rece_buf[0] = '1';
				sscanf((char*)rece_buf,"1%d,%d",&NFR_X,&NFR_Y);
				
				//test_NFR_X = (float)NFR_X;
				//test_NFR_Y = (float)NFR_Y;
					
				if(NFR_X < 800 && NFR_Y < 800)
				{
					NFR_X = NFR_X - 320; 
					NFR_Y = 240 - NFR_Y;
					
					#if 0
					if(GenZhong == 0)
					{
						for(uint8_t i=1;i<3;i++)
						{
							if(NFR_Y > 1) // 上
							{
								duoji(1,i,80);
								os_dly_wait(10);
							}
							else if(NFR_Y < -1) // 下
							{
								duoji(1,-i,80);
								os_dly_wait(10); 
							}
							
							if(NFR_X > 1) // 右
							{
								duoji(0,i,80);
								os_dly_wait(10);
								
								DuoJi_BiaoZhi = 1;
							}
							else if(NFR_X < -1) // 左
							{
								duoji(0,-i,80);
								os_dly_wait(10);
								
								DuoJi_BiaoZhi = 1;
							}
						}
						
						GenZhong = 1;
					}
					
					if(NFR_X == 0 && NFR_Y == 0)
					{
						GenZhong = 0;
					}
					#endif
					
					TuChuan_XY(NFR_X,NFR_Y);					
				}
			}
		}

		/* PS2 数据接收 */
		PS2_ClearData();
		PS2_ReadData();	
		PS2_key = PS2_DataKey();
		
		#if 0
		printf(" Data[3] %d \t Data[4]: %d \t Data[5]: %d \t Data[6]: %d \t Data[7]: %d \t Data[8] : %d\r\n",Data[3],Data[4],Data[5],Data[6],Data[7],Data[8]);
		if(PS2_key != 0)
		{
			printf("PS2_key: %d \r\n", PS2_key);
		}
		#endif

		PS2_FangXiang();		
		os_dly_wait(20);

	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStatistic
*	功能说明: 统计任务，用于实现CPU利用率的统计。为了测试更加准确，可以开启注释调用的全局中断开关
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1 (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
void  OSStatInit (void)
{
	OSStatRdy = FALSE;
	
    os_dly_wait(2u);            /* 时钟同步 */
	
    //__disable_irq();
    OSIdleCtr    = 0uL;         /* 清空闲计数 */
	//__enable_irq();
	
    os_dly_wait(100);           /* 统计100ms内，最大空闲计数 */
	
   	//__disable_irq();
    OSIdleCtrMax = OSIdleCtr;   /* 保存最大空闲计数 */
    OSStatRdy    = TRUE;
	//__enable_irq();
}
__task void  AppTaskStatistic (void)
{
    while (OSStatRdy == FALSE) 
	{
        os_dly_wait(200);     /* 等待统计任务就绪 */
    }

    OSIdleCtrMax /= 100uL;
    if (OSIdleCtrMax == 0uL) 
	{
        OSCPUUsage = 0u;
    }
	
    //__disable_irq();
    OSIdleCtr = OSIdleCtrMax * 100uL;  /* 设置初始CPU利用率 0% */
	//__enable_irq();
	
    for (;;) 
	{
        //__disable_irq();
        OSIdleCtrRun = OSIdleCtr;    /* 获得100ms内空闲计数 */
        OSIdleCtr    = 0uL;          /* 复位空闲计数 */
       	//__enable_irq();            /* 计算100ms内的CPU利用率 */
        OSCPUUsage   = (100uL - (float)OSIdleCtrRun / OSIdleCtrMax);
		
        os_dly_wait(100);            /* 每100ms统计一次 */
    }
}
