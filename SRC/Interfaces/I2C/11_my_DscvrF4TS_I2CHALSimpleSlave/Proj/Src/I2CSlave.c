#include "I2CSlave.h"


#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID

//прием
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался
#define	RECEIVE_WAIT	0	//ожидаем запрос
#define	RECEIVE_YES_ANY_DATA	1	//какието данные приняты

//предача
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался
//#define	SEND_CLEAN		0	//был запрос на который потребуется очистить - разблокируем возможность передачи
#define	SEND_START_NOW		0	//запустить передачу
#define	SEND_WAIT		1	//простой

//сбор ID ведомых этапы
#define	PH1_GET_ID__SEND_ANSW			1	//получен запрос «дай ID» - послать ответ
#define	PH1_GET_ID__SEND_ANSW_MADE			2

#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10


struct I2CUsrData
{
	uint8_t PhaseSend;
	uint8_t	_aTxBuffer[SZ_ARR_TX_BUFF];
	uint8_t PhaseReceive;
	uint8_t	_aRxBuffer[SZ_ARR_RX_BUFF];
	uint8_t PhaseSetAddr;
};

struct I2CUsrData _usrI2CData[3];

void I2CInit()
{
	// статусы для запуска драйвераобмена
//	phases.Receive = RECEIVE_YES_ANY_DATA;
//	phaseSend = SEND_WAIT;
}



void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState;
	switch (_usrI2CData[nI2C].PhaseReceive)
	{
		case RECEIVE_YES_ANY_DATA:
			if(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C]._aRxBuffer), 2) == HAL_OK)
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_WAIT;
			break;
		case RECEIVE_WAIT:
			resGetState = HAL_I2C_GetState(hi2c);
			if(resGetState == HAL_I2C_STATE_READY)
			{
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_YES_ANY_DATA;
			}
			break;

		default:
			break;
	}

}

void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	if(HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
		return;

	switch (_usrI2CData[nI2C].PhaseSend)
	{
		case SEND_START_NOW:
			if(HAL_I2C_Slave_Transmit_IT(&hi2c, (uint8_t*)(_usrI2CData[nI2C]._aTxBuffer), 2) == HAL_OK)
				_usrI2CData[nI2C].PhaseSend = SEND_WAIT;
			break;

		default:
			break;
	}

}


void PrepData()
{
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		if(_usrI2CData[nI2C]._aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
			_usrI2CData[nI2C].PhaseSetAddr = PH1_GET_ID__SEND_ANSW;

		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case PH1_GET_ID__SEND_ANSW:
				_usrI2CData[nI2C]._aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//код тот же
				_usrI2CData[nI2C]._aTxBuffer[1] =  nI2C;//ID слейва
				_usrI2CData[nI2C].PhaseSend = SEND_START_NOW;
				_usrI2CData[nI2C].PhaseSetAddr = PH1_GET_ID__SEND_ANSW_MADE;
				break;

			default:
				break;
		}
	}


}
