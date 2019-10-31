#include "includes.h"
/* �汾��2019,10,04 */

/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
__task void AppTaskStart(void);
__task void  AppTaskStatistic (void);
void  OSStatInit (void);

/*
**********************************************************************************************************
									  ����ջ��������
**********************************************************************************************************
*/
static uint64_t AppTaskStatStk[1024/8]   ;         /* ����ջ */
static uint64_t AppTaskStartStk[512/8];  		   /* ����ա�
� */

/* ������ */
OS_TID HandleTaskStat = NULL;


/*
**********************************************************************************************************
											 ����
**********************************************************************************************************
*/
RCC_ClocksTypeDef RCC_Structure;
unsigned char rece_buf[32];		//2401

/* ͳ������ʹ�� */
__IO uint8_t   OSStatRdy;        /* ͳ�����������־ */
__IO uint32_t  OSIdleCtr;        /* ����������� */
__IO float     OSCPUUsage;       /* CPU�ٷֱ� */
uint32_t       OSIdleCtrMax;     /* 1�������Ŀ��м��� */
uint32_t       OSIdleCtrRun;     /* 1���ڿ�������ǰ���� */

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
	/* Ӳ����ʼ�� */
	bsp_Init();
	bsp_DelayMS(2000);
	PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();
	nRF24L01_InitHard(); /* nRF24L01��ʼ�� */
	bsp_DelayMS(100);
	NRF24L01_RT_Init();
	IO_InitHard();
	
	RCC_GetClocksFreq(&RCC_Structure);		//��ȡϵͳʱ��
	
	/* ������������ */
	os_sys_init_user(AppTaskStart,			 	  /* ������   */
					 10,						  /* �������ȼ���Խ�����ȼ�ԽС */
					 &AppTaskStartStk,		  	  /* ����ջ 	*/
					 sizeof(AppTaskStartStk));	  /* ����ջ��С����λ�ֽ���     */
	
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
*	�� �� ��: AppTaskStart
*	����˵��: ��������Ҳ��������ȼ����������źŴ���
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 10  
*********************************************************************************************************
*/

__task void AppTaskStart(void)
{
	/* ���ȴ���ͳ������-----------------*/
	HandleTaskStat = os_tsk_create_user(AppTaskStatistic,        /* ������ */ 
	                                    1,                       /* �������ȼ� */ 
					                    &AppTaskStatStk,         /* ����ջ */
					                    sizeof(AppTaskStatStk)); /* ����ջ��С����λ�ֽ��� */
	
	OSStatInit();
	UsartTaskCreate();
	ControlTaskCreate();
	
	duoji(0,0,1000); // �¶����λ
	bsp_DelayMS(10);
	duoji(1,0,1000); // �϶����λ
	bsp_DelayMS(10);
	
	DuoJi_BiaoZhi = 1;
	
	BEEP_OFF();
	
	while(1)
	{
		buttons = KEY_Scan();
		
		/* NRF2401���ݽ��� */
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
							if(NFR_Y > 1) // ��
							{
								duoji(1,i,80);
								os_dly_wait(10);
							}
							else if(NFR_Y < -1) // ��
							{
								duoji(1,-i,80);
								os_dly_wait(10); 
							}
							
							if(NFR_X > 1) // ��
							{
								duoji(0,i,80);
								os_dly_wait(10);
								
								DuoJi_BiaoZhi = 1;
							}
							else if(NFR_X < -1) // ��
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

		/* PS2 ���ݽ��� */
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
*	�� �� ��: AppTaskStatistic
*	����˵��: ͳ����������ʵ��CPU�����ʵ�ͳ�ơ�Ϊ�˲��Ը���׼ȷ�����Կ���ע�͵��õ�ȫ���жϿ���
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 1 (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
void  OSStatInit (void)
{
	OSStatRdy = FALSE;
	
    os_dly_wait(2u);            /* ʱ��ͬ�� */
	
    //__disable_irq();
    OSIdleCtr    = 0uL;         /* ����м��� */
	//__enable_irq();
	
    os_dly_wait(100);           /* ͳ��100ms�ڣ������м��� */
	
   	//__disable_irq();
    OSIdleCtrMax = OSIdleCtr;   /* ���������м��� */
    OSStatRdy    = TRUE;
	//__enable_irq();
}
__task void  AppTaskStatistic (void)
{
    while (OSStatRdy == FALSE) 
	{
        os_dly_wait(200);     /* �ȴ�ͳ��������� */
    }

    OSIdleCtrMax /= 100uL;
    if (OSIdleCtrMax == 0uL) 
	{
        OSCPUUsage = 0u;
    }
	
    //__disable_irq();
    OSIdleCtr = OSIdleCtrMax * 100uL;  /* ���ó�ʼCPU������ 0% */
	//__enable_irq();
	
    for (;;) 
	{
        //__disable_irq();
        OSIdleCtrRun = OSIdleCtr;    /* ���100ms�ڿ��м��� */
        OSIdleCtr    = 0uL;          /* ��λ���м��� */
       	//__enable_irq();            /* ����100ms�ڵ�CPU������ */
        OSCPUUsage   = (100uL - (float)OSIdleCtrRun / OSIdleCtrMax);
		
        os_dly_wait(100);            /* ÿ100msͳ��һ�� */
    }
}
