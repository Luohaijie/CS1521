
#include <CS1521.H>
#include "flash.h"
#include "function.h"


unsigned char Flash_Read(uint16 addr)
{	        
	uchar aTemp;	
	aTemp = *((unsigned char code *)addr);
	return aTemp;		//Read data form code dat
}

/****************************************************************
��������Flash��
���룺Ҫ���ĵ�ַ
�������
�������ܣ���Flashָ���ĵ�ַ������Ӧ�����ݣ����ŵ���Ӧ������
*****************************************************************/

unsigned int Flash_Read_Int(uint16 addr)
{	
	uchar j=0;
    uchar aTemp_H,aTemp_L; 
	uint16 aTemp;	
	aTemp_H = Flash_Read(addr);
	aTemp_L = Flash_Read(addr+1);
	aTemp =  aTemp_H*256+aTemp_L;
	return aTemp;		//Read data form code dat
}
/****************************************************************
��������Flashд
���룺Ҫд��ĵ�ַ������
�������
�������ܣ���Flashָ���ĵ�ַд����Ӧ������
 ****************************************************************/

void Flash_Write(uint16 addr,uchar dat)
{	     
    	uchar j=0;
		EEAH=addr>>8;				//���ø�8λ��ַ
		EEAL=addr&0xff;			//��8λ��ַ
		EED=dat;
		EEC = 0X01;	
	    j++;								//�˴������޸�
		j--;								//�˴������޸�
	
		
		
}

/****************************************************************
��������Flash����
���룺Ҫ�����ĵ�ַ
�������
�������ܣ�����Flash�ƶ�λ��
****************************************************************/	
void Flash_Erase(uint16 addr)
{
//	bit fgTemp =EAL;			//Backup EA

//	EAL =0;	//Closed interrupt when operation Flash
	if((addr%1024)==0) {   
		uchar j=0;
		EEAH=addr>>8;				//���ò���������ַ
		EEAL=addr&0xff;
		EED=0X00;
		EEC=0X02;	
		j++;								//�˴������޸�
	 	j--;								//�˴������޸�
	  }
//	EAL =fgTemp;	//Recover EA
}	