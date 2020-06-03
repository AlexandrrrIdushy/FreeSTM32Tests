#ifndef __I2CMNG_H
#define __I2CMNG_H
#include "I2CDrvCmm.h"

extern void I2CInit(void);
extern void PrepData(void);
void SetPhases(uint8_t, uint8_t, uint8_t, uint8_t);
#endif//#define __DRVCMMI2C_H
