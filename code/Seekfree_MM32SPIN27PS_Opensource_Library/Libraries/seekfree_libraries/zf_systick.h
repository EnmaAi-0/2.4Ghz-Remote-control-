/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				systick
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#ifndef _zf_systick_h
#define _zf_systick_h

#include "common.h"

#define DELAY_CLK SystemCoreClock

#define USEC_TO_COUNT(us, clockFreqInHz)		(uint64_t)(((uint64_t)(us) * (clockFreqInHz)) / 1000000U)
#define COUNT_TO_USEC(count, clockFreqInHz)		(uint64_t)((uint64_t)count * 1000000U / clockFreqInHz)

#define MSEC_TO_COUNT(ms, clockFreqInHz)		(uint64_t)((uint64_t)ms * clockFreqInHz / 1000U)
#define COUNT_TO_MSEC(count, clockFreqInHz)		(uint64_t)((uint64_t)count * 1000U / clockFreqInHz)

//------------------------------------���º궨��������ʱ------------------------------------
void systick_delay (uint32 time);
void systick_delay_ms(uint32 ms);														// ���뼶��ʱ���� ���ڵδ�ʱ�����ֻ��24λ������ú���ʵ�֣�������ʱʱ�䷶Χ�Ͽ�
#define systick_delay_us(time)		systick_delay(USEC_TO_COUNT(time, DELAY_CLK))		// ������ʱʱ�� ��λus ��Χ0 - 16777215(0xffffff)/(DELAY_CLK*1000)ms	��Ƶ96M systick�����ʱʱ���ԼΪ33ms
#define systick_delay_ns(time)		systick_delay(USEC_TO_COUNT(time, DELAY_CLK/1000))	// ������ʱʱ�� ��λns ��Χ0 - 16777215(0xffffff)*1000/(DELAY_CLK)ns	��Ƶ96M systick�����ʱʱ���ԼΪ33ms

//------------------------------------���º궨�����ڶ�ʱ------------------------------------  
void systick_timing(uint32 time);
#define systick_timing_ms(time)		systick_timing(MSEC_TO_COUNT(time, DELAY_CLK))		// ���ö�ʱʱ�� ��λms ��Χ0 - 16777215(0xffffff)/(DELAY_CLK*1000)ms	��Ƶ96M ���ʱʱ���ԼΪ33ms
#define systick_timing_us(time)		systick_timing(USEC_TO_COUNT(time, DELAY_CLK))		// ���ö�ʱʱ�� ��λus ��Χ0 - 16777215(0xffffff)/(DELAY_CLK)us			��Ƶ96M ���ʱʱ���ԼΪ33ms
#define systick_timing_ns(time)		systick_timing(USEC_TO_COUNT(time, DELAY_CLK/1000))	// ���ö�ʱʱ�� ��λns ��Χ0 - 16777215(0xffffff)*1000/(DELAY_CLK)ns	��Ƶ96M ���ʱʱ���ԼΪ33ms


//------------------------------------���º궨�����ڻ�ȡ��ǰʱ��------------------------------------
void systick_start (void);
uint32 systick_getval(void);
#define systick_getval_ms()			COUNT_TO_MSEC(systick_getval(),DELAY_CLK)			// ��ȡ��ǰ��ʱʱ�� ��λms
#define systick_getval_us()			COUNT_TO_USEC(systick_getval(),DELAY_CLK)			// ��ȡ��ǰ��ʱʱ�� ��λus
#define systick_getval_ns()			COUNT_TO_USEC(systick_getval()*1000,DELAY_CLK)		// ��ȡ��ǰ��ʱʱ�� ��λns

#endif
