#ifndef __PID_H__
#define __PID_H__

typedef struct{
	float Kp_Stand;
	float Kd_Stand;
	float V;
	float Kp_Turn;
	float Kd_Turn;
}param_PID;

extern param_PID data_PID;

int Stand_Balance(float Angle, float balance, float Gyro);
int Velocity_Balance(int16_t Expect_Speed, int16_t Encoder_Left, int16_t Encoder_Right);
int Turn_Balance(int16_t Expect_Angle, float Gyro_Z);

#endif
