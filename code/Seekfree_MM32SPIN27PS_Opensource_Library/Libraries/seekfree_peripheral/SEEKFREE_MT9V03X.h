/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_MT9V03X
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
* 					SDA(51��RX)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_TX�궨��
* 					SCL(51��TX)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_RX�궨��
* 					���ж�(VSY)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_VSYNC_PIN�궨��
* 					���ж�(HREF)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_HREF_PIN�궨��
* 					�����ж�(PCLK)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_PCLK_PIN�궨��
* 					���ݿ�(D0-D7)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_DATA_PIN�궨��
* 					------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_MT9V03X_h
#define _SEEKFREE_MT9V03X_h

#include "common.h"
#include "zf_uart.h"

//��������ͷ����
// MM32SPIN27 �Ͼ�ֻ�Ǹ� M0 �ں˵ĵ�Ƭ�� SRAM ֻ�� 12KB �ɼ����������ֱ��ʵ�ͼ��
// 188*50=9400 ����� 9.18KB
#define MT9V03X_W               188		// ͼ����  ��Χ1-752
#define MT9V03X_H               50		// ͼ��߶�	��Χ1-480

//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define MT9V03X_COF_UART		UART_2										// ��������ͷ��ʹ�õ��Ĵ���
#define MT9V03X_COF_UART_TX		UART2_TX_C04
#define MT9V03X_COF_UART_RX		UART2_RX_C05

#define MT9V03X_DMA_CH			DMA1_Channel4
#define MT9V03X_DMA_IRQN		DMA1_Channel4_5_IRQn

#define MT9V03X_TIMETR_PCLK		TIM_3_ETR_D06								// GPIO����TIM���Ž�ֹ�����޸�

#define MT9V03X_VSYNC_PIN		D7											// ���ж�����
#define MT9V03X_VSYNC_PINT		EXTI_Line7									// ���ж���ʹ�õ�PINT�ж�ͨ��
#define MT9V03X_VSYNC_IRQN		EXTI4_15_IRQn								// �жϺ�

#define MT9V03X_DATA_PIN		B0
#define MT9V03X_DATA_ADD		GPIO_IDR_ADDR(MT9V03X_DATA_PIN)

                        
//����ͷ����ö��
typedef enum
{
	INIT = 0,																// ����ͷ��ʼ������
	AUTO_EXP,																// �Զ��ع�����
	EXP_TIME,																// �ع�ʱ������
	FPS,																	// ����ͷ֡������
	SET_COL,																// ͼ��������
	SET_ROW,																// ͼ��������
	LR_OFFSET,																// ͼ������ƫ������
	UD_OFFSET,																// ͼ������ƫ������
	GAIN,																	// ͼ��ƫ������
	CONFIG_FINISH,															// ������λ����Ҫ����ռλ����

	COLOR_GET_WHO_AM_I = 0xEF,
	SET_EXP_TIME = 0XF0,													// ���������ع�ʱ������
	GET_STATUS,																// ��ȡ����ͷ��������
	GET_VERSION,															// �̼��汾������

	SET_ADDR = 0XFE,														// �Ĵ�����ַ����
	SET_DATA																// �Ĵ�����������
}CMD;

extern uint8    mt9v03x_finish_flag;//һ��ͼ��ɼ���ɱ�־λ
extern uint8    mt9v03x_image[MT9V03X_H][MT9V03X_W];

void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
uint16 get_version(UARTN_enum uartn);
uint16 set_exposure_time(UARTN_enum uartn, uint16 light);

void mt9v03x_init(void);
void mt9v03x_vsync(void);
void mt9v03x_dma(void);
void mt9v03x_uart_callback(void);
void seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);

#endif
