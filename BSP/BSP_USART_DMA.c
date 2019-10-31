/*
*********************************************************************************************************
*
*	模块名称 : USART驱动模块
*	文件名称 : BSP_USART_DMA.c
*	版    本 : V1.0
*	说    明 : 
*
*
*********************************************************************************************************
*/

#include "includes.h"

usartRecv_t g_usartRecv_1 = {0, {0},{0}};             			//接收数据资源
#define  USART1_DMABuffAddr (uint32_t)g_usartRecv_1.recvBuf
#define  USART1_buffsize 	128

usartRecv_t g_usartRecv_2 = {0, {0},{0}};             			//接收数据资源
#define  USART2_DMABuffAddr (uint32_t)g_usartRecv_2.recvBuf
#define  USART2_buffsize 	128

usartRecv_t g_usartRecv_3 = {0, {0},{0}};             			//接收数据资源
#define  USART3_DMABuffAddr (uint32_t)g_usartRecv_3.recvBuf
#define  USART3_buffsize 	128

usartRecv_t g_usartRecv_6 = {0, {0},{0}};             			//接收数据资源
#define  USART6_DMABuffAddr (uint32_t)g_usartRecv_6.recvBuf
#define  USART6_buffsize 	128

usartRecv_t g_usartRecv_4 = {0, {0},{0}};             			//接收数据资源
#define  UART4_DMABuffAddr (uint32_t)g_usartRecv_4.recvBuf
#define  UART4_buffsize 	128

usartRecv_t g_usartRecv_5 = {0, {0},{0}};             			//接收数据资源
#define  UART5_DMABuffAddr (uint32_t)g_usartRecv_5.recvBuf
#define  UART5_buffsize 	128

/* 串口1的GPIO为 TX_PA9  RX_PA10,用于 *///TX/RX均支持DMA(测试通过)
/* 串口2的GPIO为 TX_PD5  RX_PD6 ,用于 *///仅RX支持DMA	(测试通过)
/* 串口3的GPIO为 TX_PB10 RX_PB11,用于 *///TX/RX均支持DMA(测试通过)
/* 串口4的GPIO为 TX_PC10 RX_PC11,用于 *///仅RX支持DMA	(测试通过)
/* 串口5的GPIO为 TX_PC12 RX_PD2 ,用于 *///TX/RX均支持DMA(测试通过)
/* 串口6的GPIO为 TX_PC6  RX_PC7 ,用于 *///TX/RX均支持DMA(测试通过)
/* 串口7的GPIO为 TX_PE8  RX_PE7 ,用于 *///仅RX支持DMA
/* 串口8的GPIO为 TX_PE1  RX_PE0 ,用于 *///仅RX支持DMA

static void USART_Config(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority);
static void USART_DMATxConfig(USART_TypeDef* USARTx);
static void USART_DMARxConfig(USART_TypeDef* USARTx,uint32_t DMABuffAddr, uint32_t buffsize);

/*
*********************************************************************************************************
*	函 数 名: Usart1_DriverInit
*	功能说明: 串口一硬件初始化，支持DMA接收和发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Usart1_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART1);
	/* 使能GPIO时钟和USART1时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* 配置USART1的TX/RX对应的GPIO */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);			//PA9 复用为USART1_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);		//PA10复用为USART1_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9与GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOA,&GPIO_InitStructure); 							//初始化PA9，PA10  
	
	USART_Config(USART1, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART1);
	USART_DMARxConfig(USART1,USART1_DMABuffAddr, USART1_buffsize);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		
}

/*
*********************************************************************************************************
*	函 数 名: Usart2_DriverInit
*	功能说明: 串口二硬件初始化，支持DMA接收，不支持DMA发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Usart2_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART2);
	/* 使能GPIO时钟和USART1时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* 配置USART2的TX/RX对应的GPIO */
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);			//PD5 复用为USART2_TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);			//PD6 复用为USART2_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 			//GPIOA9与GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOD,&GPIO_InitStructure); 							//初始化PD5，PD6  
	
	USART_Config(USART2, BuadRate, preemptionPriority, subPriority);
	//USART_DMATxConfig(USART2);
	USART_DMARxConfig(USART2,USART2_DMABuffAddr, USART2_buffsize);
	//USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		
}

