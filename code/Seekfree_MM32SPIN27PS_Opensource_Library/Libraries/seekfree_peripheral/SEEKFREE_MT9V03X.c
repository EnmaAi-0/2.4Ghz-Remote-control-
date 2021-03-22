/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_MT9V03X
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
* 					SDA(51��RX)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_TX�궨��
* 					SCL(51��TX)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_RX�궨��
* 					���ж�(VSY)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_VSYNC_PIN�궨��
* 					���ж�(HREF)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_HREF_PIN�궨��
* 					�����ж�(PCLK)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_PCLK_PIN�궨��
* 					���ݿ�(D0-D7)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_DATA_PIN�궨��
* 					------------------------------------
********************************************************************************************************************/

#include "zf_systick.h"
#include "zf_uart.h"
#include "zf_gpio.h"
#include "zf_exti.h"
#include "zf_camera.h"

#include "SEEKFREE_MT9V03X.h"

uint8 mt9v03x_finish_flag = 0;												// һ��ͼ��ɼ���ɱ�־λ
uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];

static uint8	receive[3];
static uint8	receive_num = 0;
static vuint8	uart_receive_flag;

//��Ҫ���õ�����ͷ������
int16 MT9V03X_CFG[CONFIG_FINISH][2]=
{
	{AUTO_EXP,			0},													// �Զ��ع�����		��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
																			// һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
	{EXP_TIME,			450},												// �ع�ʱ��			����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
	{FPS,				50},												// ͼ��֡��			����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
	{SET_COL,			MT9V03X_W},											// ͼ��������		��Χ1-752     K60�ɼ���������188
	{SET_ROW,			MT9V03X_H},											// ͼ��������		��Χ1-480
	{LR_OFFSET,			0},													// ͼ������ƫ����	��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
	{UD_OFFSET,			0},													// ͼ������ƫ����	��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
	{GAIN,				32},												// ͼ������			��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�


	{INIT,				0}													// ����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
	{AUTO_EXP,			0},													// �Զ��ع�����
	{EXP_TIME,			0},													// �ع�ʱ��
	{FPS,				0},													// ͼ��֡��
	{SET_COL,			0},													// ͼ��������
	{SET_ROW,			0},													// ͼ��������
	{LR_OFFSET,			0},													// ͼ������ƫ����
	{UD_OFFSET,			0},													// ͼ������ƫ����
	{GAIN,				0},													// ͼ������
};

