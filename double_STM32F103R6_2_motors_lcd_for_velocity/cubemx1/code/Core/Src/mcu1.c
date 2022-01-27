/*
 * mcu1.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "mcu1.h"


#include "FreeRTOS.h"

#include "task.h"

#include "queue.h"
#include "uart_driver.h"

#include <string.h>

static uint8_t velocity[2] = {0, 0};

static void mcu1Task(void *p)
{

	while(1)
	{
		UART_AsyncTransmitString(VT, "Unesite operaciju u formatu <#motor><komanda>: ");
		char* command =	UART_BlockReceiveString(VT);

		if(command != NULL)
		{
			uint8_t error_happened = 0;
			if(strlen(command) == 3) {

			} else {
				//UART_AsyncTransmitString(VT, "Operacija mora imati 2 karaktera!\r");
				error_happened = 1;
			}
			if('1' <= command[0] && command[0] <= '2') {

			} else {
				//UART_AsyncTransmitString(VT, "Indeks motora mora biti ili 1 ili 2!\r");
				error_happened = 1;
			}
			if((command[1] == 'i' || command[1] == 'd')) {

			} else {
				//UART_AsyncTransmitString(VT, "Komanda mora bili ili 'i' (ubrzavanje) ili 'd' (usporavanje)!\r");
				error_happened = 1;
			}
			if(error_happened == 1)
			{
				vPortFree(command);
				continue;
			}

			MotorCommand mc;
			mc.motor = command[0] - '0' - 1;
			mc.velocity = velocity[mc.motor];
			if(command[1] == 'i' && mc.velocity < 16)
			{
				velocity[mc.motor]++;
				mc.velocity++;
			}
			if(command[1] == 'd' && mc.velocity > 0)
			{
				velocity[mc.motor]--;
				mc.velocity--;
			}



			UART_AsyncTransmitMotorCommand(MCU2, mc);
			vPortFree(command);
		}

	}
}

void MCU1_Init()
{
	UART_Init();

	xTaskCreate(mcu1Task, "mcu1Task", 64, NULL, 10, NULL);

}
