#include "I2CMaster.h"
#include <stdint.h>
#include <string.h>




struct I2CUsrData
{
	uint8_t	aTxBuffer[SZ_ARR_TX_BUFF];
	uint16_t sizeTxCmd;

	uint8_t	aRxBuffer[SZ_ARR_RX_BUFF];
	uint16_t sizeRxCmd;

	uint8_t PhaseSend;
	uint8_t PhaseReceive;
	uint8_t PhaseSetAddr;
};

struct I2CUsrData _usrI2CData;

I2C_HandleTypeDef* _hi2c1;

void SetHederMasterI2C(I2C_HandleTypeDef* hi2c1)
{
	_hi2c1 = hi2c1;

}

#define	BTN_PUSH	0
#define	BTN_RELEASE	1

uint8_t _btnState, _lastBtnState;

void I2CInit()
{
		_usrI2CData.PhaseSend = 0;
		_usrI2CData.PhaseReceive = 0;
		_usrI2CData.PhaseSetAddr = 0;
		_usrI2CData.aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;	//тип
		_usrI2CData.aTxBuffer[1] = ADDR_BY_MASTER;			//адрес
		_usrI2CData.aTxBuffer[2] = 0xEE;					//адрес
		memset((uint8_t*)(_usrI2CData.aRxBuffer), (uint16_t) 0, (uint8_t)(SZ_ARR_RX_BUFF));

//		_lastBtnState = 1;

		//выключаем включаем шину
		I2C1 ->CR1 &= (~I2C_CR1_PE);
		HAL_Delay(100);
		I2C1 ->CR1 |= I2C_CR1_PE;

		I2C1 ->CR1 |= I2C_CR1_SWRST;//сброс логики после каждого рестарта

		_lastBtnState = BTN_RELEASE;
}


#define DELAY_RECEIVE_END	400	//(10 * 50/*sec*/) //задержка вызова. при значении в скобках 10 = 1 секунде
void __attribute__((optimize("O0"))) I2CReceive()
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(_hi2c1);
	static uint32_t startLocalCounter = 0;

	switch (_usrI2CData.PhaseReceive)
	{
		case RECEIVE_START:
			HAL_I2C_Slave_Receive_IT(_hi2c1, _usrI2CData.aRxBuffer, _usrI2CData.sizeRxCmd);
			_usrI2CData.PhaseReceive = RECEIVE_WAIT_DATA;
			startLocalCounter = GetSysCounter100MSec();
			break;
		case RECEIVE_WAIT_DATA:
			asm("nop");
//			if(resGetState == HAL_I2C_STATE_READY)
//				_usrI2CData.PhaseReceive = RECEIVE_YES_ANY_DATA;
//
//			//если вышло время выделенное на прием
//			else if((GetSysCounter100MSec() - startLocalCounter) > DELAY_RECEIVE_END)
//			{
//				_usrI2CData.PhaseReceive = RECEIVE_TIMOUT;
//
//				//!!! костыль? для сброс линии И флагов I2C HAL в начальное состояние. может потребоваться более всеобъемлещий сброс
//				_hi2c1->State = HAL_I2C_STATE_READY;
//			}

			break;
		default:
			break;
	}

}

void __attribute__((optimize("O0"))) I2CSend()
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(_hi2c1);
	switch (_usrI2CData.PhaseSend)
	{
		case SEND_START_NOW:
//			if(resGetState == HAL_I2C_STATE_READY)
//			{
				HAL_I2C_Master_Transmit_IT(_hi2c1, 102, (uint8_t*)(_usrI2CData.aTxBuffer), _usrI2CData.sizeTxCmd);
				_usrI2CData.PhaseSend = SEND_WAS_START;
//			}
			break;

		case SEND_WAS_START:
			//ожидается что сначала будет HAL_I2C_STATE_BUSY_TX а потом перейдет на HAL_I2C_STATE_READY

//			if(resGetState == HAL_I2C_STATE_READY)
//				_usrI2CData.PhaseSend = SEND_WAS_GOOD_END;
			break;

		default:
			break;
	}

}


void __attribute__((optimize("O0"))) PrepData()
{


	//старт после нажатия кнопки
	_btnState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
	if(_btnState == BTN_RELEASE && _lastBtnState == BTN_PUSH &&
			_usrI2CData.PhaseSend == SEND_NEUTRAL && _usrI2CData.PhaseReceive == RECEIVE_NEUTRAL)
	{
		_usrI2CData.PhaseSend = SEND_START_NOW;
		_usrI2CData.sizeTxCmd = ST1_SIZE_REQUEST;
	}
	_lastBtnState = _btnState;



//	if(_usrI2CData.PhaseSend == SEND_WAS_START)
//	{
//
//		_usrI2CData.PhaseSend == SEND_WAS_GOOD_END;
//	}

	if(_usrI2CData.PhaseSend == SEND_WAS_GOOD_END)
	{
		_usrI2CData.PhaseSend = SEND_NEUTRAL;
		_usrI2CData.PhaseReceive = RECEIVE_START;
	}

	//начнем сначала
	if(_usrI2CData.PhaseReceive == RECEIVE_TIMOUT)
	{
		_usrI2CData.PhaseReceive = RECEIVE_NEUTRAL;
	}


	//какието данные приняты И код = "раздача адресов фаза 1 сбор ID"
	if(_usrI2CData.PhaseReceive == RECEIVE_YES_ANY_DATA &&
			_usrI2CData.aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
	{
		_usrI2CData.PhaseReceive = RECEIVE_NEUTRAL;
		_usrI2CData.PhaseSetAddr = ST1__ID_GRANTED;
	}
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





