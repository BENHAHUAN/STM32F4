/*
*********************************************************************************************************
*
*	ģ������ : USART����ģ��
*	�ļ����� : BSP_USART_DMA.c
*	��    �� : V1.0
*	˵    �� : 
*
*
*********************************************************************************************************
*/

#include "includes.h"

usartRecv_t g_usartRecv_1 = {0, {0},{0}};             			//����������Դ
#define  USART1_DMABuffAddr (uint32_t)g_usartRecv_1.recvBuf
#define  USART1_buffsize 	128

usartRecv_t g_usartRecv_2 = {0, {0},{0}};             			//����������Դ
#define  USART2_DMABuffAddr (uint32_t)g_usartRecv_2.recvBuf
#define  USART2_buffsize 	128

usartRecv_t g_usartRecv_3 = {0, {0},{0}};             			//����������Դ
#define  USART3_DMABuffAddr (uint32_t)g_usartRecv_3.recvBuf
#define  USART3_buffsize 	128

usartRecv_t g_usartRecv_6 = {0, {0},{0}};             			//����������Դ
#define  USART6_DMABuffAddr (uint32_t)g_usartRecv_6.recvBuf
#define  USART6_buffsize 	128

usartRecv_t g_usartRecv_4 = {0, {0},{0}};             			//����������Դ
#define  UART4_DMABuffAddr (uint32_t)g_usartRecv_4.recvBuf
#define  UART4_buffsize 	128

usartRecv_t g_usartRecv_5 = {0, {0},{0}};             			//����������Դ
#define  UART5_DMABuffAddr (uint32_t)g_usartRecv_5.recvBuf
#define  UART5_buffsize 	128

/* ����1��GPIOΪ TX_PA9  RX_PA10,���� *///TX/RX��֧��DMA(����ͨ��)
/* ����2��GPIOΪ TX_PD5  RX_PD6 ,���� *///��RX֧��DMA	(����ͨ��)
/* ����3��GPIOΪ TX_PB10 RX_PB11,���� *///TX/RX��֧��DMA(����ͨ��)
/* ����4��GPIOΪ TX_PC10 RX_PC11,���� *///��RX֧��DMA	(����ͨ��)
/* ����5��GPIOΪ TX_PC12 RX_PD2 ,���� *///TX/RX��֧��DMA(����ͨ��)
/* ����6��GPIOΪ TX_PC6  RX_PC7 ,���� *///TX/RX��֧��DMA(����ͨ��)
/* ����7��GPIOΪ TX_PE8  RX_PE7 ,���� *///��RX֧��DMA
/* ����8��GPIOΪ TX_PE1  RX_PE0 ,���� *///��RX֧��DMA

static void USART_Config(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority);
static void USART_DMATxConfig(USART_TypeDef* USARTx);
static void USART_DMARxConfig(USART_TypeDef* USARTx,uint32_t DMABuffAddr, uint32_t buffsize);

