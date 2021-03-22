/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				uart
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#ifndef _zf_uart_h
#define _zf_uart_h

#include <stdio.h>
#include "common.h"
#include "hal_uart.h"

//ö�ٴ�������	��ö�ٶ��岻�����û��޸�
typedef enum
{
// UART 1 ���� TX
	UART1_TX_A09					= 0x109,										// 0x010[AF1] 0x000[A group] 0x009[pin  9]
	UART1_TX_A10					= 0x30A,										// 0x030[AF3] 0x000[A group] 0x00A[pin 10]
	UART1_TX_B06					= 0x016,										// 0x000[AF0] 0x100[B group] 0x006[pin  6]
	UART1_TX_B09					= 0x019,										// 0x000[AF0] 0x100[B group] 0x009[pin  9]
	UART1_TX_C10					= 0x02A,										// 0x000[AF0] 0x200[C group] 0x00A[pin 10]
	UART1_TX_C12					= 0x02C,										// 0x000[AF0] 0x200[C group] 0x00C[pin 12]
	UART1_TX_D00					= 0x330,										// 0x030[AF3] 0x300[D group] 0x000[pin  0]

// UART 1 ���� RX
	UART1_RX_A09					= 0x309,										// 0x030[AF3] 0x000[A group] 0x009[pin  9]
	UART1_RX_A10					= 0x10A,										// 0x010[AF1] 0x000[A group] 0x00A[pin 10]
	UART1_RX_B07					= 0x017,										// 0x000[AF0] 0x100[B group] 0x007[pin  7]
	UART1_RX_B08					= 0x018,										// 0x000[AF0] 0x100[B group] 0x008[pin  8]
	UART1_RX_C11					= 0x02B,										// 0x000[AF0] 0x200[C group] 0x00B[pin 11]
	UART1_RX_D01					= 0x331,										// 0x030[AF3] 0x300[D group] 0x001[pin  1]

// UART 2 ���� TX
	UART2_TX_A02					= 0x102,										// 0x010[AF1] 0x000[A group] 0x002[pin  2]
	UART2_TX_A14					= 0x10E,										// 0x010[AF1] 0x000[A group] 0x00E[pin 14]
	UART2_TX_C04					= 0x324,										// 0x030[AF3] 0x200[C group] 0x004[pin  4]

// UART 2 ���� RX
	UART2_RX_A03					= 0x103,										// 0x010[AF1] 0x000[A group] 0x003[pin  3]
	UART2_RX_A15					= 0x10F,										// 0x010[AF1] 0x000[A group] 0x00F[pin 15]
	UART2_RX_C05					= 0x325,										// 0x030[AF3] 0x200[C group] 0x005[pin  5]
}UARTPIN_enum;

//ö�ٴ��ں�	��ö�ٶ��岻�����û��޸�
typedef enum
{
	UART_1,
	UART_2,
}UARTN_enum;

extern UART_TypeDef *uart_index[2];

void	uart_init		(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);
void	uart_putchar	(UARTN_enum uartn, uint8 dat);
void	uart_putbuff	(UARTN_enum uartn, uint8 *buff, uint32 len);
void	uart_putstr		(UARTN_enum uartn, const uint8 *str);
void	uart_getchar	(UARTN_enum uartn, uint8 *dat);
uint8	uart_query		(UARTN_enum uartn, uint8 *dat);
void	uart_tx_irq		(UARTN_enum uartn, uint32 status);
void	uart_rx_irq		(UARTN_enum uartn, uint32 status);

#endif
