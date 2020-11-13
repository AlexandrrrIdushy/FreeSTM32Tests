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
//функция записи байта в регистр
void w5500_writeReg(uint8_t op, uint16_t addres, uint8_t data)

{

  uint8_t buf[] = {addres >> 8, addres, op|(RWB_WRITE<<2), data};

  SS_SELECT();

  HAL_SPI_Transmit(&hspi1, buf, 4, 0xFFFFFFFF);

  SS_DESELECT();

}


//-----------------------------------------------

void w5500_ini(void)

{

  uint8_t dtt=0;

  uint8_t opcode=0;

  //Hard Reset

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  HAL_Delay(70);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

  HAL_Delay(70);

}


//-----------------------------------------------

static void Error (void)

{

HAL_UART_Transmit(&huart2,(uint8_t*)"Error!rn",8,0x1000);

}

//-----------------------------------------------
