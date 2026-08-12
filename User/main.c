#include "Init.h"

uint8_t Bluetooth_RxData;
uint16_t Result;

int16_t Encoder_Left,Encoder_Right;

int LeftMotor_Speed,RightMotor_Speed;
uint8_t MPU6050_EXIT_Flag;
int Stand_Balance_out;
int Velocity_Balance_out;
int Turn_Balance_out;

float Expect_Angle=7;			//机械平衡中值
int16_t Expect_Speed=0;			//期望平衡时速度
int16_t Expect_TurnAngle=0;		//期望平衡时转动角速度

int main(void)
{
	All_Init();
	
	while(1)
	{
		OLED_ShowSignedNum(1,1,Encoder_Left,5);			//左轮编码器反馈速度
		OLED_ShowSignedNum(1,8,Encoder_Right,5);		//右轮编码器反馈速度
		OLED_ShowSignedNum(2,1,imu_Angle.Pitch,5);		//倾斜角度
		OLED_ShowSignedNum(2,8,imu_data.GZ,5);			//Z轴角速度
		OLED_ShowSignedNum(3,1,LeftMotor_Speed,5);		//左轮速度
		OLED_ShowSignedNum(3,8,RightMotor_Speed,5);		//右轮速度
	}
}

/**
  * 函    数：MPU6050中断(采样率:200Hz)
  * 参    数：无
  * 返 回 值：无
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
		{
			MPU6050_EXIT_Flag=1;
			if(MPU6050_EXIT_Flag==1)
			{
				Encoder_Left = Read_Speed(4);	//编码器速度的获取
				Encoder_Right = Read_Speed(2);
				
				IMU_GetEuleranAngle();
				
				Stand_Balance_out = Stand_Balance(imu_Angle.Pitch, Expect_Angle, imu_data.GY);
				
				Velocity_Balance_out = Velocity_Balance(Expect_Speed,Encoder_Left, Encoder_Right);
				
				Turn_Balance_out = Turn_Balance(Expect_TurnAngle, imu_data.GZ);
				
				LeftMotor_Speed = Stand_Balance_out + Velocity_Balance_out - Turn_Balance_out;
				RightMotor_Speed = Stand_Balance_out + Velocity_Balance_out + Turn_Balance_out;
				
				Motor_Limitation(&LeftMotor_Speed, &RightMotor_Speed);
				
				Motor_SetSpeed_Left(LeftMotor_Speed);
				Motor_SetSpeed_Right(RightMotor_Speed);
			}
			MPU6050_EXIT_Flag=0;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