/*
*********************************************************************************************************
*	�� �� ��: Usart1_DriverInit
*	����˵��: ����һӲ����ʼ����֧��DMA���պͷ���
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Usart1_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART1);
	/* ʹ��GPIOʱ�Ӻ�USART1ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* ����USART1��TX/RX��Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);			//PA9 ����ΪUSART1_TX
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);		//PA10����ΪUSART1_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9��GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOA,&GPIO_InitStructure); 							//��ʼ��PA9��PA10  
	
	USART_Config(USART1, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART1);
	USART_DMARxConfig(USART1,USART1_DMABuffAddr, USART1_buffsize);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		
}

/*
*********************************************************************************************************
*	�� �� ��: Usart2_DriverInit
*	����˵��: ���ڶ�Ӳ����ʼ����֧��DMA���գ���֧��DMA����
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Usart2_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART2);
	/* ʹ��GPIOʱ�Ӻ�USART1ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* ����USART2��TX/RX��Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);			//PD5 ����ΪUSART2_TX
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);			//PD6 ����ΪUSART2_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 			//GPIOA9��GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOD,&GPIO_InitStructure); 							//��ʼ��PD5��PD6  
	
	USART_Config(USART2, BuadRate, preemptionPriority, subPriority);
	//USART_DMATxConfig(USART2);
	USART_DMARxConfig(USART2,USART2_DMABuffAddr, USART2_buffsize);
	//USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
		
}

/*
*********************************************************************************************************
*	�� �� ��: Usart3_DriverInit
*	����˵��: ����3Ӳ����ʼ����֧��DMA���գ�֧��DMA����
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Usart3_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART3);
	/* ʹ��GPIOʱ�Ӻ�USART3ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* ����USART2��TX/RX��Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);			//PB10 ����ΪUSART3_TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);			//PB11 ����ΪUSART3_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 		//GPIOA9��GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOB,&GPIO_InitStructure); 							//��ʼ��PB10��PB11  
	
	USART_Config(USART3, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART3);
	USART_DMARxConfig(USART3,USART3_DMABuffAddr, USART3_buffsize);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
}

/*
*********************************************************************************************************
*	�� �� ��: Usart6_DriverInit
*	����˵��: ����6Ӳ����ʼ����֧��DMA���գ�֧��DMA����
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Usart6_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(USART6);
	/* ʹ��GPIOʱ�Ӻ�USART3ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	/* ����USART2��TX/RX��Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);			//PC6 ����ΪUSART3_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);			//PC7 ����ΪUSART3_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 		//GPIOA9��GPIOA10  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 							//��ʼ��PB10��PB11  
	
	USART_Config(USART6, BuadRate, preemptionPriority, subPriority);
	USART_DMATxConfig(USART6);
	USART_DMARxConfig(USART6,USART6_DMABuffAddr, USART6_buffsize);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: Uart4_DriverInit
*	����˵��: ����4Ӳ����ʼ����֧��DMA���գ���֧��DMA����
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart4_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(UART4);
	/* ʹ��GPIOʱ�Ӻ�USART3ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* ����USART2��TX/RX��Ӧ��GPIO */
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);			//PC10 ����ΪUART4_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);			//PC11 ����ΪUART4_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 							//��ʼ��PB10��PB11  
	
	USART_Config(UART4, BuadRate, preemptionPriority, subPriority);
	//USART_DMATxConfig(UART4);
	USART_DMARxConfig(UART4,UART4_DMABuffAddr, UART4_buffsize);
	//USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: Uart5_DriverInit
