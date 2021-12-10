/*
 * stopwatch.c
 *
 *  Created on: Dec 10, 2021
 *      Author: matij
 */

#include "stopwatch.h"

/*
 *
 *                              A
 *                           *******
 *                         *         *
 *                       F *         * B
 *                         *    G    *
 *                           *******
 *                         *         *
 *                       E *         * C
 *                         *    D    *
 *                           *******
 *
 *
 * |-----|||-----|-----|-----|-----|-----|-----|-----|-----|||-----|
 * |     ||| PC7 | PC6 | PC5 | PC4 | PC3 | PC2 | PC1 | PC0 |||     |
 * |-----|||-----|-----|-----|-----|-----|-----|-----|-----|||-----|
 * | DIG ||| DP  |  A  |  B  |  C  |  D  |  E  |  F  |  G  ||| VAL |
 * |-----|||-----|-----|-----|-----|-----|-----|-----|-----|||-----|
 * |  0  |||  1  |  0  |  0  |  0  |  0  |  0  |  0  |  1  ||| 81  |
 * |  1  |||  1  |  1  |  0  |  0  |  1  |  1  |  1  |  1  ||| CF  |
 * |  2  |||  1  |  0  |  0  |  1  |  0  |  0  |  1  |  0  ||| 92  |
 * |  3  |||  1  |  0  |  0  |  0  |  0  |  1  |  1  |  0  ||| 86  |
 * |  4  |||  1  |  1  |  0  |  0  |  1  |  1  |  0  |  0  ||| CC  |
 * |  5  |||  1  |  0  |  1  |  0  |  0  |  1  |  0  |  0  ||| A4  |
 * |  6  |||  1  |  0  |  1  |  0  |  0  |  0  |  0  |  0  ||| A0  |
 * |  7  |||  1  |  0  |  0  |  0  |  1  |  1  |  1  |  1  ||| 8F  |
 * |  8  |||  1  |  0  |  0  |  0  |  0  |  0  |  0  |  0  ||| 80  |
 * |  9  |||  1  |  0  |  0  |  0  |  0  |  1  |  0  |  0  ||| 84  |
 * |  d  |||  1  |  1  |  0  |  0  |  0  |  0  |  1  |  0  ||| C2  |
 * |  A  |||  1  |  0  |  0  |  0  |  1  |  0  |  0  |  0  ||| 88  |
 * |-----|||-----|-----|-----|-----|-----|-----|-----|-----|||-----|
 *
 */
uint8_t seven_segment_digits[] =
{ 0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, 0xC2, 0x88};

extern TIM_HandleTypeDef htim1;

void stopwatch_init()
{
	HAL_TIM_Base_Start_IT(&htim1);

}

volatile uint32_t digits_to_be_shown[] =
{0, 0, 0, 0};

uint32_t current_field_on_display = 0;

uint32_t tim1_ticks_counter = 0; // TIM1 is configured to set UE every 10ms
uint32_t seconds = 0;
uint32_t minutes = 0;

uint32_t stopwatch_paused = 1;

#define STOPWATCH 0
#define POINTS_IN_GAM 1
#define POINTS_IN_SET 2

uint32_t mode_of_display = STOPWATCH;

void fill_digits_to_be_shown()
{
	switch (mode_of_display) {
		case STOPWATCH:

			digits_to_be_shown[0] = minutes / 10;
			digits_to_be_shown[1] = minutes % 10;
			digits_to_be_shown[2] = seconds / 10;
			digits_to_be_shown[3] = seconds % 10;

			break;
		case POINTS_IN_GAM:

			digits_to_be_shown[0] = 10;
			digits_to_be_shown[1] = 10;
			digits_to_be_shown[2] = 10;
			digits_to_be_shown[3] = 10;

			break;
		case POINTS_IN_SET:

			digits_to_be_shown[0] = 11;
			digits_to_be_shown[1] = 11;
			digits_to_be_shown[2] = 11;
			digits_to_be_shown[3] = 11;

			break;
		default:
			break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim1.Instance == htim->Instance)
	{
		if(stopwatch_paused == 0)
		{
			if(++tim1_ticks_counter == 100)
			{
				tim1_ticks_counter = 0;
				if(++seconds == 60)
				{
					seconds = 0;
					if(++minutes == 60)
					{
						minutes = 0;
					}
				}

			}
		}

		fill_digits_to_be_shown();

		current_field_on_display = (current_field_on_display + 1) % 4;

		GPIOC->ODR &= ~0xFFF;

		GPIOC->ODR |= seven_segment_digits[digits_to_be_shown[current_field_on_display]];
		GPIOC->ODR |= 0x1 << (current_field_on_display + 8);

	}
}




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin == GPIO_PIN_8)
	{
		mode_of_display = (mode_of_display + 1) % 3;
	}
	if(GPIO_Pin == GPIO_PIN_9)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		stopwatch_paused = 1 - stopwatch_paused;
	}
	if(GPIO_Pin == GPIO_PIN_10)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		stopwatch_paused = 1 - stopwatch_paused;
	}
	if(GPIO_Pin == GPIO_PIN_11)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
		stopwatch_paused = 1 - stopwatch_paused;
	}


}


