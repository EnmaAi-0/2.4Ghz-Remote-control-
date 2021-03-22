/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				pit
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#ifndef _zf_pit_h
#define _zf_pit_h

#include "common.h"
#include "zf_tim.h"

void tim_interrupt_init (TIM_enum tim, uint32 freq, uint8 priority);
void tim_interrupt_init_us (TIM_enum tim, uint32 timer, uint8 priority);
void tim_interrupt_init_ms (TIM_enum tim, uint32 timer, uint8 priority);
void tim_interrupt_enable (TIM_enum tim);
void tim_interrupt_disable (TIM_enum tim);

#endif
