#ifndef _BNO055_H_
#define _BNO055_H_

/** 
  * @brief ÍÓÂÝÒÇÊý¾Ý
  */
typedef struct
{
	float ROLL;
	float PITCH;
	float YAW;
	float Q4[4];	
}BNO055_HandleTypeDef;

/** @defgroup Bno055_Exported_Functions
  * @{
  */
void pose_read_PTZ(BNO055_HandleTypeDef* BNO055_Handle,uint8_t *raw_data);
void pose_read_Chassis(BNO055_HandleTypeDef* BNO055_Handle,uint8_t *raw_data);
void pose_read(BNO055_HandleTypeDef* BNO055_Handle,uint8_t *raw_data);
void Bno055_write(USART_TypeDef* USARTx,uint8_t data);
extern BNO055_HandleTypeDef Chassis_Pose;
extern BNO055_HandleTypeDef PTZ_Pose;

#endif
