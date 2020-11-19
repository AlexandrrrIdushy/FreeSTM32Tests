#ifndef W5500_H_

#define W5500_H_

//--------------------------------------------------

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "fatfs.h"

//--------------------------------------------------
#define CS_GPIO_PORT GPIOB
#define CS_PIN GPIO_PIN_6
#define SS_SELECT() HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET)
#define SS_DESELECT() HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET)
//--------------------------------------------------
#define MAC_ADDR {0x00,0x15,0x42,0xBF,0xF0,0x51}
//--------------------------------------------------
#define BSB_COMMON 0x00
#define BSB_S0 0x01
#define BSB_S0_TX 0x02
#define BSB_S0_RX 0x03

//--------------------------------------------------
#define RWB_WRITE 1
#define RWB_READ 0

//--------------------------------------------------

#define OM_FDM0 0x00//����� �������� ������ ���������� �����
#define OM_FDM1 0x01//����� �������� ������ �� ������ �����
#define OM_FDM2 0x02//����� �������� ������ �� ��� �����
#define OM_FDM3 0x03//����� �������� ������ �� ������ �����

//--------------------------------------------------

#define MR 0x0000//Mode Register
//--------------------------------------------------
//������ ��� ����������
#define SHAR0 0x0009//Source Hardware Address Register MSB
#define SHAR1 0x000A
#define SHAR2 0x000B
#define SHAR3 0x000C
#define SHAR4 0x000D
#define SHAR5 0x000E// LSB
#define GWR0 0x0001//Gateway IP Address Register MSB
#define GWR1 0x0002
#define GWR2 0x0003
#define GWR3 0x0004// LSB
#define SUBR0 0x0005//Subnet Mask Register MSB
#define SUBR1 0x0006
#define SUBR2 0x0007
#define SUBR3 0x0008// LSB
#define SIPR0 0x000F//Source IP Address Register MSB
#define SIPR1 0x0010
#define SIPR2 0x0011
#define SIPR3 0x0012// LSB
//--------------------------------------------------
//������ �������� ���� ��� ������ 0.
#define Sn_PORT0 0x0004 // Socket 0 Source Port Register MSB
#define Sn_PORT1 0x0005 // Socket 0 Source Port Register LSB
//--------------------------------------------------
//--------------------------------------------------

#define Sn_MR 0x0000 // Socket 0 Mode Register
#define Sn_CR 0x0001 // Socket 0 Command Register
#define Sn_SR 0x0003 // Socket 0 Status Register
//--------------------------------------------------

//Socket mode
#define Mode_CLOSED 0x00
#define Mode_TCP 0x01
#define Mode_UDP 0x02
#define Mode_MACRAV 0x04

//--------------------------------------------------

//Socket states
#define SOCK_CLOSED 0x00
#define SOCK_INIT 0x13
#define SOCK_LISTEN 0x14
#define SOCK_ESTABLISHED 0x17

//-------------------------------------------
#define Sn_MSSR0 0x0012
#define Sn_MSSR1 0x0013
#define Sn_TX_FSR0 0x0020
#define Sn_TX_FSR1 0x0021
#define Sn_TX_RD0 0x0022
#define Sn_TX_RD1 0x0023
#define Sn_TX_WR0 0x0024
#define Sn_TX_WR1 0x0025
#define Sn_RX_RSR0 0x0026
#define Sn_RX_RSR1 0x0027
#define Sn_RX_RD0 0x0028
#define Sn_RX_RD1 0x0029
//--------------------------------------------------

#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
//--------------------------------------------------

typedef struct tcp_prop
{
	volatile uint8_t cur_sock;//�������� �����
} tcp_prop_ptr;
//--------------------------------------------------

extern tcp_prop_ptr tcpprop;

extern void w5500_ini(void);

void OpenSocket(uint8_t, uint16_t);
void SocketInitWait(uint8_t);
void ListenSocket(uint8_t);
void SocketListenWait(uint8_t);
void w5500_packetReceive(void);
uint8_t GetSocketStatus(uint8_t);
extern uint8_t w5500_readReg(uint8_t, uint16_t);
#endif /* W5500_H_ */
