#include "includes.h"
//(未测试)
/*
	

    nRF24L01模块   STM32F427
	  VCC   ------  3.3V
	  GND   ------  GND
      SCK   ------  PA5
      MISO  ------  PA6
      MOSI  ------  PA7
      CSN   ------  PC4
      IRQ   ------  PC5
      CE    ------  PA4
*/

/* 定义GPIO端口 */
#define nRF24L01_RCC_SPI  RCC_APB2Periph_SPI1
#define nRF24L01_RCC_ALL  (RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC )

#define nRF24L01_PORT_CE   GPIOA
#define nRF24L01_PIN_CE    GPIO_Pin_4

#define nRF24L01_PORT_CSN  GPIOC
#define nRF24L01_PIN_CSN   GPIO_Pin_4

#define nRF24L01_PORT_IRQ  GPIOC
#define nRF24L01_PIN_IRQ   GPIO_Pin_5

/* 引脚操作，采用寄存器的形式，加快操作速度 */
#define nRF24L01_CSN_1()    nRF24L01_PORT_CSN->BSRRL = nRF24L01_PIN_CSN
#define nRF24L01_CSN_0()    nRF24L01_PORT_CSN->BSRRH = nRF24L01_PIN_CSN

#define nRF24L01_CE_1()     nRF24L01_PORT_CE->BSRRL = nRF24L01_PIN_CE
#define nRF24L01_CE_0()     nRF24L01_PORT_CE->BSRRH = nRF24L01_PIN_CE
                
#define nRF24L01_IRQ_Read()	  (nRF24L01_PORT_IRQ->IDR & nRF24L01_PIN_IRQ)

/* 发射端和接收端地址 */
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  
uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_CfgSpiHard
*	功能说明: 配置STM32内部SPI硬件的工作模式、速度等参数，用于访问nRF24L01。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void nRF24L01_CfgSpiHard(void)
{
	
	/*
	STM32F4XX 时钟计算.
		HCLK = 168M
		PCLK1 = HCLK / 4 = 42M
		PCLK2 = HCLK / 2 = 84M

		SPI2、SPI3 在 PCLK1, 时钟42M
		SPI1       在 PCLK2, 时钟84M

		STM32F4 支持的最大SPI时钟为 37.5 Mbits/S, 因此需要分频。下面使用的是SPI1。
	*/
	bsp_InitSPIBus();
	/* 设置波特率预分频系数 84MHz / 16 = 5.25MHz，24L01支持的最大SPI时钟为10MHz
	   这里选择SPI_BaudRatePrescaler_8容易出错。
	*/
	//bsp_SPI_Init(SPI_BaudRatePrescaler_16);	
}
/*
*********************************************************************************************************
*	函 数 名: nRF24L01_InitHard
*	功能说明: SPI配置
*	形    参：无
*	返 回 值: 无		                   			  
*********************************************************************************************************
*/
void nRF24L01_InitHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/*使能GPIO时钟 */
	RCC_AHB1PeriphClockCmd(nRF24L01_RCC_ALL, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	/* 配置nRF24L01的CE引脚 */
	/* 配置为推挽输出模式 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = nRF24L01_PIN_CE;
	GPIO_Init(nRF24L01_PORT_CE, &GPIO_InitStructure);
	
	/* 配置nRF24L01的 CSN 引脚 */
	/* 配置为推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = nRF24L01_PIN_CSN;
	GPIO_Init(nRF24L01_PORT_CSN, &GPIO_InitStructure);
	
	/* 配置nRF24L01的 IRQ 引脚 */
	/* 配置为输入带上拉 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 	
	GPIO_InitStructure.GPIO_Pin = nRF24L01_PIN_IRQ; 
	GPIO_Init(nRF24L01_PORT_IRQ, &GPIO_InitStructure);
	
	/* 配置 SPI1工作模式 */
	nRF24L01_CfgSpiHard();
	
	/* 用于拉高csn引脚，NRF进入空闲状态 */
	nRF24L01_CSN_1();  
} 
/*
*********************************************************************************************************
*	函 数 名: nRF24L01_WriteReadByte
*	功能说明: 用于向NRF读/写一字节数据
*	形    参: 写入的数据
*	返 回 值: 读取得的数据		        
*********************************************************************************************************
*/
static uint8_t SPI_RW(uint8_t dat)
{  	
	/* 当SPI发送缓冲器非空时等待 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	/* 通过SPI发送一字节数据 */
	SPI_I2S_SendData(SPI1, dat);		
	
	/* 当SPI接收缓冲器为空时等待 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* 通过SPI接收一字节数据 */
	return SPI_I2S_ReceiveData(SPI1);
}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_WriteReg
*	功能说明: 用于向nRF24L01特定的寄存器写入数据
*	形    参: _ucRegAddr: nRF24L01的命令+寄存器地址。
 		      _usValue:   将要向寄存器写入的数据
