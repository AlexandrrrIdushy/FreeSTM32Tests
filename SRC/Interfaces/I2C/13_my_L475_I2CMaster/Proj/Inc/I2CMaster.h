#ifndef __MASTERI2C_H
#define __MASTERI2C_H
#include "stm32l4xx_hal.h"


#define	ADDR_BY_MASTER		1	//адрес мастера
#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID
#define	SIZE_GET_ID_REQUEST		3	//число байт в запросе "дай ID" устройства ведущего
#define	SIZE_FACTORY_NUM		4	//длина заводского номера ведомого
#define	SIZE_SEND_ID_REQUEST		(SIZE_FACTORY_NUM + 1)//ответ - ведомый предьявляет ID

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
#define	PH1_GET_ID__CMD_GET_ID_SENT		1	//запрос «дай ID» послан
#define	PH1_GET_ID__ID_GRANTED		2	//пришел ответ на «дай ID»



#define	SZ_ARR_RX_BUFF		10

#define	SZ_ARR_TX_BUFF		10



extern void I2CInit(void);
extern void I2CReceive();
extern void I2CSend();
extern void PrepData(void);

#endif /* __MASTERI2C_H */
