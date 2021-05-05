#ifndef __Loop_H_
#define __Loop_H_


#include "headfile.h"



typedef struct
{
	void (*task_func)(void);		//函数指针
	uint32 interval_ticks;			//执行时间间隔
	uint32 last_run;				//最后一次运行时间
}sched_task_t;

//定义结构体,用于发送float数据
//https://blog.csdn.net/ls667/article/details/50811519博客
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