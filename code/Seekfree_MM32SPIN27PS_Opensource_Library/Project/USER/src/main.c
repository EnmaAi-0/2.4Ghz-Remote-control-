/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				main
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "headfile.h"


int main(void)
{
	board_init(true);																// 初始化 debug 输出串口
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
		
		
				//如果使用USB转NRF接收数据，这位表示本次数据包 有效个数
        NRF_RX_Buff[0] = 11;  
        //当USB转NRF收到数据后，输出的数据个数等于此位的值
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
// **************************** 代码区域 ****************************
