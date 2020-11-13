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
//	Мы последовательно записываем в 4-байтный буфер адрес регистра (ячейки памяти), затем код операции с присоединённым к нему с помощью операции ИЛИ бита записи и собственно байт данных.
  uint8_t buf[] = {addres >> 8, addres, op|(RWB_WRITE<<2), data};

//  Затем мы опускаем ножку выбора, передаём эти 4 байта, используя соответствующую функцию библиотеки HAL, а затем поднимаем ножку выбора устройства.
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

  //Soft Reset- код программной перезагрузки модуля
  opcode = (BSB_COMMON<<3)|OM_FDM1;
  w5500_writeReg(opcode, MR, 0x80);
  HAL_Delay(100);

  //Configure Net
  w5500_writeReg(opcode, SHAR0,macaddr[0]);
  w5500_writeReg(opcode, SHAR1,macaddr[1]);
  w5500_writeReg(opcode, SHAR2,macaddr[2]);
  w5500_writeReg(opcode, SHAR3,macaddr[3]);
  w5500_writeReg(opcode, SHAR4,macaddr[4]);
  w5500_writeReg(opcode, SHAR5,macaddr[5]);
  w5500_writeReg(opcode, GWR0,ipgate[0]);
  w5500_writeReg(opcode, GWR1,ipgate[1]);
  w5500_writeReg(opcode, GWR2,ipgate[2]);
  w5500_writeReg(opcode, GWR3,ipgate[3]);
  w5500_writeReg(opcode, SUBR0,ipmask[0]);
  w5500_writeReg(opcode, SUBR1,ipmask[1]);
  w5500_writeReg(opcode, SUBR2,ipmask[2]);
  w5500_writeReg(opcode, SUBR3,ipmask[3]);
  w5500_writeReg(opcode, SIPR0,ipaddr[0]);
  w5500_writeReg(opcode, SIPR1,ipaddr[1]);
  w5500_writeReg(opcode, SIPR2,ipaddr[2]);
  w5500_writeReg(opcode, SIPR3,ipaddr[3]);
}


//-----------------------------------------------

static void Error (void)

{

HAL_UART_Transmit(&huart2,(uint8_t*)"Error!rn",8,0x1000);

}

//-----------------------------------------------
