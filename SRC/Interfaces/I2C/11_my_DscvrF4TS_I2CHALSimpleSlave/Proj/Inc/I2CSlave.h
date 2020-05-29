#ifndef __I2CSLAVE_H
#define __I2CSLAVE_H
#include "stm32f4xx_hal.h"

#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID
#define	SIZE_GET_ID_REQUEST		3	//число байт в запросе "дай ID" устройства ведущего
#define	SIZE_FACTORY_NUM		4	//длина заводского номера
#define	SIZE_SEND_ID_REQUEST		(SIZE_FACTORY_NUM + 1)//ответ - ведомый предьявляет ID


//номера байтов в запросе [Код команды «дай ID» = 0xFA] [Адрес устройства-ведущего] [Зарезервирован]
#define	GET_ID_REQUEST__I_B_CODCMD		0
#define	GET_ID_REQUEST__I_B_ADRMAST 	1
#define	GET_ID_REQUEST__I_B_REZ		 	2

//прием
#define	RECEIVE_NEUTRAL			0	//по сути состояние ожидания перехода в рабочий режим
#define	RECEIVE_START			1	//запуск приема
#define	RECEIVE_WAIT_DATA		2	//ожидаем запрос
#define	RECEIVE_YES_ANY_DATA	3	//какието данные приняты
#define	RECEIVE_TIMOUT			4	//время вышло, данных не было

//предача
#define	SEND_NEUTRAL		0	//по сути состояние ожидания перехода в рабочий режим
#define	SEND_START_NOW	1	//запустить передачу
#define	SEND_WAS_START		2	//запущена передача
#define	SEND_WAS_GOOD_END	3	//передача успешно завершена


//сбор ID ведомых этапы
#define	PH1_GET_ID__DEFVAL				0
#define	PH1_GET_ID__SEND_ANSW			1	//получен запрос «дай ID» - послать ответ
#define	PH1_GET_ID__SEND_ANSW_MADE		2

#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10




extern void I2CInit(void);
extern void I2CReceive(I2C_HandleTypeDef*, uint8_t);
extern void I2CSend(I2C_HandleTypeDef*, uint8_t);
extern void PrepData(void);
#endif//I2CSLAVE
