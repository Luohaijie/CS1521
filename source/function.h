#ifndef __FUNCTION_H
#define __FUNCTION_H

#define uchar unsigned char data
#define uint16 unsigned int data
#define int16 signed int data

					  
#define FLAG_ADDR		0x4000      //���湤��ģʽ��ַ
#define WORK_STAR_BASE	0x2000      //ѹ�������׵�ַ
#define	TMP_AD_H	    1280			//���£�100�ȣ���Ӧ��AD����ֵ
#define	TMP_AD_L	    3328		//���£�-60�ȣ���Ӧ��AD����ֵ
#define TMP_AD_40L      3300        //���£�-40�ȣ���Ӧ��AD����ֵ
#define TMP_AD_85H      1500         //���£�85�ȣ���Ӧ��AD����ֵ
#define	VCP_AD_H	    4096
#define	VCP_AD_L	    0
#define TMP_ADD_LEN	    4096
#define SN_ADDR			0x4400
#define PR_ADDR			0x4800
#define VR_ADDR			0x4C00
#define PWM_AD_L        0x1800
#define Correct_Flag    0x5000
#define Correct_Addr    0x6000

/********************���鳤��******************/
#define RX_BUFF_LEN	    262
#define TX_BUFF_LEN	    262

/********************������********************/
#define READ_PR         6
#define READ_SN         7
#define READ_VR         8
#define WRITE_SN        16
#define ERASE_FLASH     17
#define WRITE_MODE      18
#define READ_CP		    19
#define WRITE_FLASH     20
#define READ_FLASH      21
#define READ_MODE   	25


typedef struct package{
    unsigned char Head;
    unsigned char Commond;
    unsigned char Error;
    unsigned char Frame;
    unsigned char Null;
    unsigned char Data[256];
    unsigned char Tail;
}Package;

extern Package ReceiveData;


/*
signed int vcp_sum;
unsigned int tmp_sum;
int vpp_f;
unsigned int flag;
signed int diff,diff_last;
signed int correct_x,correct_y;
unsigned int correct_f1,correct_f2,correct_f3;
unsigned int tmp_last=0;
unsigned int  tmp_sumChange=0;

unsigned int  tmp_sum_last[4]={0xffff,0xffff,0xffff,0xffff};
char ntab[17] = "0123456789ABCDEF";
unsigned int  vpull_sum;
unsigned int  vpull_sum1[2];
unsigned int  vcp[10];
unsigned int  tmp[10];
//unsigned int  vpull1[66];
signed int  vpp;
signed int 	  vpp_PWM;
signed int  vpp_Last=0;
char TEM_Change=1;
char TEM_CNT=0;
signed int  vpp1;
signed int  VPP2_Last;
signed int  vpp2;
signed int  vpp4;

signed int  vpull;
signed int  vpp5;
unsigned int  vcp_flash;
//unsigned int  tmp_flash;

//unsigned char Rx_buff[RX_BUFF_LEN];

unsigned int PWM_UP,PWM_LOW;
unsigned int PWM_UP_H,PWM_UP_L,PWM_LOW_H,PWM_LOW_L;
unsigned int PWM_flag ;		//�ߵ͵�ƽ��־λ
//unsigned int CALC_PWM;
//unsigned char byte;

*/
extern uchar lock_b;
extern float data K;
extern int16 Badd;
extern uchar first_time;//�Ƿ��һ������
extern uchar first_correct;//�Ƿ��һ������
extern uchar flag_uart;
extern uint16 flag_mode;
extern uint16 rx_buff_cnt;
extern uint16 VPP2_Last;

extern void work_mode(void);
extern void vpull_mode(void);
extern void vpull_mode2(void);
extern void uart_op(void);
extern void write_info(void);

static void ReadCp(void);
static void WriteFlash(void);
static void ReadFlash(void);
static void ReadMode(void);
static void ReadPr(void);
static void ReadSn(void);
static void WriteSn(void);
static void ReadVr(void);
static void write_flash(void);
static void read_flash(void);
extern void erase_flash(void);
static void write_flag(void);

#endif
