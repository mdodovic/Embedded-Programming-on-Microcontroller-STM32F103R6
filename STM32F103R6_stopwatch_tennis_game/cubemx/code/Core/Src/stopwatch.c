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
 * |-----|||-----|-----|-----|-----|-----|-----|-----|-----|||-----|
 *
 */
uint8_t seven_segment_digits[] =
{ 0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84 };

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

				digits_to_be_shown[0] = minutes / 10;
				digits_to_be_shown[1] = minutes % 10;
				digits_to_be_shown[2] = seconds / 10;
				digits_to_be_shown[3] = seconds % 10;

			}
		}
		current_field_on_display = (current_field_on_display + 1) % 4;

		GPIOC->ODR &= ~0xFFF;

		GPIOC->ODR |= seven_segment_digits[digits_to_be_shown[current_field_on_display]];
		GPIOC->ODR |= 0x1 << (current_field_on_display + 8);

	}
}
