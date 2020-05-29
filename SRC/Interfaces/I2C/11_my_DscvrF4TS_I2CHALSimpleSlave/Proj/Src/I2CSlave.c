#include "I2CSlave.h"
#include <string.h>



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
	// ������� ��� ������� ��������������
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		_usrI2CData[nI2C].PhaseSend = SEND_NEUTRAL;
		_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
		_usrI2CData[nI2C].PhaseSetAddr = 0;
		memset(_usrI2CData[nI2C]._aRxBuffer, 0, SZ_ARR_RX_BUFF);
	}

//	//��������� �������� ����
//	I2C1 ->CR1 &= (~I2C_CR1_PE);
//	HAL_Delay(100);
//	I2C1 ->CR1 |= I2C_CR1_PE;
//
//	I2C1 ->CR1 |= I2C_CR1_SWRST;//����� ������ ����� ������� ��������
}


#define DELAY_RECEIVE_END	2000
void __attribute__((optimize("O0"))) I2CReceive(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);
	static uint32_t startLocalCounter = 0;

	switch (_usrI2CData[nI2C].PhaseReceive)
	{
		case RECEIVE_START:
			while(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C]._aRxBuffer), SIZE_GET_ID_REQUEST)!= HAL_OK){}
//			if(HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)(_usrI2CData[nI2C]._aRxBuffer), SIZE_GET_ID_REQUEST) == HAL_OK)
//			{
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_WAIT_DATA;
				startLocalCounter = GetSysCounter100MSec();
//			}
			break;
		case RECEIVE_WAIT_DATA:
			if(resGetState == HAL_I2C_STATE_READY)
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_YES_ANY_DATA;
			//���� ����� ����� ���������� �� �����
			else if((GetSysCounter100MSec() - startLocalCounter) > DELAY_RECEIVE_END)
			{
				_usrI2CData[nI2C].PhaseReceive = RECEIVE_TIMOUT;

				//!!! �������? ��� ����� ����� � ������ I2C HAL � ��������� ���������. ����� ������������� ����� ������������� �����
				resGetState = HAL_I2C_STATE_READY;
			}
			break;

		default:
			break;
	}

}
uint8_t	_adrOfMaster;
void __attribute__((optimize("O0"))) I2CSend(I2C_HandleTypeDef* hi2c, uint8_t nI2C)
{
	HAL_I2C_StateTypeDef resGetState = HAL_I2C_GetState(hi2c);


	switch (_usrI2CData[nI2C].PhaseSend)
	{
		case SEND_START_NOW:
			if(resGetState == HAL_I2C_STATE_READY)
			{
				HAL_I2C_Master_Transmit_IT(hi2c, _adrOfMaster, (uint8_t *)(_usrI2CData[nI2C]._aTxBuffer), 3);//SIZE_SEND_ID_REQUEST);
				_usrI2CData[nI2C].PhaseSend = SEND_WAS_START;
			}
			break;

		case SEND_WAS_START:
			//��������� ��� ������� ����� HAL_I2C_STATE_BUSY_TX � ����� �������� �� HAL_I2C_STATE_READY

			if(resGetState == HAL_I2C_STATE_READY)
				_usrI2CData[nI2C].PhaseSend = SEND_WAS_GOOD_END;
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

		//��������� ������������� �����
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT)
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;


		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case PH1_GET_ID__SEND_ANSW:
				_adrOfMaster = _usrI2CData[nI2C]._aRxBuffer[GET_ID_REQUEST__I_B_ADRMAST];
				_usrI2CData[nI2C]._aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//��� ��� ��
				//ID ������
				_usrI2CData[nI2C]._aTxBuffer[1] =  nI2C;
				_usrI2CData[nI2C]._aTxBuffer[2] =  0x22;
				_usrI2CData[nI2C]._aTxBuffer[3] =  0x33;
				_usrI2CData[nI2C]._aTxBuffer[4] =  0x44;
				//������ �������� �����
				memset(_usrI2CData[nI2C]._aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].PhaseSend = SEND_START_NOW;
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
