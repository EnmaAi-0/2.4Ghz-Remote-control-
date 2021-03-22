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

#include "zf_uart.h"
#include "zf_gpio.h"

UART_TypeDef *uart_index[2] = {UART1, UART2};
IRQn_Type uart_irq[2] = {UART1_IRQn, UART2_IRQn};

//-------------------------------------------------------------------------------------------------------------------
// @brief		UART ���ų�ʼ�� �ڲ�����
// @param		tx_pin			ѡ�� TX ����
// @param		rx_pin			ѡ�� RX ����
// @return		void
// Sample usage:				uart_pin_init(tx_pin, rx_pin);
//-------------------------------------------------------------------------------------------------------------------
static void uart_pin_init (UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
	afio_init((PIN_enum)(tx_pin &0xff), GPO, (GPIOAF_enum)((tx_pin &0xf00)>>8), GPO_AF_PUSH_PUL);// ��ȡ��ӦIO���� AF���ܱ���
	afio_init((PIN_enum)(rx_pin &0xff), GPI, (GPIOAF_enum)((rx_pin &0xf00)>>8), GPI_PULL_UP);	// ��ȡ��ӦIO���� AF���ܱ���
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڳ�ʼ��
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		baud			���ڲ�����
// @param		tx_pin			���ڷ�������
// @param		rx_pin			���ڽ�������
// @return		void			
// Sample usage:				uart_init(USART_1,115200,UART1_TX_A09,UART1_RX_A10);			// ��ʼ������1 ������115200 ��������ʹ��PA09 ��������ʹ��PA10
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
	uart_pin_init(tx_pin, rx_pin);																// ���ų�ʼ��

	switch(uartn)
	{
		case UART_1:
			RCC->APB2ENR |= RCC_APB2ENR_UART1;													// ʹ�� UART1
			UART1->BRR = (SystemCoreClock / baud) / 16;											// ���ò�����
			UART1->FRA = (SystemCoreClock / baud) % 16;											// ���ò�����
			UART1->CCR |= UART_CCR_CHAR;														// 8bits ����λ
			UART1->GCR |= UART_GCR_TX | UART_GCR_RX | UART_GCR_UART;							// ʹ�� TX RX UART
			break;
		case UART_2:
			RCC->APB1ENR |= RCC_APB1ENR_UART2;
			UART2->BRR = (SystemCoreClock / baud) / 16;											// ���ò�����
			UART2->FRA = (SystemCoreClock / baud) % 16;											// ���ò�����
			UART2->CCR |= UART_CCR_CHAR;														// 8bits ����λ
			UART2->GCR |= UART_GCR_TX | UART_GCR_RX | UART_GCR_UART;							// ʹ�� TX RX UART
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		�����ֽ����
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		dat				��Ҫ���͵��ֽ�
// @return		void        
// Sample usage:				uart_putchar(USART_1,0xA5);										// ����1����0xA5
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTN_enum uartn, uint8 dat)
{
	uart_index[uartn]->TDR = dat;																// д�뷢������
	while(!(uart_index[uartn]->CSR & UART_CSR_TXC));											// �ȴ��������
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڷ�������
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		*buff			Ҫ���͵������ַ
// @param		len				���ͳ���
// @return		void
// Sample usage:				uart_putbuff(USART_1,&a[0],5);
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(UARTN_enum uartn, uint8 *buff, uint32 len)
{
	while(len)																					// ѭ����������
	{
		uart_index[uartn]->TDR = *buff++;														// д�뷢������
		while(!(uart_index[uartn]->CSR & UART_CSR_TXC));										// �ȴ��������
		len--;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڷ����ַ���
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		*str			Ҫ���͵��ַ�����ַ
// @return		void
// Sample usage:				uart_putstr(USART_1,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(UARTN_enum uartn, const uint8 *str)
{
	while(*str)																					// һֱѭ������β
	{
		uart_putchar(uartn, *str++);															// ��������
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ���ڽ��յ����ݣ�whlie�ȴ���
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		*dat			�������ݵĵ�ַ
// @return		void        
// Sample usage:				uint8 dat; uart_getchar(USART_1,&dat);							// ���մ���1����  ������dat������
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar(UARTN_enum uartn, uint8 *dat)
{
	while(!(uart_index[uartn]->CSR & UART_CSR_RXAVL));											// �ȴ���ȡ��һ������
	*dat = (uint8)uart_index[uartn]->RDR;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ���ڽ��յ����ݣ���ѯ���գ�
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		*dat			�������ݵĵ�ַ
// @return		uint8			1�����ճɹ�   0��δ���յ�����
// Sample usage:				uint8 dat; uart_query(USART_1,&dat);							// ���մ���1����  ������dat������
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UARTN_enum uartn, uint8 *dat)
{
	if(uart_index[uartn]->CSR & UART_CSR_RXAVL)													// ��ȡ��һ������
	{
		*dat = (uint8)uart_index[uartn]->RDR;													// �洢һ������
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڷ����ж�����
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		status			1�����ж�   0���ر��ж�
// @return		void        
// Sample usage:				uart_tx_irq(USART_1,1);											// �򿪴���1��������ж�
//-------------------------------------------------------------------------------------------------------------------
void uart_tx_irq(UARTN_enum uartn, uint32 status)
{
	if(status)
		uart_index[uartn]->IER |= UART_IER_TX;													// ʹ�ܷ�������ж�
	else
		uart_index[uartn]->IER &= ~(UART_IER_TX);												// �رշ�������ж�

	nvic_init(uart_irq[uartn], 0x00, (FunctionalState)status);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���ڽ����ж�����
// @param		uartn			����ģ���(USART_1,USART_2)
// @param		status			1�����ж�   0���ر��ж�
// @return		void        
// Sample usage:				uart_rx_irq(USART_1,1);											// �򿪴���1��������ж�
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq(UARTN_enum uartn, uint32 status)
{
	if(status)
		uart_index[uartn]->IER |= UART_IER_RX;													// ʹ�ܽ�������ж�
	else
		uart_index[uartn]->IER &= ~(UART_IER_RX);												// �رս�������ж�

	nvic_init(uart_irq[uartn], 0x00, (FunctionalState)status);
}

//--------------------------------------------------------------------------------				// printf �ض��� �˲��ֲ������û�����
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE s32 __io_putchar(s32 ch)
#else
#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)
#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
	while((UART1->CSR & UART_CSR_TXC) == 0); //The loop is sent until it is finished
	UART1->TDR = (ch & (u16)0x00FF);
	return ch;
}
#else
s32 fputc(s32 ch, FILE* f)
{
	while((UART1->CSR & UART_CSR_TXC) == 0); //The loop is sent until it is finished
	UART1->TDR = (ch & (u16)0x00FF);
	return ch;
}

#endif
//--------------------------------------------------------------------------------			// printf �ض��� �˲��ֲ������û�����
