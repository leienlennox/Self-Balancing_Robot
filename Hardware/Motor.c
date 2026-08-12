 #include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "IMU.h"

int PWM_MAX=7200;
int PWM_MIN=-7200;
int deedline=250;		//死区设置

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA4,PA5,PA2,PA3引脚初始化为推挽输出	
	
	PWM_Init(7200,1);													//初始化直流电机的底层PWM
}

/**
  * 函    数：电机限速
  * 参    数：*Left_Motor	左轮速度
  *			  *Right_Motor	右轮速度
  * 返 回 值：无
  */
void Motor_Limitation(int *Left_Motor,int *Right_Motor)
{
	if(*Left_Motor>PWM_MAX)
	{
		*Left_Motor=PWM_MAX;
	}
	if(*Left_Motor<PWM_MIN)
	{
		*Left_Motor=PWM_MIN;
	}
	if(*Right_Motor>PWM_MAX)
	{
		*Right_Motor=PWM_MAX;
	}
	if(*Right_Motor<PWM_MIN)
	{
		*Right_Motor=PWM_MIN;
	}
	if(imu_Angle.Pitch>=56||imu_Angle.Pitch<=-44)
	{
		*Left_Motor=0;
		*Right_Motor=0;
	}
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-7200~7200
  * 返 回 值：无
  */
void Motor_SetSpeed_Left(int16_t Speed)
{
    
    if (Speed >= 0) 
	{
        PWM_SetCompare1(Speed+deedline);
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    } 
	else 
	{
        PWM_SetCompare1(deedline-Speed);
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
    }
}

void Motor_SetSpeed_Right(int16_t Speed)
{
    
    if (Speed >= 0) 
	{
        PWM_SetCompare4(Speed+deedline);
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
        GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    } 
	else 
	{
        PWM_SetCompare4(deedline-Speed);
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);  // 修正方向控制
        GPIO_SetBits(GPIOA, GPIO_Pin_3);
    }
}
