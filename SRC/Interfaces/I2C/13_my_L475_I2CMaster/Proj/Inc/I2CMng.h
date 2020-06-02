#ifndef __I2CMNG_H
#define __I2CMNG_H
//#include "my_type.h"
#include <stdint.h>
#include "my_type.h"

#define	MAX_N_GAS_MTR		(30 + 2)

struct I2CDataFromGM
{
	uint8_t	address;
	volatile union for_all flags[1];
};


extern struct I2CDataFromGM i2cDataFromGM[];

#define mem_anin_alarm                  i2cDataFromGM[adrGasMtr].flags[0].str.Bit10

#endif//#define __DRVCMMI2C_H
