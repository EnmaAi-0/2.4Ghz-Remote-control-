/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				main
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "headfile.h"


int main(void)
{
	board_init(true);																// ��ʼ�� debug �������
	system_init();

	while(1)
	{
		Rocker_LX=adc_convert(ADC_1, ADC1_CH00_A00);
		Rocker_LY=adc_convert(ADC_1, ADC1_CH02_A02);
		Rocker_RX=adc_convert(ADC_1, ADC1_CH03_A03);
		Rocker_RY=adc_convert(ADC_1, ADC1_CH05_A05);
		Battery_Power=(float)adc_convert(ADC_1, ADC1_CH06_A06) * 0.0046385f;
		
		oled_int16(0,0,Rocker_LX);
		oled_int16(1,1,Rocker_LY);
		oled_int16(2,2,Rocker_RX);
		oled_int16(3,3,Rocker_RY);
		oled_printf_float(4,4,Battery_Power,2,2);
		
		
				//���ʹ��USBתNRF�������ݣ���λ��ʾ�������ݰ� ��Ч����
        NRF_RX_Buff[0] = 11;  
        //��USBתNRF�յ����ݺ���������ݸ������ڴ�λ��ֵ
        NRF_RX_Buff[1] = 0;
        NRF_RX_Buff[2] = 1;
        NRF_RX_Buff[3] = 2;
        NRF_RX_Buff[4] = 3;
        NRF_RX_Buff[5] = 4;
        NRF_RX_Buff[6] = 5;
        NRF_RX_Buff[7] = 6;
        NRF_RX_Buff[8] = 7;
        NRF_RX_Buff[9] = 8;
        NRF_RX_Buff[10] = 9;
        NRF_RX_Buff[11] = 10;
        
        NRF_Send_Packet(NRF_RX_Buff);
        systick_delay_ms(100);
	}
}
// **************************** �������� ****************************
