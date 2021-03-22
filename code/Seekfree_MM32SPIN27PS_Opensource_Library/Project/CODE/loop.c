#include "loop.h"
#include "isr.h"


//系统任务配置，创建不同执行周期的“线程”
static sched_task_t sched_tasks[] = 
{
	{Ang_Loop0 ,    5, 0},
	{Ang_Loop1 ,   10, 0},
	{Ang_Loop2 ,  100, 0}
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
	
}

void Ang_Loop2(void)
{
	
}