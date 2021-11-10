#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdint.h>

#define SET_VALUE(target_register, start, end, value)\
	do{\
		uint32_t temp = (target_register); \
		uint32_t mask = ~(~0 << ((end) - (start) + 1)); \
		temp &= ~(mask << start); \
		temp |= ((value) & mask) << start; \
		target_register = temp; \
		} while(0);

#define OR_VALUE(target_register, start, end, value)\
	do{\
		uint32_t temp = (target_register); \
		uint32_t mask = ~(~0 << ((end) - (start) + 1)); \
		temp &= ~(mask << start); \
		temp |= ((value) & mask) << start; \
		target_register |= temp; \
		} while(0);


#endif // _UTILITIES_H_

// start = 8
// end = 10
// val  = 00000000000000000000000000000011
// mask = 00000000000000000000000000000111
// temp = 00000000000000000000010100000000
// temp &= ~(mask << start)
// temp>  00000000000000000000000000000000
// 							   ^^^ cleared
// temp |= ((value) & mask) << start;
// temp>  00000000000000000000001100000000
// 							   ^^^ cleared

