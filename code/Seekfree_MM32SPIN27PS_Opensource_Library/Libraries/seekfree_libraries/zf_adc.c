/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				adc
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "zf_adc.h"
#include "zf_gpio.h"

ADC_TypeDef *adc_index[2] = {ADC1, ADC2};
uint8 adc_resolution[2] = {ADC_12BIT, ADC_12BIT};

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC ���ų�ʼ�� �ڲ�����
// @param		ch				ѡ��ADCͨ��
// @return		void			NULL
// Sample usage:				adc_pin_init(ch);
//-------------------------------------------------------------------------------------------------------------------
static void adc_pin_init (ADCCH_enum ch)
{
	if((ch & 0x0f0) !=  0x020)																	// ȷ��������ͨ��
	{
		gpio_init((PIN_enum)(ch &0xff), GPI, GPIO_LOW, GPI_ANAOG_IN);
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC��ʼ��
// @param		ch				ѡ��ADCͨ��
// @param		resolution		ѡ��ѡ��ͨ���ֱ���(���ͬһ��ģ���ʼ��ʱ�����˲�ͬ�ķֱ��� ���һ����ʼ���ķֱ�����Ч)
// @return		void
// Sample usage:				adc_init(ADC_1, ADC1_CH00_A00, ADC_8BIT);						//��ʼ��A00ΪADC���� �ֱ���Ϊ8λ
//-------------------------------------------------------------------------------------------------------------------
void adc_init (ADCN_enum adc, ADCCH_enum ch, ADCRES_enum resolution)
{
	adc_pin_init(ch);																			// �������ű������ʼ������

	if((RCC->APB2ENR & (RCC_APB2ENR_ADC1 << adc)) == 0)											// ��� ADC δ��ʹ��
	{
		RCC->APB2ENR |= (RCC_APB2ENR_ADC1 << adc);												// ʹ�� ADC ʱ��
		RCC->APB2RSTR |= (RCC_APB2RSTR_ADC1 << adc);											// ��λ ADC ����
		RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1 << adc);											// ��λ ����

		adc_resolution[adc] = resolution;														// ��¼ADC���� ���ڲɼ�ʱʹ�� ��ϸ��鿴 UM <12.5.1 �ɱ�̷ֱ���> �½�
		adc_index[adc]->ADCFG |= ((uint32)resolution << ADC_CFGR_RSLTCTL_Pos);					// ADC �����޸�
		adc_index[adc]->ADCFG |= ADC_CFGR_PRE_8;												// ADC ʱ�� 8 ��Ƶ
		adc_index[adc]->ADCR &= ~(ADC_CR_MODE | ADC_CR_ALIGN);									// ������� Ĭ�ϵ���ת�� ���������
		adc_index[adc]->ADCR |= ADC_CR_SCAN;													// ������ת��

		adc_index[adc]->ADCFG |= ADC_CFGR_ADEN;													// ADC ʹ��
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADCת��һ��
// @param		ch				ѡ��ADCͨ��
// @return		uint16			ת����ADCֵ
// Sample usage:				adc_convert(ADC_1, ADC1_CH00_A00);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert (ADCN_enum adc, ADCCH_enum ch)
{
	adc_index[adc]->ADCHS = (0x00000001 << ((ch&0xf00)>>8));									// ʹ�ܶ�Ӧͨ��
	adc_index[adc]->ADCR |= ADC_CR_ADST;														// ��ʼ����ת��
	while(((adc_index[adc]->ADSTA ) & ADC_SR_ADIF) == 0);										// �ȴ�����ת�����
	adc_index[adc]->ADSTA |= ADC_SR_ADIF;														// ���ת����ɱ�־
	return ((adc_index[adc]->ADDATA & 0xfff) >> adc_resolution[adc]);							// ��ȡ���� Ϊʲô����Ҫλ����鿴 UM <12.5.1 �ɱ�̷ֱ���> �½�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ADC��ֵ�˲�
// @param		ch				ѡ��ADCͨ��
// @param		count			��ֵ�˲�����
// @return		uint16			ת����ADCֵ
// Sample usage:				adc_convert(ADC_1, ADC1_CH00_A00, 5);//�ɼ�5�� Ȼ�󷵻�ƽ��ֵ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter (ADCN_enum adc, ADCCH_enum ch, uint8 count)
{
	uint8 i;
	uint32 sum;

	sum = 0;
	for(i=0; i<count; i++)
	{
		sum += adc_convert(adc, ch);
	}

	return sum/count;
}
