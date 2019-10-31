#include "includes.h"

/*
*********************************************************************************************************
*	�� �� ��: IO_InitHard
*	����˵��: LED���á�ǹ���á���������
*	��    �Σ���
*	�� �� ֵ: ��		                   			  
*********************************************************************************************************
*/
void IO_InitHard(void)
{
	/* ��ʼ��LED */
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_2,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);		
	GPIO_Config(RCC_AHB1Periph_GPIOE ,GPIOE,GPIO_Pin_3,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* ��ʼ��ǹ */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_9,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);
	/* ��ʼ������ */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_10,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* ��ʼ�������� */
	GPIO_Config(RCC_AHB1Periph_GPIOD ,GPIOD,GPIO_Pin_11,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_PuPd_DOWN);	
	/* ��ʼ������*/
	GPIO_Config(RCC_AHB1Periph_GPIOB ,GPIOB,GPIO_Pin_6,GPIO_Mode_IN,GPIO_OType_PP,GPIO_PuPd_UP);	
	GPIO_Config(RCC_AHB1Periph_GPIOB ,GPIOB,GPIO_Pin_6,GPIO_Mode_IN,GPIO_OType_PP,GPIO_PuPd_UP);	
}

/*
*********************************************************************************************************
*	�� �� ��: KEY_Scan
*	����˵��: ����ɨ��
*	��    �Σ���
*	�� �� ֵ: ����״̬ 0���޶��� 1������ 	                   			  
*********************************************************************************************************
*/
uint8_t KEY_Scan(void)
{
	static u8 flag_key1=1;//�������ɿ���־
	static u8 flag_key2=1;//�������ɿ���־
	//static u8 flag_key3=1;//�������ɿ���־
	/* KEY1 */
	if(flag_key1&&KEY1==0)
	{
	flag_key1=0;
	return 1;	// ��������
	}
	else if(1==KEY1)			
		flag_key1=1;
	/* KEY2 */
	if(flag_key2&&KEY2==0)
	{
	flag_key2=0;
	return 2;	// ��������
	}
	else if(1==KEY2)			
		flag_key2=1;
	
	return 0;//�ް�������
}
