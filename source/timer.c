#include <CS1521.H>

#include "timer.h"
#include "function.h"


//extern unsigned char flag_uart;
/*
void Timer0_Init(void)
{
	 NOP;
}
*/
/********************************************************************
��������
���룺
�����
���ã���ʱ����ʼ��
*********************************************************************/
/*
void Timer1_Init(void)
{
	   NOP;
} 

void Timer2_Init(void)
{	
	   NOP;
}

void Pwm_Load(unsigned int pwm_high)
{
	PCMPH = pwm_high>>8;
	PCMPL = pwm_high&0xff;    //duty ratio  pwm_high/3FFF
	PLOADH =0x3F;
	PLOADL =0xFF;    //cycle 0~3FFF 
	PCTRL = 0x04;    //enable pwm , Fosc/1
}
  */
/********************************************************************
��������delay_ms
���룺
�����
���ã���ʱ1����
*********************************************************************/
void	Delay_100us(unsigned char time)	//8Mhz�ڲ�RC����ʱXus  
{
	
	uchar i;
    uchar j;
	for(i=0;i<time;i++)
	{
	    for(j=0;j<50;j++)
			{
//					IEN0|=0X40;
//					IEN1|=0X40;
			}
							
	};
		
}

void	delay_ms(unsigned int time)	//8Mhz�ڲ�RC����ʱXms  
{
	uint16 i;
	for(i=0;i<time;i++)
	{
	   if(flag_uart == 1)
		{
				break;
		}
		Delay_100us(10);
  
	}
		

}

/***************************************
��������12λPWM����
���룺PWM_UP
�����
���ܣ�
****************************************/
/*
void PWM_Init(unsigned int u)
{
	PCMPH = u>>8;
	PCMPL = u&0xff;    //duty ratio  1/3FFF
	PLOADH =0x3F;
	PLOADL =0xFF;    //cycle 0~3FFF 
	PCTRL = 0x04;    //enable pwm , Fosc/1
}
  */