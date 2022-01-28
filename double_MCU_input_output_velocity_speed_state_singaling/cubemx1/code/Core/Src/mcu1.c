/*
 * mcu.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "mcu1.h"

#include <string.h>
#include <stdlib.h>

#include "uart_driver.h"

#include "FreeRTOS.h"
#include "task.h"

uint8_t velocity[2] = { 0, 0 };

void MCU1_Task(void* p)
{

	while (1)
	{
		UART_AsyncTransmitString(VT, "Unesite komandu:");
		char *input = UART_BlockReceiveString(VT);

		if (input != NULL)
		{
			if (strlen(input) == 2 && '1' <= input[0] && input[0] <= '2'
					&& (input[1] == 'd' || input[1] == 'i'))
			{
				uint8_t motor = input[0] - '0';
				switch (input[1])
				{
				case 'd':
					// decrease
					if (velocity[motor - 1] > 0)
					{
						velocity[motor - 1]--;
					}
					break;
				case 'i':
					// increase
					if (velocity[motor - 1] < 16)
					{
						velocity[motor - 1]++;
					}
					break;
				}
				MotorCommand motorCommand =
				{ .motor = motor, .velocity = velocity[motor - 1] };
				UART_AsyncTransmitxMotorCommand(MCU2, motorCommand);
			}
			else
			{
				UART_AsyncTransmitString(VT, "Pogresan format komande!\r");
			}

			vPortFree(input);
		}
	}

}


void MCU1_Init()
{
	UART_Init();

	xTaskCreate(MCU1_Task, "mcu", 64, NULL, 5, NULL);

}
