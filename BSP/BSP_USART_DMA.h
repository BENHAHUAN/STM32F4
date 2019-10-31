#ifndef  __BSP_USART_DMA_H__
#define  __BSP_USART_DMA_H__

#define RECV_BUF_SIZE  	  128                                 //128 Byte 的缓存空间，根据实际需要修改
#define SEND_BUF_SIZE     128
typedef struct
{
    uint16_t dataCount;
    uint8_t recvBuf[RECV_BUF_SIZE];
	uint8_t SendBuf[SEND_BUF_SIZE];
}usartRecv_t;

void Usart1_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void Usart2_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void Usart3_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void Usart6_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void Uart4_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void Uart5_DriverInit(uint32_t BuadRate,uint8_t preemptionPriority,uint8_t subPriority);
void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte);

#endif
