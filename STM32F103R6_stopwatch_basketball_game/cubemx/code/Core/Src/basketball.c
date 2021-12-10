/*
 * basketball.c
 *
 *  Created on: Dec 10, 2021
 *      Author: matij
 */

#include "basketball.h"

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
uint8_t seven_segment_map[] =
{ 0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84 };



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

volatile uint32_t digits_for_show[] =
{ 1, 0, 0, 0 };

uint32_t round_robin_digit = 0;

uint32_t counter = 0;

void fill_seven_segment_display()
{
	GPIOC->ODR &= ~0xFFF;

	GPIOC->ODR |= seven_segment_map[digits_for_show[round_robin_digit]];
	GPIOC->ODR |= 0x1 << (8 + round_robin_digit);

}

uint32_t minutes = 10;
uint32_t seconds = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		counter++;
		if(counter == 100)
		{
			counter = 0;

			seconds--;

			if(seconds == -1)
			{
				seconds = 59;
				minutes--;
				if(minutes == -1)
				{
					minutes = 10;
					seconds = 0;
				}
			}

			digits_for_show[0] = minutes / 10;
			digits_for_show[1] = minutes % 10;
			digits_for_show[2] = seconds / 10;
			digits_for_show[3] = seconds % 10;

		}

		round_robin_digit = (round_robin_digit + 1) % 4;

		fill_seven_segment_display();

	}
}
