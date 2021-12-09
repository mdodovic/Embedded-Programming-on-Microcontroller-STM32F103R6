/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "clock.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
 */
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
					| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
					| GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PC0 PC1 PC2 PC3
	 PC4 PC5 PC6 PC7
	 PC8 PC9 PC10 PC11 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
			| GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PB10 */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PB12 PB13 PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */

void setGPIOB(uint16_t GPIO_PIN, GPIO_PinState GPIO_PIN_STATE)
{

	if (GPIO_PIN_12 <= GPIO_PIN && GPIO_PIN <= GPIO_PIN_15)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN, GPIO_PIN_STATE);
	}
}

extern volatile uint8_t seven_segment_digits[4];
extern uint32_t show_stopwatch_invert;
extern uint32_t tim1_update_event_ticks;
extern uint32_t seconds;
extern uint32_t minutes;

volatile uint32_t frequency = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// PIN11 is configured to accept interrupts on falling edge
	// PIN11 is configured to accept interrupts on both rising and falling edge

	if (GPIO_Pin == GPIO_PIN_11)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
	}
	if (GPIO_Pin == GPIO_PIN_10)
	{

		seven_segment_digits[0] = (frequency / 1000) % 10;
		seven_segment_digits[1] = (frequency / 100) % 10;
		seven_segment_digits[2] = (frequency / 10) % 10;
		seven_segment_digits[3] = frequency % 10;

		show_stopwatch_invert = ~show_stopwatch_invert;

		tim1_update_event_ticks = 0;
		seconds = 0;
		minutes = 0;

	}

}

#define FREQUENCY_CNT_CLK 8000

/*
 * STATE MACHINE
 * WAIT_FIRST_RISING_EDGE
 * WAIT_FALLING_EDGE
 * WAIT_SECOND_RISING_EDGE
 */

#define WAIT_FIRST_RISING_EDGE 0
#define WAIT_FALLING_EDGE 1
#define WAIT_SECOND_RISING_EDGE 2

volatile uint32_t state = WAIT_FIRST_RISING_EDGE;

volatile uint32_t overflowCounter = 0;

volatile uint32_t first_rising_edge_ticks = 0;
volatile uint32_t falling_edge_ticks = 0;
volatile uint32_t second_rising_edge_ticks = 0;



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	switch (state) {
		case WAIT_FIRST_RISING_EDGE:
			if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				first_rising_edge_ticks = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				overflowCounter = 0;
				state = WAIT_SECOND_RISING_EDGE;
			}
			break;
		case WAIT_FALLING_EDGE:

			break;
		case WAIT_SECOND_RISING_EDGE:
			if(htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				second_rising_edge_ticks = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

				uint32_t ticks = (second_rising_edge_ticks + (overflowCounter * 79)) - first_rising_edge_ticks;
				// this is number of ticks in 1 period
				// FREQUENCY_CNT_CLK - frequency for counter clock
				// 1 / FREQUENCY_CNT_CLK = period of counter clock

				frequency = 1000 * FREQUENCY_CNT_CLK / ticks; // 1000 * is for milli Hz

				overflowCounter = 0;
				state = WAIT_FIRST_RISING_EDGE;
			}

			break;
		default:
			break;
	}

}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
