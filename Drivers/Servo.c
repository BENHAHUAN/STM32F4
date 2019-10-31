#include "includes.h"

#define CW            0x00
#define CCW           0x80
#define Wheel_Time    0x03
#define Wheel_Circle  0x02
#define Wheel_Start   0x01
#define Wheel_Stop    0x00
#define CRC           uart_buff[uart_buff[3]+4]    //uart_buff[Header+內容長度]
#define Uart_length   (uart_buff[3]+5)        //Header+內容長度+CRC

unsigned char uart_buff[12];

void write_servo(uint8_t ch)
{
	USART_sendByte(USART3,ch);
}

void  Uart_Send(void)
{
	unsigned char i;

	uart_buff[0]=0x12;  //HEADER
	uart_buff[1]=0x4C;  //HEADER

	CRC=0;
	for(i=0;i<Uart_length;i++)
	{
		write_servo(uart_buff[i]);
		CRC += uart_buff[i];  //CRC Calc
	}
}

void  Angle_Mode(unsigned char servo_id,float angle,unsigned int interval,unsigned int power)
{
	int angle_int=(int)angle*10;

	uart_buff[2]=0x08;  //封包編號
	uart_buff[3]=0x07;  //內容長度
	uart_buff[4]=servo_id;
	uart_buff[5]=angle_int & 0xFF;
	uart_buff[6]=angle_int >> 8;
	uart_buff[7]=interval & 0xFF;
	uart_buff[8]=interval >> 8;
	uart_buff[9]=power & 0xFF;
	uart_buff[10]=power >> 8;
	Uart_Send();
}

void  duoji(unsigned char id,float angle,unsigned int interval)
{
	float angle_L = -90,angle_H = 90;
	
	if(angle < angle_L)
	{
		angle = angle_L;
	}
	if(angle > angle_H)
	{
		angle = angle_H;
	}
	
	Angle_Mode(id,angle,interval,1200);
}


