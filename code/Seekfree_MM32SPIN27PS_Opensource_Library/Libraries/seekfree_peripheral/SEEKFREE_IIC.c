/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_IIC
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
* 					SCL					�鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
* 					SDA					�鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
* 					------------------------------------
********************************************************************************************************************/

#include "zf_gpio.h"
#include "zf_systick.h"
#include "SEEKFREE_IIC.h"

//�ڲ����ݶ���
uint8 IIC_ad_main;															// �����ӵ�ַ
uint8 IIC_ad_sub;															// �����ӵ�ַ
uint8 *IIC_buf;																// ����|�������ݻ�����
uint8 IIC_num;																// ����|�������ݸ���

static uint16 simiic_delay_time=5;										// ICM�ȴ�����Ӧ����Ϊ100

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʱ ʱ������
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
//static void simiic_delay_set(uint16 time)
//{
//	simiic_delay_time = time;												// ICM�ȴ�����Ӧ����Ϊ100
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʱ ʱ������
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void simiic_delay(void)
{
	uint16 delay_time;
	delay_time = simiic_delay_time;
	while(delay_time--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʼ�ź�
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void simiic_start(void)
{
	SDA1();
	SCL1();
	simiic_delay();
	SDA0();
	simiic_delay();
	SCL0();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IICֹͣ�ź�
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void simiic_stop(void)
{
	SDA0();
	SCL0();
	simiic_delay();
	SCL1();
	simiic_delay();
	SDA1();
	simiic_delay();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC ACK�ź�
// @param		ack_dat			��Ӧ�� (����ack:SDA=0��no_ack:SDA=0)
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void simiic_sendack(unsigned char ack_dat)
{
	SCL0();
	simiic_delay();
	if(ack_dat)
		SDA0();
	else
		SDA1();

	SCL1();
	simiic_delay();
	SCL0();
	simiic_delay();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC�ȴ�Ӧ��
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static int sccb_waitack(void)
{
	SCL0();
	DIR_IN();
	simiic_delay();

	SCL1();
	simiic_delay();

	if(SDA)																	// Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
	{
		DIR_OUT();
		SCL0();
		return 0;
	}
	DIR_OUT();
	SCL0();
	simiic_delay();
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC �ֽڷ��� �������մ�Ӧ�� �����Ǵ�Ӧ��λ
// @param		c				����c(����������Ҳ���ǵ�ַ)
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void send_ch(uint8 c)
{
	uint8 i = 8;
	while(i--)
	{
	if(c & 0x80)
		SDA1();																		// SDA �������
	else
		SDA0();
	c <<= 1;
	simiic_delay();
	SCL1();																			// SCL ���ߣ��ɼ��ź�
	simiic_delay();
	SCL0();																			// SCL ʱ��������
	}
	sccb_waitack();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC �ֽڽ��� �˳���Ӧ���|��Ӧ����|ʹ��
// @param		ack_x			Ӧ��
// @return		uint8			����
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static uint8 read_ch(uint8 ack_x)
{
	uint8 i;
	uint8 c;
	c=0;
	SCL0();
	simiic_delay();
	SDA1();             
	DIR_IN();																		// ��������Ϊ���뷽ʽ
	for(i=0;i<8;i++)
	{
		simiic_delay();
		SCL0();																		// ��ʱ����Ϊ�ͣ�׼����������λ
		simiic_delay();
		SCL1();																		// ��ʱ����Ϊ�ߣ�ʹ��������������Ч
		simiic_delay();
		c<<=1;
		if(SDA)
			c+=1;																	// ������λ�������յ����ݴ�c
	}
	DIR_OUT();
	SCL0();
	simiic_delay();
	simiic_sendack(ack_x);

	return c;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IICд���ݵ��豸�Ĵ�������
// @param		dev_add			�豸��ַ(����λ��ַ)
// @param		reg				�Ĵ�����ַ
// @param		dat				д�������
// @return		void						
// @since		v1.0
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	simiic_start();
	send_ch( (dev_add<<1) | 0x00);													// ����������ַ��дλ
	send_ch( reg );																	// ���ʹӻ��Ĵ�����ַ
	send_ch( dat );																	// ������Ҫд�������
	simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC���豸�Ĵ�����ȡ����
// @param		dev_add			�豸��ַ(����λ��ַ)
// @param		reg				�Ĵ�����ַ
// @param		type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
// @return		uint8			���ؼĴ���������
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 simiic_read_reg(uint8 dev_add, uint8 reg, IIC_type type)
{
	uint8 dat;
	simiic_start();
	send_ch( (dev_add<<1) | 0x00);													// ����������ַ��дλ
	send_ch( reg );																	// ���ʹӻ��Ĵ�����ַ
	if(type == SCCB)simiic_stop();

	simiic_start();
	send_ch( (dev_add<<1) | 0x01);													// ����������ַ�Ӷ�λ
	dat = read_ch(SEEKFREE_NACK);													// ��ȡ����
	simiic_stop();

	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ȡ���ֽ�����
// @param		dev_add			�豸��ַ(����λ��ַ)
// @param		reg				�Ĵ�����ַ
// @param		dat_add			���ݱ���ĵ�ַָ��
// @param		num				��ȡ�ֽ�����
// @param		type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
// @return		uint8			���ؼĴ���������
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, IIC_type type)
{
	simiic_start();
	send_ch( (dev_add<<1) | 0x00);													//����������ַ��дλ
	send_ch( reg );																	//���ʹӻ��Ĵ�����ַ
	if(type == SCCB)simiic_stop();

	simiic_start();
	send_ch( (dev_add<<1) | 0x01);													//����������ַ�Ӷ�λ
	while(--num)
	{
		*dat_add = read_ch(SEEKFREE_ACK);											//��ȡ����
		dat_add++;
	}
	*dat_add = read_ch(SEEKFREE_NACK);												//��ȡ����
	simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC�˿ڳ�ʼ��
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void simiic_init(void)
{
	gpio_init(SEEKFREE_SCL, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(SEEKFREE_SDA, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}
