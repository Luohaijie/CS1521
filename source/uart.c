#include <CS1521.H>
#include "uart.h"
#include "function.h"
#include "timer.h"
#include "config.h"

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
	S1RELH = uartbd>>8;		   //²¨ÌØÂÊÉèÖÃ
	S1RELL = uartbd&0xff;
    delay_ms(200);	//wait Boud rate overflow then load S1RELH,S1RELL

}
#if UART_PRINTF
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
	unsigned char tab[11];
	signed char i=0;
	if(n==0) USART_SendData(ntab[0]);
	while (n)
	{
		i++;
		if(i==11)i=0;
		tab[i]=ntab[n%b];
		n = n / b;

	}
	while(i>0)
	{							   
		USART_SendData(tab[i]);
		i--;
	}

}


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
#endif
