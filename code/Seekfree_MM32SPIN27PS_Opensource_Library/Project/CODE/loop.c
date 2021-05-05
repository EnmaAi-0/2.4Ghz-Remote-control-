#include "loop.h"
#include "isr.h"

uint8 vodka_i=0;

/*
��������fת��Ϊ4���ֽ����ݴ����byte[4]��
ԭ��ο�CSDN�����ﲻ������
https://blog.csdn.net/ls667/article/details/50811519
*/
void Float_to_Byte(float f,uint8 byte[])   
{
	FloatLongType fl;
	fl.fdata=f;
	byte[0]=(unsigned char)fl.ldata;
	byte[1]=(unsigned char)(fl.ldata>>8);
	byte[2]=(unsigned char)(fl.ldata>>16);
	byte[3]=(unsigned char)(fl.ldata>>24);
}


//ϵͳ�������ã�������ִͬ�����ڵġ��̡߳�
static sched_task_t sched_tasks[] = 
{
	{Ang_Loop0 ,    5, 0},
	{Ang_Loop1 ,   10, 0},
	{Ang_Loop2 ,   50, 0}
};

//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

void Loop_Run(void)
{
	uint8 index = 0;
	
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��
	for(index=0;index < TASK_NUM;index++)
	{
					//����ʱ�ӣ��ɶ�ʱ������
		uint32 tnow = SysTickTime;
					//�ж�ִ��ʱ��
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
					//�����̵߳�ִ��ʱ��
			sched_tasks[index].last_run = tnow;
					//ִ���̺߳���
			sched_tasks[index].task_func();
		}
	}
}



void Ang_Loop0(void)
{
	
}


void Ang_Loop1(void)
{
		//��һ������ҡ�˷�Χ: 0 - 99
		Rocker_LX=(4096-adc_convert(ADC_1, ADC1_CH02_A02))/40.95f;
		Rocker_LY=(4096-adc_convert(ADC_1, ADC1_CH00_A00))/40.95f;
		Rocker_RX=adc_convert(ADC_1, ADC1_CH05_A05)/40.95f;
		Rocker_RY=adc_convert(ADC_1, ADC1_CH03_A03)/40.95f;
		//��ص�ѹ
		Battery_Power=(float)adc_convert(ADC_1, ADC1_CH06_A06) * 0.0046385f;
	
	
		NRF_RX_Buff[0] = 7;  		//���ݷ���λ��
		NRF_RX_Buff[1] = 0XFF;  
		NRF_RX_Buff[2] = 0XFE;  
		NRF_RX_Buff[3] = Rocker_LX;
		NRF_RX_Buff[4] = Rocker_LY;
		NRF_RX_Buff[5] = Rocker_RX;
		NRF_RX_Buff[6] = Rocker_RY;
	
		switch(Button_Press())		//���°���������ָ��λ
		{
			case 1: 
				NRF_RX_Buff[7] = 1 ;		//����
				break;
			case 2: 
				NRF_RX_Buff[7] = 3 ;
				break;			
			case 3:	
				break;	
			case 4:
				break;
		}
	NRF_Send_Packet(NRF_RX_Buff);
}

void Ang_Loop2(void)
{
		oled_int16(0,0,Rocker_LX);
		oled_int16(1,1,Rocker_LY);
		oled_int16(2,2,Rocker_RX);
		oled_int16(3,3,Rocker_RY);
		oled_printf_float(4,4,Battery_Power,2,2);

}