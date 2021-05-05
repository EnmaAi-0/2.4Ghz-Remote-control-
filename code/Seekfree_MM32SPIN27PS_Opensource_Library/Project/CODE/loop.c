#include "loop.h"
#include "isr.h"

uint8 vodka_i=0;

/*
将浮点数f转化为4个字节数据存放在byte[4]中
原理参考CSDN，这里不做解释
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


//系统任务配置，创建不同执行周期的“线程”
static sched_task_t sched_tasks[] = 
{
	{Ang_Loop0 ,    5, 0},
	{Ang_Loop1 ,   10, 0},
	{Ang_Loop2 ,   50, 0}
};

//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

void Loop_Run(void)
{
	uint8 index = 0;
	
	//循环判断所有线程，是否应该执行
	for(index=0;index < TASK_NUM;index++)
	{
					//心跳时钟，由定时器产生
		uint32 tnow = SysTickTime;
					//判断执行时间
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
					//更新线程的执行时间
			sched_tasks[index].last_run = tnow;
					//执行线程函数
			sched_tasks[index].task_func();
		}
	}
}



void Ang_Loop0(void)
{
	
}


void Ang_Loop1(void)
{
		//归一化处理，摇杆范围: 0 - 99
		Rocker_LX=(4096-adc_convert(ADC_1, ADC1_CH02_A02))/40.95f;
		Rocker_LY=(4096-adc_convert(ADC_1, ADC1_CH00_A00))/40.95f;
		Rocker_RX=adc_convert(ADC_1, ADC1_CH05_A05)/40.95f;
		Rocker_RY=adc_convert(ADC_1, ADC1_CH03_A03)/40.95f;
		//电池电压
		Battery_Power=(float)adc_convert(ADC_1, ADC1_CH06_A06) * 0.0046385f;
	
	
		NRF_RX_Buff[0] = 7;  		//数据发送位数
		NRF_RX_Buff[1] = 0XFF;  
		NRF_RX_Buff[2] = 0XFE;  
		NRF_RX_Buff[3] = Rocker_LX;
		NRF_RX_Buff[4] = Rocker_LY;
		NRF_RX_Buff[5] = Rocker_RX;
		NRF_RX_Buff[6] = Rocker_RY;
	
		switch(Button_Press())		//按下按键，特殊指令位
		{
			case 1: 
				NRF_RX_Buff[7] = 1 ;		//解锁
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