#ifndef __MASTERI2C_H
#define __MASTERI2C_H
#include "stm32l4xx_hal.h"


#define	ADDR_BY_MASTER		1	//адрес мастера
#define	I2CCODE_GET_ID_REQUEST		0xFA	//код - раздача адресов фаза 1 сбор ID
#define	SIZE_GET_ID_REQUEST		3	//число байт в таком запросе

//прием
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался

#define	SEND_DEFVAL				0	//
#define	RECEIVE_START			1	//запуск приема
#define	RECEIVE_WAIT			2	//ожидаем запрос
#define	RECEIVE_YES_ANY_DATA	3	//какието данные приняты

//предача
//#define	PH1_GET_ID__BEFORE_CALL_RCV	0	//пока обмен не запускался
//#define	SEND_CLEAN		0	//был запрос на который потребуется очистить - разблокируем возможность передачи
#define	SEND_DEFVAL		0	//
#define	SEND_START_NOW	1	//запустить передачу
#define	SEND_WAS_START		2	//запущена передача



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
