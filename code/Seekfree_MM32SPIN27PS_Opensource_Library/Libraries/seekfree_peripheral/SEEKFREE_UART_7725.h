/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_UART_7725
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
* @note
* 					���߶��壺
* 					------------------------------------
* 					ģ��ܽ�			��Ƭ���ܽ�
* 					RXD(51��RX)			�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_COF_UART_TX�궨��
* 					TXD(51��TX)			�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_COF_UART_RX�궨��
* 					���ж�(VSY)			�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_VSYNC_PIN�궨��
* 					���ж�(HREF)		�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_HREF_PIN�궨��
* 					�����ж�(PCLK)		�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_PCLK_PIN�궨��
* 					���ݿ�(D0-D7)		�鿴SEEKFREE_UART_7725.h�ļ��е�OV7725_UART_DATA_PIN�궨��
* 					------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_UART_7725_h
#define _SEEKFREE_UART_7725_h

#include "common.h"
#include "zf_uart.h"

#define OV7725_UART_W					160
#define OV7725_UART_H					120

#define OV7725_UART_SIZE				(OV7725_UART_W * OV7725_UART_H /8)
#define OV7725_UART_DMA_NUM				(OV7725_UART_SIZE )

#define OV7725_UART_COF_UART			UART_2								// С������ô���
#define OV7725_UART_COF_UART_TX			UART2_TX_C04						// С���TX��������
#define OV7725_UART_COF_UART_RX			UART2_RX_C05						// С���RX��������

#define OV7725_UART_DMA_CH				DMA1_Channel4						// TIM����DMAͨ����ֹ�����޸�
#define OV7725_UART_DMA_IRQN			DMA1_Channel4_5_IRQn				// DMA�ж�ͨ��

#define OV7725_UART_PCLK				TIM_3_ETR_D06						// GPIO����TIM���Ž�ֹ�����޸�

#define OV7725_UART_VSYNC_PIN			D7									// ���ж�����
#define OV7725_UART_VSYNC_PINT			EXTI_Line7							// ���ж���ʹ�õ�PINT�ж�ͨ��
#define OV7725_UART_VSYNC_IRQN			EXTI4_15_IRQn						// �жϺ�

#define OV7725_UART_DATA_PIN			B0
#define OV7725_UART_DATA_ADD			GPIO_IDR_ADDR(OV7725_UART_DATA_PIN)

typedef enum
{
	OV7725_INIT			= 0x00,
	OV7725_RESERVE,
	OV7725_CONTRAST,
	OV7725_FPS,
	OV7725_COL,
	OV7725_ROW,
	OV7725_CONFIG_FINISH,

	OV7725_GET_WHO_AM_I	= 0xEF,
	OV7725_GET_STATUS	= 0xF1,
	OV7725_GET_VERSION	= 0xF2,

	OV7725_SET_ADDR		= 0xFE,
	OV7725_SET_DATA		= 0xFF,
}OV7725_CMD;

extern uint8_t ov7725_uart_finish_flag;
extern uint8_t ov7725_uart_image_bin [OV7725_UART_H][OV7725_UART_W/8];
//extern uint8_t ov7725_uart_image_bin [OV7725_UART_SIZE];

void	ov7725_uart_dma				(void);
void	ov7725_uart_vsync			(void);
void	ov7725_uart_exti_init		(void);
void	ov7725_uart_tim_etr_init	(void);
void	ov7725_uart_dma_init		(void);
uint8_t	ov7725_uart_init			(void);
void	ov7725_cof_uart_interrupt	(void);
void	image_decompression			(uint8 *data1,uint8 *data2);
void	seekfree_sendimg_7725		(UARTN_enum uartn, void *imgaddr, uint32 imgsize);

#endif
