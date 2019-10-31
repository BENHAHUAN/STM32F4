 #include "includes.h"


/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
void UsartTaskCreate(void);
static void AppObjCreate (void);
__task void TaskPTZMsg(void);
__task void TaskChassisMsg(void);



/*
**********************************************************************************************************
											 ����
**********************************************************************************************************
*/
static uint64_t TaskChassisMsgStk[1024/8];  /* ����ջ */
static uint64_t TaskPTZMsgStk[1024/8]; 	   /* ����ջ */

/* ������ */
OS_TID HandleTaskChassisMsg = NULL;
OS_TID HandleTaskPTZMsg = NULL;

/* ����һ��֧��20����Ϣ����Ϣ���� */
os_mbx_declare(Chassis_Pose_Box,20);		/* �������������� */
os_mbx_declare(PTZ_Pose_Box    ,20);		/* ��̨���������� */

/*
*********************************************************************************************************
*	�� �� ��: TaskChassisMsg
*	����˵��: �������������ݲɼ����ȴ������жϷ�������Ϣ��������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 3  
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
*	�� �� ��: TaskPTZMsg
*	����˵��: ��̨���������ݲɼ����ȴ������жϷ�������Ϣ��������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 3  
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
*	�� �� ��: UsartTaskCreate
*	����˵��: ����USART����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UsartTaskCreate(void)
{
	os_mbx_init (&Chassis_Pose_Box, sizeof(Chassis_Pose_Box));
	os_mbx_init (&PTZ_Pose_Box    , sizeof(PTZ_Pose_Box));
	
	Usart1_DriverInit(115200,1,0);	//DEBUG
	
	//Usart2_DriverInit(115200,2,0);
	Usart3_DriverInit(115200,3,0);	//���
	//Usart6_DriverInit(115200,4,0);
	
	Uart4_DriverInit (115200,5,0);	//PTZ
	Uart5_DriverInit (115200,6,0);	//CHASSIS
	
	HandleTaskChassisMsg = os_tsk_create_user(TaskChassisMsg,             /* ������ */ 
	                                      8,                        	 /* �������ȼ� */ 
	                                      &TaskChassisMsgStk,        	 /* ����ջ */
	                                      sizeof(TaskChassisMsgStk)); 	 /* ����ջ��С����λ�ֽ��� */
	
	HandleTaskPTZMsg = os_tsk_create_user(TaskPTZMsg,            		  /* ������ */ 
										   9,                      		  /* �������ȼ� */ 
										   &TaskPTZMsgStk,                /* ����ջ */
										   sizeof(TaskPTZMsgStk)); 		  /* ����ջ��С����λ�ֽ��� */
	
}
