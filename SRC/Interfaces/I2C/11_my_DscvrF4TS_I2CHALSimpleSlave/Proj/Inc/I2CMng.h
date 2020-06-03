#ifndef __I2CMNG_H
#define __I2CMNG_H
#include "I2CDrvCmm.h"
#include <string.h>

extern void I2CInit(void);
extern void PrepData(int8_t nI2C);
void SetPhases(uint8_t, uint8_t, uint8_t, uint8_t);
#endif//#define __DRVCMMI2C_H