//-------------------------------------------------------------------------------------------------------------------
// @brief		MT9V03X����ͷ�����жϺ���
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
// @note
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_uart_callback(void)
{
	receive[receive_num] = (uart_index[MT9V03X_COF_UART])->RDR & 0x01FF;;
	receive_num++;

	if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
	if(3 == receive_num)
	{
		receive_num = 0;
		uart_receive_flag = 1;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��������ͷ�ڲ�������Ϣ
// @param		uartn			ѡ��ʹ�õĴ���
// @param		buff			����������Ϣ�ĵ�ַ
// @return		void
// @since		v1.0
// Sample usage:				���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2])
{
	uint16 temp, i;
	uint8  send_buffer[4];

	uart_receive_flag = 0;

	//���ò���  ������ο���������ֲ�
	//��ʼ��������ͷ�����³�ʼ��
	for(i=0; i<CONFIG_FINISH; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = buff[i][0];
		temp = buff[i][1];
		send_buffer[2] = temp>>8;
		send_buffer[3] = (uint8)temp;

		uart_putbuff(uartn,send_buffer,4);
		systick_delay_ms(2);
	}
	//�ȴ�����ͷ��ʼ���ɹ�
	while(!uart_receive_flag);
	uart_receive_flag = 0;
	while((0xff != receive[1]) || (0xff != receive[2]));
	//���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
	//����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ����ͷ�ڲ�������Ϣ
// @param		uartn			ѡ��ʹ�õĴ���
// @param		buff			����������Ϣ�ĵ�ַ
// @return		void
// @since		v1.0
// Sample usage:				���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2])
{
	uint16 temp, i;
	uint8  send_buffer[4];

	for(i=0; i<CONFIG_FINISH-1; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = GET_STATUS;
		temp = buff[i][0];
		send_buffer[2] = temp>>8;
		send_buffer[3] = (uint8)temp;

		uart_putbuff(uartn,send_buffer,4);

		//�ȴ����ܻش�����
		while(!uart_receive_flag);
		uart_receive_flag = 0;

		buff[i][1] = receive[1]<<8 | receive[2];
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ȡ����ͷ�̼��汾
// @param		uartn			ѡ��ʹ�õĴ���
// @return		void
// @since		v1.0
// Sample usage:				���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(UARTN_enum uartn)
{
	uint16 temp;
	uint8  send_buffer[4];
	send_buffer[0] = 0xA5;
	send_buffer[1] = GET_STATUS;
	temp = GET_VERSION;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;

	uart_putbuff(uartn,send_buffer,4);

	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;

	return ((uint16)(receive[1]<<8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		������������ͷ�ع�ʱ��
// @param		uartn			ѡ��ʹ�õĴ���
// @param		light			�����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
// @return		uint16			��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
// @since		v1.0
// Sample usage:				���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(UARTN_enum uartn, uint16 light)
{
	uint16 temp;
	uint8  send_buffer[4];

	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_EXP_TIME;
	temp = light;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;

	uart_putbuff(uartn,send_buffer,4);

	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;

	temp = receive[1]<<8 | receive[2];
	return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		������ͷ�ڲ��Ĵ�������д����
// @param		uartn			ѡ��ʹ�õĴ���
// @param		addr			����ͷ�ڲ��Ĵ�����ַ
// @param		data			��Ҫд�������
// @return		uint16			�Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
// @since		v1.0
// Sample usage:				���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v03x_reg(UARTN_enum uartn, uint8 addr, uint16 data)
{
	uint16 temp;
	uint8  send_buffer[4];

	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_ADDR;
	temp = addr;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;

	uart_putbuff(uartn,send_buffer,4);
	systick_delay_ms(10);

	send_buffer[0] = 0xA5;
	send_buffer[1] = SET_DATA;
	temp = data;
	send_buffer[2] = temp>>8;
	send_buffer[3] = (uint8)temp;

	uart_putbuff(uartn,send_buffer,4);

	//�ȴ����ܻش�����
	while(!uart_receive_flag);
	uart_receive_flag = 0;

	temp = receive[1]<<8 | receive[2];
	return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ʼ������ͷ���ж�
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_exti_init(void)
{
	gpio_init(MT9V03X_VSYNC_PIN, GPI, GPIO_LOW, GPI_PULL_UP);
	exti_interrupt_init(MT9V03X_VSYNC_PIN, EXTI_Trigger_Falling, 0x00);
	nvic_init(MT9V03X_VSYNC_IRQN, 0, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ʼ������ͷ��PCLK����
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_tim1_etr_init(void)
{
	camera_tim_etr_init(MT9V03X_TIMETR_PCLK);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��ʼ������ͷ��DMA
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_dma_init(void)
{
	uint8 num;
	for(num=0; num<8; num++)
	{
		gpio_init((PIN_enum)(MT9V03X_DATA_PIN + num), GPI, GPIO_LOW, GPI_PULL_UP);
	}

	//DMA1���߳�ʼ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMA����ͷ��ʼ��
	camera_dma_init(MT9V03X_DMA_CH, (uint32)MT9V03X_DATA_ADD, (uint32)camera_buffer_addr, MT9V03X_H*MT9V03X_W);
	//�ж�����
	nvic_init(MT9V03X_DMA_IRQN, 1, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		MT9V03X����ͷ��ʼ��
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:				ʹ��FLEXIO�ӿڲɼ�����ͷ
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_init(void)
{
	camera_type = CAMERA_GRAYSCALE;//������������ͷ����
	camera_buffer_addr = mt9v03x_image[0];

	// ��ʼ������ ��������ͷ
	uart_init (MT9V03X_COF_UART, 9600,MT9V03X_COF_UART_TX, MT9V03X_COF_UART_RX);
	uart_rx_irq(MT9V03X_COF_UART, ENABLE);
	systick_delay_ms(200);

	uart_receive_flag = 0;
	//�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
	//systick_delay_ms(1000);//��ʱ��ʽ
	get_config(MT9V03X_COF_UART, GET_CFG);//��ȡ���õķ�ʽ

	uart_receive_flag = 0;
	set_config(MT9V03X_COF_UART, MT9V03X_CFG);

	uart_receive_flag = 0;
	//��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
	get_config(MT9V03X_COF_UART, GET_CFG);

	//DMA��ʼ��
	mt9v03x_dma_init();
	//GPIO������ʱ����ʼ��,PLCK���ų�ʼ��
	mt9v03x_tim1_etr_init();
	//VSYNC��ʼ��
	mt9v03x_exti_init();
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		MT9V03X����ͷ���ж�
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_vsync(void)
{
	MT9V03X_DMA_CH->CNDTR = MT9V03X_H*MT9V03X_W;							// ���õ�ǰDMA�����ʣ������ ���µݼ� �üĴ���ֻ����DMA������ʱ���ġ�
	MT9V03X_DMA_CH->CCR |= DMA_CCR_EN;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		MT9V03X����ͷDMA����ж�
// @param		NULL
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_dma(void)
{
	MT9V03X_DMA_CH->CCR &= (uint16)(~DMA_CCR_EN);							// �ر�DMA1
	mt9v03x_finish_flag = 1;												// һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.8MS����(50FPS��188*120�ֱ���)
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		���������ͷͼ��������λ���鿴ͼ��
// @param		uartn			ʹ�õĴ��ں�
// @param		image			��Ҫ���͵�ͼ���ַ
// @param		width			ͼ�����
// @param		height			ͼ�����
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
	// ��������
	uart_putchar(uartn,0x00);
	uart_putchar(uartn,0xff);
	uart_putchar(uartn,0x01);
	uart_putchar(uartn,0x01);
	// ����ͼ��
	uart_putbuff(uartn, image, width*height);
}
