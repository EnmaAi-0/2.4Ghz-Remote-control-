#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "headfile.h"


extern int16 Rocker_LX,Rocker_LY,Rocker_RX,Rocker_RY;
extern float Battery_Power;

void system_init(void);
void Get_Power(void);



#endif