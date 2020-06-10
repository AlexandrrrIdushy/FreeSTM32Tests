/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2020-04-24

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"



void __attribute__((optimize("O0"))) i2c_init2()
{
    GPIO_InitTypeDef gpio_init;
    I2C_InitTypeDef i2c_init;
    NVIC_InitTypeDef NVIC_InitStructure, NVIC_InitStructure2;

    I2C_DeInit(I2C2 );       //Deinit and reset the I2C to avoid it locking up
    I2C_SoftwareResetCmd(I2C2, ENABLE);
    I2C_SoftwareResetCmd(I2C2, DISABLE);

        /*!< I2C Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        /* setup SCL and SDA pins
         * SCL on PB10 and SDA on PB11
         */
    gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;     // we are going to use PB10 and PB11
    gpio_init.GPIO_Mode = GPIO_Mode_AF;                                 // set pins to alternate function
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;                        // set GPIO speed
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;                                 //Pull up resistor
    gpio_init.GPIO_OType = GPIO_OType_OD;                               //Open Drain
    GPIO_Init(GPIOB, &gpio_init);

        // Connect I2C2 pins to AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2 ); // SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2 ); // SDA

        /* Configure the Priority Group to 1 bit */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure2.NVIC_IRQChannel = I2C2_ER_IRQn;
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure2);

    I2C_ITConfig(I2C2, I2C_IT_EVT, ENABLE);
    I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
    I2C_ITConfig(I2C2, I2C_IT_BUF, ENABLE);


    i2c_init.I2C_ClockSpeed = 1000;
    i2c_init.I2C_Mode = I2C_Mode_I2C;
    i2c_init.I2C_DutyCycle = I2C_DutyCycle_2;
//    i2c_init.I2C_OwnAddress1 = 0x30;
    i2c_init.I2C_OwnAddress1 = (3 << 1);
    i2c_init.I2C_Ack = I2C_Ack_Enable;
    i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &i2c_init);

    I2C_StretchClockCmd(I2C2, ENABLE);
    I2C_Cmd(I2C2, ENABLE);


    //�������������� �������������

    I2C_GeneralCallCmd(I2C2, ENABLE);//��������� ������ ������ = ���

}

void __attribute__((optimize("O0"))) I2C2_ER_IRQHandler(void)
{
        /* Read SR1 register to get I2C error */
    if ((I2C_ReadRegister(I2C2, I2C_Register_SR1 ) & 0xFF00) != 0x00)
    {
//            STM_EVAL_LEDOn(LED6);
        /* Clears error flags */
        I2C2 ->SR1 &= 0x00FF;
    }
}
uint8_t _bufTx[20] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
uint8_t _iBufTx = 0;
void __attribute__((optimize("O0"))) I2C2_EV_IRQHandler(void)
{
    uint8_t dataRX;
    uint32_t Event = I2C_GetLastEvent(I2C2 );
    uint32_t valSR1 = I2C2->SR1;
    uint32_t valSR2 = I2C2->SR2;
    asm("nop");

	//����� ������.
    //����������� ��� ������� ������
	if((I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED & Event) == I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)
	{
		//��� ������ ����� ADDR
		I2C2 ->SR1;
		I2C2 ->SR2;

		I2C_SendData(I2C2, _bufTx[0]);
	}

	//���� ������
	if((I2C_EVENT_SLAVE_BYTE_RECEIVED & Event) == I2C_EVENT_SLAVE_BYTE_RECEIVED)
	{
		//������ ��������� ����
		dataRX = I2C_ReceiveData(I2C2 );
	}

	//������ ������������
	if((I2C_EVENT_SLAVE_ACK_FAILURE & Event) == I2C_EVENT_SLAVE_ACK_FAILURE)
	{
		I2C2 ->SR1 &= 0x00FF;
	}

	//���������� ���� �������
	if((I2C_EVENT_SLAVE_STOP_DETECTED & Event) == I2C_EVENT_SLAVE_STOP_DETECTED)
	{
		I2C2 ->SR1;
		I2C2 ->CR1 |= I2C_CR1_PE;//��������� ����
	}

	//����� ������ ������ ������
	if((I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED & Event) == I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED)
	{
		//��� ������ ����� ADDR
	//			I2C2 ->SR1;
	//			I2C2 ->SR2;

	}


	if((I2C_EVENT_SLAVE_BYTE_TRANSMITTED & Event) == I2C_EVENT_SLAVE_BYTE_TRANSMITTED)
	{
		if(_iBufTx > 10)
			_iBufTx = 0;
		else
			_iBufTx++;
		I2C_SendData(I2C2, _bufTx[_iBufTx]);
	}



	//SR1
//	if(valSR1 == I2C_IT_TIMEOUT)
//		asm("nop");
//	if((valSR1 & I2C_IT_TXE) == I2C_IT_TXE)
//	{
//		asm("nop");
//	}

}

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  int i = 0;

  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used. 
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates 
  *  SCB->VTOR register.  
  *  E.g.  SCB->VTOR = 0x20000000;  
  */

  /* TODO - Add your application code here */
  i2c_init2();
//  I2C2 ->CR1 |= I2C_CR1_SWRST;//�������� �����
  /* Infinite loop */
  while (1)
  {
	i++;
  }
}
