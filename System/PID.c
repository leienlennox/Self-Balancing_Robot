#include "stm32f10x.h"                  // Device header
#include "PID.h"

// PID参数结构体定义
param_PID data_PID = {
    .Kp_Stand = 400*0.6,    	// 直立环比例系数
    .Kd_Stand = 2000*0.6,   	// 直立环微分系数
    .V = 120,				// 速度环系数
    .Kp_Turn = 200,      	// 转向环比例系数
    .Kd_Turn = 500       	// 转向环微分系数
};
													//极性
//float Balance_Kp=400*0.6,Balance_Kd=1700*0.6;		//+
//float Velocity_Kp=0,Velocity_Ki=0;				//+
//float Turn_Kp=0,Turn_Kd=0;						//+

/**
  * 函    数：直立环
  * 参    数：Angle			实际角度
  *			  balance		平衡位置(目标角度) 
  *			  Gyro			角速度
  * 返 回 值：PWM			速度大小
  */
int Stand_Balance(float Angle, float balance, float Gyro)
{
	float bias;
	int PWM;
	bias = Angle - balance;
	PWM = data_PID.Kp_Stand*bias + data_PID.Kd_Stand*Gyro;
	return PWM;
}

/**
  * 函    数：速度环
  * 参    数：Encoder_Left		左轮编码器大小
  *			  Encoder_Right		右轮编码器大小
  * 返 回 值：Velocity			速度大小
  */
int Velocity_Balance(int16_t Expect_Speed, int16_t Encoder_Left, int16_t Encoder_Right)
{
	static int16_t Velocity, Encoder_Least, Encoder;
	static int16_t Encoder_Integral;
	Encoder_Least = Encoder_Left + Encoder_Right - Expect_Speed*2;
	Encoder *= 0.7f;
	Encoder += Encoder_Least*0.3f;
	Encoder_Integral += Encoder;
	if(Encoder_Integral>8000)Encoder_Integral=8000;
	if(Encoder_Integral<-8000)Encoder_Integral=-8000;
	Velocity = data_PID.V*Encoder + data_PID.V*Encoder_Integral/200;
	return Velocity;
}

int Turn_Balance(int16_t Expect_Angle, float Gyro_Z)
{
	int PWM;
	if(Expect_Angle==0)
	{
		PWM=data_PID.Kd_Turn*Gyro_Z;
	}
	else
	{
		PWM=data_PID.Kp_Turn*Expect_Angle;
	}
	return PWM;
}
