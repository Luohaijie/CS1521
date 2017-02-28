
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
函数名：Flash读
输入：要读的地址
输出：无
函数功能：从Flash指定的地址读出相应的数据，并放到相应变量中
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
函数名：Flash写
输入：要写入的地址和数据
输出：无
函数功能：给Flash指定的地址写入相应的数据
 ****************************************************************/

void Flash_Write(uint16 addr,uchar dat)
{	     
    	uchar j=0;
		EEAH=addr>>8;				//设置高8位地址
		EEAL=addr&0xff;			//低8位地址
		EED=dat;
		EEC = 0X01;	
	    j++;								//此处不可修改
		j--;								//此处不可修改
	
		
		
}

/****************************************************************
函数名：Flash擦出
输入：要擦除的地址
输出：无
函数功能：擦除Flash制定位置
****************************************************************/	
void Flash_Erase(uint16 addr)
{
//	bit fgTemp =EAL;			//Backup EA

//	EAL =0;	//Closed interrupt when operation Flash
	if((addr%1024)==0) {   
		uchar j=0;
		EEAH=addr>>8;				//设置擦除扇区地址
		EEAL=addr&0xff;
		EED=0X00;
		EEC=0X02;	
		j++;								//此处不可修改
	 	j--;								//此处不可修改
	  }
//	EAL =fgTemp;	//Recover EA
}	