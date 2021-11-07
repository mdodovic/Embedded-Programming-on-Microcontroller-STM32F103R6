#ifndef _UTILITY_H_
#define _UTILITY_H_

#define SET_VALUE(target, startBitIncl, endBitIncl, value) \
	do { \
		uint32_t temp = (target); \
		uint32_t mask = ~(~0 << ((endBitIncl) - (startBitIncl) + 1)); \
		temp &= ~(mask << startBitIncl); \
		temp |= ((value) & mask) << startBitIncl; \
		(target) = temp; \
	} while(0)

#endif
