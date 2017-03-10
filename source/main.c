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
//	uint16 tmp_sum;

    Time_Init();
	delay_ms(1000);
    Power_Init();
    Port_Init();
	Sin_Select(CMOS);
	Uart_Init();

	flag_mode = Flash_Read(FLAG_ADDR);
	
	write_info();
	Iterrup_Init(); 
//	correct_f1 = Flash_Read(Correct_Addr+610-TMP_AD_H);
//	correct_f2 = Flash_Read(Correct_Addr+580-TMP_AD_H);

//	if(flag_mode!=0xff)
	{
	delay_ms(2000);
	Calc_K_B();
//	delay_ms(1000);
	tmp_sum=Temp_Collect();
	VPP2_Last=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
	
	}

	first_correct = 0;
	rx_buff_cnt= 0;
	flag_uart=0;
	lock_b=0;
//	tmp_sum = TMP_AD_H;
	DAC_Output(2000);	
	while(1)
	{
		
//		USART_SendData(0x55);
/*		tmp_sum = Temp_Collect();
		UART_printf("  Temp_ADC: ");
		printn(tmp_sum,10);
	 	UART_printf("  \n"); 
 */
		if(flag_uart == 1)
		{
 			uart_op(); 			
		}


		if((flag_mode == 0))
		{
#if CORRECT_MODE		        
			Correct_Mode();	
#endif			 			
			work_mode();
		}

		else if((flag_mode == 1))
		{
#if CORRECT_MODE
			Correct_Mode();	
#endif		
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

			tmp[i]=Temp_Collect();			
		}

		Insert_Sort (tmp,10);
		tmp_sum=(unsigned int)((tmp[4]+tmp[5]+tmp[6]+tmp[7])>>2);

       	correct_f3=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
#if CORRECT_DEBUG
				UART_printf("  correct_f3: ");
				printn(correct_f3,10);
#endif

	    if(correct_f3==0xffff)
        {
	        correct_f3=0;
        if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
       	{
        	vpp=Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);

			while((signed int)(vpp-vcp_sum)>0)  
			{      
				vpp_PWM=(int)((vpp+diff)*K)+Badd;
				DAC_Output(vpp_PWM);		

	            delay_ms(300);
	
	            for(i=0;i<10;i++)
				{
					tmp[i]=Vcp_Collect();
					delay_ms(10);
				}
				Insert_Sort(tmp,10);

	        	vcp_sum=(tmp[3]+tmp[4]+tmp[5]+tmp[6])/4; 		 	

				diff =diff+vpp-vcp_sum+1;
								
	 		 }	
	 		       
	        	diff =diff+1024;
	        	if(first_correct==0)
	        	{
		        	first_correct = 1;
					VPP2_Last = diff;
	        	}
	        	else //(first_correct==1)
	        	{
	
		        	correct_y=diff-diff_last;
		        	correct_x=tmp_sum-tmp_last;
	
		        	if(((correct_y<-20)||(correct_y>20))&&((correct_x>-3)&&(correct_x<3)))
		        	{
			        	diff=diff_last;
	
		        	}
#if CORRECT_DEBUG
				UART_printf("  diff: ");
				printn(diff,10);
			 	UART_printf("  \n"); 
#endif
	    		Flash_Write(Correct_Addr+(tmp_sum-TMP_AD_H)*2,diff>>8);
				Flash_Write(Correct_Addr+(tmp_sum-TMP_AD_H)*2+1,diff&0xff);
	    	
	        	}
	        	
		        diff_last=diff;
		       	tmp_last = tmp_sum;
	
	    }
        }
}
#endif
