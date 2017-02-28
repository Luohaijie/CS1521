#ifndef __FUNCTION_H
#define __FUNCTION_H
					  
#define FLAG_ADDR		0x1000      //���湤��ģʽ��ַ
#define WORK_STAR_BASE	0x1020      //ѹ�������׵�ַ
#define	TMP_AD_H	    256			//���£�100�ȣ���Ӧ��AD����ֵ
#define	TMP_AD_L	    768			//���£�-60�ȣ���Ӧ��AD����ֵ
#define TMP_AD_40L       600        //���£�-40�ȣ���Ӧ��AD����ֵ
#define TMP_AD_85H       370        //���£�85�ȣ���Ӧ��AD����ֵ
#define	VCP_AD_H	    1024
#define	VCP_AD_L	    0
#define TMP_ADD_LEN	    512
#define SN_ADDR			0x1240
#define PR_ADDR			0x1260
#define VR_ADDR			0x1280
#define PWM_AD_L        0x1800
#define Correct_Flag 0x12A0
#define Correct_Addr 0x12C0

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


struct package{
    unsigned char Head;
    unsigned char Commond;
    unsigned char Error;
    unsigned char Frame;
    unsigned char Null;
    unsigned char Data[256];
    unsigned char Tail;
};

struct package ReceiveData;

signed int vcp_sum;
unsigned int tmp_sum;
int vpp_f;
unsigned int flag;
signed int diff,diff_last;
signed int correct_x,correct_y;
unsigned int correct_f1,correct_f2,correct_f3;
unsigned int tmp_last=0;
unsigned int  tmp_sumChange=0;
unsigned char first_time=0;//�Ƿ��һ������
unsigned char first_correct=0;//�Ƿ��һ������
unsigned int  tmp_sum_last[4]={0xffff,0xffff,0xffff,0xffff};
//char ntab[17] = "0123456789ABCDEF";
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
float K;
int B;
signed int  vpull;
signed int  vpp5;
unsigned int  vcp_flash;
//unsigned int  tmp_flash;
unsigned char flag_uart=0;
unsigned int flag_mode;
//unsigned char Rx_buff[RX_BUFF_LEN];
unsigned char rx_buff_cnt;
unsigned int PWM_UP,PWM_LOW;
unsigned int PWM_UP_H,PWM_UP_L,PWM_LOW_H,PWM_LOW_L;
unsigned int PWM_flag ;		//�ߵ͵�ƽ��־λ
unsigned int CALC_PWM;
unsigned char byte;
unsigned char lock_b;

extern void collect_mode(void);
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
static void erase_flash(void);
static void write_flag(void);

#endif
