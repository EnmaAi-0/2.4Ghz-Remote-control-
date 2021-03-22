/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_7725
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
* 					SDA					A9
* 					SCL					A8
* 					���ж�(VSY)			A0
* 					���ж�(HREF)		δʹ�ã���˲���
* 					�����ж�(PCLK)		A1
* 	                ���ݿ�(D0-D7)		B24-B31
* 					------------------------------------
********************************************************************************************************************/

#include "zf_exti.h"
#include "zf_systick.h"
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_camera.h"

#include "SEEKFREE_7725.h"

uint8_t ov7725_finish_flag = 0;
uint8_t ov7725_image_bin [OV7725_W][OV7725_H/8];
static uint16 ov7725_simiic_delay_time=100;											// ICM�ȴ�����Ӧ����Ϊ100

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʱ ʱ������
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
//static void ov7725_simiic_delay_set(uint16 time)
//{
//	ov7725_simiic_delay_time = time;												// ICM�ȴ�����Ӧ����Ϊ100
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʱ ʱ������
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_delay(void)
{
	uint16 delay_time;
	delay_time = ov7725_simiic_delay_time;
	while(delay_time--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC��ʼ�ź�
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_start(void)
{
	OV7725_SDA1();
	OV7725_SCL1();
	ov7725_simiic_delay();
	OV7725_SDA0();
	ov7725_simiic_delay();
	OV7725_SCL0();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IICֹͣ�ź�
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_stop(void)
{
	OV7725_SDA0();
	OV7725_SCL0();
	ov7725_simiic_delay();
	OV7725_SCL1();
	ov7725_simiic_delay();
	OV7725_SDA1();
	ov7725_simiic_delay();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC ACK�ź�
// @param		ack_dat			��Ӧ�� (����ack:SDA=0��no_ack:SDA=0)
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_sendack(unsigned char ack_dat)
{
	OV7725_SCL0();
	ov7725_simiic_delay();
	if(ack_dat)
		OV7725_SDA0();
	else
		OV7725_SDA1();

	OV7725_SCL1();
	ov7725_simiic_delay();
	OV7725_SCL0();
	ov7725_simiic_delay();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC�ȴ�Ӧ��
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static int ov7725_sccb_waitack(void)
{
	OV7725_SCL0();
	OV7725_DIR_IN();
	ov7725_simiic_delay();

	OV7725_SCL1();
	ov7725_simiic_delay();

	if(OV7725_SDA)																	// Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
	{
		OV7725_DIR_OUT();
		OV7725_SCL0();
		return 0;
	}
	OV7725_DIR_OUT();
	OV7725_SCL0();
	ov7725_simiic_delay();
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC �ֽڷ��� �������մ�Ӧ�� �����Ǵ�Ӧ��λ
// @param		c				����c(����������Ҳ���ǵ�ַ)
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_send_ch(uint8 c)
{
	uint8 i = 8;
	while(i--)
	{
		if(c & 0x80)
			OV7725_SDA1();															// SDA �������
		else
			OV7725_SDA0();
		c <<= 1;
		ov7725_simiic_delay();
		OV7725_SCL1();																// SCL ���ߣ��ɼ��ź�
		ov7725_simiic_delay();
		OV7725_SCL0();																// SCL ʱ��������
	}
	ov7725_sccb_waitack();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC �ֽڽ��� �˳���Ӧ���|��Ӧ����|ʹ��
// @param		ack_x			Ӧ��
// @return		uint8			����
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static uint8 ov7725_read_ch(uint8 ack_x)
{
	uint8 i;
	uint8 c;
	c=0;
	OV7725_SCL0();
	ov7725_simiic_delay();
	OV7725_SDA1();             
	OV7725_DIR_IN();																// ��������Ϊ���뷽ʽ
	for(i=0;i<8;i++)
	{
		ov7725_simiic_delay();
		OV7725_SCL0();																// ��ʱ����Ϊ�ͣ�׼����������λ
		ov7725_simiic_delay();
		OV7725_SCL1();																// ��ʱ����Ϊ�ߣ�ʹ��������������Ч
		ov7725_simiic_delay();
		c<<=1;
		if(OV7725_SDA)
			c+=1;																	// ������λ�������յ����ݴ�c
	}
	OV7725_DIR_OUT();
	OV7725_SCL0();
	ov7725_simiic_delay();
	ov7725_simiic_sendack(ack_x);

	return c;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC д���ݵ��豸�Ĵ���
// @param		dev_add			�豸��ַ
// @param		reg				�Ĵ�����ַ
// @param		dat				д�������
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	ov7725_simiic_start();
	ov7725_send_ch( (dev_add<<1) | 0x00);											// ����������ַ��дλ
	ov7725_send_ch( reg );															// ���ʹӻ��Ĵ�����ַ
	ov7725_send_ch( dat );															// ������Ҫд�������
	ov7725_simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC ���豸�Ĵ�����ȡ����
// @param		dev_add			�豸��ַ
// @param		reg				�Ĵ�����ַ
// @param		type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
// @return		uint8			���ؼĴ���������
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static uint8 ov7725_simiic_read_reg(uint8 dev_add, uint8 reg, IIC_type type)
{
	uint8 dat;
	ov7725_simiic_start();
	ov7725_send_ch( (dev_add<<1) | 0x00);											// ����������ַ��дλ
	ov7725_send_ch( reg );															// ���ʹӻ��Ĵ�����ַ
	if(type == SCCB)ov7725_simiic_stop();

	ov7725_simiic_start();
	ov7725_send_ch( (dev_add<<1) | 0x01);											// ����������ַ�Ӷ�λ
	dat = ov7725_read_ch(OV7725_NACK);												// ��ȡ����
	ov7725_simiic_stop();

	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC ��ȡ���ֽ�����
// @param		dev_add			�豸��ַ
// @param		reg				�Ĵ�����ַ
// @param		dat_add			���ݱ���ĵ�ַָ��
// @param		num				��ȡ�ֽ�����
// @param		type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
// @return		uint8			���ؼĴ���������
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
//static void ov7725_simiic_read_regs (uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 num, IIC_type type)
//{
//	ov7725_simiic_start();
//	ov7725_send_ch( (dev_add<<1) | 0x00);											// ����������ַ��дλ
//	ov7725_send_ch( reg );															// ���ʹӻ��Ĵ�����ַ
//	if(type == SCCB)ov7725_simiic_stop();
//
//	ov7725_simiic_start();
//	ov7725_send_ch( (dev_add<<1) | 0x01);											// ����������ַ�Ӷ�λ
//	while(--num)
//	{
//		*dat_add = ov7725_read_ch(OV7725_ACK);										// ��ȡ����
//		dat_add++;
//	}
//	*dat_add = ov7725_read_ch(OV7725_NACK);											// ��ȡ����
//	ov7725_simiic_stop();
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ģ��IIC�˿ڳ�ʼ��
// @return		void
// Sample usage:
// @note		�ڲ����� �û�������� ���IICͨѶʧ�ܿ��Գ�������ov7725_simiic_delay_time��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void ov7725_simiic_init(void)
{
	gpio_init(OV7725_SCL_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(OV7725_SDA_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		С�������ͷ�ڲ��Ĵ�����ʼ��(�ڲ�ʹ�ã��û��������)
// @param		NULL
// @return		uint8			����0���������1��ɹ�
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_reg_init(void)
{
	uint8 ov7725_idcode = 0;

	ov7725_simiic_write_reg ( OV7725_DEV_ADD, OV7725_COM7, 0x80 );					// ��λ����ͷ
	systick_delay_ms(50);
	ov7725_idcode = ov7725_simiic_read_reg( OV7725_DEV_ADD, OV7725_VER ,SCCB);
	if( ov7725_idcode != OV7725_ID )    return 0;									// У������ͷID��

	//ID��ȷ������   Ȼ�����üĴ���
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_COM4			, 0xC1);  
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_CLKRC		, 0x01);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_COM2			, 0x03);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_COM3			, 0xD0);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_COM7			, 0x40);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_COM8			, 0xCE);			// 0xCE:�ر��Զ��ع�  0xCF�������Զ��ع�
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HSTART		, 0x3F);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HSIZE		, 0x50);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VSTRT		, 0x03);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VSIZE		, 0x78);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HREF			, 0x00);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_SCAL0		, 0x0A);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_AWB_Ctrl0	, 0xE0);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_DSPAuto		, 0xff);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl2	, 0x0C);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl3	, 0x00);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl4	, 0x00);

	if(OV7725_W == 80)			ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize, 0x14);
	else if(OV7725_W == 160)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize, 0x28);
	else if(OV7725_W == 240)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize, 0x3c);
	else if(OV7725_W == 320)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize, 0x50);

	if(OV7725_H == 60)			ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize, 0x1E);
	else if(OV7725_H == 120)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize, 0x3c);
	else if(OV7725_H == 180)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize, 0x5a);
	else if(OV7725_H == 240)	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize, 0x78);

	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_REG28		, 0x01);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCH		, 0x10);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCL		, 0x1F);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM1			, 0x0c);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM2			, 0x16);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM3			, 0x2a);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM4			, 0x4e);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM5			, 0x61);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM6			, 0x6f);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM7			, 0x7b);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM8			, 0x86);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM9			, 0x8e);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM10		, 0x97);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM11		, 0xa4);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM12		, 0xaf);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM13		, 0xc5);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM14		, 0xd7);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM15		, 0xe8);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_SLOP			, 0x20);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_RADI		, 0x00);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEF		, 0x13);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_XC		, 0x08);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFB		, 0x14);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFR		, 0x17);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_CTR		, 0x05);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_BDBase		, 0x99);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_BDMStep		, 0x03);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_SDE			, 0x04);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_BRIGHT		, 0x00);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_CNST			, 0x40);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_SIGN			, 0x06);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ0		, 0x11);
	ov7725_simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ1		, 0x02);
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		С�������ͷ��ʼ��(����֮�����ú�����жϺ������ɲɼ�ͼ��)
// @param		NULL
// @return		0
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_init(void)
{
	uint8 temp = 0;
	camera_type = CAMERA_BIN;														// ������������ͷ����
	camera_buffer_addr = ov7725_image_bin[0];

	gpio_init(OV7725_UART_VSYNC_PIN, GPI, GPIO_LOW, GPI_PULL_UP);
	while(!temp)
	{
		temp = gpio_get(OV7725_UART_VSYNC_PIN);
	}

	ov7725_simiic_init();
	ov7725_reg_init();																// ����ͷ�Ĵ�������

	//DMA��ʼ��
	ov7725_uart_dma_init();
	//GPIO������ʱ����ʼ��,PLCK���ų�ʼ��
	ov7725_uart_tim_etr_init();
	//VSYNC��ʼ��
	ov7725_uart_exti_init();
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ov7725 ����ͷ���ж�
// @param		NULL
// @return		void			
// @since		v1.0
// Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_vsync(void)
{
	OV7725_UART_DMA_CH->CNDTR = OV7725_UART_SIZE;									// ���õ�ǰDMA�����ʣ�����������µݼ����üĴ���ֻ����DMA������ʱ���ġ�
	OV7725_UART_DMA_CH->CCR |= DMA_CCR_EN;											// ����DMA1
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		ov7725 ����ͷDMA����ж�
// @param		NULL
// @return		void			
// @since		v1.0
// Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void ov7725_dma(void)
{
	OV7725_UART_DMA_CH->CCR &= (uint16)(~DMA_CCR_EN);								//�ر�DMA1
	ov7725_finish_flag = 1;															//����ͷ���ݲɼ���1
}
