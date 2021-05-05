#ifndef _KEY_H_
#define _KEY_H_

#include "headfile.h"


#define Top_Left_Key		A15
#define Top_Right_Key		A1
#define Lower_Left_Key		D6
#define Lower_Right_Key		D7



extern int8 key_sate;
extern int8 key_sate_last;


void key_init(void);
int8 key_scan(void);
uint8 Button_Press(void);

#endif