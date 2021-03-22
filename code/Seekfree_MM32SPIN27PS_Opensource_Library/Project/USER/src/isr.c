/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				isr
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.28
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "headfile.h"
#include "isr.h"


uint32 SysTickTime=0;		//��Ϊϵͳʱ��
uint32 SysTickTime_s=0;

void TIM1_BRK_UP_TRG_COM_IRQHandler (void)
{
	uint32 state = TIM1->SR;														// ��ȡ�ж�״̬
	TIM1->SR &= ~state;																// ����ж�״̬
}

void TIM2_IRQHandler (void)
{
	uint32 state = TIM2->SR;														// ��ȡ�ж�״̬
	TIM2->SR &= ~state;																// ����ж�״̬
}

void TIM3_IRQHandler (void)
{
	uint32 state = TIM3->SR;														// ��ȡ�ж�״̬
	TIM3->SR &= ~state;																// ����ж�״̬
}

void TIM8_BRK_UP_TRG_COM_IRQHandler (void)
{
	uint32 state = TIM8->SR;														// ��ȡ�ж�״̬
	TIM8->SR &= ~state;																// ����ж�״̬
}

void TIM14_IRQHandler (void)
{
	uint32 state = TIM14->SR;														// ��ȡ�ж�״̬
	TIM14->SR &= ~state;															// ����ж�״̬
	
	if(SysTickTime++%1000 == 0)
		SysTickTime_s++;
}

void TIM16_IRQHandler (void)
{
	uint32 state = TIM16->SR;														// ��ȡ�ж�״̬
	TIM16->SR &= ~state;															// ����ж�״̬
}

void TIM17_IRQHandler (void)
{
	uint32 state = TIM17->SR;														// ��ȡ�ж�״̬
	TIM17->SR &= ~state;															// ����ж�״̬
}

void UART1_IRQHandler(void)
{
	if(UART1->ISR & UART_ISR_TX_INTF)												// ���ڷ��ͻ�����ж�
	{
		UART1->ICR |= UART_ICR_TXICLR;												// ����жϱ�־λ
	}
	if(UART1->ISR & UART_ISR_RX_INTF)												// ���ڽ��ջ����ж�
	{
		UART1->ICR |= UART_ICR_RXICLR;												// ����жϱ�־λ
	}
}

void UART2_IRQHandler(void)
{
	if(UART2->ISR & UART_ISR_TX_INTF)												// ���ڷ��ͻ�����ж�
	{
		UART2->ICR |= UART_ICR_TXICLR;												// ����жϱ�־λ
	}
	if(UART2->ISR & UART_ISR_RX_INTF)												// ���ڽ��ջ����ж�
	{
		switch(camera_type)															// ��ѯ����ͷ���� δ��ʼ������ͷ��˴������default
		{
			case CAMERA_BIN_UART: 													// ����С���
				ov7725_cof_uart_interrupt();										// ���ô���С���Ĵ��ڽ��մ���
				break;
			case CAMERA_GRAYSCALE: 													// �����
				mt9v03x_uart_callback();											// ���������Ĵ��ڽ��մ���
				break;
			default:
				break;
		}
		UART2->ICR |= UART_ICR_RXICLR;												// ����жϱ�־λ
	}
}

void EXTI0_1_IRQHandler(void)
{
	// ���������жϱ�־���Ը���ʵ��Ӧ�ý���ɾ��
	if(EXTI_GetITStatus(EXTI_Line0))												// ��� line0 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line0);													// ��� line0 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line1))												// ��� line1 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line1);													// ��� line1 ������־
	}
//	EXTI_ClearFlag(0x0003);															// EXTI_Line0 | EXTI_Line1
}

void EXTI2_3_IRQHandler(void)
{
	// ���������жϱ�־���Ը���ʵ��Ӧ�ý���ɾ��
	if(EXTI_GetITStatus(EXTI_Line2))												// ��� line2 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line2);													// ��� line2 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line3))												// ��� line3 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line3);													// ��� line3 ������־
	}
