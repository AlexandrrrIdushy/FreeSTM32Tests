#include "I2CTests.h"


//#define ALL_SEG_OFF() LL_GPIO_SetOutputPin(SEG_PORT,SH|SG|SF|SE|SD|SC|SB|SA);
//#define I2C_REQUEST_WRITE                       0x00
//#define I2C_REQUEST_READ                        0x01
//#define SLAVE_OWN_ADDRESS                       0xD0	//0xD0	//0xA0
//
//extern uint16_t num_gl;
//uint8_t rd_value[20] = {0};
//uint8_t wr_value[20] = {0x14,0x13,0x12,0x11,0x10,
//                        0x0F,0x0E,0x0D,0x0C,0x0B,
//                        0x0A,0x09,0x08,0x07,0x06,
//                        0x05,0x04,0x03,0x02,0x01};
//
////подготовка к старту и старт условие
//void  __attribute__((optimize("O0"))) AT24C_PrepStart ()
//{
//	  LL_I2C_DisableBitPOS(I2C1);//стандартна€ схема ACK - у текущего байта
//	  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);//включаем генерацию подтверждени€ ACK
//	  LL_I2C_GenerateStartCondition(I2C1);//генерируем старт условие
//	  while(!LL_I2C_IsActiveFlag_SB(I2C1)){};//ƒождЄмс€ установки бита SB в регистре SR1. ƒанный бит устанавливаетс€ в случае наличие на шине услови€ START
//	  //read state
//	  (void) I2C1->SR1;//	  —читаем регистр SR1
//
//}
//
//
//void  __attribute__((optimize("O0"))) AT24C_ReadBytes (uint16_t addr, uint8_t *buf, uint16_t bytes_count)
//{  uint16_t i;
//	AT24C_PrepStart();
//
//  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_READ);//адрес ведомого отправл€ем в шину
//  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};
//  LL_I2C_ClearFlag_ADDR(I2C1);
//  for(i=0;i<bytes_count;i++)
//  {
//    if(i<(bytes_count-1))
//    {
//      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
//      buf[i] = LL_I2C_ReceiveData8(I2C1);
//    }
//    else
//    {
//      LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
//      LL_I2C_GenerateStopCondition(I2C1);
//      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
//      buf[i] = LL_I2C_ReceiveData8(I2C1);
//    }
//  }
//
//  asm("nop");
//}
//
//
//void __attribute__((optimize("O0"))) AT24C_WriteBytes (uint16_t addrInMem,uint8_t *buf, uint16_t bytes_count)
//{
//	  uint16_t i;
//	  AT24C_PrepStart();
//
//	  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_WRITE);//отправим адрес устройства в шину
//	  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};//дождЄмс€ установки бита ADDR в регистре SR1, что будет означать окончание передачи адреса SLAVE в шину
//	  LL_I2C_ClearFlag_ADDR(I2C1);//ќчистим флаг ADDR, что делаетс€ посредством считывани€ регистров SR1 и SR2
//
//	  //передаем адрес в пам€ти сначала H затем L часть. тут мы их заносим в DR но не передаем реально
////	  LL_I2C_TransmitData8(I2C1, (uint8_t) (addrInMem>>8));//H
////	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?
////	  LL_I2C_TransmitData8(I2C1, (uint8_t) addrInMem);//L
////	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?
//
//	  //передаем данные дл€ записи EEPROM
//	  for(i=0;i<bytes_count;i++)
//	  {
//	    LL_I2C_TransmitData8(I2C1, buf[i]);
//	    while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};
//	  }
//
//	  LL_I2C_GenerateStopCondition(I2C1);
//	  asm("nop");
//}
