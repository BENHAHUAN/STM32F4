/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* 底盘 偏航角 PD2  PC12 串口5*/
/* 顶部 偏航角 PC11 PC10 串口4*/

#ifndef BNO055_DEBUG
#define BNO055_DEBUG  0
#endif

BNO055_HandleTypeDef Chassis_Pose;
BNO055_HandleTypeDef PTZ_Pose;

#define Chassis_USART UART5
#define PTZ_USART UART4

//0x38 读欧拉角和四元数指令
/**
  * @brief  向陀螺仪发送命令
  * @param  data: 命令
  * @retval None
  */
void Bno055_write(USART_TypeDef* USARTx,uint8_t data)
{
//	uint8_t bytes[3]={0};
	//bytes[0]=0xaa;
	//bytes[1]=data;//功能字节
	USART_sendByte(USARTx, 0xaa);
	USART_sendByte(USARTx, data);
	USART_sendByte(USARTx, 0);
	
}

/**
  * @brief  对陀螺仪返回的数据进行处理
  * @param  raw_data: 串口接收到的数据包
  * @retval None
  */
void pose_read_Chassis(BNO055_HandleTypeDef* BNO055_Handle,uint8_t *raw_data)
{
	uint8_t sum=0,i=0;
//	uint16_t data=0;
	int16_t DATA[7];
	for(sum=0,i=0;i<(raw_data[3]+4);i++)//rgb_data[3]=15
		sum+=raw_data[i];
	if(sum==raw_data[i])				//校验和判断
	{
		DATA[0] = (raw_data[4]  << 8)|raw_data[5];
		DATA[1] = (raw_data[6]  << 8)|raw_data[7];
		DATA[2] = (raw_data[8]  << 8)|raw_data[9];
		BNO055_Handle->YAW   = (float)((uint16_t)DATA[0])/100;
		BNO055_Handle->ROLL  = (float)DATA[1]/100;
		BNO055_Handle->PITCH = (float)(DATA[2])/100;


#if BNO055_DEBUG		
		printf("Chassis[%.2f,%.2f,%.2f]\r\n",BNO055_Handle->ROLL,BNO055_Handle->PITCH,BNO055_Handle->YAW);

#endif
	}	
	else
	{
		printf("Chassis_err\r\n");
	}
}
/**
  * @brief  对陀螺仪返回的数据进行处理
  * @param  raw_data: 串口接收到的数据包
  * @retval None
  */
void pose_read_PTZ(BNO055_HandleTypeDef* BNO055_Handle,uint8_t *raw_data)
{
	uint8_t sum=0,i=0;
//	uint16_t data=0;
	int16_t DATA[7];
	for(sum=0,i=0;i<(raw_data[3]+4);i++)//rgb_data[3]=15
		sum+=raw_data[i];
	if(sum==raw_data[i])				//校验和判断
	{
		DATA[0] = (raw_data[4]  << 8)|raw_data[5];
		DATA[1] = (raw_data[6]  << 8)|raw_data[7];
		DATA[2] = (raw_data[8]  << 8)|raw_data[9];
		BNO055_Handle->YAW   = (float)((uint16_t)DATA[0])/100;
		BNO055_Handle->ROLL  = (float)DATA[1]/100;
		BNO055_Handle->PITCH = (float)(DATA[2])/100;


#if BNO055_DEBUG		
		printf("PTZ[%.2f,%.2f,%.2f]\r\n",BNO055_Handle->ROLL,BNO055_Handle->PITCH,BNO055_Handle->YAW);

#endif
	}	
	else
	{
		printf("PTZ_err\r\n");
	}
}
