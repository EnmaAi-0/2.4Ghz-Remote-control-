/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				spi
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/

#include "zf_spi.h"
#include "zf_gpio.h"

SPI_TypeDef *spi_index[2] = {SPI1, SPI2};

//-------------------------------------------------------------------------------------------------------------------
// @brief		SPI ���ų�ʼ�� �ڲ�����
// @param		tx_pin			ѡ�� TX ����
// @param		rx_pin			ѡ�� RX ����
// @param		rx_pin			ѡ�� RX ����
// @return		void			NULL
// Sample usage:				spi_pin_init(sck_pin, mosi_pin, miso_pin, cs_pin);
//-------------------------------------------------------------------------------------------------------------------
static void spi_pin_init (SPI_SCKPIN_enum sck_pin, SPI_MOSIPIN_enum mosi_pin, SPI_MISOPIN_enum miso_pin, SPI_NSSPIN_enum cs_pin)
{
	afio_init((PIN_enum)(sck_pin &0xff), GPO, (GPIOAF_enum)((sck_pin &0xf00)>>8), GPO_AF_PUSH_PUL);			// ��ȡ��ӦIO���� AF���ܱ���
	afio_init((PIN_enum)(mosi_pin &0xff), GPO, (GPIOAF_enum)((mosi_pin &0xf00)>>8), GPO_AF_PUSH_PUL);		// ��ȡ��ӦIO���� AF���ܱ���
	if(SPI_MISO_NULL != miso_pin)
		afio_init((PIN_enum)(miso_pin &0xff), GPI, (GPIOAF_enum)((miso_pin &0xf00)>>8), GPI_PULL_UP);		// ��ȡ��ӦIO���� AF���ܱ���
	if(SPI_NSS_NULL != cs_pin)
		afio_init((PIN_enum)(cs_pin &0xff), GPO, (GPIOAF_enum)((cs_pin &0xf00)>>8), GPO_AF_PUSH_PUL);		// ��ȡ��ӦIO���� AF���ܱ���
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		SPI��ʼ��
// @param		spi_n			ѡ��SPIģ�� (SPI_1-SPI_2)
// @param		cs_pin			ѡ��SPIƬѡ����
// @param		sck_pin			ѡ��SPIʱ������
// @param		mosi_pin		ѡ��SPI MOSI����
// @param		miso_pin		ѡ��SPI MISO����
// @param		mode			SPIģʽ	0��CPOL=0 CPHA=0	1��CPOL=0 CPHA=1	2��CPOL=1 CPHA=0	3��CPOL=1 CPHA=1		//����ϸ�ڿ����аٶ�
// @param		baud			����SPI�Ĳ�����
// @return		void
// Sample usage:				spi_init(SPI_1, SPI1_SCK_A04, SPI1_MOSI_A07, SPI1_MISO_A06, SPI1_NSS_A04, 0, 1*1000*1000);	//Ӳ��SPI��ʼ��  ģʽ0 ������Ϊ1Mhz   
//-------------------------------------------------------------------------------------------------------------------
void spi_init (SPIN_enum spi_n, SPI_SCKPIN_enum sck_pin, SPI_MOSIPIN_enum mosi_pin, SPI_MISOPIN_enum miso_pin, SPI_NSSPIN_enum cs_pin, uint8 mode, uint32 baud)
{
	spi_pin_init(sck_pin, mosi_pin, miso_pin, cs_pin);											// ��ʼ���������

	if(spi_n == SPI_1)
		RCC->APB2ENR |= RCC_APB2ENR_SPI1;
	else
		RCC->APB1ENR |= RCC_APB1ENR_SPI2;

	spi_index[spi_n]->GCTL |= SPI_GCR_MODE;														// ����ģʽ
	spi_index[spi_n]->CCTL |= SPI_CCR_SPILEN;													// 8bits ����
	switch(mode)
	{
		case 0:
			spi_index[spi_n]->CCTL &= ~SPI_CCR_CPOL;											// SCK ����ʱ�͵�ƽ
			spi_index[spi_n]->CCTL |= SPI_CCR_CPHA;												// ��һ��ʱ�ӱ��ؿ�ʼ����
			break;
		case 1:
			spi_index[spi_n]->CCTL &= ~SPI_CCR_CPOL;											// SCK ����ʱ�͵�ƽ
			spi_index[spi_n]->CCTL &= ~SPI_CCR_CPHA;											// �ڶ���ʱ�ӱ��ؿ�ʼ����
			break;
		case 2:
			spi_index[spi_n]->CCTL |= SPI_CCR_CPOL;												// SCK ����ʱ�ߵ�ƽ
			spi_index[spi_n]->CCTL |= SPI_CCR_CPHA;												// ��һ��ʱ�ӱ��ؿ�ʼ����
			break;
		case 3:
			spi_index[spi_n]->CCTL |= SPI_CCR_CPOL;												// SCK ����ʱ�ߵ�ƽ
			spi_index[spi_n]->CCTL &= ~SPI_CCR_CPHA;											// �ڶ���ʱ�ӱ��ؿ�ʼ����
			break;
	}
	spi_index[spi_n]->GCTL &= ~SPI_GCR_NSS;														// Ƭѡ�������
	spi_index[spi_n]->CCTL |= SPI_CCR_RXEDGE;													// �ڴ�������λ��βʱ���ز������� ���ڸ���ģʽ
	spi_index[spi_n]->SPBRG = (SystemCoreClock / baud);											// ���ò�����
	spi_index[spi_n]->CCTL &= ~SPI_CCR_LSBFE;													// MSB
	spi_index[spi_n]->GCTL |= SPI_GCR_TXEN | SPI_GCR_RXEN;										// ʹ�� TX/RX
	spi_index[spi_n]->GCTL |= SPI_GCR_SPIEN;													// ʹ��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		SPI���ͽ��պ���
// @param		spi_n			ѡ��SPIģ�� (SPI_1-SPI_2)
// @param		modata			���͵����ݻ�������ַ
// @param		midata			��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
// @param		len				���͵��ֽ���
// @param		continuous		����ͨ����CS�Ƿ����������Ч״̬ 1:�������� 0:ÿ������һ���ֽڹر�CS(һ������Ϊ1 ����)
// @return		void				
// @since		v2.0
// Sample usage:				spi_mosi(SPI_1,buf,buf,1);										//����buff�����ݣ������յ�buf�����Ϊ1�ֽ� ͨ���ڼ�CS��������
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi (SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len)
{
	while(len--)																				// �жϳ���
	{
		while(!(spi_index[spi_n]->CSTAT & SPI_SR_TXEPT));										// ����û��
		spi_index[spi_n]->TXREG = *modata++;													// ��������
		if(midata != NULL)																		// ���ղ�Ϊ��
		{
			while(!(spi_index[spi_n]->CSTAT & SPI_SR_RXAVL));									// ����û��
			*midata++ = spi_index[spi_n]->RXREG;												// ��ȡ����
		}
	}
}
