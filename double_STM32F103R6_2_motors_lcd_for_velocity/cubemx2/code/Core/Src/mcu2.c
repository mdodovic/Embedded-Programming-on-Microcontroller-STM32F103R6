/*
 * mcu2.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu2.h"

#include "tim.h"

void MCU2_Init()
{

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);


}


