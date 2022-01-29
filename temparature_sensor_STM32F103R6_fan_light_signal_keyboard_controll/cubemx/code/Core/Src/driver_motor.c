/*
 * driver_motor.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Marko Micovic
 */

#include "tim.h"

#define ARR 9
//#define STEP 2
uint32_t STEP;
//#define INCREMENT ((ARR + 1) / STEP)
uint32_t INCREMENT;
//static uint32_t volatile compareRegisterValue = 0;
//
//void MOTOR_SpeedIncrease()
//{
//	if (compareRegisterValue < ARR + 1)
//	{
//		compareRegisterValue += INCREMENT;
//	}
//	htim3.Instance->CCR1 = compareRegisterValue;
//}
//
//void MOTOR_SpeedDecrease()
//{
//	if (compareRegisterValue > 0)
//	{
//		compareRegisterValue -= INCREMENT;
//	}
//	htim3.Instance->CCR1 = compareRegisterValue;
//}

void MOTOR_Init()
{
	STEP = 3;
	INCREMENT = ((ARR + 1) / STEP);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}
