#include "w5500.h"

//-----------------------------------------------

extern SPI_HandleTypeDef hspi1;

extern UART_HandleTypeDef huart2;

//-----------------------------------------------

extern char str1[60];

//-----------------------------------------------

uint8_t macaddr[6]=MAC_ADDR;

extern uint8_t ipaddr[4];

extern uint8_t ipgate[4];

extern uint8_t ipmask[4];

extern uint16_t local_port;

//-----------------------------------------------

static void Error (void)

{

HAL_UART_Transmit(&huart2,(uint8_t*)"Error!rn",8,0x1000);

}

//-----------------------------------------------
