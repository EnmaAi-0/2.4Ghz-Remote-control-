/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_ABSOLUTE_ENCODER
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
* 					SCK					�鿴 ABS_ENCODER_SPI_SCK_PIN �궨��
* 					MISO				�鿴 ABS_ENCODER_SPI_MISO_PIN �궨��
* 					MOSI				�鿴 ABS_ENCODER_SPI_MOSI_PIN �궨��
* 					CS					�鿴 ABS_ENCODER_SPI_PCS_PIN �궨��
* 					------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_ABSOLUTE_ENCODER_H
#define _SEEKFREE_ABSOLUTE_ENCODER_H

#include "headfile.h"

//------------------------Ӳ��SPI--------------------------//
#define ABS_ENCODER_SPI_NUM			SPI_2
#define ABS_ENCODER_SPI_SCK_PIN		SPI2_SCK_B10
#define ABS_ENCODER_SPI_MOSI_PIN	SPI2_MOSI_B12
#define ABS_ENCODER_SPI_MISO_PIN	SPI2_MISO_B13
#define ABS_ENCODER_SPI_PCS_PIN		C6

#define ABS_ENCODER_SPI_CSN(x)		((x == 0)? (GPIO_PIN_RESET(ABS_ENCODER_SPI_PCS_PIN))  :(GPIO_PIN_SET(ABS_ENCODER_SPI_PCS_PIN)))
//------------------------Ӳ��SPI--------------------------//

//�Ƕȴ���������
#define ABS_ENCODER_SPI_W		0x80
#define ABS_ENCODER_SPI_R		0x40

#define ZERO_L_REG				0x00
#define ZERO_H_REG				0x01
#define DIR_REG					0X09

uint16	encoder_angle_spi		(void);
int16	encoder_speed_spi		(void);
void	encoder_init_spi		(void);

#endif
