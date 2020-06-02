#include "I2CDrvCmm.h"



void I2CInit()
{
	// ������� ��� ������� ��������������
	for (int nI2C = 0; nI2C < 3; nI2C++)
	{
		_usrI2CData[nI2C].PhaseSend = SEND_NEUTRAL;
		_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
		_usrI2CData[nI2C].sizeRxCmd = P1S1_SZ_REQUEST;
		_usrI2CData[nI2C].PhaseSetAddr = 0;
		memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
	}


//	//��������� �������� ����
//	I2C1 ->CR1 &= (~I2C_CR1_PE);
//	HAL_Delay(100);
//	I2C1 ->CR1 |= I2C_CR1_PE;
//
//	I2C1 ->CR1 |= I2C_CR1_SWRST;//����� ������ ����� ������� ��������
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

		//���������� �������� ������ "��� ID"
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_YES_ANY_DATA &&
				_usrI2CData[nI2C].aRxBuffer[0] == I2CCODE_GET_ID_REQUEST &&
				_usrI2CData[nI2C].aRxBuffer[2] == 0xEE)
			_usrI2CData[nI2C].PhaseSetAddr = PH1_GET_ID__SEND_ANSW;




		//��������� ������������� �����
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT)
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;


		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case PH1_GET_ID__SEND_ANSW:
				_adrOfReceiver = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADRMAST];
				_usrI2CData[nI2C].aTxBuffer[0] = I2CCODE_GET_ID_REQUEST;//��� ��� ��
				//ID ������
				_usrI2CData[nI2C].aTxBuffer[1] =  nI2C;
				_usrI2CData[nI2C].aTxBuffer[2] =  0x22;
				_usrI2CData[nI2C].aTxBuffer[3] =  0x33;
				_usrI2CData[nI2C].aTxBuffer[4] =  0x44;
				//������ �������� �����
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
		//��������� ������������� �����
		if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_TIMOUT)
			_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;

		switch (_usrI2CData[nI2C].PhaseSetAddr)
		{
			case P1S0_S__DEFVAL:
				//���������� �������� ������ "��� ID"
				if(_usrI2CData[nI2C].PhaseReceive == RECEIVE_YES_ANY_DATA &&
						_usrI2CData[nI2C].aRxBuffer[P1S1__I_B_CODCMD] == I2CCODE_GET_ID_REQUEST)
					_usrI2CData[nI2C].PhaseSetAddr = P1S1_S__CH_ADR_CMD_DETECT;
				break;

			case P1S1_S__CH_ADR_CMD_DETECT:
				//����� ����������-�������� �������� ������ ������ �������
				_adrOfReceiver = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADRMAST];
				_adr4Update2Me = _usrI2CData[nI2C].aRxBuffer[P1S1__I_B_ADR4WR];
				//��������� �������
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_CODCMD] = I2CCODE_GET_ID_REQUEST;//��� ��� ��
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_ADR4WR] =  _adr4Update2Me;
				_usrI2CData[nI2C].aTxBuffer[P1S2__I_B_REZ] =  0xEE;

				//��������� ��� ����� �����
				//to do

				//������ �������� �����
				memset(_usrI2CData[nI2C].aRxBuffer, 0, SZ_ARR_RX_BUFF);
				_usrI2CData[nI2C].sizeTxCmd = P1S2_SIZE_ANSW;
				_usrI2CData[nI2C].PhaseSend = SEND_START_CAN;
				_usrI2CData[nI2C].PhaseSetAddr = P1S2_S__SEND_ANSW_WAIT;
				break;

			case P1S2_S__SEND_ANSW_WAIT:
				if(_usrI2CData[nI2C].PhaseSend == SEND_WAS_GOOD_END)
					_usrI2CData[nI2C].PhaseReceive = RECEIVE_START;
				break;

			default:
				break;
		}
	}
#endif//#ifdef	GIVE_OUT_ADR_V1

}
