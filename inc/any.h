/*bin/echo
cc -E $0
exit
*/

#ifndef _ANY_H_
#define _ANY_H_

#include <stdint.h>

typedef union any {
	void *ptr;
	char  c;
	short s;
	int   i;
	long  l;
	union {
		unsigned char  c;
		unsigned short s;
		unsigned int   i;
		unsigned long  l;
	} u;
	int8_t  i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;
	uint8_t  u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
} any_t;

#define ANY_GENERATE_UNION(TYPE, BITS) \
	typedef union any_ ## TYPE {       \
		int  ## BITS ## _t i ## BITS;  \
		uint ## BITS ## _t u ## BITS;  \
		unsigned TYPE u;               \
		TYPE i;                        \
	} __attribute__((transparent_union)) any_ ## TYPE ## _t


ANY_GENERATE_UNION(char,   8);
ANY_GENERATE_UNION(short, 16);
ANY_GENERATE_UNION(int,   32);
ANY_GENERATE_UNION(long,  64);

#endif
