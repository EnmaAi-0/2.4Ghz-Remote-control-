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

#include "zf_systick.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick��ʱ����
// @param		time			��Ҫ��ʱ��ʱ��
// @return		void
// Sample usage:				�����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_delay (uint32 time)
{
	SysTick->CTRL	= 0x00;
	SysTick->LOAD	= time-1;
	SysTick->VAL	= 0x00;
	SysTick->CTRL	=	SysTick_CTRL_CLKSOURCE_Msk |					//ʱ��Դѡ�� (core clk)
						SysTick_CTRL_ENABLE_Msk;						//ʹ�� systick
	while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���뼶systick��ʱ����
// @param		time			��ʱ���ٺ���
// @return		void
// Sample usage:				systick_delay_ms(1000);   //��ʱ1000����
//-------------------------------------------------------------------------------------------------------------------
void systick_delay_ms (uint32 time)
{
	while(time--) systick_delay(SystemCoreClock / 1000);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick��ʱ��
// @param		time			��ʱʱ��(0-0x00ffffff)
// @return		void
// Sample usage:				�����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_timing(uint32 time)
{
	SysTick->CTRL	= 0x00;
	SysTick->LOAD = time-1;												// ������ʱʱ��
	SysTick->VAL = 0x00;												// ��ռ�����
	NVIC_SetPriority(SysTick_IRQn, 0x0);								// �������ȼ�
	SysTick->CTRL = ( 0 
		| SysTick_CTRL_ENABLE_Msk										// ʹ�� systick
		| SysTick_CTRL_TICKINT_Msk										// ʹ���ж�
		| SysTick_CTRL_CLKSOURCE_Msk									// ʱ��Դѡ�� (core clk)
		);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		systick��ʱ������
// @param		void
// @return		void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void systick_start(void)
{
	SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;							// ������ʱʱ��
	SysTick->VAL = 0x00;												// ��ռ�����
	SysTick->CTRL = ( 0
		| SysTick_CTRL_ENABLE_Msk										// ʹ�� systick
		| SysTick_CTRL_CLKSOURCE_Msk									// ʱ��Դѡ�� (core clk)
		);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��õ�ǰSystem tick timer��ֵ
// @return		���ص�ǰSystem tick timer��ֵ
// Sample usage:				uint32 tim = systick_getval();
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
	return (SysTick_LOAD_RELOAD_Msk - SysTick->VAL);
}

