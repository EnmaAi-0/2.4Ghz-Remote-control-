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
* 					SCK					�鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_SCK_PIN �궨��
* 					MISO				�鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_MISO_PIN�궨��
* 					MOSI				�鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_MOSI_PIN�궨��
* 					CS					�鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_PCS_PIN  �궨��
* 					------------------------------------
********************************************************************************************************************/

#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "zf_systick.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
// @param		byte			���͵�����
// @return		uint8			return ����status״̬
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8 spi_wr_byte(uint8 byte)
{
	spi_mosi(ABS_ENCODER_SPI_NUM, &byte, &byte, 1);
	return(byte);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��valд��cmd��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
// @param		cmd				������
// @param		val				��д��Ĵ�������ֵ
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
//static void spi_w_reg_byte(uint8 cmd, uint8 val)
//{
//	ABS_ENCODER_SPI_CSN (0);
//	cmd |= ABS_ENCODER_SPI_W;
//	spi_wr_byte(cmd);
//	spi_wr_byte(val);
//	ABS_ENCODER_SPI_CSN (1);
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡcmd����Ӧ�ļĴ�����ַ
// @param		cmd				������
// @param		*val			�洢��ȡ�����ݵ�ַ
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
//static void spi_r_reg_byte(uint8 cmd, uint8 *val)
//{
//	ABS_ENCODER_SPI_CSN (0);
//	cmd |= ABS_ENCODER_SPI_R;
//	spi_wr_byte(cmd);
//	*val = spi_wr_byte(0);
//	ABS_ENCODER_SPI_CSN (1);
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief		д��һ�����ݵ��������ļĴ���
// @param		cmd				�Ĵ�����ַ
// @param		*val			д�����ݵĵ�ַ
// @return		uint8			0������  1��ʧ��
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8 encoder_spi_w_reg_byte(uint8 cmd, uint8 val)
{
	uint8 dat;
	ABS_ENCODER_SPI_CSN (0);
	cmd |= ABS_ENCODER_SPI_W;
	spi_wr_byte(cmd);
	spi_wr_byte(val);
	ABS_ENCODER_SPI_CSN (1);
	systick_delay_us(1);
	ABS_ENCODER_SPI_CSN (0);
	dat = spi_wr_byte(0x00);
	spi_wr_byte(0x00);
	ABS_ENCODER_SPI_CSN (1);

	if(val != dat)  return 1;														// д��ʧ��
	return 0;																		// д��ɹ�
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ�Ĵ���
// @param		cmd				�Ĵ�����ַ
// @param		*val			�洢��ȡ�����ݵ�ַ
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void encoder_spi_r_reg_byte(uint8 cmd, uint8 *val)
{
	ABS_ENCODER_SPI_CSN (0);
	cmd |= ABS_ENCODER_SPI_R;
	spi_wr_byte(cmd);
	spi_wr_byte(0x00);

	ABS_ENCODER_SPI_CSN (1);
	systick_delay_us(1);
	ABS_ENCODER_SPI_CSN (0);
	*val = spi_wr_byte(0x00);
	spi_wr_byte(0x00);
	ABS_ENCODER_SPI_CSN (1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		������ƫ
// @param		zero_position	��Ҫ���õ���ƫ
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void set_zero_position_spi(uint16 zero_position)
{
	zero_position = (uint16)(4096 - zero_position);
	zero_position = zero_position << 4;
	encoder_spi_w_reg_byte(ZERO_L_REG,(uint8)zero_position);						// ������λ
	encoder_spi_w_reg_byte(ZERO_H_REG,zero_position>>8);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		�������Լ캯��
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void encoder_self5_check(void)
{
	uint8 val;
	do
	{
		encoder_spi_r_reg_byte(6,&val);
		//��������ԭ�������¼���
		//1 ���������ˣ�������µ������ĸ��ʼ���
		//2 ���ߴ������û�нӺ�
	}while(0x1C != val);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ��ǰ�Ƕ�
// @param		void
// @return		uint16			���ؽǶ�ֵ0-4096 ��Ӧ0-360��
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 encoder_angle_spi(void)
{
	uint16 angle;
	angle = 0;
	ABS_ENCODER_SPI_CSN (0);
	angle = (uint16)spi_wr_byte(0x00);      
	angle <<= 8;																	// �洢�߰�λ
	angle |= (uint16)spi_wr_byte(0x00);												// �洢�Ͱ�λ
	ABS_ENCODER_SPI_CSN (1);   

	return angle>>4;																// 12λ���ȣ����������λ
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��������ȡ
// @param		void
// @return		uint16			���ؽǶ�ֵ0-1023
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int16 encoder_speed_spi(void)
{
	static uint16 old_angle ,new_angle;
	uint16 angle_diff;

	//��ȡ���νǶ�����
	ABS_ENCODER_SPI_CSN (0);
	new_angle = (uint16)spi_wr_byte(0x00);
	new_angle <<= 8;																// �洢�߰�λ
	new_angle |= (uint16)spi_wr_byte(0x00);											// �洢�Ͱ�λ
	ABS_ENCODER_SPI_CSN (1);
	new_angle >>= 6;

	angle_diff = (old_angle - new_angle)&0x3ff;
	old_angle = new_angle;															// �������

	return ((int16)angle_diff - ((angle_diff & 0x200)<<1));
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��������ʼ������
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_spi(void)
{
	//CS���ų�ʼ��
	gpio_init(ABS_ENCODER_SPI_PCS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	//��ʼ��SPI��ʹ���������CS����
	spi_init(ABS_ENCODER_SPI_NUM, ABS_ENCODER_SPI_SCK_PIN, ABS_ENCODER_SPI_MOSI_PIN, ABS_ENCODER_SPI_MISO_PIN, SPI_NSS_NULL, 0, SystemCoreClock/32);
	encoder_self5_check();
	encoder_spi_w_reg_byte(DIR_REG,0x00);											// ������ת���� ��ת��ֵ��С��0x00   ��ת��ֵ���0x80
	set_zero_position_spi(0);														// ������ƫ
}

