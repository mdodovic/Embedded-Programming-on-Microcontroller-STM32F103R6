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

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
void basketball_init()
{
	HAL_TIM_Base_Start_IT(&htim1);

	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
}



uint8_t seven_segment_map[] =
{ 0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84 };

#define MATCH_TIME 0
#define POINTS 1
#define TIME_FOR_ATTACK 2

uint32_t what_to_show = MATCH_TIME;


volatile uint32_t digits_for_show[] =
{ 1, 0, 0, 0 };

volatile uint32_t one_attack_for_show[] =
{ 2, 4, 0, 0 };

volatile uint32_t points_for_show[] =
{ 0, 0, 0, 0 };


uint32_t round_robin_digit = 0;


uint32_t counter = 0;

void fill_seven_segment_display()
{
	GPIOC->ODR &= ~0xFFF;

	switch (what_to_show) {
		case MATCH_TIME:
			GPIOC->ODR |= seven_segment_map[digits_for_show[round_robin_digit]];
			GPIOC->ODR |= 0x1 << (8 + round_robin_digit);

			break;

		case POINTS:
			GPIOC->ODR |= seven_segment_map[points_for_show[round_robin_digit]];
			GPIOC->ODR |= 0x1 << (8 + round_robin_digit);

			break;

		case TIME_FOR_ATTACK:
			if(round_robin_digit == 1)
				GPIOC->ODR |= seven_segment_map[one_attack_for_show[round_robin_digit]] & ~(1 << 7);
			else
				GPIOC->ODR |= seven_segment_map[one_attack_for_show[round_robin_digit]];
			GPIOC->ODR |= 0x1 << (8 + round_robin_digit);

			break;
	}


}

uint32_t hundrets_attack = 2400;

uint32_t minutes = 10;
uint32_t seconds = 0;
uint32_t pause_game = 0;

uint32_t tim3_overflow_counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		if(pause_game == 0)
		{

			hundrets_attack--;
			if(hundrets_attack == 0)
			{
				hundrets_attack = 2400;
			}

			one_attack_for_show[0] = (hundrets_attack / 1000) % 10;
			one_attack_for_show[1] = (hundrets_attack / 100) % 10;
			one_attack_for_show[2] = (hundrets_attack / 10) % 10;
			one_attack_for_show[3] = (hundrets_attack / 1) % 10;

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
		}
		round_robin_digit = (round_robin_digit + 1) % 4;

		fill_seven_segment_display();

	}
	if(htim->Instance == htim3.Instance)
	{
		tim3_overflow_counter++;
	}
}

uint32_t player1_points = 0;
uint32_t player2_points = 0;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_14)
	{
		// Diode
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
		pause_game = 1 - pause_game;
	}
	if(GPIO_Pin == GPIO_PIN_13)
	{
		// P2 points
		if(pause_game == 0)
		{
			player2_points += 2;
			points_for_show[2] = (player2_points / 10) % 10;
			points_for_show[3] = player2_points % 10;
		}
	}
	if(GPIO_Pin == GPIO_PIN_12)
	{
		// P1 points
		if(pause_game == 0)
		{
			player1_points += 2;
			points_for_show[0] = (player1_points / 10) % 10;
			points_for_show[1] = player1_points % 10;
		}
	}
	if(GPIO_Pin == GPIO_PIN_11)
	{
		// Change showing on display
		what_to_show = (what_to_show + 1) % 3;
	}

}

#define ARR 9
uint32_t rising_edge_ticks = 0;
uint32_t falling_edge_ticks = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim3.Instance)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			rising_edge_ticks = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
			tim3_overflow_counter = 0;
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			falling_edge_ticks = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
			falling_edge_ticks = falling_edge_ticks + tim3_overflow_counter * ARR;

			if(falling_edge_ticks - rising_edge_ticks >= 2000)
			{

				player1_points = 0;
				player2_points = 0;

				points_for_show[0] = (player1_points / 10) % 10;
				points_for_show[1] = player1_points % 10;
				points_for_show[2] = (player2_points / 10) % 10;
				points_for_show[3] = player2_points % 10;

				hundrets_attack = 2400;

				one_attack_for_show[0] = (hundrets_attack / 1000) % 10;
				one_attack_for_show[1] = (hundrets_attack / 100) % 10;
				one_attack_for_show[2] = (hundrets_attack / 10) % 10;
				one_attack_for_show[3] = (hundrets_attack / 1) % 10;

				minutes = 10;
				seconds = 0;

				digits_for_show[0] = minutes / 10;
				digits_for_show[1] = minutes % 10;
				digits_for_show[2] = seconds / 10;
				digits_for_show[3] = seconds % 10;

				pause_game = 0;

				counter = 0;

				what_to_show = MATCH_TIME;

				round_robin_digit = 0;


			}

			tim3_overflow_counter = 0;
		}

	}
}
