
#include "I2CDrvCmm.h"
#include <stdint.h>






struct I2CUsrData _usrI2CData[3];
//struct I2CUsrData _usrI2CData;

I2C_HandleTypeDef* _hi2c1;

//void SetHederMasterI2C(I2C_HandleTypeDef* hi2c1)
//{
//	_hi2c1 = hi2c1;
//
//}

#define	BTN_PUSH	0
#define	BTN_RELEASE	1
uint8_t	_adrOfReceiver;
uint8_t _btnState, _lastBtnState;




#ifdef	ITS_MASTER


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
#endif//#ifdef	ITS_MASTER



























//SLAVE






//void SetPhaseReceive(uint8_t nI2CUsrData, uint8_t phase)
//{
//	_usrI2CData[nI2CUsrData].PhaseReceive = phase;
//	return;
//}












//ДРАЙВЕР

#define DELAY_RECEIVE_END	2000
void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
//	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t startLocalCounter = 0;

	switch (_usrI2CData[nI2C].PhaseReceive)
	{
		case RECEIVE_START:
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

#define	DELAY_SEND_START 2
#define	DELAY_SEND_END 200
void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
//	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t locCntWaitStart = 0;
	static uint32_t locCntWaitEndSend = 0;

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
				HAL_I2C_Master_Transmit_IT(hi2c, (_adrOfReceiver << 1), (uint8_t *)(_usrI2CData[nI2C].aTxBuffer), _usrI2CData[nI2C].sizeTxCmd);
				_usrI2CData[nI2C].PhaseSend = SEND_WAS_START;
				locCntWaitEndSend = GetSysCounter100MSec();
			break;

		case SEND_WAS_START:
			if((GetSysCounter100MSec() - locCntWaitEndSend) > DELAY_SEND_END)
				_usrI2CData[nI2C].PhaseSend = SEND_TIMOUT;
			break;

		default:
			break;
	}

}





void  __attribute__((optimize("O0"))) HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	_usrI2CData[0].PhaseSend = SEND_WAS_GOOD_END;
}
void  __attribute__((optimize("O0"))) HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	asm("nop");
}
void  __attribute__((optimize("O0"))) HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	asm("nop");
}
void  __attribute__((optimize("O0"))) HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	_usrI2CData[0].PhaseReceive = RECEIVE_YES_ANY_DATA;
}
