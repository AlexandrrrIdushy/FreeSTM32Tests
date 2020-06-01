#ifndef __MASTERI2C_H
#define __MASTERI2C_H
#include "stm32l4xx_hal.h"


//фаза1 шаг1. сбор ID с слейвов. запрос мастера
//фаза1 шаг2. сбор ID с слейвов. ответ слейва

#define	ADDR_BY_MASTER		1	//адрес мастера
#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID
#define	ST1_SIZE_REQUEST		3	//число байт в запросе "дай ID" устройства ведущего
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
#define	SEND_START_NOW		1	//запустить передачу
#define	SEND_WAS_START		2	//запущена передача
#define	SEND_WAS_GOOD_END	3	//передача успешно завершена
#define	SEND_TIMOUT			4	//время вышло, данные не переданы



//сбор ID ведомых этапы
#define	ST1__DEFVAL				0
#define	ST1__CMD_GET_ID_SENT		1	//запрос «дай ID» послан
#define	ST1__ID_GRANTED		2	//пришел ответ на «дай ID»



#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10



extern void I2CInit(void);
extern void I2CReceive();
extern void I2CSend();
extern void PrepData(void);

#endif /* __MASTERI2C_H */
