#include <CS1521.H>
#include "function.h" 
#include "adc.h"
#include "cacul.h"
#include "flash.h"
#include "uart.h"
#include "timer.h" 
#include "config.h"
#include "string.h"

uchar lock_b;
float data K;
int16 Badd;
uchar first_time;//�Ƿ��һ������
uchar first_correct;//�Ƿ��һ������
uchar flag_uart;
uint16 flag_mode;
uint16 rx_buff_cnt;
uint16 VPP2_Last;
Package ReceiveData;


void work_mode(void)
{
#if CORRECT_MODE
	signed int data vpp2;
#endif 	
	uchar i;
	uint16 tmp[10];
	static unsigned int tmp_sum_last[5]={0xffff,0xffff,0xffff,0xffff,0xffff};
	uint16 tmp_sum;
	signed int data vpp1,vpp_PWM;

//	uchar table[5];	 
//	char ntab[17] = "0123456789ABCDEF";

	for(i=0;i<10;i++)
	{
		if(flag_uart == 1)
		{
				return;
		}

		tmp[i]=Temp_ADC();

	}

	Insert_Sort (tmp,10);
	tmp_sum=(unsigned int)((tmp[4]+tmp[5]+tmp[6]+tmp[7])>>2);
	if(tmp_sum_last[0]!=0xffff)
	{
		tmp_sum=(tmp_sum+tmp_sum_last[0]+tmp_sum_last[1]+tmp_sum_last[2]+tmp_sum_last[3])/5;
	}
	if((tmp_sum<TMP_AD_40L )&&(tmp_sum>TMP_AD_85H))
	{
		tmp_sum_last[0]=tmp_sum_last[1];
		tmp_sum_last[1]=tmp_sum_last[2];
		tmp_sum_last[2]=tmp_sum_last[3];
		tmp_sum_last[3]=tmp_sum;
	}
	else
	{
		tmp_sum_last[0]=0xffff;
	}	
	if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
	{
		vpp1=Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);
	
	
#if CORRECT_MODE
		vpp2=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
		if(((vpp2-VPP2_Last)>5)||((vpp2-VPP2_Last)<-5))
		{
			vpp2=VPP2_Last;
		}
		
	UART_printf("  Vpp2: ");
	printn(vpp2,10);
		vpp1=vpp1+vpp2-1024;
		VPP2_Last=vpp2;
#endif			
	 vpp_PWM=(int)(K*vpp1)+Badd;	
	 
	}

/*  
	UART_printf("  Temprature: ");
	printn(tmp_sum,10);

	UART_printf("  Vpp: ");
	printn(vpp1,10);

	UART_printf("  vpp_PWM: ");
	printn(vpp_PWM,10);
 	UART_printf("  \n"); 

	UART_printf("  Tmp: ");
	printn(tmp_sum,10);
	UART_printf("  Vpp: ");
	printn(vpp_PWM,10);
	UART_printf("  \n");
*/
	if(vpp_PWM<2)vpp_PWM=2;
	else if(vpp_PWM>4095)vpp_PWM=4095;
	DAC_Output(vpp_PWM);	
	delay_ms(200);
 /*	  
	vpp1 = Vcp_Collect();
	UART_printf("  vcp: ");
	printn(vpp1,10);
 	UART_printf("  \n");
*/	
	

	
/*
	 	table[0]=ntab[tmp_sum/1000];
		table[1]=ntab[tmp_sum%1000/100];
		table[2]=ntab[tmp_sum%100/10];
		table[3]=ntab[tmp_sum%10];
		UART_printf("temperature= ");
		Uart_Send(table,4);

		table[0]=ntab[vpp/1000];
		table[1]=ntab[vpp%1000/100];
		table[2]=ntab[vpp%100/10];
		table[3]=ntab[vpp%10];
		UART_printf(" ,  Flash= ");
		Uart_Send(table,4);
		
	    for(i=0;i<10;i++)
		{
			vcp[i]=Vcp_Collect();
			delay_ms(10);
		}
		Insert_Sort(vcp,10);
#if TEN_BIT
		vcp_sum=(vcp[3]+vcp[4]+vcp[5]+vcp[6])/8;
#else
        vcp_sum=(vcp[3]+vcp[4]+vcp[5]+vcp[6])/2;
#endif
		table[0]=ntab[vcp_sum/1000];
		table[1]=ntab[vcp_sum%1000/100];
		table[2]=ntab[vcp_sum%100/10];
		table[3]=ntab[vcp_sum%10];
		UART_printf(" ,  VCP= ");
		Uart_Send(table,4);
		UART_printf("\n");
*/
}