*	����˵��: ����5Ӳ����ʼ����֧��DMA���գ�֧��DMA����
*	��    ��: ������ �ж����ȼ� �����ȼ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart5_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_DeInit(UART5);
	/* ʹ��GPIOʱ�Ӻ�UART5ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	
	/* ����UART5��TX/RX��Ӧ��GPIO *///TX_PC12 RX_PD2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);			//PC10 ����ΪUART5_TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);			//PC11 ����ΪUART5_RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 			  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					//���ù���  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  			    //�ٶ�50MHz  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				    //���츴�����  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					//����  
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	
	//��ʼ��PB10��PB11  
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
*	�� �� ��: USART_Config
*	����˵��: ���ڲ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void USART_Config(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority)
{
	
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	//USART1 ��ʼ������  
    USART_InitStructure.USART_BaudRate   = baudRate;				//����������  
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ  
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;		//һ��ֹͣλ  
    USART_InitStructure.USART_Parity     = USART_Parity_No;			//����żУ��λ  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������  
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ  
    USART_Init(USARTx, &USART_InitStructure); 						//��ʼ������1  
  
	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);					//��������ж�  
    USART_Cmd(USARTx, ENABLE);  									//ʹ�ܴ���1   
  
    USART_ClearFlag(USARTx, USART_FLAG_TC);  
       
    //Usart1 NVIC ����  
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preemptionPriority;	//��ռ���ȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 		=subPriority;       	//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd 				= ENABLE;        		//IRQͨ��ʹ��  
    NVIC_Init(&NVIC_InitStructure); 											//����ָ���Ĳ�����ʼ��VIC�Ĵ����� 
}
/*
*********************************************************************************************************
*	�� �� ��: USART_dmaTxConfig
*	����˵��: ���ڷ���ʹ�õ�DMA���ã�ע�����Ҫ�����ڴ��ַ�ʹ����С֮�����ʹ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void USART_DMATxConfig(USART_TypeDef* USARTx)
{
	DMA_InitTypeDef     DMA_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ��   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ��  
	
	if(USARTx == USART1)
	{
		DMA_DeInit(DMA2_Stream7);	
		while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);							//�ȴ�DMA������
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;			//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_1.SendBuf; //DMA �洢��0��ַ		
	}
	
	if(USARTx == USART3)
	{
		DMA_DeInit(DMA1_Stream4);	
		while(DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);							//�ȴ�DMA������
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;			//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_3.SendBuf; //DMA �洢��0��ַ		
	}
	if(USARTx == USART6)
	{
		DMA_DeInit(DMA2_Stream6);	
		while(DMA_GetCmdStatus(DMA2_Stream6) != DISABLE);							//�ȴ�DMA������
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;								//ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;			//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_6.SendBuf; //DMA �洢��0��ַ		
	}
	if(USARTx == UART5)
	{
		DMA_DeInit(DMA1_Stream7);	
		while(DMA_GetCmdStatus(DMA1_Stream7) != DISABLE);							//�ȴ�DMA������
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;								//ͨ��ѡ��
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;			//DMA�����ַ
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)g_usartRecv_5.SendBuf; //DMA �洢��0��ַ		
	}
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ  
    DMA_InitStructure.DMA_BufferSize = SEND_BUF_SIZE;//���ݴ�����   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;           
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ��� 
	
	if(USARTx == USART1)
    {
		DMA_Init(DMA2_Stream7, &DMA_InitStructure);//��ʼ��DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  
    }  
	if(USARTx == USART3)
    {
		DMA_Init(DMA1_Stream4, &DMA_InitStructure);//��ʼ��DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;  
    }	
	if(USARTx == USART6)
    {
		DMA_Init(DMA2_Stream6, &DMA_InitStructure);//��ʼ��DMA Stream  
		//DMA NVIC    
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;  
    }
	if(USARTx == UART5)
    {
		DMA_Init(DMA1_Stream7, &DMA_InitStructure);//��ʼ��DMA Stream  
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
*	�� �� ��: USART_DMARxConfig
*	����˵��: ���ڷ���ʹ�õ�DMA���ã�ע�����Ҫ�����ڴ��ַ�ʹ����С֮�����ʹ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void USART_DMARxConfig(USART_TypeDef* USARTx,uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef     DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA2ʱ��ʹ��   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ��   
	if(USARTx == USART1)
	{
		DMA_DeInit(DMA2_Stream5);  
		while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  	
	}
	if(USARTx == USART2)
	{
		DMA_DeInit(DMA1_Stream5);  
		while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  
	}
	if(USARTx == USART3)
	{
		DMA_DeInit(DMA1_Stream1);  
		while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  
	}
	if(USARTx == USART6)
	{
		DMA_DeInit(DMA2_Stream1);  
		while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_5; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  
	}	
	if(USARTx == UART4)
	{
		DMA_DeInit(DMA1_Stream2);  
		while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  
	}
	if(USARTx == UART5)
	{
		DMA_DeInit(DMA1_Stream0);  
		while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE);				 //�ȴ�DMA������   
		/* ���� DMA Stream */  
		DMA_InitStructure.DMA_Channel = DMA_Channel_4; 					 //ͨ��ѡ��  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;//DMA�����ַ  
		DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)DMABuffAddr;//DMA �洢��0��ַ  
	}
	DMA_InitStructure.DMA_DIR			  = DMA_DIR_PeripheralToMemory ;//���赽�洢��ģʽ  
    DMA_InitStructure.DMA_BufferSize 	  = buffsize;					//���ݴ�����   
    DMA_InitStructure.DMA_PeripheralInc   = DMA_PeripheralInc_Disable;	//���������ģʽ  
    DMA_InitStructure.DMA_MemoryInc		  = DMA_MemoryInc_Enable;		//�洢������ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ  
    DMA_InitStructure.DMA_MemoryDataSize  = DMA_MemoryDataSize_Byte;	//�洢�����ݳ���:8λ  
    DMA_InitStructure.DMA_Mode 			  = DMA_Mode_Normal;			// ʹ����ͨģʽ   
    DMA_InitStructure.DMA_Priority 		  = DMA_Priority_Medium;		//�е����ȼ�  
    DMA_InitStructure.DMA_FIFOMode		  = DMA_FIFOMode_Disable;           
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_Full;  
    DMA_InitStructure.DMA_MemoryBurst 	  = DMA_MemoryBurst_Single;		//�洢��ͻ�����δ���  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//����ͻ�����δ���  
      
  //  DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);  
	
	if(USARTx == USART1)
    {
		DMA_Init(DMA2_Stream5, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA2_Stream5, ENABLE);  //����DMA����  
    } 
	
	if(USARTx == USART2)
    {
		DMA_Init(DMA1_Stream5, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA1_Stream5, ENABLE);  //����DMA����    
    }  
	
	if(USARTx == USART3)
    {
		DMA_Init(DMA1_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA1_Stream1, ENABLE);  //����DMA����    
    }  
	
	if(USARTx == USART6)
    {
		DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA2_Stream1, ENABLE);  //����DMA����    
    } 
	if(USARTx == UART4)
    {
		DMA_Init(DMA1_Stream2, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA1_Stream2, ENABLE);  //����DMA����    
    } 
	if(USARTx == UART5)
    {
		DMA_Init(DMA1_Stream0, &DMA_InitStructure);//��ʼ��DMA Stream  
		DMA_Cmd(DMA1_Stream0, ENABLE);  //����DMA����    
    } 
}

