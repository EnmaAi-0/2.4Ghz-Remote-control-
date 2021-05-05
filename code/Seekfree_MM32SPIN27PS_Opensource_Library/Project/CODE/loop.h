#ifndef __Loop_H_
#define __Loop_H_


#include "headfile.h"



typedef struct
{
	void (*task_func)(void);		//����ָ��
	uint32 interval_ticks;			//ִ��ʱ����
	uint32 last_run;				//���һ������ʱ��
}sched_task_t;

//����ṹ��,���ڷ���float����
//https://blog.csdn.net/ls667/article/details/50811519����
typedef union     
{
	float fdata;
	unsigned long ldata;
}FloatLongType;


void Loop_Run(void);
void Ang_Loop0(void);
void Ang_Loop1(void);
void Ang_Loop2(void);

#endif