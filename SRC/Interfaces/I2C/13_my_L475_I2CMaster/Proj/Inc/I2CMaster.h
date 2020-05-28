#ifndef __MASTERI2C_H
#define __MASTERI2C_H
#include "stm32l4xx_hal.h"


#define	ADDR_BY_MASTER		1	//����� �������
#define	I2CCODE_GET_ID_REQUEST		0xFA	//��� - ������� ������� ���� 1 ���� ID
#define	SIZE_GET_ID_REQUEST		3	//����� ���� � ������� "��� ID" ���������� ��������
#define	SIZE_FACTORY_NUM		4	//����� ���������� ������ ��������
#define	SIZE_SEND_ID_REQUEST		(SIZE_FACTORY_NUM + 1)//����� - ������� ����������� ID

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
#define	PH1_GET_ID__CMD_GET_ID_SENT		1	//������ ���� ID� ������
#define	PH1_GET_ID__ID_GRANTED		2	//������ ����� �� ���� ID�



#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10



extern void I2CInit(void);
extern void I2CReceive();
extern void I2CSend();
extern void PrepData(void);

#endif /* __MASTERI2C_H */
