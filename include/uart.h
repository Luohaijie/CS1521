#ifndef _UART_H_
#define _UART_H_

 
void Uart_Init(void);
void Uart_Send(unsigned char *send_buff,int len);
void USART_SendData(char date);
void UART_printf(char *fmt, ...);
void printn(unsigned int n, unsigned int b);
void UART_puts(char *pch);

#endif 