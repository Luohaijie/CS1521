#include <CS1521.H>
#include "adc.h"
#include "cacul.h"
#include "function.h"
#include "config.h"
#include "timer.h"

enum chanl{ch_vp,ch_cp,ch_ts,ch_vdd18};
uint16 buf[20];
/***************************************
���룺��
������ɼ����10λ�¶�
���ܣ��ɼ����Ծ�����¶ȶ�Ӧ�ĵ�ѹֵ,�õ�������
****************************************/
#if MCU_temperture
unsigned int MCU_Temp(void)
{
    uint16 adcVal=0;  
//	CLKC = 0x03;
	
	ADCR = 0xe0;  	// setting,Enable ADC,fadc = fx/2
	TEMPC = 0x01;   //enable tempsensor and output
	ADCR = (ADCR & 0xf0) | (ch_ts & 0x0f);	// select channel ch_ts
	ADCR |= 0x10; 	// start ADC
	while(!(ADCR & 0x08));	// wait ADC busy
	adcVal = (ADRH << 8) | ADRL;	// read ADC

//	CLKC = 0x00;
	return	adcVal;

}
#else
unsigned int  KDS_Temp(void)
{
    uint16 adcVal=0;
	return adcVal;
}

#endif
unsigned int  Temp_ADC(void)
{
    #if MCU_temperture
    	return MCU_Temp();
   	#else
   		return KDS_Temp();
   	#endif
}

unsigned int Temp_Collect(void)
{
	char data i,j;
	uint16 Data;
	long Sum=0;
	for(j=0;j<16;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Temp_ADC();

		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>4;
	return(Data);
}

/*****************************************
���룺��
������ɼ�10λcp��ѹ
���ܣ��ɼ�ʹ���幤��������Ƶ�ʵ�ѹ�ص�ѹ,�õ�������
******************************************/
unsigned int Vcp_ADC(void)
{
	uint16 adcVal=0;

    
	ADCR = 0xe0;  	// setting,Enable ADC,fadc = fx/2
	ADCR = (ADCR & 0xf0) | (ch_cp & 0x0f);	// select channel ch_cp
	ADCR |= 0x10; 	// start ADC
	while(!(ADCR & 0x08));	// wait ADC busy
	adcVal = (ADRH << 8) | ADRL;	// read ADC
	
	return	adcVal;
}


unsigned int Vcp_Collect(void)
{
    char data i,j;
    uint16 Data;
    long Sum=0;
	for(j=0;j<16;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Vcp_ADC();

		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>4;
	return(Data);
}

/**************************************
���룺��
������ɼ����10λ��ƫ��ѹֵ
���ܣ��ɼ����Ծ������ƫ��ѹֵ���õ�������
**************************************/
unsigned int Vpull_ADC(void)
{	
	uint16 adcVal=0;
    
	ADCR = 0xe0;  	// setting,Enable ADC,fadc = fx/2
	ADCR = (ADCR & 0xf0) | (ch_vp & 0x0f);	// select channel ch_vp	 0
	ADCR |= 0x10; 	// start ADC
	while(!(ADCR & 0x08));	// wait ADC busy
	adcVal = (ADRH << 8) | ADRL;	// read ADC

	return	adcVal;
}

unsigned int Vpull_Collect(void)
{
    char data i,j;
    uint16 Data;
    long Sum=0;
	for(j=0;j<16;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Vpull_ADC();

		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>4;
	return(Data);
}
void DAC_Output(uint16 Dacval)
{
	DAH = (Dacval>>8)&0x0f;
	DAL = Dacval&0xff;
	DAH |= 0x10; 				  //dacʹ��
}
