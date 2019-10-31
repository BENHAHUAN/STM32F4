 #include "includes.h"


/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
void UsartTaskCreate(void);
static void AppObjCreate (void);
__task void TaskPTZMsg(void);
__task void TaskChassisMsg(void);



/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/
static uint64_t TaskChassisMsgStk[1024/8];  /* 任务栈 */
static uint64_t TaskPTZMsgStk[1024/8]; 	   /* 任务栈 */

/* 任务句柄 */
OS_TID HandleTaskChassisMsg = NULL;
OS_TID HandleTaskPTZMsg = NULL;

/* 声明一个支持20个消息的消息邮箱 */
os_mbx_declare(Chassis_Pose_Box,20);		/* 底盘陀螺仪邮箱 */
os_mbx_declare(PTZ_Pose_Box    ,20);		/* 云台陀螺仪邮箱 */

/*
*********************************************************************************************************
*	函 数 名: TaskChassisMsg
*	功能说明: 底盘陀螺仪数据采集，等待串口中断发来的消息邮箱数据
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
__task void TaskChassisMsg(void)
{
	uint8_t *pMsg;
	OS_RESULT xResult;
	
	Bno055_write(UART5,0x48);
    while(1)
    {
		if(os_mbx_wait(&Chassis_Pose_Box, (void *)&pMsg, 0) == OS_R_OK)
		{
			pose_read_Chassis(&Chassis_Pose,pMsg);
		}
		else
		{
			os_dly_wait(8);
		}
		
    }
}
/*
*********************************************************************************************************
*	函 数 名: TaskPTZMsg
*	功能说明: 云台陀螺仪数据采集，等待串口中断发来的消息邮箱数据
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
u8 GY955_cishu = 0;

__task void TaskPTZMsg(void)
{
	uint8_t *pMsg;
	OS_RESULT xResult;
	
	Bno055_write(UART4,0x48);
	
    while(1)
    {
		if(os_mbx_wait(&PTZ_Pose_Box, (void *)&pMsg, 0) == OS_R_OK)
		{
			pose_read_PTZ(&PTZ_Pose,pMsg);
			
			if(DuoJi_BiaoZhi == 1)
			{
				DuoJi_BiaoZhi = 0;
				GY955_cishu = 80;
			}
			if(GY955_cishu > 0)
			{
				GY955_cishu--;
				GY955_last = PTZ_Pose.YAW;
			}
			
			GY955_CTRL(); // GY955 PID
		}
		else
		{
			os_dly_wait(8);
		}
		
    }
}
/*
*********************************************************************************************************
*	函 数 名: UsartTaskCreate
*	功能说明: 创建USART任务
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UsartTaskCreate(void)
{
	os_mbx_init (&Chassis_Pose_Box, sizeof(Chassis_Pose_Box));
	os_mbx_init (&PTZ_Pose_Box    , sizeof(PTZ_Pose_Box));
	
	Usart1_DriverInit(115200,1,0);	//DEBUG
	
	//Usart2_DriverInit(115200,2,0);
	Usart3_DriverInit(115200,3,0);	//舵机
	//Usart6_DriverInit(115200,4,0);
	
	Uart4_DriverInit (115200,5,0);	//PTZ
	Uart5_DriverInit (115200,6,0);	//CHASSIS
	
	HandleTaskChassisMsg = os_tsk_create_user(TaskChassisMsg,             /* 任务函数 */ 
	                                      8,                        	 /* 任务优先级 */ 
	                                      &TaskChassisMsgStk,        	 /* 任务栈 */
	                                      sizeof(TaskChassisMsgStk)); 	 /* 任务栈大小，单位字节数 */
	
	HandleTaskPTZMsg = os_tsk_create_user(TaskPTZMsg,            		  /* 任务函数 */ 
										   9,                      		  /* 任务优先级 */ 
										   &TaskPTZMsgStk,                /* 任务栈 */
										   sizeof(TaskPTZMsgStk)); 		  /* 任务栈大小，单位字节数 */
	
}
