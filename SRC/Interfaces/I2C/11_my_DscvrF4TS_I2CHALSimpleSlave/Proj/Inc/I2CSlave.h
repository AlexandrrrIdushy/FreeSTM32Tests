#ifndef __I2CSLAVE_H
#define __I2CSLAVE_H
#include "stm32f4xx_hal.h"

#define	I2CCODE_GET_ID_REQUEST		0xFA	//��� - ������� ������� ���� 1 ���� ID
#define	SIZE_GET_ID_REQUEST		3	//����� ���� � ������� "��� ID" ���������� ��������
#define	SIZE_FACTORY_NUM		4	//����� ���������� ������
#define	SIZE_SEND_ID_REQUEST		(SIZE_FACTORY_NUM + 1)//����� - ������� ����������� ID


//������ ������ � ������� [��� ������� ���� ID� = 0xFA] [����� ����������-��������] [��������������]
#define	GET_ID_REQUEST__I_B_CODCMD		0
#define	GET_ID_REQUEST__I_B_ADRMAST 	1
#define	GET_ID_REQUEST__I_B_REZ		 	2

//�����
#define	RECEIVE_NEUTRAL			0	//�� ���� ��������� �������� �������� � ������� �����
#define	RECEIVE_START			1	//������ ������
#define	RECEIVE_WAIT_DATA		2	//������� ������
#define	RECEIVE_YES_ANY_DATA	3	//������� ������ �������
#define	RECEIVE_TIMOUT			4	//����� �����, ������ �� ����

//�������
#define	SEND_NEUTRAL		0	//�� ���� ��������� �������� �������� � ������� �����
#define	SEND_START_NOW	1	//��������� ��������
#define	SEND_WAS_START		2	//�������� ��������
#define	SEND_WAS_GOOD_END	3	//�������� ������� ���������


//���� ID ������� �����
#define	PH1_GET_ID__DEFVAL				0
#define	PH1_GET_ID__SEND_ANSW			1	//������� ������ ���� ID� - ������� �����
#define	PH1_GET_ID__SEND_ANSW_MADE		2

#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10




extern void I2CInit(void);
extern void I2CReceive(I2C_HandleTypeDef*, uint8_t);
extern void I2CSend(I2C_HandleTypeDef*, uint8_t);
extern void PrepData(void);
#endif//I2CSLAVE
