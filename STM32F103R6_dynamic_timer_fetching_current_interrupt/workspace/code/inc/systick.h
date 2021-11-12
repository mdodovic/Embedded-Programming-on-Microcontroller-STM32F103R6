#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>

typedef struct{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;

} STK_Register_Map;

#define STK ((STK_Register_Map*)(0xE000E010))


#endif // _SYSTICK_H_
