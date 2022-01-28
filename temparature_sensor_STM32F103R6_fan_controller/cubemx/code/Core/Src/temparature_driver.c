/*
 * temparature_driver.c
 *
 *  Created on: Jan 27, 2022
 *      Author: matij
 */

#include "temparature_driver.h"
#include <stdint.h>

float current_temparature = 45.0;

float TEMP_GetCurrentTemparatureValue()
{

	current_temparature -= 0.5;

	return current_temparature;
}

