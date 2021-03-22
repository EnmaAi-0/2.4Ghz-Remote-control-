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

#include "zf_pit.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM �жϳ�ʼ��
// @param		tim				ѡ���ж�ʹ�õ� TIM
// @param		freq			����Ƶ�� ͬ��ģ��ֻ�����һ��������Ч
// @param		priority		ѡ����ж����ȼ� ��Χ [0-3]
// @return		void
// Sample usage:				tim_interrupt_init(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init (TIM_enum tim, uint32 freq, uint8 priority)
{
	IRQn_Type irq_index[7] = 
	{
		TIM1_BRK_UP_TRG_COM_IRQn,
		TIM8_BRK_UP_TRG_COM_IRQn,
		TIM2_IRQn,
		TIM3_IRQn,
		TIM14_IRQn,
		TIM16_IRQn,
		TIM17_IRQn
	};

	NVIC_InitTypeDef NVIC_InitStructure;														// �ж����ýṹ��
	uint16 freq_div = ((SystemCoreClock / freq) >> 15);											// ����Ԥ��Ƶ
	uint16 period_temp = (SystemCoreClock / freq / (freq_div+1));								// �����Զ���װ��ֵ

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��

	tim_index[(tim&0x0f)]->ARR = period_temp;													// װ���Զ���װ��ֵ
	tim_index[(tim&0x0f)]->PSC = freq_div;														// װ��Ԥ��Ƶ
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// ʹ���жϸ���

	NVIC_InitStructure.NVIC_IRQChannel = irq_index[(tim&0x0f)];									// �ж�ѡ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = priority & 0x03;								// �������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												// ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);																// ��ʼ���ж�����

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM �жϳ�ʼ�� us�����ж�
// @param		tim				ѡ���ж�ʹ�õ� TIM
// @param		timer			����ʱ�� ͬ��ģ��ֻ�����һ��������Ч
// @param		priority		ѡ����ж����ȼ� ��Χ [0-3]
// @return		void
// Sample usage:				tim_interrupt_init(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init_us (TIM_enum tim, uint32 timer, uint8 priority)
{
	if(timer <= 0 || timer > 1000)		return;
	IRQn_Type irq_index[7] = 
	{
		TIM1_BRK_UP_TRG_COM_IRQn,
		TIM8_BRK_UP_TRG_COM_IRQn,
		TIM2_IRQn,
		TIM3_IRQn,
		TIM14_IRQn,
		TIM16_IRQn,
		TIM17_IRQn
	};

	NVIC_InitTypeDef NVIC_InitStructure;														// �ж����ýṹ��
	uint16 freq_div = 1;																		// ����Ԥ��Ƶ
	uint16 period_temp = timer*48-1;															// �����Զ���װ��ֵ

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��

	tim_index[(tim&0x0f)]->ARR = period_temp;													// װ���Զ���װ��ֵ
	tim_index[(tim&0x0f)]->PSC = freq_div;														// װ��Ԥ��Ƶ
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// ʹ���жϸ���

	NVIC_InitStructure.NVIC_IRQChannel = irq_index[(tim&0x0f)];									// �ж�ѡ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = priority & 0x03;								// �������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												// ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);																// ��ʼ���ж�����

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM �жϳ�ʼ�� ms�����ж�
// @param		tim				ѡ���ж�ʹ�õ� TIM
// @param		timer			����ʱ�� ͬ��ģ��ֻ�����һ��������Ч
// @param		priority		ѡ����ж����ȼ� ��Χ [0-3]
// @return		void
// Sample usage:				tim_interrupt_init(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init_ms (TIM_enum tim, uint32 timer, uint8 priority)
{
	if(timer <= 0 || timer > 5000)		return;
	IRQn_Type irq_index[7] = 
	{
		TIM1_BRK_UP_TRG_COM_IRQn,
		TIM8_BRK_UP_TRG_COM_IRQn,
		TIM2_IRQn,
		TIM3_IRQn,
		TIM14_IRQn,
		TIM16_IRQn,
		TIM17_IRQn
	};

	NVIC_InitTypeDef NVIC_InitStructure;														// �ж����ýṹ��
	uint16 freq_div = 8000-1;																	// ����Ԥ��Ƶ
	uint16 period_temp = 12*timer-1;															// �����Զ���װ��ֵ

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��

	tim_index[(tim&0x0f)]->ARR = period_temp;													// װ���Զ���װ��ֵ
	tim_index[(tim&0x0f)]->PSC = freq_div;														// װ��Ԥ��Ƶ
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// ʹ���жϸ���

	NVIC_InitStructure.NVIC_IRQChannel = irq_index[(tim&0x0f)];									// �ж�ѡ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = priority & 0x03;								// �������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												// ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);																// ��ʼ���ж�����

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM �ж�ʹ��
// @param		tim				ѡ�� TIM
// @return		void
// Sample usage:				tim_interrupt_enable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_enable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM �ж�ʧ��
// @param		tim				ѡ�� TIM
// @return		void
// Sample usage:				tim_interrupt_disable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_disable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 &= ~TIM_CR1_CEN;													// ʧ�ܶ�ʱ��
}