/*
*********************************************************************************************************
*	函 数 名: Usart3_DriverInit
*	功能说明: 串口3硬件初始化，支持DMA接收，支持DMA发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Usart3_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART3);
	/* 使能GPIO时钟和USART3时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* 配置USART2的TX/RX对应的GPIO */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);			//PB10 复用为USART3_TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);			//PB11 复用为USART3_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 		//GPIOA9与GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOB,&GPIO_InitStructure); 							//初始化PB10，PB11  
	
	USART_Config(USART3, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART3);
	USART_DMARxConfig(USART3,USART3_DMABuffAddr, USART3_buffsize);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
}

/*
*********************************************************************************************************
*	函 数 名: Usart6_DriverInit
*	功能说明: 串口6硬件初始化，支持DMA接收，支持DMA发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Usart6_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART6);
	/* 使能GPIO时钟和USART3时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	/* 配置USART2的TX/RX对应的GPIO */
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);			//PC6 复用为USART3_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);			//PC7 复用为USART3_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 		//GPIOA9与GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 							//初始化PB10，PB11  
	
	USART_Config(USART6, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART6);
	USART_DMARxConfig(USART6,USART6_DMABuffAddr, USART6_buffsize);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: Uart4_DriverInit
*	功能说明: 串口4硬件初始化，支持DMA接收，不支持DMA发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart4_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(UART4);
	/* 使能GPIO时钟和USART3时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* 配置USART2的TX/RX对应的GPIO */
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);			//PC10 复用为UART4_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);			//PC11 复用为UART4_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 							//初始化PB10，PB11  
	
	USART_Config(UART4, BuadRate, preemptionPriority, subPriority);
	//USART_DMATxConfig(UART4);
	USART_DMARxConfig(UART4,UART4_DMABuffAddr, UART4_buffsize);
	//USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: Uart5_DriverInit
*	功能说明: 串口5硬件初始化，支持DMA接收，支持DMA发送
*	形    参: 波特率 中断优先级 子优先级
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart5_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(UART5);
	/* 使能GPIO时钟和UART5时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
	/* 配置UART5的TX/RX对应的GPIO *///TX_PC12 RX_PD2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);			//PC10 复用为UART5_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);			//PC11 复用为UART5_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//复用功能  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //速度50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //推挽复用输出  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//上拉  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	
	//初始化PB10，PB11  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 	
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
	
	USART_Config(UART5, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(UART5);
	USART_DMARxConfig(UART5,UART5_DMABuffAddr, UART5_buffsize);
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);
}
void Uart7_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	USART_DeInit(UART7);
}
void Uart8_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	USART_DeInit(UART8);
}

