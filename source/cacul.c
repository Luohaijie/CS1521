#include "cacul.h"
#include "timer.h"
#include "adc.h"
#include "flash.h"
#include "function.h"
/*******************************************************
���룺no
�����K,B
���ܣ�����Y=KX+B
********************************************************/
/*
void Calc_K_B_Tenbit(void)
{
	unsigned int vcp500,vcp1500;
	unsigned int i;
	unsigned int temp_buff[10];

	
	PWM_Init(300);
	delay_ms(500);
	
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect()/2;
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
    vcp500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	PWM_Init(800);
	delay_ms(500);				
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect()/2;
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
	vcp1500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	K = 500.00/(vcp1500-vcp500); 
	Badd =300-(int)(K*vcp500);
	

}
void Calc_K_B_Elebit(void)
{
	unsigned int vcp500,vcp1500;
	unsigned int i;
	unsigned int temp_buff[10];

	
	Pwm_Load(500);
	delay_ms(500);
	
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect();
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
    vcp500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	Pwm_Load(1500);
	delay_ms(500);				
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect();
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
	vcp1500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	K = 1000.00/(vcp1500-vcp500); 
	Badd =500-(int)(K*vcp500);
}
*/
void Calc_K_B(void)
{
	uint16 temp,table[10];
	uchar i;
/*
	#if TEN_BIT
		Calc_K_B_Tenbit();
	#else
		Calc_K_B_Elebit();
	#endif
*/
	K=1.0;
	DAC_Output(2000);
	for(i=0;i<10;i++){
		table[i] = Vcp_Collect();
		delay_ms(100);		 
	}
	Insert_Sort(table,10);
	temp = (table[3]+table[4]+table[5]+table[6])/4;
	Badd = (int)(2000-temp);

}

/***************************************
���룺���ݺʹ�С
�������
���ܣ�ð������
****************************************/
void Insert_Sort (unsigned int *Data,unsigned short size) 
{ 
	 int16 temp;
	 int16 j;
	 int16 i;
	 char data flag = 0;
	for (i = 0; i < size-1; i++) 
	{  
		flag = 0;
		for (j=0; j<size-i-1; j++)
		{
			if(Data[j]>Data[j+1])
			{
				temp = Data[j+1];
		      	Data[j+1] = Data[j];
		    	Data[j] = temp;
				flag = 1;
			}
		}
		if(flag==0)
		{
			break;
		}
	}
} 
/********************************************************************
��������
���룺
�����
���ã��������
********************************************************************/
/*
void Line_Fitting(unsigned int databuf[],unsigned char first_flag,unsigned char length)
{
	
	unsigned int first_val,last_val;
	float temp_val;
	unsigned char i;
	first_val = databuf[first_flag];
	last_val = databuf[length];
	if(last_val>first_val)
	{
		temp_val = (last_val-first_val)/(length-first_flag);
		for(i=first_flag;i<=length;i++)
		databuf[i+1]=(int)(databuf[first_flag]+i*temp_val);
	}
	else 
	{
		temp_val = (first_val-last_val)/(length-first_flag);
		for(i=first_flag;i<length;i++)
		databuf[i+1]=(int)(databuf[first_flag]-i*temp_val);
	}

} 		*/