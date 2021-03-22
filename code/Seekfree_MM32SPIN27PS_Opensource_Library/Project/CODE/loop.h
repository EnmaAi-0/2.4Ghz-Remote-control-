#ifndef __Loop_H_
#define __Loop_H_


#include "headfile.h"



typedef struct
{
	void (*task_func)(void);		//函数指针
	uint32 interval_ticks;			//执行时间间隔
	uint32 last_run;				//最后一次运行时间
}sched_task_t;


void Loop_Run(void);
void Ang_Loop0(void);
void Ang_Loop1(void);
void Ang_Loop2(void);

#endif