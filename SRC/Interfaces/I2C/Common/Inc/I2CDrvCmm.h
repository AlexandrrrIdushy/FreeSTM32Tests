#ifndef __DRVCMMI2C_H
#define __DRVCMMI2C_H

#include "MyDefine.h"

#ifdef	ITS_MASTER
#include "stm32l4xx_hal.h"
#endif//#ifdef	ITS_MASTER

#ifdef	ITS_SLAVE
#include "stm32f4xx_hal.h"
#endif//#ifdef	ITS_SLAVE


#define	SZ_ARR_RX_BUFF		10
#define	SZ_ARR_TX_BUFF		10

//����1 ���1. ���� ID � �������. ������ �������
//����1 ���2. ���� ID � �������. ����� ������

#define	ADDR_BY_MASTER		1	//����� �������
#define	I2CCODE_GET_ID_REQUEST		0xFA	//��� - ������� ������� ���� 1 ���� ID

#define	SIZE_FACTORY_NUM		4	//����� ���������� ������ ��������
#ifdef	GIVE_OUT_ADR_V2
#define	P1S1_SZ_REQUEST		3	//����� ���� � ������� "��� ID" ���������� ��������
#define	P1S2_SIZE_ANSW		(SIZE_FACTORY_NUM + 1)//����� - ������� ����������� ID

//������ ������ � ������� [��� ������� ���� ID� = 0xFA] [����� ����������-��������] [��������������]
#define	P1S1__I_B_CODCMD	0
#define	P1S1__I_B_ADRMAST 	1
#define	P1S1__I_B_REZ		2
#endif



#ifdef	GIVE_OUT_ADR_V1
#define	P1S1_SZ_REQUEST		3	//����� ���� � ������� "��� ID" ���������� ��������
#define	P1S2_SIZE_ANSW		P1S1_SZ_REQUEST//����� - ������� ����������� ID

//������ ������ � ������� [��� ������� ���� ID� = 0xFA] [����� ����������-��������] [����� ����� ����������� ��������]
#define	P1S1__I_B_CODCMD	0
#define	P1S1__I_B_ADRMAST 	1
#define	P1S1__I_B_ADR4WR	2
//������ ������ � ������ [��� ������� ���� ID� = 0xFA] [����� ����� ����������� ��������] [������]
#define	P1S2__I_B_CODCMD	0
#define	P1S2__I_B_ADR4WR	1
#define	P1S2__I_B_REZ	 	2

#endif//#ifdef	GIVE_OUT_ADR_V1

//�����
#define	RECEIVE_NEUTRAL			0	//�� ���� ��������� �������� �������� � ������� �����
#define	RECEIVE_START			1	//������ ������
#define	RECEIVE_WAIT_DATA		2	//������� ������
#define	RECEIVE_YES_ANY_DATA	3	//������� ������ �������
#define	RECEIVE_TIMOUT			4	//����� �����, ������ �� ����

//�������
#define	SEND_NEUTRAL		0	//�� ���� ��������� �������� �������� � ������� �����
#define	SEND_START_CAN		1	//��������� �������� ����� ��������
#define	SEND_START_WAIT		2	//������� �������
#define	SEND_START_NOW		3	//��������� �������� ������
#define	SEND_WAS_START		4	//�������� ��������
#define	SEND_WAS_GOOD_END	5	//�������� ������� ���������
#define	SEND_TIMOUT			6	//����� �����, ������ �� ����




//������
//���� ID ������� �����
#define	ST1__DEFVAL				0
#define	ST1__CMD_GET_ID_SENT		1	//������ ���� ID� ������
#define	ST1__ID_GRANTED		2	//������ ����� �� ���� ID�

//���������� ������. ������� 1 ������� ��������� ���� �� ������ � ���� �����
#define	P0S0__DEFVAL				0
#define	P1S1__SEND_ADR_SLV_BEGIN	1	//
#define	P1S2__SEND_WAIT_END_SENDING	2
#define	P1S3__RCV_WAIT_RCV_DATA		3
#define	P1S2__WAIT_CONFIRM			4	//





//�����
//���� ID ������� �����
#define	PH1_GET_ID__DEFVAL				0
#define	PH1_GET_ID__SEND_ANSW			1	//������� ������ ���� ID� - ������� �����
#define	PH1_GET_ID__SEND_ANSW_MADE		2




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

extern struct I2CUsrData _usrI2CData[3];
extern uint8_t	_adrOfReceiver;


extern void I2CInit(void);
extern void I2CReceive(I2C_HandleTypeDef*, uint8_t);
extern void I2CSend(I2C_HandleTypeDef*, uint8_t);
extern void PrepData(void);
//extern void SetPhaseReceive(uint8_t, uint8_t);

extern uint32_t GetSysCounter100MSec(void);

#endif//#define __DRVCMMI2C_H
