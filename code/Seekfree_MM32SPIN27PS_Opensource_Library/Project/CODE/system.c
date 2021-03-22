#include "system.h"

int16 Rocker_LX,Rocker_LY,Rocker_RX,Rocker_RY;
float Battery_Power;

void system_init(void)
{
	
	//oled��ʼ��
	oled_init();
	
	//ҡ�˳�ʼ��
	adc_init(ADC_1, ADC1_CH00_A00, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH02_A02, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH03_A03, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH05_A05, ADC_12BIT);
	
	//�������
	adc_init(ADC_1, ADC1_CH06_A06, ADC_12BIT);
	
	//2.4GͨѶģ�� SI24R1��ʼ��
	NRF_Dev_Init();
}


void Get_Power(void)		//�˴���ѹ������ݵ����������
{
	Battery_Power=(float)adc_mean_filter(ADC_1, ADC1_CH06_A06,5) * 0.0046385f;
}