*	返 回 值: nRF24L01的status寄存器的状态		        
*********************************************************************************************************
*/
static uint8_t NRF24L01_Write_Reg(uint8_t _ucRegAddr, uint8_t _usValue)
{
	uint8_t ucStatus;
	
	//nRF24L01_CE_0();
	
	/* 置低CSN，使能SPI传输 */
	nRF24L01_CSN_0();
				
	/* 发送命令及寄存器号 */
	ucStatus = SPI_RW(_ucRegAddr);
		 
	 /* 向寄存器写入数据 */
	SPI_RW(_usValue); 
	          
	/* CSN拉高，完成 */	   
	nRF24L01_CSN_1();	
		
	/* 返回状态寄存器的值 */
	return(ucStatus);
}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_ReadReg
*	功能说明: 用于从nRF24L01特定的寄存器读出数据
*	形    参: reg: nRF24L01的命令+寄存器地址
*	返 回 值: 寄存器中的数据		        
*********************************************************************************************************
*/
static uint8_t NRF24L01_Read_Reg(uint8_t _ucRegAddr)
{
 	uint8_t usValue;

	nRF24L01_CE_0();
	
	/* 置低CSN，使能SPI传输 */
 	nRF24L01_CSN_0();
				
  	 /* 发送寄存器号 */
	SPI_RW(_ucRegAddr); 

	 /* 读取寄存器的值 */
	usValue = SPI_RW(NOP);
	            
   	/*CSN拉高，完成*/
	nRF24L01_CSN_1();		
   	
	return usValue;
}	

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_ReadBuf
*	功能说明: 用于从nRF24L01的寄存器中读出一串数据
*	形    参: _ucRegAddr: nRF24L01的命令+寄存器地址。
 		      _pBuf：     数据缓冲地址。
		      _ucLen:     _pBuf据长度
*	返 回 值: nRF24L01的status寄存器的状态		        
*********************************************************************************************************
*/
static uint8_t nRF24L01_ReadBuf(uint8_t _ucRegAddr, uint8_t *_pBuf, uint8_t _ucLen)
{
 	uint8_t ucStatus, i;

	//nRF24L01_CE_0();
	
	/* 置低CSN，使能SPI传输 */
	nRF24L01_CSN_0();
		
	/*发送寄存器号*/		
	ucStatus = SPI_RW(_ucRegAddr); 

 	/*读取缓冲区数据*/
	for(i = 0; i < _ucLen; i++)
	{
		_pBuf[i] = SPI_RW(0xff);
	}
	    
	/* CSN拉高，完成 */
	nRF24L01_CSN_1();	
		
 	return ucStatus;	
}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_WriteBuf
*	功能说明: 用于向nRF24L01的寄存器中写入一串数据
*	形    参：_ucRegAddr: nRF24L01的命令+寄存器地址。
 		      _pBuf：     数据缓冲地址。
		      _ucLen:     _pBuf的数据长度。
