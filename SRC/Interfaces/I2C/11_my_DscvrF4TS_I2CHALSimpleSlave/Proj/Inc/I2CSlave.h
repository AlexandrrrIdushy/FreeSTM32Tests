#ifndef __I2CSLAVE_H
#define __I2CSLAVE_H
#include "stm32f4xx_hal.h"

extern void I2CInit(void);
extern void I2CReceive(I2C_HandleTypeDef*, uint8_t);
extern void I2CSend(I2C_HandleTypeDef*, uint8_t);
extern void PrepData(void);
#endif//I2CSLAVE
