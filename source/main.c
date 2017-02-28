#include <CS1521.H>
#include "stdio.h"
#include "string.h"	
#include "adc.h"
#include "flash.h"
#include "timer.h"
#include "uart.h"
#include "function.h"
#include "cacul.h"
#include "config.h"	
 
void Correct_Mode(void);

void UART0(void) interrupt K_UART1
{
	if((S1CON&0x01)==0x01){			 	
		S1CON &= 0xfe;
		if (rx_buff_cnt < RX_BUFF_LEN)
		{
			*((unsigned char *)(&ReceiveData)+rx_buff_cnt) = S1BUF;
			 
			if(ReceiveData.Head == 0x7E)
			{
				if(rx_buff_cnt == RX_BUFF_LEN-1)  //计数满一个定长
				{
					if(ReceiveData.Tail==0xAA)
					{
						flag_uart=1;
						rx_buff_cnt=0;
					}
				}
				else
				{
					rx_buff_cnt++;
				}
			}
			else
			{
				rx_buff_cnt=0;
			}  	    	
		   
		}		 
	 }

}

int main()
{	 
#if  LOCK_SELF
	uint16 tmp_sum,vcp_sum,correct_f3;
#endif
//	uint16 vpp;	
//	uchar table[5];	 
//	char ntab[17] = "0123456789ABCDEF";
	uint16 tmp_sum;

    Time_Init();
	delay_ms(1000);
    Power_Init();
    Port_Init();
	Sin_Select(CSIN);
	Uart_Init();

	flag_mode = Flash_Read(FLAG_ADDR);
	
	write_info();
	Iterrup_Init(); 
//	correct_f1 = Flash_Read(Correct_Addr+610-TMP_AD_H);
//	correct_f2 = Flash_Read(Correct_Addr+580-TMP_AD_H);

//	if(flag_mode!=0xff)
	{
	Calc_K_B();
//	delay_ms(1000);
	tmp_sum=Temp_ADC();
	VPP2_Last=Flash_Read(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
	}
	
	rx_buff_cnt= 0;
	flag_uart=0;
	lock_b=0;
	tmp_sum = TMP_AD_H;
	DAC_Output(2000);	
	while(1)
	{
		
//		if(rx_buff_cnt!=0)	USART_SendData(rx_buff_cnt);
//		USART_SendData(0x55);
/*		vpp = Vcp_Collect();
			table[0]=ntab[vpp/1000];
			table[1]=ntab[vpp%1000/100];
			table[2]=ntab[vpp%100/10];
			table[3]=ntab[vpp%10];        	
	        table[4]='\n';
//	        Uart_Send(table,5);
*/
//		delay_ms(500);

		if(flag_uart == 1)
		{
 			uart_op(); 			
		}


		if((flag_mode == 0))
		{        
//			Correct_Mode();	 			
			work_mode();
		}

		else if((flag_mode == 1))
		{
//			Correct_Mode();			
			vpull_mode2();
		}
#if LOCK_SELF		
		if(flag_mode == 0xff)
		{
			//correct_f3=Flash_Read(FLAG_ADDR+1);
			tmp_sum=0;
			vcp_sum=0;
			tmp_sum=Temp_Collect();
			vcp_sum=Vcp_Collect();		
			if((vcp_sum<4000)&&(vcp_sum>200)&&(lock_b==1))
			{
				correct_f3=Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);
				if(correct_f3==0xffff)
				{
					Flash_Write(WORK_STAR_BASE + (tmp_sum-TMP_AD_H)*2,vcp_sum>>8);
					Flash_Write(WORK_STAR_BASE + (tmp_sum-TMP_AD_H)*2+1,vcp_sum&0xff);
				}
			}
 /*
			vpp = Flash_Read(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);
			vpp = vpp*256+Flash_Read(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2+1);
			table[0]=ntab[vpp/1000];
			table[1]=ntab[vpp%1000/100];
			table[2]=ntab[vpp%100/10];
			table[3]=ntab[vpp%10];        	
	        table[4]='\n';
	        Uart_Send(table,5);

			tmp_sum=tmp_sum+1;
			if(tmp_sum == 4096)tmp_sum=0;
*/			
		}
#endif
			
	}
			
}

#if CORRECT_MODE
void Correct_Mode(void)
{
	uchar i;
	uint16 tmp[10];
//	unsigned int table[5];
	uint16 tmp_sum=0,vcp_sum=0,correct_f3,vpp;
	signed int data vpp_PWM,diff_last=0,tmp_last=0,diff=0,correct_y,correct_x;
	
	
	for(i=0;i<10;i++)
		{
			if(flag_uart == 1)
			{
				uart_op();
			}

			tmp[i]=Temp_ADC();
			delay_ms(1);
		}

		Insert_Sort (tmp,10);
		tmp_sum=(unsigned int)((tmp[4]+tmp[5]+tmp[6]+tmp[7])>>2);
#if CORRECT_DEBUG 
		table[0]=ntab[tmp_sum/1000];
		table[1]=ntab[tmp_sum%1000/100];
		table[2]=ntab[tmp_sum%100/10];
		table[3]=ntab[tmp_sum%10];        	
        table[4]='v';
        Uart_Send(table,5);	
#endif
       	correct_f3=Flash_Read(Correct_Addr+tmp_sum-TMP_AD_H);

	    if(correct_f3==0xffff)
        {
	        correct_f3=0;
        if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
       	{
        	vpp=Flash_Read(tmp_sum+WORK_STAR_BASE-TMP_AD_H);
#if CORRECT_DEBUG     	
        	table[0]=ntab[vpp/1000];
			table[1]=ntab[vpp%1000/100];
			table[2]=ntab[vpp%100/10];
			table[3]=ntab[vpp%10];        	
	        table[4]='d';
	        Uart_Send(table,5);
#endif
			while((signed int)(vpp-vcp_sum)>0)  
			{      
				
	
				vpp_PWM=(int)((vpp+diff)*K)+B;
				DAC_Output(vpp_PWM);		

	            delay_ms(600);
	
	            for(i=0;i<10;i++)
				{
					tmp[i]=Vcp_Collect();
					delay_ms(10);
				}
				Insert_Sort(tmp,10);
	#if TEN_BIT
				vcp_sum=(tmp[3]+tmp[4]+tmp[5]+tmp[6])/8;
	#else
	        	vcp_sum=(tmp[3]+tmp[4]+tmp[5]+tmp[6])/2;
	#endif
				diff =diff+vpp-vcp_sum+1;
								
	 		 }	
	 		       
	        	diff =diff+1024;
	        	
	        	
	               
	        	if(first_correct==0)
	        	{
		        	first_correct = 1;
	
	        	}
	        	else //(first_correct==1)
	        	{
	
		        	correct_y=diff-diff_last;
		        	correct_x=tmp_sum-tmp_last;
	
		        	if(((correct_y<-12)||(correct_y>12))&&((correct_x>-3)&&(correct_x<3)))
		        	{
			        	diff=diff_last;
	
		        	}
		   	   	
		#if CORRECT_DEBUG         	
				table[0]=ntab[diff/1000];
				table[1]=ntab[diff%1000/100];
				table[2]=ntab[diff%100/10];
				table[3]=ntab[diff%10];        	
	        	table[4]='t';
	        	Uart_Send(table,5);
	    #else
	    		Flash_Write(Correct_Addr+tmp_sum-TMP_AD_H,diff);
	    #endif  	
	        	}
	        	
		        diff_last=diff;
		       	tmp_last = tmp_sum;
	
	    }
        }
}
#endif
