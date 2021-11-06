#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include <stdint.h>

typedef struct
{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;

} STK_Register_Map;

#define STK ((STK_Register_Map*)(0xE000E010))

#define CTRL_ENABLE    (1<<0)
#define CTRL_TICKINT   (1<<1)
#define CTRL_CLKSOURCE (1<<2)
#define CTRL_COUNTFLAG (1<<16)

extern void systick_callback();
extern void systick_init();

#endif //_SYSTICK_H_



