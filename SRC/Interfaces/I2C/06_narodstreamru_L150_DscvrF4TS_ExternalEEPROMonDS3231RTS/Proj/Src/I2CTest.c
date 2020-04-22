#include "I2CTest.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_i2c.h"



//СЛЕЙВ

void __attribute__((optimize("O0"))) SLAVETest_Any()
{
	uint32_t res = 0;
	uint8_t rd = 0;


	//совпал адрес?
	while(res = LL_I2C_IsActiveFlag_ADDR(I2C1))
		asm("nop");

	//появились данные в регистре данных
	while(LL_I2C_IsActiveFlag_RXNE(I2C1))
		asm("nop");
//	res = LL_I2C_IsActiveFlag_STOP(I2C1);
//	while(!res){}

	//генерировать ACK
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

//	rd = LL_I2C_ReceiveData8(I2C1);
//	LL_I2C_EnableReset(I2C1);

//	//#1 зафиксировать старт условие
//	LL_I2C_DisableBitPOS(I2C1);//стандартная схема ACK - у текущего байта
//	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);//включаем генерацию подтверждения ACK
//	LL_I2C_EnableReset(I2C1);
	asm("nop");
}

//создаем какуюто кативность подтверждаем, правда остаемся висеть потом..
void __attribute__((optimize("O0"))) SLAVETest_SetACK()
{
	//генерировать ACK
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
}




//МАСТЕР
#define ALL_SEG_OFF() LL_GPIO_SetOutputPin(SEG_PORT,SH|SG|SF|SE|SD|SC|SB|SA);
#define I2C_REQUEST_WRITE                       0x00
#define I2C_REQUEST_READ                        0x01
#define SLAVE_OWN_ADDRESS                       0x66	//0x66 это 51 сдвинутая влево

extern uint16_t num_gl;////
uint8_t rd_value[20] = {0};
uint8_t wr_value[20] = {0x14,0x13,0x12,0x11,0x10,//22
                        0x0F,0x0E,0x0D,0x0C,0x0B,
                        0x0A,0x09,0x08,0x07,0x06,
                        0x05,0x04,0x03,0x02,0x01};

//подготовка к старту и старт условие
void  __attribute__((optimize("O0"))) AT24C_PrepStart ()
{
	  LL_I2C_DisableBitPOS(I2C1);//стандартная схема ACK - у текущего байта
	  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);//включаем генерацию подтверждения ACK
	  LL_I2C_GenerateStartCondition(I2C1);//генерируем старт условие
	  while(!LL_I2C_IsActiveFlag_SB(I2C1)){};//Дождёмся установки бита SB в регистре SR1. Данный бит устанавливается в случае наличие на шине условия START
	  //read state
	  (void) I2C1->SR1;//	  Считаем регистр SR1

}


void  __attribute__((optimize("O0"))) AT24C_ReadBytes (uint16_t addr, uint8_t *buf, uint16_t bytes_count)
{  uint16_t i;
	AT24C_PrepStart();

  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_READ);//адрес ведомого отправляем в шину
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};
  LL_I2C_ClearFlag_ADDR(I2C1);
  for(i=0;i<bytes_count;i++)
  {
    if(i<(bytes_count-1))
    {
      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
      buf[i] = LL_I2C_ReceiveData8(I2C1);
    }
    else
    {
      LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
      LL_I2C_GenerateStopCondition(I2C1);
      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
      buf[i] = LL_I2C_ReceiveData8(I2C1);
    }
  }

  asm("nop");
}


void __attribute__((optimize("O0"))) AT24C_WriteBytes (uint16_t addrInMem,uint8_t *buf, uint16_t bytes_count)
{
	  uint16_t i;
	  AT24C_PrepStart();

	  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_WRITE);//отправим адрес устройства в шину
	  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};//дождёмся установки бита ADDR в регистре SR1, что будет означать окончание передачи адреса SLAVE в шину
	  LL_I2C_ClearFlag_ADDR(I2C1);//Очистим флаг ADDR, что делается посредством считывания регистров SR1 и SR2

	  //передаем адрес в памяти сначала H затем L часть. тут мы их заносим в DR но не передаем реально
//	  LL_I2C_TransmitData8(I2C1, (uint8_t) (addrInMem>>8));//H
//	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?
//	  LL_I2C_TransmitData8(I2C1, (uint8_t) addrInMem);//L
//	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?

	  //передаем данные для записи EEPROM
	  for(i=0;i<bytes_count;i++)
	  {
	    LL_I2C_TransmitData8(I2C1, buf[i]);
	    while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};
	  }

	  LL_I2C_GenerateStopCondition(I2C1);
	  asm("nop");
}

