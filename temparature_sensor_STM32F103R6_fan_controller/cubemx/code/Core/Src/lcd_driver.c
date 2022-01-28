/*
 * lcd_driver.c
 *
 *  Created on: Jan 28, 2022
 *      Author: matij
 */

#include "lcd_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "gpio.h"

static void LCD_WRITE(LCD_CommandReg reg, LCD_CommandVal val)
{
	GPIOC->ODR = ((reg & 0x01) << 5) | (val & 0x0F);
	GPIOC->ODR |= 0x01 << 6;
	GPIOC->ODR &= ~(0x01 << 6);
}

static void LCD_CommandInitiate(LCD_CommandReg reg, LCD_CommandVal val)
{
	LCD_WRITE(reg, val >> 4);
	LCD_WRITE(reg, val);
	vTaskDelay(pdMS_TO_TICKS(2));
}

QueueHandle_t LCD_QueueHandle;
TaskHandle_t LCD_TaskHandle;

void LCD_Task(void* p)
{

	// Init

	vTaskDelay(pdMS_TO_TICKS(20));

	LCD_WRITE(LCD_INSTRUCTION, (LCD_FUNCTION_SET_INSTRUCTION | LCD_FUNCTION_SET_4_BIT_INTERFACE) >> 4);
	vTaskDelay(pdMS_TO_TICKS(2));


	LCD_CommandInitiate(LCD_INSTRUCTION, (LCD_FUNCTION_SET_INSTRUCTION |
											LCD_FUNCTION_SET_2_LINE |
											LCD_FUNCTION_SET_5x8_DOTS |
											LCD_FUNCTION_SET_4_BIT_INTERFACE
											));

	LCD_CommandInitiate(LCD_INSTRUCTION, (LCD_CONTROL_INSTRUCTION |
											LCD_CONTROL_BLINK_OFF |
											LCD_CONTROL_CURSOR_OFF |
											LCD_CONTROL_DISPLAY_ON
											));

	LCD_CommandInitiate(LCD_INSTRUCTION, (LCD_ENTRY_MODE_INSTRUCTION |
											LCD_ENTRY_MODE_INC_ADR |
											LCD_ENTRY_MODE_SHIFT_OFF
											));

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);

	// Write

	LCD_Command cmd;
	while(1)
	{
		xQueueReceive(LCD_QueueHandle, &cmd, portMAX_DELAY);
		LCD_CommandInitiate(cmd.reg, cmd.val);
	}
}


void LCD_Init()
{
	LCD_QueueHandle = xQueueCreate(128, sizeof(LCD_Command));
	xTaskCreate(LCD_Task, "LCD_Task", 64, NULL, 2, NULL);
}



void LCD_CommandEnqueue(LCD_CommandReg reg, LCD_CommandVal val)
{
	LCD_Command cmd = { reg, val };
	xQueueSend(LCD_QueueHandle, &cmd, portMAX_DELAY);

}


void LCD_CommandEnqueueFromISR(LCD_CommandReg reg, LCD_CommandVal val, BaseType_t *pxHigherPriorityTaskWoken)
{
	LCD_Command cmd = { reg, val };
	xQueueSendFromISR(LCD_QueueHandle, &cmd, pxHigherPriorityTaskWoken);

}
