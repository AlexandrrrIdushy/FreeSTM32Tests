#include "I2CMng.h"
#include "I2CDrvCmm.h"


uint8_t _btnState, _lastBtnState;
#define	BTN_PUSH	0
#define	BTN_RELEASE	1

void I2CInit()
{
	_adrOfReceiver = 51;
	_usrI2CData[0].PhaseSend = SEND_NEUTRAL;
	_usrI2CData[0].PhaseReceive = RECEIVE_NEUTRAL;
	_usrI2CData[0].PhaseSetAddr = 0;
	_usrI2CData[0].sizeRxCmd = P1S1_SZ_REQUEST;
	memset(_usrI2CData[0].aRxBuffer, 0, SZ_ARR_RX_BUFF);


	_usrI2CData[0].aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;	//тип
	_usrI2CData[0].aTxBuffer[1] = ADDR_BY_MASTER;			//адрес
	_usrI2CData[0].aTxBuffer[2] = 0xEE;					//адрес
	memset((uint8_t*)(_usrI2CData[0].aRxBuffer), (uint16_t) 0, (uint8_t)(SZ_ARR_RX_BUFF));

	//		_lastBtnState = 1;

	//выключаем включаем шину
	I2C1 ->CR1 &= (~I2C_CR1_PE);
	HAL_Delay(100);
	I2C1 ->CR1 |= I2C_CR1_PE;

	I2C1 ->CR1 |= I2C_CR1_SWRST;//сброс логики после каждого рестарта

	_lastBtnState = BTN_RELEASE;
}

void __attribute__((optimize("O0"))) PrepData()
{


	//старт после нажатия кнопки
	_btnState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
	if(_btnState == BTN_RELEASE && _lastBtnState == BTN_PUSH &&
			_usrI2CData[0].PhaseSend == SEND_NEUTRAL && _usrI2CData[0].PhaseReceive == RECEIVE_NEUTRAL)
	{
		_usrI2CData[0].PhaseSend = SEND_START_NOW;
		_usrI2CData[0].sizeTxCmd = P1S1_SZ_REQUEST;
	}
	_lastBtnState = _btnState;



//	if(_usrI2CData.PhaseSend == SEND_WAS_START)
//	{
//
//		_usrI2CData.PhaseSend == SEND_WAS_GOOD_END;
//	}

	if(_usrI2CData[0].PhaseSend == SEND_WAS_GOOD_END)
	{
		_usrI2CData[0].PhaseSend = SEND_NEUTRAL;
		_usrI2CData[0].PhaseReceive = RECEIVE_START;
		_usrI2CData[0].sizeRxCmd = P1S2_SIZE_ANSW;
	}

	//начнем сначала
	if(_usrI2CData[0].PhaseReceive == RECEIVE_TIMOUT)
		_usrI2CData[0].PhaseReceive = RECEIVE_NEUTRAL;

	if(_usrI2CData[0].PhaseSend == SEND_TIMOUT)
		_usrI2CData[0].PhaseSend = SEND_NEUTRAL;


	//какието данные приняты И код = "раздача адресов фаза 1 сбор ID"
	if(_usrI2CData[0].PhaseReceive == RECEIVE_YES_ANY_DATA &&
			_usrI2CData[0].aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
	{
		_usrI2CData[0].PhaseReceive = RECEIVE_NEUTRAL;
		_usrI2CData[0].PhaseSetAddr = ST1__ID_GRANTED;
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


void __attribute__((optimize("O0"))) PrepDataGetAdrV1Simple()
{


	//старт после нажатия кнопки
	_btnState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
	if(_btnState == BTN_RELEASE && _lastBtnState == BTN_PUSH &&
			_usrI2CData[0].PhaseSend == SEND_NEUTRAL && _usrI2CData[0].PhaseReceive == RECEIVE_NEUTRAL)
	{
		_usrI2CData[0].PhaseSetAddr = P1S1__SEND_ADR_SLV_BEGIN;
	}
	_lastBtnState = _btnState;



	switch (_usrI2CData[0].PhaseSetAddr)
	{
		case P1S1__SEND_ADR_SLV_BEGIN://пробуем отправить адрес
			_usrI2CData[0].PhaseSend = SEND_START_NOW;
			_usrI2CData[0].sizeTxCmd = P1S1_SZ_REQUEST;
			_usrI2CData[0].PhaseSetAddr = P1S2__WAIT_CONFIRM;
			break;

		case P1S2__SEND_WAIT_END_SENDING://ожидаем завершения отправки
			if(_usrI2CData[0].PhaseSend == SEND_WAS_GOOD_END)
			{
				_usrI2CData[0].PhaseSend = SEND_NEUTRAL;
				_usrI2CData[0].PhaseReceive = RECEIVE_START;
				_usrI2CData[0].sizeRxCmd = P1S2_SIZE_ANSW;
				_usrI2CData[0].PhaseSetAddr = P1S3__RCV_WAIT_RCV_DATA;
			}
			break;

		case P1S3__RCV_WAIT_RCV_DATA://пробуем принять какие нибудь данные
			//начнем сначала
			if(_usrI2CData[0].PhaseReceive == RECEIVE_TIMOUT)
				_usrI2CData[0].PhaseReceive = RECEIVE_NEUTRAL;

			if(_usrI2CData[0].PhaseSend == SEND_TIMOUT)
				_usrI2CData[0].PhaseSend = SEND_NEUTRAL;


			//какието данные приняты И код = "раздача адресов фаза 1 сбор ID"
			if(_usrI2CData[0].PhaseReceive == RECEIVE_YES_ANY_DATA &&
					_usrI2CData[0].aRxBuffer[0] == I2CCODE_GET_ID_REQUEST)
			{
				_usrI2CData[0].PhaseReceive = RECEIVE_NEUTRAL;
				_usrI2CData[0].PhaseSetAddr = ST1__ID_GRANTED;
			}
			break;

		default:
			break;
	}


}
