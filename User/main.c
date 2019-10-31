#include "includes.h"
/* ∞Ê±æ£¨2019,10,04 */

/*
**********************************************************************************************************
											∫Ø ˝…˘√˜
**********************************************************************************************************
*/
__task void AppTaskStart(void);
__task void  AppTaskStatistic (void);
void  OSStatInit (void);

/*
**********************************************************************************************************
									  »ŒŒÒ’ª∫Õ»ŒŒÒæ‰±˙
**********************************************************************************************************
*/
static uint64_t AppTaskStatStk[1024/8]   ;         /* »ŒŒÒ’ª */
static uint64_t AppTaskStartStk[512/8];  		   /* »ŒŒÒ’°¢
ª */

/* »ŒŒÒæ‰±˙ */
OS_TID HandleTaskStat = NULL;


/*
**********************************************************************************************************
											 ±‰¡ø
**********************************************************************************************************
*/
RCC_ClocksTypeDef RCC_Structure;
unsigned char rece_buf[32];		//2401

/* Õ≥º∆»ŒŒÒ π”√ */
__IO uint8_t   OSStatRdy;        /* Õ≥º∆»ŒŒÒæÕ–˜±Í÷æ */
__IO uint32_t  OSIdleCtr;        /* ø’œ–»ŒŒÒº∆ ˝ */
__IO float     OSCPUUsage;       /* CPU∞Ÿ∑÷±» */
uint32_t       OSIdleCtrMax;     /* 1√Îƒ⁄◊Ó¥Ûµƒø’œ–º∆ ˝ */
uint32_t       OSIdleCtrRun;     /* 1√Îƒ⁄ø’œ–»ŒŒÒµ±«∞º∆ ˝ */