/***************************************
���룺��
������ɼ����10λ�¶�
���ܣ��ɼ����Ծ�����¶ȶ�Ӧ�ĵ�ѹֵ,�õ�������
****************************************/
void vpull_mode2(void) 
{
#if CORRECT_MODE
	signed int data vpp2;
#endif
	uchar i;
    unsigned int vpull1[66]=0;
	uint16 tmp[10];
	static unsigned int tmp_sum_last[5]={0xffff,0xffff,0xffff,0xffff,0xffff};
	int16 tmp_sum;
	long vpull_sum;
	signed int data vpp1,vpp_PWM,vpull;

//	uchar table[5];	 
//	char ntab[17] = "0123456789ABCDEF";

	for(i=0;i<8;i++)
	{
		if(flag_uart == 1)
		{
			return;
		}
		tmp[i]=Temp_ADC();
	}

	Insert_Sort (tmp,8);
	tmp_sum=((tmp[2]+tmp[3]+tmp[4]+tmp[5])>>2);
	if(tmp_sum>4095)
	{
		tmp_sum=4095;
	}
	if(tmp_sum<=0)
	{
		tmp_sum=0;
	}


  
	for(i=0;i<32;i++)
	{
		if(flag_uart == 1)
		{
			return;
		}

		vpull1[i]=Vpull_Collect();

	}
	Insert_Sort (vpull1,32); 
	vpull_sum=0;	
	for(i=16;i<20;i++)//��β�������ݲ�������������ƫ��Ӧ��
	{ 
	    vpull_sum=vpull_sum+vpull1[i];
	}
	vpull_sum = vpull_sum/4; 	
	if(vpull_sum>4095)
	{
		vpull_sum=4095;
	}

	vpull=(signed int)vpull_sum-2048;

	if(tmp_sum_last[0]!=0xffff)
	{
		tmp_sum=(tmp_sum+tmp_sum_last[0]+tmp_sum_last[1]+tmp_sum_last[2]+tmp_sum_last[3])/5;
	}
	// ƽ���¶Ȳ���ֵ
	// �����¶��²�ƽ���¶�
	if((tmp_sum<TMP_AD_40L )&&(tmp_sum>TMP_AD_85H))
	{
		tmp_sum_last[0]=tmp_sum_last[1];
		tmp_sum_last[1]=tmp_sum_last[2];
		tmp_sum_last[2]=tmp_sum_last[3];
		tmp_sum_last[3]=tmp_sum;
	}
	else
	{
		tmp_sum_last[0]=0xffff;
	}

	
    if((tmp_sum<TMP_AD_L)&&(tmp_sum>TMP_AD_H ))
    {
	    vpp1=Flash_Read_Int(WORK_STAR_BASE+(tmp_sum-TMP_AD_H)*2);
#if CORRECT_MODE
		vpp2=Flash_Read_Int(Correct_Addr+(tmp_sum-TMP_AD_H)*2);
		if(((vpp2-VPP2_Last)>5)||((vpp2-VPP2_Last)<-5))
		{
			vpp2=VPP2_Last;
		}
		
		vpp1=vpp1+vpp2-1024;
		VPP2_Last=vpp2;
#endif
	}  
	vpp_PWM=vpp1+vpull;
	
	vpp_PWM=(int)(K*vpp_PWM)+Badd;

	if(vpp_PWM<2)vpp_PWM=2;
	else if(vpp_PWM>4095)vpp_PWM=4095;
	DAC_Output(vpp_PWM);
	delay_ms(500); 

}
/*****************��CP��ѹ*************************/
static void ReadCp(void)
{
	int16 tmp_sum,vcp_sum;
    lock_b = 1;   

    tmp_sum=Temp_Collect();	
	vcp_sum=Vcp_Collect();     
    ReceiveData.Data[0] = tmp_sum>>8;
    ReceiveData.Data[1] = tmp_sum;
    ReceiveData.Data[2] = vcp_sum>>8;
    ReceiveData.Data[3] = vcp_sum;    
}
/*****************дFLASHѹ������*******************/
static void WriteFlash(void)
{   
    uchar Fream_cnt;
    uchar  flash_CP;
    uint16 i;
    Fream_cnt = ReceiveData.Frame;
 	  
    for(i = 0 ; i < 256 ; i++)
    {
        flash_CP = ReceiveData.Data[i];        
        Flash_Write(WORK_STAR_BASE + i+(Fream_cnt*256),flash_CP);	
    }
/*		 
	 for(i = 0 ; i < 2048 ; i++)
    {	
	    flash_CP=flash_CP;      
        Flash_Write(WORK_STAR_BASE + i*2,0x08);
		Flash_Write(WORK_STAR_BASE + i*2+1,0x98);		
    }
	Flash_Erase(FLAG_ADDR);
	delay_ms(10);  		
	Flash_Write(FLAG_ADDR,0x00);
	flag_mode=0;
 */		
}
/*****************��ѹ������*******************/
static void ReadFlash(void)
{      
//    uchar k;
    uint16 i;    
    uchar  flash_CP,Fream_cnt;
    
	Fream_cnt = ReceiveData.Frame;
    for(i = 0 ; i < 256 ; i++ )
    {							 
        flash_CP=Flash_Read(WORK_STAR_BASE+(Fream_cnt*256)+i); 			
        ReceiveData.Data[i] = flash_CP;            			               
    }
//	ReceiveData.Null = k;	       
    ReceiveData.Error = 0;
    Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);	         
}		
/********��ģʽ--0����ģʽ����0����ģʽ********/
static void ReadMode(void)
{
    
    ReceiveData.Data[1] = 0xff;
	ReceiveData.Data[2] = Flash_Read(FLAG_ADDR);      	
}
/*****************����Ʒ     *******************/
static void ReadPr(void)
{   
	uchar i; 
    for(i = 0 ; i < 7; i++ )
    {
        ReceiveData.Data[i] = 	Flash_Read(PR_ADDR+i);
    }
}
/*****************��SN     *******************/
static void ReadSn(void)
{    
	uchar i;
    for(i=0;i<12;i++)
    {
        ReceiveData.Data[i]= Flash_Read(SN_ADDR+i);         
    }
}		
/*****************дSN*******************/
static void WriteSn(void)
{
	uchar i;
    Flash_Erase(SN_ADDR);
    for(i = 0 ; i < 12 ; i++)
    {         
        Flash_Write(SN_ADDR + i,ReceiveData.Data[i]);        
    }
}
/*****************������汾     *******************/
static void ReadVr(void)
{
	uchar i;
    for(i = 0 ; i < 7; i++ )
    {
        ReceiveData.Data[i] = Flash_Read(VR_ADDR+i);
    }
}
/***************************************
���룺��
�������
���ܣ��������ڶ��������ݲ���������
****************************************/
void uart_op(void)
{
	uchar tx_flag;	
	if(flag_uart==1)
	{
        switch (ReceiveData.Commond){
            case READ_CP:
                ReadCp();
                tx_flag=1;
                break;
            case WRITE_FLASH:
                WriteFlash();
                tx_flag=1;
                break;
            case READ_FLASH:
                ReadFlash();
                break;
            case ERASE_FLASH:
                erase_flash();
                tx_flag=1;
                break;
            case WRITE_MODE:
                write_flag();
                tx_flag=1;
                break;
            case READ_MODE:
                ReadMode();
                tx_flag=1;
                break;
            case READ_PR:
                ReadPr();
                tx_flag=1;
                break;
            case READ_VR:
                ReadVr();
                tx_flag=1;
                break;
            case READ_SN:
                ReadSn();
                tx_flag=1;                
                break;
            case WRITE_SN:
                WriteSn();
                tx_flag=1;
                break;
            default :
                tx_flag=2;
                break;   
        }
/*****************��������*******************/
        if(tx_flag==1)
        {
            tx_flag=0;
			ReceiveData.Error = 0;
        	Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);

        }
        else if(tx_flag==2)
        {
            tx_flag=0;
            ReceiveData.Head = 0x7E;
            ReceiveData.Tail = 0xAA;
            ReceiveData.Error = 1;
            Uart_Send((unsigned char *)(&ReceiveData),RX_BUFF_LEN);
        }
         
			
	}
	flag_uart=0;
	memset((unsigned char *)(&ReceiveData),0x00,RX_BUFF_LEN);
}


