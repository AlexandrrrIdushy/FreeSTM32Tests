#include "I2CMng.h"
#include "I2CDrvCmm.h"
#include "my_type.h"


uint8_t _btnState, _lastBtnState;
#define	BTN_PUSH	0
#define	BTN_RELEASE	1

struct I2CDataFromGM i2cDataFromGM[MAX_N_GAS_MTR];
uint8_t _curAdr4TryWrite2GM;

void I2CInit()
{
	_curAdr4TryWrite2GM = 2;
	_adrOfReceiver = 51;
	SetPhases(0, SEND_NEUTRAL, RECEIVE_NEUTRAL, P0S0__DEFVAL);
	_usrI2CData[0].sizeRxCmd = P1S1_SZ_REQUEST;
	memset(_usrI2CData[0].aRxBuffer, 0, SZ_ARR_RX_BUFF);

#ifdef	GIVE_OUT_ADR_V2
	_usrI2CData[0].aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;	//тип
	_usrI2CData[0].aTxBuffer[1] = ADDR_BY_MASTER;			//адрес
	_usrI2CData[0].aTxBuffer[2] = 0xEE;					//адрес
#endif
	memset((uint8_t*)(_usrI2CData[0].aRxBuffer), (uint16_t) 0, (uint8_t)(SZ_ARR_RX_BUFF));

	//		_lastBtnState = 1;

	//выключаем включаем шину
	I2C1 ->CR1 &= (~I2C_CR1_PE);
	HAL_Delay(100);
	I2C1 ->CR1 |= I2C_CR1_PE;

	I2C1 ->CR1 |= I2C_CR1_SWRST;//сброс логики после каждого рестарта

	_lastBtnState = BTN_RELEASE;
}


#ifdef	GIVE_OUT_ADR_V2
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
#endif//#ifdef	GIVE_OUT_ADR_V2

#ifdef	GIVE_OUT_ADR_V1
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
			//готовим посылку
			_usrI2CData[0].aTxBuffer[P1S1__I_B_CODCMD] = P1_CODE_SEND_ADR;
			_usrI2CData[0].aTxBuffer[P1S1__I_B_ADRMAST] = ADDR_BY_MASTER;
			_usrI2CData[0].aTxBuffer[P1S1__I_B_ADR4WR] = _curAdr4TryWrite2GM;
			_usrI2CData[0].sizeTxCmd = P1S1_SZ_REQUEST;
			memset(_usrI2CData[0].aRxBuffer, 0, SZ_ARR_RX_BUFF);
			SetPhases(0, SEND_START_CAN, RECEIVE_NEUTRAL, P1S2__SEND_WAIT_END_SENDING);
			break;

		case P1S2__SEND_WAIT_END_SENDING://ожидаем завершения отправки
			if(_usrI2CData[0].PhaseSend == SEND_TIMOUT)
			{
				//отправка не удалась
				SetPhases(0, SEND_NEUTRAL, RECEIVE_NEUTRAL, P0S0__DEFVAL);
			}
			if(_usrI2CData[0].PhaseSend == SEND_WAS_GOOD_END)
			{
				_usrI2CData[0].sizeRxCmd = P1S2_SIZE_ANSW;
				SetPhases(0, SEND_NEUTRAL, RECEIVE_START, P1S3__RCV_WAIT_RCV_DATA);
			}
			break;

		case P1S3__RCV_WAIT_RCV_DATA://пробуем принять какие нибудь данные
			//если прием не удался. все фазы откатываем к началу
			if(_usrI2CData[0].PhaseReceive == RECEIVE_TIMOUT)
			{
				SetPhases(0, SEND_NEUTRAL, RECEIVE_NEUTRAL, P0S0__DEFVAL);
			}

			//какието данные приняты
			if(_usrI2CData[0].PhaseReceive == RECEIVE_YES_ANY_DATA)
			{
				//проверяем - записался ли адрес
				if(_usrI2CData[0].aRxBuffer[P1S2__I_B_CODCMD] == P1_CODE_SEND_ADR &&
						_usrI2CData[0].aRxBuffer[P1S2__I_B_ADR4WR] == _curAdr4TryWrite2GM)

				{
					SetPhases(0, SEND_NEUTRAL, RECEIVE_NEUTRAL, P0S0__DEFVAL);
					_curAdr4TryWrite2GM++;
				}
			}
			break;


		default:
			break;
	}


}
#endif//#ifdef	GIVE_OUT_ADR_V1


void SetPhases(uint8_t nI2C, uint8_t phaseSend, uint8_t phaseReceive, uint8_t phaseSetAddr)
{
	_usrI2CData[nI2C].PhaseSend = phaseSend;
	_usrI2CData[nI2C].PhaseReceive = phaseReceive;
	_usrI2CData[nI2C].PhaseSetAddr = phaseSetAddr;
}
