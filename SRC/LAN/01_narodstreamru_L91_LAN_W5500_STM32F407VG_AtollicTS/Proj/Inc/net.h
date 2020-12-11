#ifndef __NET_H

#define __NET_H

//--------------------------------------------------

#include "stm32f4xx_hal.h"

#include <string.h>

#include <stdlib.h>

#include <stdint.h>

#include "w5500.h"

//--------------------------------------------------

#define IP_ADDR {192,168,1,197}

#define IP_GATE {192,168,1,1}
//#define IP_GATE {0x12,0x4C,0xAA,0xEF}

//#define IP_MASK {255,255,255,0}//так изначально
#define IP_MASK {255,255,0,0}//так показывает ipconfig
//#define IP_MASK {255,255,255,255}

#define LOCAL_PORT 80

//--------------------------------------------------

#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))

//--------------------------------------------------


extern void net_ini(void);
extern void net_poll(void);
#endif /* __NET_H */
