#include "I2CMaster.h"
#include <stdint.h>





struct I2CUsrData
{
	uint8_t PhaseSend;
	uint8_t	aTxBuffer[SZ_ARR_TX_BUFF];
	uint8_t PhaseReceive;
	uint8_t	aRxBuffer[SZ_ARR_RX_BUFF];
	uint8_t PhaseSetAddr;
};

struct I2CUsrData _usrI2CData;

I2C_HandleTypeDef* _hi2c1;

void SetHederMasterI2C(I2C_HandleTypeDef* hi2c1)
{
	_hi2c1 = hi2c1;

}

void I2CInit()
{
		_usrI2CData.PhaseSend = 0;
		_usrI2CData.PhaseReceive = 0;
		_usrI2CData.PhaseSetAddr = 0;
		_usrI2CData.aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;	//тип
		_usrI2CData.aTxBuffer[1] = ADDR_BY_MASTER;			//адрес
		memset(_usrI2CData.aRxBuffer, 0, SZ_ARR_RX_BUFF);

}



void __attribute__((optimize("O0"))) I2CReceive()
{
	HAL_I2C_StateTypeDef resGetState;
	switch (_usrI2CData.PhaseReceive)
	{
		case RECEIVE_YES_ANY_DATA:
			if(HAL_I2C_Slave_Receive_IT(_hi2c1, (uint8_t *)(_usrI2CData.aRxBuffer), 2) == HAL_OK)
				_usrI2CData.PhaseReceive = RECEIVE_WAIT;
			break;
		case RECEIVE_WAIT:
			resGetState = HAL_I2C_GetState(_hi2c1);
			if(resGetState == HAL_I2C_STATE_READY)
			{
				_usrI2CData.PhaseReceive = RECEIVE_YES_ANY_DATA;
			}
			break;

		default:
			break;
	}

}

void __attribute__((optimize("O0"))) I2CSend()
{
	HAL_I2C_StateTypeDef resGetState;
//	resGetState = HAL_I2C_GetState(_hi2c1);
//	if(resGetState != HAL_I2C_STATE_READY)
//		return;

	switch (_usrI2CData.PhaseSend)
	{
		case SEND_START_NOW:
			HAL_I2C_Master_Transmit_IT(_hi2c1, 102, (uint8_t*)(_usrI2CData.aTxBuffer), SIZE_GET_ID_REQUEST);
//			HAL_I2C_Master_Transmit(_hi2c1, 102, (uint8_t*)(_usrI2CData.aTxBuffer), (uint16_t)SIZE_GET_ID_REQUEST, (uint32_t)100);
			_usrI2CData.PhaseSend = SEND_WAS_START;
			break;

		default:
			break;
	}

}


void __attribute__((optimize("O0"))) PrepData()
{
	if(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3))
		_usrI2CData.PhaseSend = SEND_START_NOW;
//	else
//		asm("nop");
//	if(_usrI2CData.aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
//		_usrI2CData.PhaseSetAddr = PH1_GET_ID__SEND_ANSW;
//
//	switch (_usrI2CData.PhaseSetAddr)
//	{
//		case PH1_GET_ID__SEND_ANSW:
//			_usrI2CData.aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//код в ответной команде слейва тот же
////				_usrI2CData.aTxBuffer[1] =  nI2C;//ID слейва
//			memset(_usrI2CData.aRxBuffer, 0, SZ_ARR_RX_BUFF);
//			_usrI2CData.PhaseSend = SEND_START_NOW;
//			_usrI2CData.PhaseSetAddr = PH1_GET_ID__SEND_ANSW_MADE;
//			break;
//
//		default:
//			break;
//	}


}
