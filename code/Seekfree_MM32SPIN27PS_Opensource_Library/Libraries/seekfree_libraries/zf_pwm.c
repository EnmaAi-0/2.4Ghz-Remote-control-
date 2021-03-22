/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				pwm
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "zf_pwm.h"
#include "zf_gpio.h"

extern TIM_TypeDef *tim_index[7];

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM ���ų�ʼ�� �ڲ�����
// @param		pin				ѡ�� PWM ����
// @return		void			NULL
// Sample usage:				pwm_pin_init(pin);
//-------------------------------------------------------------------------------------------------------------------
static void pwm_pin_init (TIM_PWMPIN_enum pin)
{
	afio_init((PIN_enum)(pin &0xff), GPO, (GPIOAF_enum)((pin &0xf00)>>8), GPO_AF_PUSH_PUL);		// ��ȡ��ӦIO���� AF���ܱ���
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM ��ʼ��
// @param		tim				ѡ�� PWM ʹ�õ� TIM
// @param		pin				ѡ�� PWM ����
// @param		freq			����Ƶ�� ͬ��ģ��ֻ�����һ��������Ч
// @param		duty			����ռ�ձ�
// @return		void
// Sample usage:						pwm_init(TIM_1, TIM_1_CH1_A08, 10000, 50000/100*ch1);
//-------------------------------------------------------------------------------------------------------------------
void pwm_init (TIM_enum tim, TIM_PWMPIN_enum pin, uint32 freq, uint32 duty)
{
	if(duty > PWM_DUTY_MAX)	return;																// ռ�ձ�д�����
	uint16 freq_div = ((SystemCoreClock / freq) >> 15);											// ����Ԥ��Ƶ
	uint16 period_temp = (SystemCoreClock / freq / (freq_div+1));								// �����Զ���װ��ֵ
	uint16 match_temp = (uint16)(period_temp*((float)duty/PWM_DUTY_MAX));						// ����ռ�ձ�

	pwm_pin_init(pin);																			// ��ʼ������
	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��

	tim_index[(tim&0x0f)]->ARR = period_temp;													// װ���Զ���װ��ֵ
	tim_index[(tim&0x0f)]->PSC = freq_div;														// װ��Ԥ��Ƶ
	tim_index[(tim&0x0f)]->CR1 = TIM_CR1_ARPEN;													// �����Զ���װ��ֵ��Ԥװ��
	tim_index[(tim&0x0f)]->BDTR = TIM_BDTR_MOEN;												// PWM ���ʹ��

	switch(pin&0xf000)
	{
		case 0x1000:
			tim_index[(tim&0x0f)]->CCMR1 |=														// OC1M [6:4] 110
					TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2;										// PWM ģʽ 1
			tim_index[(tim&0x0f)]->CCMR1 |= TIM_CCMR1_OC1PEN;									// ��������Ƚ�ֵ��Ԥװ��
			tim_index[(tim&0x0f)]->CCER |= TIM_CCER_CC1EN;										// ʹ��ͨ�� 1
			tim_index[(tim&0x0f)]->CCR1 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x2000:
			tim_index[(tim&0x0f)]->CCMR1 |=														// OC1M [6:4] 110
					TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2;										// PWM ģʽ 1
			tim_index[(tim&0x0f)]->CCMR1 |= TIM_CCMR1_OC2PEN;									// ��������Ƚ�ֵ��Ԥװ��
			tim_index[(tim&0x0f)]->CCER |= TIM_CCER_CC2EN;										// ʹ��ͨ�� 2
			tim_index[(tim&0x0f)]->CCR2 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x3000:
			tim_index[(tim&0x0f)]->CCMR2 |=														// OC1M [6:4] 110
					TIM_CCMR2_IC3F_1 | TIM_CCMR2_IC3F_2;										// PWM ģʽ 1
			tim_index[(tim&0x0f)]->CCMR2 |= TIM_CCMR2_OC3PEN;									// ��������Ƚ�ֵ��Ԥװ��
			tim_index[(tim&0x0f)]->CCER |= TIM_CCER_CC3EN;										// ʹ��ͨ�� 2
			tim_index[(tim&0x0f)]->CCR3 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x4000:
			tim_index[(tim&0x0f)]->CCMR2 |=														// OC1M [6:4] 110
					TIM_CCMR2_IC4F_1 | TIM_CCMR2_IC4F_2;										// PWM ģʽ 0
			tim_index[(tim&0x0f)]->CCMR2 |= TIM_CCMR2_OC4PEN;									// ��������Ƚ�ֵ��Ԥװ��
			tim_index[(tim&0x0f)]->CCER |= TIM_CCER_CC4EN;										// ʹ��ͨ�� 2
			tim_index[(tim&0x0f)]->CCR4 = match_temp;											// װ�رȽ�ֵ
			break;
	}

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM ʹ��
// @param		tim				ѡ�� PWM ʹ�õ� TIM
// @return		void
// Sample usage:				pwm_enable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void pwm_enable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM ʧ��
// @param		tim				ѡ�� PWM ʹ�õ� TIM
// @return		void
// Sample usage:				pwm_disable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void pwm_disable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 &= ~TIM_CR1_CEN;													// ʧ�ܶ�ʱ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		PWM ����ռ�ձ�
// @param		tim				ѡ�� PWM ʹ�õ� TIM
// @param		pin				ѡ�� PWM ����
// @param		duty			����ռ�ձ�
// @return		void
// Sample usage:				pwm_duty_updata(TIM_1, TIM_1_CH1_A08, 500*duty);
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty_updata (TIM_enum tim, TIM_PWMPIN_enum pin, uint32 duty)
{
	if(duty > PWM_DUTY_MAX)	return;																// ռ�ձ�д�����
	uint16 period_temp = tim_index[(tim&0x0f)]->ARR;											// ��ȡ�Զ���װ��ֵ
	uint16 match_temp = (uint16)(period_temp*((float)duty/PWM_DUTY_MAX));						// ����ռ�ձ�

	switch(pin&0xf000)																			// ��ȡͨ��ֵ
	{
		case 0x1000:
			tim_index[(tim&0x0f)]->CCR1 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x2000:
			tim_index[(tim&0x0f)]->CCR2 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x3000:
			tim_index[(tim&0x0f)]->CCR3 = match_temp;											// װ�رȽ�ֵ
			break;
		case 0x4000:
			tim_index[(tim&0x0f)]->CCR4 = match_temp;											// װ�رȽ�ֵ
			break;
	}
}
