#include "Init.h"

void All_Init(void)
{
	OLED_Init();
	
	Motor_Init();
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	
	MPU6050_Init();
	MPU6050_EXIT_Init();
	
	Bluetooth_Init();
}
