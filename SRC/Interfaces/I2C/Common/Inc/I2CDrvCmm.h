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

//фаза1 шаг1. сбор ID с слейвов. запрос мастера
//фаза1 шаг2. сбор ID с слейвов. ответ слейва

#define	ADDR_BY_MASTER		1	//адрес мастера
#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID
#define	P1S1_SZ_REQUEST		3	//число байт в запросе "дай ID" устройства ведущего
#define	SIZE_FACTORY_NUM		4	//длина заводского номера ведомого
#define	P1S2_SIZE_ANSW		(SIZE_FACTORY_NUM + 1)//ответ - ведомый предьявляет ID

//номера байтов в запросе [Код команды «дай ID» = 0xFA] [Адрес устройства-ведущего] [Зарезервирован]
#define	P1S1__I_B_CODCMD	0
#define	P1S1__I_B_ADRMAST 	1
#define	P1S1__I_B_REZ		2

//прием
#define	RECEIVE_NEUTRAL			0	//по сути состояние ожидания перехода в рабочий режим
#define	RECEIVE_START			1	//запуск приема
#define	RECEIVE_WAIT_DATA		2	//ожидаем запрос
#define	RECEIVE_YES_ANY_DATA	3	//какието данные приняты
#define	RECEIVE_TIMOUT			4	//время вышло, данных не было

//предача
#define	SEND_NEUTRAL		0	//по сути состояние ожидания перехода в рабочий режим
#define	SEND_START_CAN		1	//запустить передачу после таймаута
#define	SEND_START_WAIT		2	//ожидаем запуска
#define	SEND_START_NOW		3	//запустить передачу сейчас
#define	SEND_WAS_START		4	//запущена передача
#define	SEND_WAS_GOOD_END	5	//передача успешно завершена
#define	SEND_TIMOUT			6	//время вышло, данных не было




//МАСТЕР
//сбор ID ведомых этапы
#define	ST1__DEFVAL				0
#define	ST1__CMD_GET_ID_SENT		1	//запрос «дай ID» послан
#define	ST1__ID_GRANTED		2	//пришел ответ на «дай ID»





//СЛЕЙВ
//сбор ID ведомых этапы
#define	PH1_GET_ID__DEFVAL				0
#define	PH1_GET_ID__SEND_ANSW			1	//получен запрос «дай ID» - послать ответ
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




extern void I2CInit(void);
extern void I2CReceive(I2C_HandleTypeDef*, uint8_t);
extern void I2CSend(I2C_HandleTypeDef*, uint8_t);
extern void PrepData(void);
//extern void SetPhaseReceive(uint8_t, uint8_t);

extern uint32_t GetSysCounter100MSec(void);

#endif//#define __DRVCMMI2C_H
