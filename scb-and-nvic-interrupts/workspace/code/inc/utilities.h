#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdint.h>

#define SET_VALUE(target_register, start_bit, end_bit, value)\
	do{ \
		uint32_t temp = (target_register); \
		uint32_t mask = ~(~0 << ((end_bit) - (start_bit) + 1)); \
		temp &= ~(mask << start_bit); \
		temp |= ((value) & mask) << start_bit; \
		(target_register) = temp; \
	} while(0);

#endif //_UTILITIES_H_



