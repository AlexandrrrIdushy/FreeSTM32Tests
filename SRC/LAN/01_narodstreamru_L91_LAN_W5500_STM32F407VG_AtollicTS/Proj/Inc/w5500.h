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

#define BSB_COMMON 0x00

#define BSB_S0 0x01

#define BSB_S0_TX 0x02

#define BSB_S0_RX 0x03

//--------------------------------------------------

#define RWB_WRITE 1

#define RWB_READ 0

//--------------------------------------------------

#define OM_FDM0 0x00//режим передачи данных переменной длины

#define OM_FDM1 0x01//режим передачи данных по одному байту

#define OM_FDM2 0x02//режим передачи данных по два байта

#define OM_FDM3 0x03//режим передачи данных по четыре байта

//--------------------------------------------------

#define MR 0x0000//Mode Register

//--------------------------------------------------

#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))

//--------------------------------------------------

#endif /* W5500_H_ */
