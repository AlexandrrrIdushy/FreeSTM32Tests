#ifndef W5500_H_

#define W5500_H_

//--------------------------------------------------

#include "stm32f4xx_hal.h"

#include <string.h>

#include <stdlib.h>

#include <stdint.h>

#include "fatfs.h"

//--------------------------------------------------

#define CS_GPIO_PORT GPIOB

#define CS_PIN GPIO_PIN_6

#define SS_SELECT() HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET)

#define SS_DESELECT() HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET)

//--------------------------------------------------

#define MAC_ADDR {0x00,0x15,0x42,0xBF,0xF0,0x51}

//--------------------------------------------------

#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))

//--------------------------------------------------

#endif /* W5500_H_ */
