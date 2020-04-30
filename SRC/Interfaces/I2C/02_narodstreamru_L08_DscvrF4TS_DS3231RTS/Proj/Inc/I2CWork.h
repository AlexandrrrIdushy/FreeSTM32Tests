#ifndef __WRKI2C_H
#define __WRKI2C_H
#include "stm32f4xx_hal.h"

extern void SetHederI2C(I2C_HandleTypeDef*);
extern void ReadTimeFromRTC(void);
extern void Scaning(void);
extern void Test(void);
#endif /* __MAIN_H */
