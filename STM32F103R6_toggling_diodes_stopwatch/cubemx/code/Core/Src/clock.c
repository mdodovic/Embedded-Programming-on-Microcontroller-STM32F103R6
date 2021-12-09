/*
 * clock.c
 *
 *  Created on: Dec 9, 2021
 *      Author: matij
 */

#include "clock.h"

/*
 * Niz vrednosti koje je potrebno postaviti
 * na katode sedmosegmentnog displeja sa zajednickom anodom
 * kako bi se prikazala odgovarajuca decimalna cifra
 * ukoliko je povezivanje pinova izvrseno na naznacen nacin.
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
uint8_t seven_segment_hexa[] =
{ 0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84 };

extern TIM_HandleTypeDef htim1;

void clock_init()
{
	GPIOC->ODR = seven_segment_hexa[1];

	HAL_TIM_Base_Start_IT(&htim1);
}

uint8_t current_digit = 0;
volatile uint8_t seven_segment_digits[4] =
{ 0, 0, 0, 0};

uint32_t show_stopwatch_invert = 0;

uint32_t tim1_update_event_ticks = 0;
uint32_t seconds = 0;
uint32_t minutes = 0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim1.Instance == htim->Instance)
	{

		if(++tim1_update_event_ticks == 100)
		{
			tim1_update_event_ticks = 0; // local counter 100 ticks x 10ms = 1000ms = 1s;

			seconds++;

			if(seconds == 60)
			{
				seconds = 0;
				minutes++;
				if(minutes == 60)
				{
					minutes = 0;
				}
			}

			if(show_stopwatch_invert == 0)
			{
				seven_segment_digits[0] = minutes / 10;
				seven_segment_digits[1] = minutes % 10;
				seven_segment_digits[2] = seconds / 10;
				seven_segment_digits[3] = seconds % 10;
			}
		}


		current_digit = (current_digit + 1) % 4;

		// what is going to be shown on next showing digit
		GPIOC->ODR &= ~0xFFF;
		GPIOC->ODR |= seven_segment_hexa[seven_segment_digits[current_digit]];

		GPIOC->ODR |= 0x1 << (8 + current_digit);


//		current_digit = (current_digit + 1) % 4;
//
//		GPIOC->ODR &= ~0xF00;
//
//		GPIOC->ODR &= ~0xFF;
//		GPIOC->ODR |= seven_segment_hexa[seven_segment_digits[current_digit]];
//
//		GPIOC->ODR |= 0x1 << (8 + current_digit);

	}
}
