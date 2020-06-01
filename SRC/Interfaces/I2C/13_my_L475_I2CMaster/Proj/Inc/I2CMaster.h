#ifndef __MASTERI2C_H
#define __MASTERI2C_H
#include "stm32l4xx_hal.h"


//����1 ���1. ���� ID � �������. ������ �������
//����1 ���2. ���� ID � �������. ����� ������

#define	ADDR_BY_MASTER		1	//����� �������
#define	I2CCODE_GET_ID_REQUEST		0xFA	//��� - ������� ������� ���� 1 ���� ID
#define	ST1_SIZE_REQUEST		3	//����� ���� � ������� "��� ID" ���������� ��������
#define	SIZE_FACTORY_NUM		4	//����� ���������� ������ ��������
#define	P1S2_SIZE_ANSW		(SIZE_FACTORY_NUM + 1)//����� - ������� ����������� ID

//������ ������ � ������� [��� ������� ���� ID� = 0xFA] [����� ����������-��������] [��������������]
#define	P1S1__I_B_CODCMD	0
#define	P1S1__I_B_ADRMAST 	1
#define	P1S1__I_B_REZ		2

//�����
#define	RECEIVE_NEUTRAL			0	//�� ���� ��������� �������� �������� � ������� �����
#define	RECEIVE_START			1	//������ ������
#define	RECEIVE_WAIT_DATA		2	//������� ������
#define	RECEIVE_YES_ANY_DATA	3	//������� ������ �������
#define	RECEIVE_TIMOUT			4	//����� �����, ������ �� ����

//�������
#define	SEND_NEUTRAL		0	//�� ���� ��������� �������� �������� � ������� �����
#define	SEND_START_NOW		1	//��������� ��������
#define	SEND_WAS_START		2	//�������� ��������
#define	SEND_WAS_GOOD_END	3	//�������� ������� ���������
#define	SEND_TIMOUT			4	//����� �����, ������ �� ��������



//���� ID ������� �����
#define	ST1__DEFVAL				0
#define	ST1__CMD_GET_ID_SENT		1	//������ ���� ID� ������
#define	ST1__ID_GRANTED		2	//������ ����� �� ���� ID�



#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10



extern void I2CInit(void);
extern void I2CReceive();
extern void I2CSend();
extern void PrepData(void);

#endif /* __MASTERI2C_H */
