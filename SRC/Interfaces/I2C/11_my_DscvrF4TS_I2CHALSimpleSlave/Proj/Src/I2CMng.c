#include "I2CMng.h"
#include "MyDefine.h"
#include <string.h>

void I2CInit()
{
	// статусы для запуска драйвераобмена
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		SetPhases(nI2C, SEND_NEUTRAL, RECEIVE_START, P0S0__DEFVAL);
		_usrI2CData[nI2C].sizeRxCmd = P1S1_SZ_REQUEST;

		memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
	}


//	//выключаем включаем шину
//	I2C1 ->CR1 &= (~I2C_CR1_PE);
//	HAL_Delay(100);
//	I2C1 ->CR1 |= I2C_CR1_PE;
//
//	I2C1 ->CR1 |= I2C_CR1_SWRST;//сброс логики после каждого рестарта
}


#ifdef	GIVE_OUT_ADR_V2
void   __attribute__((optimize("O0"))) PrepData()
{
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
#ifdef	DEBUG_1
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_YES_ANY_DATA ||
				_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT ||
				_usrI2CData[nI2C].PhaseReceive == RECEIVE_NEUTRAL)
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
#endif

		//распознали входящий запрос "дай ID"
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_YES_ANY_DATA &&
				_usrI2CData[nI2C].aRxBuffer[0] == I2CCODE_GET_ID_REQUEST &&
				_usrI2CData[nI2C].aRxBuffer[2] == 0xEE)
			_usrI2CData[nI2C].PhaseSetAddr = PH1_GET_ID__SEND_ANSW;




		//постоянно перезапускать прием
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT)
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;


		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case PH1_GET_ID__SEND_ANSW:
				_adrOfReceiver = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADRMAST];
				_usrI2CData[nI2C].aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//код тот же
				//ID слейва
				_usrI2CData[nI2C].aTxBuffer[1] =  nI2C;
				_usrI2CData[nI2C].aTxBuffer[2] =  0x22;
				_usrI2CData[nI2C].aTxBuffer[3] =  0x33;
				_usrI2CData[nI2C].aTxBuffer[4] =  0x44;
				//чистим приемный буфер
				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].sizeTxCmd = P1S2_SIZE_ANSW;

				_usrI2CData[nI2C].PhaseSend = SEND_START_CAN;
				_usrI2CData[nI2C].PhaseSetAddr = PH1_GET_ID__SEND_ANSW_MADE;
				break;

			case PH1_GET_ID__SEND_ANSW_MADE:
				if(_usrI2CData[nI2C].PhaseSend == SEND_WAS_GOOD_END)
					_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
				break;


			default:
				break;
		}
	}
#endif//#ifdef	GIVE_OUT_ADR_V2



#ifdef	GIVE_OUT_ADR_V1
void   __attribute__((optimize("O0"))) PrepData()
{
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		//постоянно перезапускать прием
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT)
			SetPhases(nI2C, SEND_NEUTRAL, RECEIVE_NEUTRAL, P0S0__DEFVAL);


		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case P1S0_S__DEFVAL:
				_usrI2CData[nI2C].PhaseSetAddr = P1S1_S__START_RECEIVE_DATA;
				break;

			case P1S1_S__START_RECEIVE_DATA:
				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].sizeRxCmd = P1S1_SZ_REQUEST;
				SetPhases(nI2C, SEND_NEUTRAL, RECEIVE_START, P1S1_S__WAIT_RECEIVE_DATA);
				break;

			case P1S1_S__WAIT_RECEIVE_DATA:
				//при наличии данных пытаемся распознать входящий запрос "дай ID"
				if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_YES_ANY_DATA)
				{
					if(_usrI2CData[nI2C].aRxBuffer[P1S1__I_B_CODCMD] == P1_CODE_SEND_ADR)
						_usrI2CData[nI2C].PhaseSetAddr = P1S1_S__CH_ADR_CMD_DETECT;
					else
						_usrI2CData[nI2C].PhaseSetAddr = P0S0__DEFVAL;//если данные не разобрать - начинаем все с начала
				}
				break;


			case P1S1_S__CH_ADR_CMD_DETECT:
				//извлекаем из входящего пакета адрес устройства-ведущего которому сейчас пойдет команда и адрес для обновления
				_adrOfReceiver = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADRMAST];
				_adr4Update2Me = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADR4WR];
				//формируем ответную команду подтверждения обновления адреса
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_CODCMD] = P1_CODE_SEND_ADR;//код тот же
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_ADR4WR] =  _adr4Update2Me;
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_REZ] =  0xEE;

				//назначить мне новый адрес
				//to do

				//чистим приемный буфер
				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].sizeTxCmd = P1S2_SIZE_ANSW;
				SetPhases(nI2C, SEND_START_CAN, RECEIVE_NEUTRAL, P1S2_S__SEND_ANSW_WAIT);
				break;

			case P1S2_S__SEND_ANSW_WAIT:
				if(_usrI2CData[nI2C].PhaseSend == SEND_WAS_GOOD_END)
				{
					SetPhases(nI2C, SEND_NEUTRAL, RECEIVE_START, P1S0_S__DEFVAL);
					_usrI2CData[nI2C].sizeRxCmd = P1S1_SZ_REQUEST;
#ifdef	DEBUG_ALLWAYS_GET_ADR
					_usrI2CData[nI2C].PhaseSetAddr = P1S0_S__DEFVAL;//для отладки снова быть готовым получить адрес
#endif
				}
				break;

			default:
				break;
		}
	}
#endif//#ifdef	GIVE_OUT_ADR_V1

}

void SetPhases(uint8_t nI2C, uint8_t phaseSend, uint8_t phaseReceive, uint8_t phaseSetAddr)
{
	_usrI2CData[nI2C].PhaseSend = phaseSend;
	_usrI2CData[nI2C].PhaseReceive = phaseReceive;
	_usrI2CData[nI2C].PhaseSetAddr = phaseSetAddr;
}
