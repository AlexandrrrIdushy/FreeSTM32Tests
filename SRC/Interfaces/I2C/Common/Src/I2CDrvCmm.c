
#include "I2CDrvCmm.h"
#include <stdint.h>


struct I2CUsrData _usrI2CData[3];
//struct I2CUsrData _usrI2CData;

I2C_HandleTypeDef* _hi2c1;

uint8_t	_adrOfReceiver;
uint8_t	_adr4Update2Me;



//void SetPhaseReceive(uint8_t nI2CUsrData, uint8_t phase)
//{
//	_usrI2CData[nI2CUsrData].PhaseReceive = phase;
//	return;
//}




//ДРАЙВЕР

#define DELAY_RECEIVE_END	2000
void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
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
			{
//				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_TIMOUT;
#ifdef	DEBUG_TRY_TESET_I2C
				HAL_I2C_Init(hi2c);
#endif
			}
			break;

		default:
			break;
	}

}


#define	DELAY_SEND_START 2
#define	DELAY_SEND_END 200
void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t locCntWaitStart = 0;
	static uint32_t locCntWaitEndSend = 0;

	switch (_usrI2CData[nI2C].PhaseSend)
	{
		case SEND_START_CAN:
			locCntWaitStart = GetSysCounter100MSec();
			_usrI2CData[nI2C].PhaseSend = SEND_START_WAIT;
//			hi2c->State = 20;//!костыль
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
			{
#ifdef	DEBUG_TRY_TESET_I2C
				HAL_I2C_Init(hi2c);
#endif
				_usrI2CData[nI2C].PhaseSend = SEND_TIMOUT;
			}
			break;

		default:
			break;
	}

}





void  __attribute__((optimize("O0"))) HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	_usrI2CData[0].PhaseSend = SEND_WAS_GOOD_END;
#ifdef	DEBUG_TRY_TESET_I2C
	HAL_I2C_Init(hi2c);
#endif
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
#ifdef	DEBUG_TRY_TESET_I2C
	HAL_I2C_Init(hi2c);
#endif
}
