#include "key.h"

int8 key_sate;
int8 key_sate_last=15;

void key_init(void)
{
	gpio_init( Top_Left_Key, GPI, 1, GPI_PULL_UP);		//KEY1 --KEY4
	gpio_init( Top_Right_Key , GPI, 1, GPI_PULL_UP);
	gpio_init( Lower_Left_Key , GPI, 1, GPI_PULL_UP);
	gpio_init( Lower_Right_Key , GPI, 1, GPI_PULL_UP);
	
	
}

//扫描按钮状态
int8 key_scan(void)
{
	key_sate=0;
	if(gpio_get(Top_Left_Key))
		key_sate +=1;
	if(gpio_get(Top_Right_Key))
		key_sate +=2;
	if(gpio_get(Lower_Left_Key))
		key_sate +=4;
	if(gpio_get(Lower_Right_Key))
		key_sate +=8;
	return key_sate;
}

//判断哪个按钮按下，返回按键1-4,只判断按下瞬间
uint8 Button_Press(void)
{
	uint8 key_press=0;
	key_scan();
	if(key_sate_last == 15)
	{
		switch( key_sate_last - key_sate)
		{
			case 1: key_press=1; break;
			case 2: key_press=2; break;
			case 4: key_press=3; break;
			case 8: key_press=4; break;
		}
	}
	if(key_sate != key_sate_last)	//松手判断
		key_sate_last = key_sate;
	return key_press;
}

