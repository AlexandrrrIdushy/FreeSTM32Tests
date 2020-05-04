#include "I2CSlave.h"


#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID

//прием
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался
#define	RECEIVE_WAIT	0	//ожидаем запрос
#define	RECEIVE_YES_ANY_DATA	1	//какието данные приняты

//предача
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался
#define	SEND_WAIT		0	//ожидаем в передаче
#define	SEND_NOW		1	//запустить передачу

//сбор ID ведомых этапы
#define	PH1_GET_ID__REQUEST			4	//получен запрос «дай ID»

#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10


struct I2CPhases
{
	uint8_t Send;
	uint8_t	_aTxBuffer[SZ_ARR_TX_BUFF];
	uint8_t Receive;
	uint8_t	_aRxBuffer[SZ_ARR_RX_BUFF];
};

struct I2CPhases _usrI2CData[3];

void I2CInit()
{
	// статусы для запуска драйвераобмена
//	phases.Receive = RECEIVE_YES_ANY_DATA;
//	phaseSend = SEND_WAIT;
}



void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState;
	switch (_usrI2CData[nI2C].Receive)
	{
		case RECEIVE_YES_ANY_DATA:
			if(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C]._aRxBuffer), 2) == HAL_OK)
				_usrI2CData[nI2C].Receive = RECEIVE_WAIT;
			break;
		case RECEIVE_WAIT:
			resGetState = HAL_I2C_GetState(hi2c);
			if(resGetState == HAL_I2C_STATE_READY)
			{
				_usrI2CData[nI2C].Receive = RECEIVE_YES_ANY_DATA;
			}
			break;

		default:
			break;
	}

}

void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	switch (_usrI2CData[nI2C].Send)
	{
		case SEND_NOW:
			if(HAL_I2C_Slave_Transmit_IT(&hi2c, (uint8_t*)(_usrI2CData[nI2C]._aTxBuffer), 2) == HAL_OK)
				_usrI2CData[nI2C].Send = SEND_WAIT;
			break;

		default:
			break;
	}

}


void PrepData()
{
//	for (int var = 0; var < 3; var++)
//	{
//		if(_usrI2CData[nI2C]._aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
//			asm("nop");
//	}


}
