/*
 * casovnik.c
 *
 *  Created on: Dec 4, 2021
 *      Author: matij
 */



#include "casovnik.h"

TIM_HandleTypeDef *casovnik_tim_handle = &htim1;

uint32_t counter = 0;

void casovnik_TIM_odbrojao(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == casovnik_tim_handle->Instance)
		{
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			if(++counter == 5)
			{
				HAL_TIM_Base_Stop_IT(casovnik_tim_handle);
			}
		}
}

void casovnik()
{

	HAL_TIM_Base_Start(casovnik_tim_handle);


	for(int i = 0; i < 5; i ++)
	{

		while((casovnik_tim_handle->Instance->SR & TIM_SR_UIF) == 0)
		{
			// busy wait
		}

		casovnik_tim_handle->Instance->SR = ~TIM_SR_UIF;

		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

	}

	HAL_TIM_Base_Stop(casovnik_tim_handle);

	HAL_TIM_RegisterCallback(casovnik_tim_handle, HAL_TIM_PERIOD_ELAPSED_CB_ID, &casovnik_TIM_odbrojao);

	HAL_TIM_Base_Start_IT(casovnik_tim_handle);

}


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//	if(htim->Instance == casovnik_tim_handle->Instance)
//	{
//		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
//		if(++counter == 5)
//		{
//			HAL_TIM_Base_Stop_IT(casovnik_tim_handle);
//		}
//	}
//}


