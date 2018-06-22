/*
 * int.h
 *
 * Created: 27/03/2018 2:24:52
 *  Author: BADROUS
 */ 


#ifndef INT_H_
#define INT_H_
//-----------------------------------------new data types
typedef unsigned char boolean;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;
typedef unsigned long long u64;
typedef signed long long s64;
//----------------------------------------------------------
typedef struct
{
	u32 mantissa : 23;
	s32 exponent : 8;
	u8 sign : 1;
} FLOAT_STRUCT;
//----------------------------------------------------------
typedef union
{
	float f;
	FLOAT_STRUCT fs;
} FLOAT_DATA;
//----------------------------------------------------
#endif /* INT_H_ */