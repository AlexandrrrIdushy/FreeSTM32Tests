/*
 * I2CWrk.c
 *
 *  Created on: 11 мар. 2020 г.
 *      Author: gravitau
 */


#include "I2CWork.h"
#include <stdint.h>



I2C_HandleTypeDef* _hi2c1;
uint8_t aTxBuffer[8];
extern uint8_t aTxBuffer[8];
char str[100];

void  __attribute__((optimize("O0")))  SetHederI2C(I2C_HandleTypeDef* hi2c1)
{
	_hi2c1 = hi2c1;
}

// передается последовательность байт aTxBuffer в шину устройству с адресом DEV_ADDR
void  __attribute__((optimize("O0"))) I2C_WriteBuffer(I2C_HandleTypeDef* hi, uint8_t DEV_ADDR, uint8_t sizebuf)
{
	while(HAL_I2C_Master_Transmit(hi, (uint16_t)DEV_ADDR,(uint8_t*) &aTxBuffer, (uint16_t)sizebuf, (uint32_t)1000)!= HAL_OK)
	{
		if (HAL_I2C_GetError(hi) != HAL_I2C_ERROR_AF)
		{
			asm("nop");
		}
		else
			asm("nop");
	}
}

//читает из шины
void I2C_ReadBuffer(I2C_HandleTypeDef* hi, uint8_t DEV_ADDR, uint8_t sizebuf)
{
	while(HAL_I2C_Master_Receive(hi, (uint16_t)DEV_ADDR, (uint8_t*) &aTxBuffer, (uint16_t)sizebuf, (uint32_t)1000)!= HAL_OK)
	{
		if (HAL_I2C_GetError(hi) != HAL_I2C_ERROR_AF)
		{
//			sprintf(str, "Buffer error");
//			LCD_SetPos(8, 0);
//			LCD_String(str);
		}
	}
}

uint8_t RTC_ConvertFromDec(uint8_t c)
{
	uint8_t ch = ((c>>4)*10+(0x0F&c));
	return ch;
}


uint8_t RTC_ConvertFromBinDec(uint8_t c)
{
	uint8_t ch = ((c/10)<<4)|(c%10);
	return ch;
}

uint32_t i=0;
uint8_t sec=0,min=0,hour=0,day=0,date=0,month=0,year=0;

void  __attribute__((optimize("O0"))) ReadTimeFromRTC()
{
	aTxBuffer[0]=0;
	I2C_WriteBuffer(_hi2c1,(uint16_t)0xD0,1);//
	HAL_Delay(1000);
	while (HAL_I2C_GetState(_hi2c1) != HAL_I2C_STATE_READY)
	{
	}

	I2C_ReadBuffer(_hi2c1,(uint16_t)0xD0,7);
	date=aTxBuffer[4];
	date = RTC_ConvertFromDec(date); //Преобразуем в десятичный формат
	month=aTxBuffer[5];
	month = RTC_ConvertFromDec(month); //Преобразуем в десятичный формат
	year=aTxBuffer[6];
	year = RTC_ConvertFromDec(year); //Преобразуем в десятичный формат
	day=aTxBuffer[3];
	day = RTC_ConvertFromDec(day); //Преобразуем в десятичный формат
	hour=aTxBuffer[2];
	hour = RTC_ConvertFromDec(hour); //Преобразуем в десятичный формат
	min=aTxBuffer[1];
	min = RTC_ConvertFromDec(min); //Преобразуем в десятичный формат
	sec=aTxBuffer[0];
	sec = RTC_ConvertFromDec(sec); //Преобразуем в десятичный формат
	i++;
	return;
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
			HAL_Delay(100);
			if(HAL_I2C_Master_Receive(_hi2c1, shiftAdr, arrReadData, (uint16_t)ARR_4_READ_DATA_SZ, (uint32_t)100) == HAL_OK)
			{
				arrFoundAdrDevs[adr] = 1;//можно писать идентификатор прибора
				break;
			}
			HAL_Delay(100);
		}
	}
	asm("nop");
}

void __attribute__((optimize("O0"))) Test()
{
	if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
		return;

	HAL_I2C_Master_Transmit(_hi2c1, 0, arrReadData, (uint16_t)1, (uint32_t)100);
	if(HAL_I2C_Slave_Receive(_hi2c1, arrReadData, (uint16_t)1, (uint32_t)100) == HAL_OK)
		asm("nop");
	HAL_Delay(1000);
}
