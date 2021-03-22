#include "loop.h"
#include "isr.h"


//ϵͳ�������ã�������ִͬ�����ڵġ��̡߳�
static sched_task_t sched_tasks[] = 
{
	{Ang_Loop0 ,    5, 0},
	{Ang_Loop1 ,   10, 0},
	{Ang_Loop2 ,  100, 0}
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
	
}

void Ang_Loop2(void)
{
	
}