*	返 回 值: NRF的status寄存器的状态。	        
*********************************************************************************************************
*/
static uint8_t nRF24L01_WriteBuf(uint8_t _ucRegAddr, uint8_t *_pBuf, uint8_t _ucLen)
{
	 uint8_t ucStatus,i;
	
	 //nRF24L01_CE_0();
	
   	 /*置低CSN，使能SPI传输*/
	 nRF24L01_CSN_0();			

	 /*发送寄存器号*/	
  	 ucStatus = SPI_RW(_ucRegAddr); 
 	
  	  /*向缓冲区写入数据*/
	 for(i = 0; i < _ucLen; i++)
	{
		SPI_RW(*_pBuf++);
	}
	  	   
	/*CSN拉高，完成*/
	nRF24L01_CSN_1();			
  
  	return (ucStatus);	
}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_RxMode
*	功能说明: 设置nRF24L01工作在接收模式
*	形    参: 无
*	返 回 值: 无		        
*********************************************************************************************************
*/
//void nRF24L01_RxMode(void)
//{
//	nRF24L01_CE_0();
//	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);  /* 写RX节点地址 */
//	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);               			/* 使能通道0的自动应答 */    
//	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);           			/* 使能通道0的接收地址 */    
//	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);             			/* 设置RF通信频率 */    
//	nRF24L01_WriteReg(NRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);  			/* 选择通道0的有效数据宽度 */      
//	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f); /* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */   
//	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);   /* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 */ 
//	
//	/*CE拉高，进入接收模式*/	
//	nRF24L01_CE_1(); 
//}    

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_TxMode
*	功能说明: 设置nRF24L01工作在发送模式
*	形    参: 无
*	返 回 值: 无		        
*********************************************************************************************************
*/
//void nRF24L01_TxMode(void)
//{  
//	nRF24L01_CE_0();
//	nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    /* 写TX节点地址 */ 
//	nRF24L01_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); /* 设置TX节点地址,主要为了使能ACK */   
//	nRF24L01_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     /* 使能通道0的自动应答 */    	
//	nRF24L01_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); /* 使能通道0的接收地址 */  	
//	nRF24L01_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x0f);/* 设置自动重发间隔时间:250us + 86us;最大自动重发次数:10次 */	
//	nRF24L01_WriteReg(NRF_WRITE_REG+RF_CH, CHANAL);   /* 设置RF通道为CHANAL */	
//	nRF24L01_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);  /* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */   	
//	nRF24L01_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    /* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断 */	
//	
//	/*CE拉高，进入发送模式*/	
//	nRF24L01_CE_1();
//	bsp_DelayUS(100);  /* CE要拉高一段时间才进入发送模式，时间大于10us */
//}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_ConnectCheck
*	功能说明: 主要用于nRF24L01_与MCU是否正常连接
*	形    参: 无
*	返 回 值: 1 表示连接正常，0 表示连接不正常	        
*********************************************************************************************************
*/
uint8_t nRF24L01_ConnectCheck(void)
{
	uint8_t ucBuf[TX_ADR_WIDTH];

	/*写入TX_ADR_WIDTH个字节的地址.  */  
	nRF24L01_WriteBuf(NRF_WRITE_REG+TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	/*读出写入的地址 */
	nRF24L01_ReadBuf(TX_ADDR, ucBuf, TX_ADR_WIDTH); 
	 
	/* 用于调试目的 */
//	printf("ucBuf[0] = %d, TX_ADDRESS[0] = %d\r\n", ucBuf[0], TX_ADDRESS[0]);
//	printf("ucBuf[1] = %d, TX_ADDRESS[1] = %d\r\n", ucBuf[1], TX_ADDRESS[1]);
//	printf("ucBuf[2] = %d, TX_ADDRESS[2] = %d\r\n", ucBuf[2], TX_ADDRESS[2]);
//	printf("ucBuf[3] = %d, TX_ADDRESS[3] = %d\r\n", ucBuf[3], TX_ADDRESS[3]);
//	printf("ucBuf[4] = %d, TX_ADDRESS[4] = %d\r\n", ucBuf[4], TX_ADDRESS[4]);
	
	/* 比较写入的数据和读出的是否相同 */
	if(strncmp((char *)TX_ADDRESS, (char *)ucBuf, TX_ADR_WIDTH) == 0)
	{
		return 1;  /* 相同 */
	}
	else
	{
		return 0;  /* 不相同 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_TxData
*	功能说明: 用于向nRF24L01的发送缓冲区中写入数据
*	形    参: _pTxBuf 数据缓冲地址
*	返 回 值: 发送结果，成功返回TX_DS, 失败返回MAX_RT或0		        
*********************************************************************************************************
*/
uint8_t nRF24L01_TxData(uint8_t *_pTxBuf)
{
	uint8_t ucState;
	uint32_t ulCount = 0;   

	/*CE为低，进入待机模式1*/
	nRF24L01_CE_0();

	/*写数据到_pTxBuf最大32个字节*/						
	nRF24L01_WriteBuf(WR_TX_PLOAD, _pTxBuf, TX_PLOAD_WIDTH);

	/*CE为高，_pTxBuf非空，发送数据包 */   
	nRF24L01_CE_1();
	  	
	/*
	  等待发送完成中断                            
   	  while(nRF24L01_IRQ_Read()!=0); 
      选择下面这种方式可以有效防止发送数据阻塞时，死在这个状态 
	*/
	while(nRF24L01_IRQ_Read()!=0); 
	
	
	/* 读取状态寄存器的值 */                              
	ucState = NRF24L01_Read_Reg(STATUS);

	/*清除TX_DS或MAX_RT中断标志*/                  
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, ucState); 	

	 /*判断中断类型*/ 
	/* 达到最大重发次数 */
	if(ucState & MAX_RT)                     
	{
		NRF24L01_Write_Reg(FLUSH_TX,NOP);      /* 清除TX FIFO寄存器 */ 
		return MAX_RT;
	}
	/* 发送完成 */
	else if(ucState & TX_DS)
	{
		return TX_DS;
	} 
	/* 其他原因发送失败 */	

		return 0xff;               
} 

/*
*********************************************************************************************************
*	函 数 名: nRF24L01_RxData
*	功能说明: 用于从nRF24L01的接收缓冲区中读出数据
*	形    参: _pRxBuf 数据缓冲地址
*	返 回 值: 接收结果，成功返回RX_DS, 失败返回0		        
*********************************************************************************************************
*/
uint8_t nRF24L01_RxData(uint8_t *_pRxBuf)
{
	uint8_t ucState;
	uint32_t ulCount = 0;  

	/*读取status寄存器的值  */               
	ucState=NRF24L01_Read_Reg(STATUS);
	 
	/* 清除中断标志*/      
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, ucState);

	/*判断是否接收到数据*/
	if(ucState & RX_DR)    /* 接收到数据 */
	{
		nRF24L01_CE_0();
		nRF24L01_ReadBuf(RD_RX_PLOAD, _pRxBuf, RX_PLOAD_WIDTH); /* 读取数据 */
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);                       /* 清除RX FIFO寄存器 */
		nRF24L01_CE_1();
		bsp_DelayUS(150);
		return 0; 
	}

		return 1;   /* 没收到任何数据 */			
}
void SEND_BUF(uint8_t *buf)
{
	nRF24L01_CE_0();
	NRF24L01_Write_Reg( 0x20+CONFIG,0x0e);
	nRF24L01_CE_1();
	bsp_DelayUS(15);
	nRF24L01_TxData(buf);
	nRF24L01_CE_0();
	NRF24L01_Write_Reg( 0x20+CONFIG, 0x0f);
	nRF24L01_CE_1();	
}
void NRF24L01_RT_Init(void)
{	
	nRF24L01_CE_0();	  
  	NRF24L01_Write_Reg(0x20+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
	NRF24L01_Write_Reg(FLUSH_RX,0xff);									//清除RX FIFO寄存器    
  	nRF24L01_WriteBuf(0x20+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	nRF24L01_WriteBuf(0x20+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  	NRF24L01_Write_Reg(0x20+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(0x20+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(0x20+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(0x20+RF_CH,0);        //设置RF通道为2.400GHz  频率=2.4+0GHz
  	NRF24L01_Write_Reg(0x20+RF_SETUP,0x0F);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(0x20+CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	nRF24L01_CE_1();									  //CE置高，使能发送
}















