#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdint.h>

#define SET_VALUE(target_register, start, end, value)\
	do{\
		uint32_t temp = target_register; \
		uint32_t mask = ~(~0 << (end - start + 1));\
		temp &= ~(mask << start); \
		temp |= ((value & mask) << start); \
		target_register = temp;\
	} while(0);



#endif // _UTILITY_H_
// end = 10, start = 8
// value = 101
// target = 00000000000000000000101010000000
//                               ^^^
// mask =   00000000000000000000000000000111
// temp =   00000000000000000000100010000000
// temp =   00000000000000000000110110000000