/*
*********************************************************************************************************
*	�� �� ��: DmaSendDataProc
*	����˵��: ��ʼһ��DMA����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)  
{  
   
    DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA����   
      
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}  //ȷ��DMA���Ա�����    
          
    DMA_Streamx->NDTR = ndtr;          //���ݴ�����    
   
    DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA����   
}  

/*
*********************************************************************************************************
*	�� �� ��: USART_sendByte
*	����˵��: �첽���ڷ���һ���ֽڵ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    USART_SendData(USARTx, byte);
    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

//����1  
//���͵��ֽ�  
 void SendByteInfoProc(USART_TypeDef* USARTx,u8 nSendInfo)  
{  
    u8 *pBuf = NULL;  
    //ָ���ͻ�����  
	if(USARTx == USART1)
	{
		pBuf = g_usartRecv_1.SendBuf;  
		*pBuf++ = nSendInfo;  
		DmaSendDataProc(DMA2_Stream7,1); //��ʼһ��DMA���䣡  	
	}    	
	
}  
//���Ͷ��ֽ�  
void SendBytesInfoProc(USART_TypeDef* USARTx,u8* pSendInfo, u16 nSendCount)  
{  
    u16 i = 0;  
    u8 *pBuf = NULL;  
    //ָ���ͻ�����  
	if(USARTx == USART1)
	{	
	pBuf = g_usartRecv_1.SendBuf;    
    for (i=0; i<nSendCount; i++)  
    {  
        *pBuf++ = pSendInfo[i];  
    }  
    //DMA���ͷ�ʽ  
    DmaSendDataProc(DMA2_Stream7,nSendCount); //��ʼһ��DMA���䣡  	
	}    
	  
} 


void USART1_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA2_Stream5, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���DMA2_Steam7������ɱ�־  
        DMA_SetCurrDataCounter(DMA2_Stream5, RECV_BUF_SIZE);  
        DMA_Cmd(DMA2_Stream5, ENABLE);     //��DMA,  
  
    }  
} 
void USART2_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream5, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);//���DMA2_Steam7������ɱ�־  
        DMA_SetCurrDataCounter(DMA1_Stream5, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream5, ENABLE);     //��DMA,  
  
    }  
} 

void USART3_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream1, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//���DMA2_Steam1������ɱ�־  
        DMA_SetCurrDataCounter(DMA1_Stream1, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream1, ENABLE);     //��DMA,  
  
    }  
} 

void USART6_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(USART6,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA2_Stream1, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1 | DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1);//���DMA2_Steam1������ɱ�־  
        DMA_SetCurrDataCounter(DMA2_Stream1, RECV_BUF_SIZE);  
        DMA_Cmd(DMA2_Stream1, ENABLE);     //��DMA,  
  
    }  
} 

void UART4_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(UART4,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream2, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_HTIF2);//���DMA2_Steam1������ɱ�־  
        DMA_SetCurrDataCounter(DMA1_Stream2, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream2, ENABLE);     //��DMA,  
  
    }  
} 
void UART5_IRQHandler(void)  
{  
    u16 data;  
    if(USART_GetITStatus(UART5,USART_IT_IDLE) != RESET)  
    {  
        DMA_Cmd(DMA1_Stream0, DISABLE); //�ر�DMA,��ֹ�������������  
  
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
        DMA_ClearFlag(DMA1_Stream0,DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_HTIF0);//���DMA2_Steam1������ɱ�־  
        DMA_SetCurrDataCounter(DMA1_Stream0, RECV_BUF_SIZE);  
        DMA_Cmd(DMA1_Stream0, ENABLE);     //��DMA,  
  
    }  
} 
/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	USART_sendByte(USART1, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

