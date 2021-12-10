/*
 * basketball.h
 *
 *  Created on: Dec 10, 2021
 *      Author: matij
 */

#ifndef CORE_INC_BASKETBALL_H_
#define CORE_INC_BASKETBALL_H_

#include "main.h"
#include "gpio.h"
#include "tim.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void basketball_init();

#endif /* CORE_INC_BASKETBALL_H_ */
