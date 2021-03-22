/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				exti
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#ifndef _zf_exti_h
#define _zf_exti_h

#include "common.h"
#include "hal_exti.h"
#include "hal_misc.h"
#include "hal_syscfg.h"

void exti_interrupt_init (PIN_enum pin, EXTITrigger_TypeDef trigger, uint8 priority);
void exti_even_init (PIN_enum pin, EXTITrigger_TypeDef trigger);
void exti_interrupt_enable (PIN_enum pin);
void exti_interrupt_disable (PIN_enum pin);
void exti_even_enable (PIN_enum pin);
void exti_even_disable (PIN_enum pin);

#endif
