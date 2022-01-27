/*
 * lcd.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "lcd.h"

#include "FreeRTOS.h"

#include "queue.h"
#include "task.h"

#include "gpio.h"

#define LCD_ENABLE_BIT 0x40 // GPIOC bit for Enable

QueueHandle_t LCD_QueueHandle;
TaskHandle_t LCD_TaskHandle;

static void LCD_Write(LCD_CommandReg reg, LCD_CommandVal val)
{
	GPIOC->ODR = ((0x01 & reg) << 5) // RS bit
					|
				 (0x0F & val); // data D4-D7
	GPIOC->ODR |= LCD_ENABLE_BIT;
	// immediately write
	GPIOC->ODR &= ~LCD_ENABLE_BIT;
}

static void LCD_CommandInitiate(LCD_CommandReg reg, LCD_CommandVal val)
{
	LCD_Write(reg, val >> 4); // high 4b
	LCD_Write(reg, val);      // low 4b

	vTaskDelay(pdMS_TO_TICKS(2)); // wait for maximal time 2ms
}

static void LCD_Task(void* p)
{
	vTaskDelay(pdMS_TO_TICKS(20)); // wait for system reset

	LCD_Write(LCD_INSTRUCTION,
			(LCD_FUNCTION_SET_INSTRUCTION |
			LCD_FUNCTION_SET_4_BIT_INTERFACE // set 4b datasize instead of 8b
					) >> 4); // needs to be shifted because in lcd.h is right-shifted for 4

	vTaskDelay(pdMS_TO_TICKS(2)); // wait for maximal time 2ms (as in CommandInitiate
	// now we can send 8b as 4b normally

	LCD_CommandInitiate(LCD_INSTRUCTION,
						LCD_FUNCTION_SET_INSTRUCTION |
						LCD_FUNCTION_SET_4_BIT_INTERFACE |
						LCD_FUNCTION_SET_2_LINE |
						LCD_FUNCTION_SET_5x8_DOTS );

	LCD_CommandInitiate(LCD_INSTRUCTION,
						LCD_CONTROL_INSTRUCTION |
						LCD_CONTROL_BLINK_OFF |
						LCD_CONTROL_CURSOR_OFF |
						LCD_CONTROL_DISPLAY_ON );

	LCD_CommandInitiate(LCD_INSTRUCTION,
						LCD_ENTRY_MODE_INSTRUCTION |
						LCD_ENTRY_MODE_INC_ADR |
						LCD_ENTRY_MODE_SHIFT_OFF );

	// Return to home address for DDRAM to 0 for writting

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);

	// initiallization finished
	LCD_Command cmd;
	while(1)
	{
		xQueueReceive(LCD_QueueHandle, &cmd, portMAX_DELAY);
		LCD_CommandInitiate(cmd.reg, cmd.val);
	}

}


void LCD_Init()
{
	LCD_QueueHandle = xQueueCreate(64, sizeof(LCD_Command));
	xTaskCreate(LCD_Task, "LCD_Task", 128, NULL, 2, &LCD_TaskHandle);
}


void LCD_CommandEnqueue(LCD_CommandReg reg, LCD_CommandVal val)
{
	LCD_Command cmd = {reg, val};
	xQueueSend(LCD_QueueHandle, &cmd, portMAX_DELAY);
}

void LCD_CommandEnqueueFromISR(LCD_CommandReg reg, LCD_CommandVal val, BaseType_t* xHigherPriorityTaskWoken)
{
	LCD_Command cmd = {reg, val};
	xQueueSendFromISR(LCD_QueueHandle, &cmd, xHigherPriorityTaskWoken);
}


