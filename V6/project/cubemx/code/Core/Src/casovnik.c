/*
 * casovnik.c
 *
 *  Created on: Dec 4, 2021
 *      Author: matij
 */



#include "casovnik.h"

TIM_HandleTypeDef *casovnik_tim_handle = &htim1;

void casovnik()
{

//	HAL_TIM_Base_Start(&htim1);
//
//
//	for(;;)
//	{
//
//		while((htim1.Instance->SR & TIM_SR_UIF) == 0)
//		{
//			// busy wait
//		}
//
//		htim1.Instance->SR = ~TIM_SR_UIF;
//
//		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
//
//	}


	HAL_TIM_Base_Start_IT(casovnik_tim_handle);

}
