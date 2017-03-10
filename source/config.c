#include <CS1521.H>
#include "config.h"


void Port_Init(void)
{

	P0FSR= 0x82;         // P0 functions selection IO	 1000 0010  PWM+TX
	P0PU = 0x10;        // pullup
	P0OD = 0x00;    	// open drain
	P0IN = 0x11;         // shimit shutdown
    P0IO = 0x7d;    	// direction ,0111 1101 output
	P0 = 0x10;

}

void Time_Init(void)
{
	CLKC = 0x00;             //FOSC = HIRC/2, 16.6MHz
}

void Iterrup_Init(void)
{

	IEN2 |= 0x01;			//串口中断 ？

	EAL  = 1; 				//开中断

}

void Power_Init(void)
{
	CTRL = 0x04;            //0000 0100  vreg = 3V	adc0上拉使能
}

void Sin_Select(char SEL)	 //SEL = CMOS or CSIN
{
	SIN = SEL;
}