/*
*********************************************************************************************************
*	函 数 名: USART_Config
*	功能说明: 串口参数配置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void USART_Config(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority)
{
	
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	//USART1 初始化设置  
    USART_InitStructure.USART_BaudRate   = baudRate;				//波特率设置  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式  
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;		//一个停止位  
    USART_InitStructure.USART_Parity     = USART_Parity_No;			//无奇偶校验位  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制  
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; //收发模式  
    USART_Init(USARTx, &USART_InitStructure); 						//初始化串口1  
  
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);					//开启相关中断  
    USART_Cmd(USARTx, ENABLE);  									//使能串口1   
  
    USART_ClearFlag(USARTx, USART_FLAG_TC);  
       
    //Usart1 NVIC 配置  
	if(USARTx == USART1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    }
	if(USARTx == USART2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    }
	if(USARTx == USART3)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    }
	if(USARTx == USART6)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    }
	if(USARTx == UART4)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    }
	if(USARTx == UART5)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    }
//	if(USARTx == UART7)
//    {
//        NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
//    }
//	if(USARTx == UART8)
//    {
//        NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
//    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preemptionPriority;	//抢占优先级  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		=subPriority;       	//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd 				= ENABLE;        		//IRQ通道使能  
    NVIC_Init(&NVIC_InitStructure); 											//根据指定的参数初始化VIC寄存器、 
}
/*
*********************************************************************************************************
*	函 数 名: USART_dmaTxConfig
*	功能说明: 串口发送使用的DMA配置，注意必须要更新内存地址和传输大小之后才能使能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void USART_DMATxConfig(USART_TypeDef* USARTx)
{
	DMA_InitTypeDef     DMA_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能  
	
	if(USARTx == USART1)
	{
		DMA_DeInit(DMA2_Stream7);	
		while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);							//等待DMA可配置
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;			//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_1.SendBuf; //DMA 存储器0地址		
	}
	
	if(USARTx == USART3)
	{
		DMA_DeInit(DMA1_Stream4);	
		while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);							//等待DMA可配置
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;			//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_3.SendBuf; //DMA 存储器0地址		
	}
	if(USARTx == USART6)
	{
		DMA_DeInit(DMA2_Stream6);	
		while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);							//等待DMA可配置
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;								//通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;			//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_6.SendBuf; //DMA 存储器0地址		
	}
	if(USARTx == UART5)
	{
		DMA_DeInit(DMA1_Stream7);	
		while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);							//等待DMA可配置
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//通道选择
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;			//DMA外设地址
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_5.SendBuf; //DMA 存储器0地址		
	}
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式  
    DMA_InitStructure.DMA_BufferSize = SEND_BUF_SIZE;//数据传输量   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输 
	
	if(USARTx == USART1)
    {
		DMA_Init(DMA2_Stream7, &DMA_InitStructure);//初始化DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  
    }  
	if(USARTx == USART3)
    {
		DMA_Init(DMA1_Stream4, &DMA_InitStructure);//初始化DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;  
    }	
	if(USARTx == USART6)
    {
		DMA_Init(DMA2_Stream6, &DMA_InitStructure);//初始化DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;  
    }
	if(USARTx == UART5)
    {
		DMA_Init(DMA1_Stream7, &DMA_InitStructure);//初始化DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;  
    }
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
    NVIC_Init(&NVIC_InitStructure);    
	if(USARTx == USART1)
    {
		DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);  
    }  	
	if(USARTx == USART3)
    {
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);  
    } 
	if(USARTx == USART6)
    {
		DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);  
    } 	
	if(USARTx == UART5)
    {
		DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);  
    } 
}

/*
*********************************************************************************************************
*	函 数 名: USART_DMARxConfig
*	功能说明: 串口发送使用的DMA配置，注意必须要更新内存地址和传输大小之后才能使能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void USART_DMARxConfig(USART_TypeDef* USARTx,uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef     DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA2时钟使能   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能   
	if(USARTx == USART1)
	{
		DMA_DeInit(DMA2_Stream5);  
		while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  	
	}
	if(USARTx == USART2)
	{
		DMA_DeInit(DMA1_Stream5);  
		while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  
	}
	if(USARTx == USART3)
	{
		DMA_DeInit(DMA1_Stream1);  
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  
	}
	if(USARTx == USART6)
	{
		DMA_DeInit(DMA2_Stream1);  
		while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_5; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  
	}	
	if(USARTx == UART4)
	{
		DMA_DeInit(DMA1_Stream2);  
		while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  
	}
	if(USARTx == UART5)
	{
		DMA_DeInit(DMA1_Stream0);  
		while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);				 //等待DMA可配置   
		/* 配置 DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //通道选择  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;//DMA外设地址  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA 存储器0地址  
	}
	DMA_InitStructure.DMA_DIR			  = DMA_DIR_PeripheralToMemory ;//外设到存储器模式  
    DMA_InitStructure.DMA_BufferSize 	  = buffsize;					//数据传输量   
    DMA_InitStructure.DMA_PeripheralInc   = DMA_PeripheralInc_Disable;	//外设非增量模式  
    DMA_InitStructure.DMA_MemoryInc		  = DMA_MemoryInc_Enable;		//存储器增量模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位  
    DMA_InitStructure.DMA_MemoryDataSize  = DMA_MemoryDataSize_Byte;	//存储器数据长度:8位  
    DMA_InitStructure.DMA_Mode 			  = DMA_Mode_Normal;			// 使用普通模式   
    DMA_InitStructure.DMA_Priority 		  = DMA_Priority_Medium;		//中等优先级  
    DMA_InitStructure.DMA_FIFOMode		  = DMA_FIFOMode_Disable;           
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_Full;  
    DMA_InitStructure.DMA_MemoryBurst 	  = DMA_MemoryBurst_Single;		//存储器突发单次传输  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//外设突发单次传输  
      
  //  DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);  
	
	if(USARTx == USART1)
    {
		DMA_Init(DMA2_Stream5, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA2_Stream5, ENABLE);  //开启DMA传输  
    } 
	
	if(USARTx == USART2)
    {
		DMA_Init(DMA1_Stream5, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA1_Stream5, ENABLE);  //开启DMA传输    
    }  
	
	if(USARTx == USART3)
    {
		DMA_Init(DMA1_Stream1, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA1_Stream1, ENABLE);  //开启DMA传输    
    }  
	
	if(USARTx == USART6)
    {
		DMA_Init(DMA2_Stream1, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA2_Stream1, ENABLE);  //开启DMA传输    
    } 
	if(USARTx == UART4)
    {
		DMA_Init(DMA1_Stream2, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA1_Stream2, ENABLE);  //开启DMA传输    
    } 
	if(USARTx == UART5)
    {
		DMA_Init(DMA1_Stream0, &DMA_InitStructure);//初始化DMA Stream  
		DMA_Cmd(DMA1_Stream0, ENABLE);  //开启DMA传输    
    } 
}

/*
*********************************************************************************************************
*	函 数 名: DmaSendDataProc
*	功能说明: 开始一次DMA传输
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)  
{  
   
    DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输   
      
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}  //确保DMA可以被设置    
          
    DMA_Streamx->NDTR = ndtr;          //数据传输量    
   
    DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输   
}  

/*
*********************************************************************************************************
*	函 数 名: USART_sendByte
*	功能说明: 异步串口发送一个字节的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    USART_SendData(USARTx, byte);
    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

//串口1  
//发送单字节  
 void SendByteInfoProc(USART_TypeDef* USARTx,u8 nSendInfo)  
{  
    u8 *pBuf = NULL;  
    //指向发送缓冲区  
	if(USARTx == USART1)
	{
		pBuf = g_usartRecv_1.SendBuf;  
		*pBuf++ = nSendInfo;  
		DmaSendDataProc(DMA2_Stream7,1); //开始一次DMA传输！  	
	}    	
	
}  
//发送多字节  
void SendBytesInfoProc(USART_TypeDef* USARTx,u8* pSendInfo, u16 nSendCount)  
{  
    u16 i = 0;  
    u8 *pBuf = NULL;  
    //指向发送缓冲区  
	if(USARTx == USART1)
	{	
	pBuf = g_usartRecv_1.SendBuf;    
    for (i=0; i<nSendCount; i++)  
    {  
        *pBuf++ = pSendInfo[i];  
    }  
    //DMA发送方式  
    DmaSendDataProc(DMA2_Stream7,nSendCount); //开始一次DMA传输！  	
	}    
	  
} 


void USART1_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA2_Stream5, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = USART1->SR;  
        data = USART1->DR;  
          
        g_usartRecv_1.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);  
        if(g_usartRecv_1.dataCount !=0)  
        {  
		    //isr_sem_send(&g_usartRecvSem_1);
			//printf("%d",g_usartRecv_1.dataCount);
			//printf("%s\r\n",g_usartRecv_1.recvBuf);
            //OSSemPost(DMAReceiveSize_Sem);  
        }  
        DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志  
        DMA_SetCurrDataCounter(DMA2_Stream5, RECV_BUF_SIZE);  
        DMA_Cmd(DMA2_Stream5, ENABLE);     //打开DMA,  
  
    }  
} 
void USART2_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream5, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = USART2->SR;  
        data = USART2->DR;  
          
        g_usartRecv_2.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Stream5);  
        if(g_usartRecv_2.dataCount !=0)  
        {  
		    //isr_sem_send(&g_usartRecvSem_1);
			//printf("%d",g_usartRecv_1.dataCount);
			//printf("%s\r\n",g_usartRecv_1.recvBuf);
            //OSSemPost(DMAReceiveSize_Sem);  
        }  
        DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//清除DMA2_Steam7传输完成标志  
        DMA_SetCurrDataCounter(DMA1_Stream5, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream5, ENABLE);     //打开DMA,  
  
    }  
} 

void USART3_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream1, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = USART3->SR;  
        data = USART3->DR;  
          
        g_usartRecv_3.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Stream1);  
        if(g_usartRecv_3.dataCount !=0)  
        {  
		    //isr_sem_send(&g_usartRecvSem_1);
			//printf("%d",g_usartRecv_1.dataCount);
			//printf("%s\r\n",g_usartRecv_1.recvBuf);
            //OSSemPost(DMAReceiveSize_Sem);  
        }  
        DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//清除DMA2_Steam1传输完成标志  
        DMA_SetCurrDataCounter(DMA1_Stream1, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream1, ENABLE);     //打开DMA,  
  
    }  
} 

void USART6_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART6,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA2_Stream1, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = USART6->SR;  
        data = USART6->DR;  
          
        g_usartRecv_6.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream1);  
        if(g_usartRecv_6.dataCount !=0)  
        {  
		    //isr_sem_send(&g_usartRecvSem_1);
			//printf("%d",g_usartRecv_1.dataCount);
			//printf("%s\r\n",g_usartRecv_1.recvBuf);
            //OSSemPost(DMAReceiveSize_Sem);  
        }  
        DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//清除DMA2_Steam1传输完成标志  
        DMA_SetCurrDataCounter(DMA2_Stream1, RECV_BUF_SIZE);  
        DMA_Cmd(DMA2_Stream1, ENABLE);     //打开DMA,  
  
    }  
} 

void UART4_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(UART4,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream2, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = UART4->SR;  
        data = UART4->DR;  
          
        g_usartRecv_4.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Stream2);  
        if(g_usartRecv_4.dataCount !=0)  
        {  
			if (isr_mbx_check (&PTZ_Pose_Box) != 0)
			{
				isr_mbx_send (&PTZ_Pose_Box, &g_usartRecv_4.recvBuf);		
			} 
        }  
        DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2);//清除DMA2_Steam1传输完成标志  
        DMA_SetCurrDataCounter(DMA1_Stream2, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream2, ENABLE);     //打开DMA,  
  
    }  
} 
void UART5_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(UART5,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream0, DISABLE); //关闭DMA,防止处理其间有数据  
  
        data = UART5->SR;  
        data = UART5->DR;  
          
        g_usartRecv_5.dataCount =RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Stream0);  
        if(g_usartRecv_5.dataCount !=0)  
        {  	
			if (isr_mbx_check (&Chassis_Pose_Box) != 0)
			{
				isr_mbx_send (&Chassis_Pose_Box, &g_usartRecv_5.recvBuf);		
			}
        }  
        DMA_ClearFlag(DMA1_Stream0,DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_HTIF0);//清除DMA2_Steam1传输完成标志  
        DMA_SetCurrDataCounter(DMA1_Stream0, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream0, ENABLE);     //打开DMA,  
  
    }  
} 
/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	USART_sendByte(USART1, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

