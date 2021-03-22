/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				common
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#ifndef _common_h
#define _common_h

#include "reg_common.h"
#include "reg_adc.h"
#include "reg_dma.h"
#include "reg_exti.h"
#include "reg_flash.h"
#include "reg_gpio.h"
#include "reg_i2c.h"
#include "reg_rcc.h"
#include "reg_spi.h"
#include "reg_tim.h"
#include "reg_uart.h"

//������������
typedef unsigned char		uint8;					//  8 bits 
typedef unsigned short int	uint16;					// 16 bits 
typedef unsigned long int	uint32;					// 32 bits 
typedef unsigned long long	uint64;					// 64 bits 

typedef char				int8;					//  8 bits 
typedef short int			int16;					// 16 bits 
typedef long  int			int32;					// 32 bits 
typedef long  long			int64;					// 64 bits 

typedef volatile int8		vint8;					//  8 bits 
typedef volatile int16		vint16;					// 16 bits 
typedef volatile int32		vint32;					// 32 bits 
typedef volatile int64		vint64;					// 64 bits 

typedef volatile uint8		vuint8;					//  8 bits 
typedef volatile uint16		vuint16;				// 16 bits 
typedef volatile uint32		vuint32;				// 32 bits 
typedef volatile uint64		vuint64;				// 64 bits 


typedef enum //ö�ٶ˿ڷ���
{
	A0 = 0x00,	A1 = 0x01,	A2 = 0x02,	A3 = 0x03,	A4 = 0x04,	A5 = 0x05,	A6 = 0x06,	A7 = 0x07,
	A8 = 0x08,	A9 = 0x09,	A10 = 0x0A,	A11 = 0x0B,	A12 = 0x0C,	A13 = 0x0D,	A14 = 0x0E,	A15 = 0x0F,

	B0 = 0x10,	B1 = 0x11,	B2 = 0x12,	B3 = 0x13,	B4 = 0x14,	B5 = 0x15,	B6 = 0x16,	B7 = 0x17,
	B8 = 0x18,	B9 = 0x19,	B10 = 0x1A,	B11 = 0x1B,	B12 = 0x1C,	B13 = 0x1D,	B14 = 0x1E,	B15 = 0x1F,

	C0 = 0x20,	C1 = 0x21,	C2 = 0x22,	C3 = 0x23,	C4 = 0x24,	C5 = 0x25,	C6 = 0x26,	C7 = 0x27,
	C8 = 0x28,	C9 = 0x29,	C10 = 0x2A,	C11 = 0x2B,	C12 = 0x2C,	C13 = 0x2D,	C14 = 0x2E,	C15 = 0x2F,

	D0 = 0x30,	D1 = 0x31,	D2 = 0x32,	D3 = 0x33,	D4 = 0x34,	D5 = 0x35,	D6 = 0x36,	D7 = 0x37,
}PIN_enum;

#define GPIO_IDR_ADDR(X)	(0x48000008 + ((X&0xf0)>>4)*0x400 + (X&0x0f))

// Compiler Related Definitions 
#ifdef __CC_ARM							// ARM Compiler 
	#define ALIGN(n)					__attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)		// for IAR Compiler 
	#define PRAGMA(x)					_Pragma(#x)
	#define ALIGN(n)					PRAGMA(data_alignment=n)
#elif defined (__GNUC__)				// GNU GCC Compiler 
	#define ALIGN(n)					__attribute__((aligned(n)))
#endif // Compiler Related Definitions 

#include <math.h>
#include <string.h>

typedef enum //ö�ٶ˿ڵ�ƽ
{
	CAMERA_BIN=1,			//С���
	CAMERA_BIN_UART,		//С��紮�ڰ汾
	CAMERA_GRAYSCALE,		//�����
	CAMERA_COLOR,			//��ͫ
}CAMERA_TYPE_enum;

extern u32 SystemCoreClock;
extern uint8 *camera_buffer_addr;

void nvic_init(IRQn_Type irqn, uint8 priority, FunctionalState status);

void	NMI_Handler				(void);
void	HardFault_Handler		(void);
void	MemManage_Handler		(void);
void	BusFault_Handler		(void);
void	UsageFault_Handler		(void);
void	SVC_Handler				(void);
void	DebugMon_Handler		(void);
void	PendSV_Handler			(void);
void	SysTick_Handler			(void);

#endif
