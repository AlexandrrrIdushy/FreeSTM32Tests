/*
 * I2CWrk.c
 *
 *  Created on: 11 мар. 2020 г.
 *      Author: gravitau
 */


#include "I2CWork.h"
#include <stdint.h>



I2C_HandleTypeDef* _hi2c1;


void  __attribute__((optimize("O0")))  SetHederI2C(I2C_HandleTypeDef* hi2c1)
{
	_hi2c1 = hi2c1;
}



#define	ARR_FOUND_ADR_SZ	127				//размер архива адресов найденных устройств
uint8_t arrFoundAdrDevs[ARR_FOUND_ADR_SZ];
#define	ARR_4_READ_DATA_SZ	7				//число ячеек читаемых их устройства
uint8_t arrReadData[ARR_FOUND_ADR_SZ];
#define	N_TRY_READ_4_ONE_ADR	2			//количество попыток чтения одного адреса
void __attribute__((optimize("O0"))) Scaning()
{
	for (uint8_t adr = 0x68; adr < 0x6A; adr++)
	{
		for (uint8_t iTry = 0; iTry < N_TRY_READ_4_ONE_ADR; iTry++)
		{
			uint8_t shiftAdr = adr << 1;
			HAL_I2C_Master_Transmit(_hi2c1, shiftAdr, arrReadData, (uint16_t)1, (uint32_t)100);
//			HAL_Delay(100);
			if(HAL_I2C_Master_Receive(_hi2c1, shiftAdr, arrReadData, (uint16_t)ARR_4_READ_DATA_SZ, (uint32_t)100) == HAL_OK)
			{
				arrFoundAdrDevs[adr] = 1;//можно писать идентификатор прибора
				break;
			}
//			HAL_Delay(100);
		}
	}
	asm("nop");
}