//	EXTI_ClearFlag(0x000C);															// EXTI_Line2 | EXTI_Line3
}

void EXTI4_15_IRQHandler(void)
{
	// ���������жϱ�־���Ը���ʵ��Ӧ�ý���ɾ��
	if(EXTI_GetITStatus(EXTI_Line4))												// ��� line4 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line4);													// ��� line4 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line5))												// ��� line5 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line5);													// ��� line5 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line6))												// ��� line6 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line6);													// ��� line6 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line7))												// ��� line7 �Ƿ񴥷�
	{
		switch(camera_type)															// ��ѯ����ͷ���� δ��ʼ������ͷ��˴������default
		{
			case CAMERA_BIN:														// С���
				ov7725_vsync();
				break;
			case CAMERA_BIN_UART:  													// ����С���
				ov7725_uart_vsync();
				break;
			case CAMERA_GRAYSCALE: 													// �����
				mt9v03x_vsync();
				break;
			default:
				break;
		}
		EXTI_ClearFlag(EXTI_Line7);													// ��� line8 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line8))												// ��� line8 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line8);													// ��� line8 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line9))												// ��� line9 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line9);													// ��� line9 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line10))												// ��� line10 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line10);												// ��� line10 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line11))												// ��� line11 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line11);												// ��� line11 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line12))												// ��� line12 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line12);												// ��� line12 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line13))												// ��� line13 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line13);												// ��� line13 ������־
		
		
	}
	if(EXTI_GetITStatus(EXTI_Line14))												// ��� line14 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line14);												// ��� line14 ������־
	}
	if(EXTI_GetITStatus(EXTI_Line15))												// ��� line15 �Ƿ񴥷�
	{
		EXTI_ClearFlag(EXTI_Line15);												// ��� line15 ������־
	}
//	EXTI_ClearFlag(0xFFF0);															// EXTI_Line4 | ... | EXTI_Line15
}

void DMA1_Channel1_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC1))										// �жϴ���ͨ��
	{
		DMA_ClearFlag(DMA1_FLAG_TC1);												// ��ո�ͨ���жϱ�־
	}
}

void DMA1_Channel2_3_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC2))										// �жϴ���ͨ��
	{
		DMA_ClearFlag(DMA1_FLAG_TC2);												// ��ո�ͨ���жϱ�־
	}
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC3))										// �жϴ���ͨ��
	{
		DMA_ClearFlag(DMA1_FLAG_TC3);												// ��ո�ͨ���жϱ�־
	}
}

void DMA1_Channel4_5_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC4))										// �жϴ���ͨ��
	{
		DMA_ClearFlag(DMA1_FLAG_TC4);												// ��ո�ͨ���жϱ�־
		switch(camera_type)															// ��ѯ����ͷ���� δ��ʼ������ͷ��˴������default
		{
			case CAMERA_BIN:														// С���
				ov7725_dma();
				break;
			case CAMERA_BIN_UART:  													// ����С���
				ov7725_uart_dma();
				break;
			case CAMERA_GRAYSCALE: 													// �����
				mt9v03x_dma();
				break;
			default:
				break;
		}
	}
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC5))										// �жϴ���ͨ��
	{
		DMA_ClearFlag(DMA1_FLAG_TC5);												// ��ո�ͨ���жϱ�־
	}
}

#ifdef Will_never_be_defined
WWDG_IRQHandler
PVD_IRQHandler
PWM_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_1_IRQHandler
EXTI2_3_IRQHandler
EXTI4_15_IRQHandler
HWDIV_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_3_IRQHandler
DMA1_Channel4_5_IRQHandler
ADC1_IRQHandler
TIM1_BRK_UP_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM8_BRK_UP_TRG_COM_IRQHandler
TIM8_CC_IRQHandler
TIM14_IRQHandler
ADC2_IRQHandler
TIM16_IRQHandler
TIM17_IRQHandler
I2C1_IRQHandler
COMP1_2_3_4_5_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
#endif
