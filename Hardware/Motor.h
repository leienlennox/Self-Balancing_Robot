#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed_Left(int16_t Speed);
void Motor_SetSpeed_Right(int16_t Speed);
void Motor_Limitation(int *Left_Motor,int *Right_Motor);


#endif
