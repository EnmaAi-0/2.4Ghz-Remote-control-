/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				camera
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "zf_camera.h"
#include "zf_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		����ͷDMA��ʼ��
// @param		dma_ch				DAMͨ��
// @param		src_addr			Դ��ַ
// @param		des_addr			Ŀ���ַ
// @param		size				���ݳ���
// @return		void
// Sample usage:					camera_dma_init(DMA1_Channel4, GPIOA->ODR, GPIOC->ODR, 8);
//-------------------------------------------------------------------------------------------------------------------
void camera_dma_init (DMA_Channel_TypeDef* dma_ch, uint32 src_addr, uint32 des_addr, uint32 size)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1, ENABLE);
	DMA_DeInit(dma_ch);

	//MDA���ó�ʼ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = src_addr;										// Դ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = des_addr;											// Ŀ���ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											// ������ΪԴ
	DMA_InitStructure.DMA_BufferSize = size;													// ������ٸ�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;							// �����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										// �ڴ��ַ����+1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;						// ����ÿ�δ���һ���ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;								// �ڴ�ÿ�δ���һ���ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;												// ��ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;										// ���ȼ����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;												// ���ڴ浽�ڴ�ģʽ
	DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Enable;
	DMA_Init(dma_ch, &DMA_InitStructure);

	DMA_ITConfig(dma_ch, DMA_CCR_TCIE, ENABLE);													// ����DMA��������ж�
	DMA_Cmd(dma_ch, ENABLE);																	// ����DMA1
	DMA_Cmd(dma_ch, DISABLE);																	// ����DMA1
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		����ͷGPIO������ʱ����ʼ��
// @param		timern				��ʱ��ͨ��
// @param		pin					���ź�
// @return		void
// Sample usage:					camera_tim_etr_init(TIM_3_ETR_D06);  						// ��ʱ��ERT�����ǹ̶������Ž�ֹ�����޸�
//-------------------------------------------------------------------------------------------------------------------
void camera_tim_etr_init (TIM_ETR_PIN_enum pin)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	switch((pin&0xF000))
	{
		case 0x0000:	RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM1, ENABLE);break;
		case 0x2000:	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2, ENABLE);break;
		case 0x3000:	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3, ENABLE);break;
	}

	afio_init((PIN_enum)(pin &0xff), GPI, (GPIOAF_enum)((pin &0xf00)>>8), GPI_PULL_UP);			// ��ȡ��ӦIO���� AF���ܱ���

	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(tim_index[((pin&0xF000)>>12)], &TIM_TimeBaseStructure);

	TIM_ETRClockMode1Config(tim_index[((pin&0xF000)>>12)], TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x00);
	TIM_SelectInputTrigger(tim_index[((pin&0xF000)>>12)], TIM_TS_ETRF);
	TIM_SelectSlaveMode(tim_index[((pin&0xF000)>>12)], TIM_SlaveMode_Reset);					//TIM��ģʽ�������źŵ����������³�ʼ���������ʹ����Ĵ����ĸ����¼�
	TIM_SelectMasterSlaveMode(tim_index[((pin&0xF000)>>12)], TIM_MasterSlaveMode_Enable);		//������ʱ���ı�������

	TIM_Cmd(tim_index[((pin&0xF000)>>12)], ENABLE);
	TIM_DMACmd(tim_index[((pin&0xF000)>>12)], TIM_DMA_Trigger, ENABLE);							//ʹ��TIM_DMA
}
