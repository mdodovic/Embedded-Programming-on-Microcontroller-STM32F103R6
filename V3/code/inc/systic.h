#ifndef _SYSTIC_H_
#define _SYSTIC_H_

#define STK_CTRL (*((uint32_t*) (0xE000E010 + 0x00)))
#define STK_LOAD (*((uint32_t*) (0xE000E010 + 0x04)))
#define STK_VAL (*((uint32_t*) (0xE000E010 + 0x08)))
#define STK_CALIB (*((uint32_t*) (0xE000E010 + 0x0C)))

extern void systick_init();
extern void systick_handler();

#endif //_SYSTIC_H_
