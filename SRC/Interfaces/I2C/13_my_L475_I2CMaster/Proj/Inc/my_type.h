/*
 * my_type.h
 *
 *  Created on: 1 но€б. 2017 г.
 *      Author: lukyanovkyu
 */

#ifndef MY_TYPE_H_
#define MY_TYPE_H_

#include <stdint.h>



typedef union for_all
{
	uint8_t arr[1];

	struct
	{
		uint8_t Bit0 :1, Bit1 :1, Bit2 :1, Bit3 :1, Bit4 :1, Bit5 :1, Bit6 :1,
				Bit7 :1;
	} str;

	struct
	{
		uint8_t TwoBit0 :2, TwoBit1 :2, TwoBit2 :2, TwoBit3 :2;
	} str_twobits;
} union_for_all;



typedef union for_all_32
{
	uint8_t arr[4];
	uint32_t myword32;
	struct
	{
		uint32_t Bit0 :1, Bit1 :1, Bit2 :1, Bit3 :1, Bit4 :1, Bit5 :1, Bit6 :1,
				Bit7 :1, Bit8 :1, Bit9 :1, Bit10 :1, Bit11 :1, Bit12 :1,
				Bit13 :1, Bit14 :1, Bit15 :1, Bit16 :1, Bit17 :1, Bit18 :1,
				Bit19 :1, Bit20 :1, Bit21 :1, Bit22 :1, Bit23 :1, Bit24 :1,
				Bit25 :1, Bit26 :1, Bit27 :1, Bit28 :1, Bit29 :1, Bit30 :1,
				Bit31 :1;
	} str;
	

	struct
	{
		uint32_t Bit0 :1, Bit1 :1, Bit2 :1, Bit3 :1, Bit4 :1, Bit5 :1, Bit6 :1,
				Bit7 :1, Bit8 :1, Bit9 :1, Bit10 :1, Bit11 :1, Bit12 :1,
				Bit13 :1, Bit14 :1, Bit15 :1, Bit16 :1, Bit17 :1, Bit18 :1,
				Bit19 :1, Bit20 :1, Bit21 :1, Bit22 :1, Bit23 :1, Bit24 :1,
				Bit25 :1, Bit26 :1, Bit27 :1, Bit28 :1, Bit29 :1, Bit30 :1,
				Bit31 :1;
	} str32;
	

	struct
	{
		uint32_t TwoBit0 :2, TwoBit1 :2, TwoBit2 :2, TwoBit3 :2;
	} str_twobits;
} union_for_all_32;

typedef union for_all_64
{
	uint8_t arr[8];
	uint64_t myword64;
	struct
	{
		uint64_t Bit0 :1, Bit1 :1, Bit2 :1, Bit3 :1, Bit4 :1, Bit5 :1, Bit6 :1,
				Bit7 :1, Bit8 :1, Bit9 :1, Bit10 :1, Bit11 :1, Bit12 :1,
				Bit13 :1, Bit14 :1, Bit15 :1, Bit16 :1, Bit17 :1, Bit18 :1,
				Bit19 :1, Bit20 :1, Bit21 :1, Bit22 :1, Bit23 :1, Bit24 :1,
				Bit25 :1, Bit26 :1, Bit27 :1, Bit28 :1, Bit29 :1, Bit30 :1,
				Bit31 :1, Bit32 :1, Bit33 :1, Bit34 :1, Bit35 :1, Bit36 :1,
				Bit37 :1, Bit38 :1, Bit39 :1, Bit40 :1, Bit41 :1, Bit42 :1,
				Bit43 :1, Bit44 :1, Bit45 :1, Bit46 :1, Bit47 :1, Bit48 :1,
				Bit49 :1, Bit50 :1, Bit51 :1, Bit52 :1, Bit53 :1, Bit54 :1,
				Bit55 :1, Bit56 :1, Bit57 :1, Bit58 :1, Bit59 :1, Bit60 :1,
				Bit61 :1, Bit62 :1, Bit63 :1;
	} str64;


	struct
	{
		uint64_t TwoBit0 :2, TwoBit1 :2, TwoBit2 :2, TwoBit3 :2;
	} str_twobits;
} union_for_all_64;

#endif /* MY_TYPE_H_ */
