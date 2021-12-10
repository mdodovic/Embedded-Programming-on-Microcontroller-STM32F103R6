/*
 * basketball.c
 *
 *  Created on: Dec 10, 2021
 *      Author: matij
 */

#include "basketball.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_14)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
	}

}

extern TIM_HandleTypeDef htim1;


void basketball_init()
{
	HAL_TIM_Base_Start_IT(&htim1);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}