/***************************************
���룺��
�������
���ܣ���������������������
����16�е�����4*1024=4096����ַ��
****************************************/
 void  erase_flash(void)
{
		uchar i;
		EAL = 0;
		for(i=0;i<8;i++)
		{			
			Flash_Erase(WORK_STAR_BASE+i*1024);
			delay_ms(1);				
		}

		Flash_Erase(FLAG_ADDR);
		delay_ms(1);
		EAL = 1;
		ReceiveData.Error = 0;    //�ɹ���־
		
}
/***************************************
���룺��
�������
���ܣ�ͨ����дFLASH���ù���ģʽ
****************************************/
static void write_flag(void)
{
	if(ReceiveData.Data[2]==0x00)  //����ģʽ
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);  		
		Flash_Write(FLAG_ADDR,0x00);
		ReceiveData.Error=0;
		flag_mode=0;
	}
	else if(ReceiveData.Data[2]==0xFF)  //����ģʽ
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);
		Flash_Write(FLAG_ADDR,0xFF);		
		ReceiveData.Error=0;
		flag_mode=0xff;
	}
	else if (ReceiveData.Data[2]==0x01)  //��ƫģʽ
	{
		Flash_Erase(FLAG_ADDR);
		delay_ms(10);
		Flash_Write(FLAG_ADDR,0x01); 				
		ReceiveData.Error=0;
		flag_mode=1;
	}
	else
	{
		ReceiveData.Error=1;
	}
}
void write_info(void)
{	 
	uchar PR[7] = "CS1521X";
	uchar VR[7] = "Z.00.04";
	uchar i;

    Flash_Erase(PR_ADDR);
	Flash_Erase(VR_ADDR);
	for(i=0;i<7;i++)
	{
		Flash_Write(PR_ADDR+i,PR[i]);
		Flash_Write(VR_ADDR+i,VR[i]);			
	}

}
