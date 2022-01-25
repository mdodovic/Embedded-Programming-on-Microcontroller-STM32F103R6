/*
 * lcd.c
 *
 *  Created on: Jan 25, 2022
 *      Author: matij
 */

#include "lcd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "gpio.h"


#define LCD_ENABLE_BIT 0x40


static void LCD_WRITE(LCD_CommandReg reg, LCD_CommandVal val)
{
	GPIOC->ODR = ((reg & 0x01) << 5) | (val & 0x0F);

	// Timing Characteristics ?
	GPIOC->ODR |= LCD_ENABLE_BIT;
	GPIOC->ODR &= ~LCD_ENABLE_BIT;

}

static void LCD_CommandInitiate(LCD_CommandReg reg, LCD_CommandVal val)
{
	LCD_WRITE(reg, val >> 4);
	LCD_WRITE(reg, val);
	vTaskDelay(pdMS_TO_TICKS(2));
}

static TaskHandle_t LCD_TaskHandle;

static QueueHandle_t LCD_QueueHandle;

static void LCD_Task(void *parameter)
{
	vTaskDelay(pdMS_TO_TICKS(20));

	LCD_WRITE(LCD_INSTRUCTION, (LCD_FUNCTION_SET_INSTRUCTION | LCD_FUNCTION_SET_4_BIT_INTERFACE) >> 4);

	vTaskDelay(pdMS_TO_TICKS(2));

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_FUNCTION_SET_INSTRUCTION | LCD_FUNCTION_SET_4_BIT_INTERFACE | LCD_FUNCTION_SET_2_LINE | LCD_FUNCTION_SET_5x8_DOTS);

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_CONTROL_INSTRUCTION | LCD_CONTROL_DISPLAY_ON | LCD_CONTROL_CURSOR_OFF | LCD_CONTROL_BLINK_OFF);

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_ENTRY_MODE_INSTRUCTION | LCD_ENTRY_MODE_INC_ADR | LCD_ENTRY_MODE_SHIFT_OFF);

	LCD_CommandInitiate(LCD_INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);

	LCD_Command command;
	while(1)
	{
		xQueueReceive(LCD_QueueHandle, &command, portMAX_DELAY);
		LCD_CommandInitiate(command.reg, command.val);
	}
}


void LCD_Init()
{
	LCD_QueueHandle = xQueueCreate(64, sizeof(LCD_Command));

	xTaskCreate(LCD_Task, "LCD_Task", 128, NULL, 2, &LCD_TaskHandle);
}


void LCD_CommandEnqueue(LCD_CommandReg reg, LCD_CommandVal val)
{

	LCD_Command command = {reg, val};
	xQueueSend(LCD_QueueHandle, &command, portMAX_DELAY);

}

void LCD_CommandEnqueueFromISR(LCD_CommandReg reg, LCD_CommandVal val, BaseType_t *pxHigherPriorityTaskWoken)
{

	LCD_Command command = {reg, val};
	xQueueSendFromISR(LCD_QueueHandle, &command, pxHigherPriorityTaskWoken);

}
