/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_WIRELESS
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
* @note
* 					���߶��壺
* 					------------------------------------
* 					ģ��ܽ�				��Ƭ���ܽ�
* 					RX						�鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
* 					TX						�鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
* 					RTS						�鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
* 					CMD						���ջ�������
* 					------------------------------------
********************************************************************************************************************/

#include "zf_systick.h"
#include "zf_gpio.h"
#include "zf_uart.h"
#include "SEEKFREE_WIRELESS.h"

uint8 wireless_rx_buffer[WIRELESS_BUFFER_SIZE];
uint16 wireless_rx_index = 0;

//-------------------------------------------------------------------------------------------------------------------
// @brief		����ת����ģ�� �����жϺ���
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ú�����ISR�ļ� �����жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback()
{
	wireless_rx_buffer[wireless_rx_index++] = (uart_index[WIRELESS_UART])->RDR & 0x01FF;
	if(wireless_rx_index == WIRELESS_BUFFER_SIZE)	wireless_rx_index=0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		����ת����ģ���ʼ��
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void seekfree_wireless_init (void)
{
	//������ʹ�õĲ�����Ϊ115200��Ϊ����ת����ģ���Ĭ�ϲ����ʣ�������������������������ģ�鲢�޸Ĵ��ڵĲ�����
	gpio_init(RTS_PIN, GPI, GPIO_LOW, GPI_PULL_UP);										// ��ʼ����������

	uart_init (WIRELESS_UART, WIRELESS_UART_BAUD, WIRELESS_UART_TX, WIRELESS_UART_RX);	// ��ʼ������
	uart_rx_irq(WIRELESS_UART, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		����ת����ģ�� ���ͺ���
// @param		buff			��Ҫ���͵����ݵ�ַ
// @param		len				���ͳ���
// @return		uint32			ʣ��δ���͵��ֽ���
// @since		v1.0
// Sample usage:	seekfree_wireless_send_buff(&buff[0], buff_len);
// @note
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_wireless_send_buff(uint8 *buff, uint32 len)
{
	while(len>30)
	{
		if(gpio_get(RTS_PIN))
		{
			return len;																	// ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
		}
		//while(gpio_get(RTS_PIN));														// ���RTSΪ�͵�ƽ���������������
		uart_putbuff(WIRELESS_UART,buff,30);

		buff += 30;																		// ��ַƫ��
		len -= 30;																		// ����
	}

	if(gpio_get(RTS_PIN))
	{
	return len;																			// ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
	}
	//while(gpio_get(RTS_PIN));															// ���RTSΪ�͵�ƽ���������������
	uart_putbuff(WIRELESS_UART,buff,len);												// ������������

	return 0;
}
