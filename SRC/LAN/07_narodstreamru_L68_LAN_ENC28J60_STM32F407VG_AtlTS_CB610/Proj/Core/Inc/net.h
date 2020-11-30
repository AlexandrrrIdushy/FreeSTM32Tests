#ifndef __NET_H
#define __NET_H
//--------------------------------------------------
//#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "enc28j60.h"
//--------------------------------------------------
//--------------------------------------------------
//L5. Создадим структуру для фрейма в файле net.h
typedef struct enc28j60_frame
{
  uint8_t addr_dest[6];
  uint8_t addr_src[6];
  uint16_t type;
  uint8_t data[];
} enc28j60_frame_ptr;
#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
//--------------------------------------------------


extern void net_ini(void);
extern void net_pool(void);
#endif /* __NET_H */
