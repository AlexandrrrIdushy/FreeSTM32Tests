#include "I2CSlave.h"
#include <string.h>

//#define	DEBUG_1//просто подтверждает запрос на запись не отвечая

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

struct I2CUsrData _usrI2CData[3];

void I2CInit()
{
	// статусы для запуска драйвераобмена
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		_usrI2CData[nI2C].PhaseSend = SEND_NEUTRAL;
		_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
		_usrI2CData[nI2C].sizeRxCmd = P1S1_SZ_REQUEST;
		_usrI2CData[nI2C].PhaseSetAddr = 0;
		memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
	}


//	//выключаем включаем шину
//	I2C1 ->CR1 &= (~I2C_CR1_PE);
//	HAL_Delay(100);
//	I2C1 ->CR1 |= I2C_CR1_PE;
//
//	I2C1 ->CR1 |= I2C_CR1_SWRST;//сброс логики после каждого рестарта
}


void SetPhaseReceive(uint8_t nI2CUsrData, uint8_t phase)
{
	_usrI2CData[nI2CUsrData].PhaseReceive = phase;
	return;
}

#define DELAY_RECEIVE_END	2000
void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t startLocalCounter = 0;

	switch (_usrI2CData[nI2C].PhaseReceive)
	{
		case RECEIVE_START:
//			while(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C].aRxBuffer), _usrI2CData[nI2C].sizeRxCmd)!= HAL_OK){}
//			if(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C]._aRxBuffer), SIZE_GET_ID_REQUEST) == HAL_OK)
//			{
			HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C].aRxBuffer), _usrI2CData[nI2C].sizeRxCmd);
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_WAIT_DATA;
			startLocalCounter = GetSysCounter100MSec();
//			}
			break;
		case RECEIVE_WAIT_DATA:
			//если вышло время выделенное на прием
			if((GetSysCounter100MSec() - startLocalCounter) > DELAY_RECEIVE_END)
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_TIMOUT;
			break;

		default:
			break;
	}

}
uint8_t	_adrOfMaster;
#define	DELAY_SEND_START 5
void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t locCntWaitStart = 0;
	static uint32_t startLocalCounter = 0;

	switch (_usrI2CData[nI2C].PhaseSend)
	{
		case SEND_START_CAN:
			locCntWaitStart = GetSysCounter100MSec();
			_usrI2CData[nI2C].PhaseSend = SEND_START_WAIT;
			break;

		case SEND_START_WAIT:
			if((GetSysCounter100MSec() - locCntWaitStart) > DELAY_SEND_START)
				_usrI2CData[nI2C].PhaseSend = SEND_START_NOW;
			break;

		case SEND_START_NOW:
				HAL_I2C_Master_Transmit_IT(hi2c, _adrOfMaster, (uint8_t *)(_usrI2CData[nI2C].aTxBuffer), _usrI2CData[nI2C].sizeTxCmd);
				_usrI2CData[nI2C].PhaseSend = SEND_WAS_START;
				startLocalCounter = GetSysCounter100MSec();
			break;

		case SEND_WAS_START:
//			if((GetSysCounter100MSec() - startLocalCounter) > DELAY_RECEIVE_END)
//				_usrI2CData[nI2C].PhaseSend = SEND_TIMOUT;
			break;

		default:
			break;
	}

}


void PrepData()
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
				_adrOfMaster = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADRMAST];
				_usrI2CData[nI2C].aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//код тот же
				//ID слейва
				_usrI2CData[nI2C].aTxBuffer[1] =  nI2C;
				_usrI2CData[nI2C].aTxBuffer[2] =  0x22;
				_usrI2CData[nI2C].aTxBuffer[3] =  0x33;
				_usrI2CData[nI2C].aTxBuffer[4] =  0x44;
				//чистим приемный буфер
				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].sizeTxCmd = P1S2_SZ_RESPN;

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


}



void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	_usrI2CData[0].PhaseSend = SEND_WAS_GOOD_END;
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	asm("nop");
}
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	asm("nop");
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
//	SetPhaseReceive(0, RECEIVE_YES_ANY_DATA);
	_usrI2CData[0].PhaseReceive = RECEIVE_YES_ANY_DATA;
}
