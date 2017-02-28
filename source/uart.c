#include <CS1521.H>
#include "uart.h"
#include "function.h"
#include "timer.h"

void Uart_Send(unsigned char *send_buff,uint16 len )
{
	uint16 i;
	for(i=0;i<len;i++)
	{
		USART_SendData(send_buff[i]);
	}
}

void USART_SendData(char data date)
{
	
	S1BUF = date;
	while((S1CON & 0x02) == 0x00);
	S1CON &= 0xfd;	
}

void Uart_Init(void)
{
	uint16 uartbd = 970;		   //fclk = 10264092hz, buad = 19200
	S1CON=0x90;			//UART1
	S1RELH = uartbd>>8;		   //����������
	S1RELL = uartbd&0xff;
    delay_ms(200);	//wait Boud rate overflow then load S1RELH,S1RELL
	
}
/*
void UART_puts(char *pch)
{
	while(*pch != '\0')
	{
		USART_SendData(*pch);
		pch++;
	}
}

void printn(unsigned int n, unsigned int b)
{
	unsigned char ntab[17] = "0123456789ABCDEF";
	unsigned int a, m;
	if (n / b)
	{
		a = n / b;
		printn(a, b);  
	}
	m = n % b;
	USART_SendData(ntab[m]);
}
*/	   
/***********************************************************************************************
 ??:UART ?????
 ??:fmt ???????          
 ??:0
 ??:???C?????UART_printf ????? %d %l %o %x %s
************************************************************************************************/
/*
void UART_printf(char *fmt, ...)
{
    char c;
    unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:
    while((c = *fmt++) != '%')
		{
        if (c == '\0') return;
        USART_SendData(c);
    }
    c = *fmt++;
    if (c == 'd' || c == 'l')
		{
        printn(*adx, 10);
    }
    if (c == 'o' || c == 'x')
		{
        printn(*adx, c=='o'? 8:16 );
    }
    if (c == 's')
		{
			UART_puts((char*)*adx);
    }
    adx++;
    goto _loop;
}
 */ 