/*
*********************************************************************************************************
*	∫Ø  ˝ √˚: main
*	π¶ƒ‹Àµ√˜: ±Í◊ºc≥Ã–Ú»Îø⁄°£
*	–Œ    ≤Œ: Œﬁ
*	∑µ ªÿ ÷µ: Œﬁ
*********************************************************************************************************
*/
int main(void)
{
	/* ”≤º˛≥ı ºªØ */
	bsp_Init();
	bsp_DelayMS(2000);
	PS2_Init();			 //«˝∂Ø∂Àø⁄≥ı ºªØ
	PS2_SetInit();
	nRF24L01_InitHard(); /* nRF24L01≥ı ºªØ */
	bsp_DelayMS(100);
	NRF24L01_RT_Init();
	IO_InitHard();
	
	RCC_GetClocksFreq(&RCC_Structure);		//ªÒ»°œµÕ≥ ±÷”
	
	/* ¥¥Ω®∆Ù∂Ø»ŒŒÒ */
	os_sys_init_user(AppTaskStart,			 	  /* »ŒŒÒ∫Ø ˝   */
					 10,						  /* »ŒŒÒ”≈œ»º∂£¨‘Ω¥Û”≈œ»º∂‘Ω–° */
					 &AppTaskStartStk,		  	  /* »ŒŒÒ’ª 	*/
					 sizeof(AppTaskStartStk));	  /* »ŒŒÒ’ª¥Û–°£¨µ•Œª◊÷Ω⁄ ˝     */
	
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
*	∫Ø  ˝ √˚: AppTaskStart
*	π¶ƒ‹Àµ√˜: ∆Ù∂Ø»ŒŒÒ£¨“≤ «◊Ó∏ﬂ”≈œ»º∂»ŒŒÒ£¨”√”⁄–≈∫≈¥¶¿Ì°£
*	–Œ    ≤Œ: Œﬁ
*	∑µ ªÿ ÷µ: Œﬁ
*   ”≈ œ» º∂: 10  
*********************************************************************************************************
*/

__task void AppTaskStart(void)
{
	/* ”≈œ»¥¥Ω®Õ≥º∆»ŒŒÒ-----------------*/
	HandleTaskStat = os_tsk_create_user(AppTaskStatistic,        /* »ŒŒÒ∫Ø ˝ */ 
	                                    1,                       /* »ŒŒÒ”≈œ»º∂ */ 
					                    &AppTaskStatStk,         /* »ŒŒÒ’ª */
					                    sizeof(AppTaskStatStk)); /* »ŒŒÒ’ª¥Û–°£¨µ•Œª◊÷Ω⁄ ˝ */
	
	OSStatInit();
	UsartTaskCreate();
	ControlTaskCreate();
	
	duoji(0,0,1000); // œ¬∂Êª˙∏¥Œª
	bsp_DelayMS(10);
	duoji(1,0,1000); // …œ∂Êª˙∏¥Œª
	bsp_DelayMS(10);
	
	DuoJi_BiaoZhi = 1;
	
	BEEP_OFF();
	
	while(1)
	{
		buttons = KEY_Scan();
		
		/* NRF2401 ˝æ›Ω” ’ */
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
							if(NFR_Y > 1) // …œ
							{
								duoji(1,i,80);
								os_dly_wait(10);
							}
							else if(NFR_Y < -1) // œ¬
							{
								duoji(1,-i,80);
								os_dly_wait(10); 
							}
							
							if(NFR_X > 1) // ”“
							{
								duoji(0,i,80);
								os_dly_wait(10);
								
								DuoJi_BiaoZhi = 1;
							}
							else if(NFR_X < -1) // ◊Û
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

		/* PS2  ˝æ›Ω” ’ */
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
*	∫Ø  ˝ √˚: AppTaskStatistic
*	π¶ƒ‹Àµ√˜: Õ≥º∆»ŒŒÒ£¨”√”⁄ µœ÷CPU¿˚”√¬ µƒÕ≥º∆°£Œ™¡À≤‚ ‘∏¸º”◊º»∑£¨ø…“‘ø™∆Ù◊¢ Õµ˜”√µƒ»´æ÷÷–∂œø™πÿ
*	–Œ    ≤Œ: Œﬁ
*	∑µ ªÿ ÷µ: Œﬁ
*   ”≈ œ» º∂: 1 ( ˝÷µ‘Ω–°”≈œ»º∂‘ΩµÕ£¨’‚∏ˆ∏˙uCOSœ‡∑¥)
*********************************************************************************************************
*/
void  OSStatInit (void)
{
	OSStatRdy = FALSE;
	
    os_dly_wait(2u);            /*  ±÷”Õ¨≤Ω */
	
    //__disable_irq();
    OSIdleCtr    = 0uL;         /* «Âø’œ–º∆ ˝ */
	//__enable_irq();
	
    os_dly_wait(100);           /* Õ≥º∆100msƒ⁄£¨◊Ó¥Ûø’œ–º∆ ˝ */
	
   	//__disable_irq();
    OSIdleCtrMax = OSIdleCtr;   /* ±£¥Ê◊Ó¥Ûø’œ–º∆ ˝ */
    OSStatRdy    = TRUE;
	//__enable_irq();
}
__task void  AppTaskStatistic (void)
{
    while (OSStatRdy == FALSE) 
	{
        os_dly_wait(200);     /* µ»¥˝Õ≥º∆»ŒŒÒæÕ–˜ */
    }

    OSIdleCtrMax /= 100uL;
    if (OSIdleCtrMax == 0uL) 
	{
        OSCPUUsage = 0u;
    }
	
    //__disable_irq();
    OSIdleCtr = OSIdleCtrMax * 100uL;  /* …Ë÷√≥ı ºCPU¿˚”√¬  0% */
	//__enable_irq();
	
    for (;;) 
	{
        //__disable_irq();
        OSIdleCtrRun = OSIdleCtr;    /* ªÒµ√100msƒ⁄ø’œ–º∆ ˝ */
        OSIdleCtr    = 0uL;          /* ∏¥Œªø’œ–º∆ ˝ */
       	//__enable_irq();            /* º∆À„100msƒ⁄µƒCPU¿˚”√¬  */
        OSCPUUsage   = (100uL - (float)OSIdleCtrRun / OSIdleCtrMax);
		
        os_dly_wait(100);            /* √ø100msÕ≥º∆“ª¥Œ */
    }
}
