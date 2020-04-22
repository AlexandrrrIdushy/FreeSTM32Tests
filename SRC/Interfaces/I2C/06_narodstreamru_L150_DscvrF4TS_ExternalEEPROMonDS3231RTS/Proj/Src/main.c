/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define LED1_ON() LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12)//1
#define LED1_OFF() LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12)

#define LED2_ON() LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13)
#define LED2_OFF() LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13)

#define LED3_ON() LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14)
#define LED3_OFF() LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14)

#define LED4_ON() LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_15)
#define LED4_OFF() LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_15)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ALL_SEG_OFF() LL_GPIO_SetOutputPin(SEG_PORT,SH|SG|SF|SE|SD|SC|SB|SA);
#define I2C_REQUEST_WRITE                       0x00
#define I2C_REQUEST_READ                        0x01
#define SLAVE_OWN_ADDRESS                       0x66	//0x66 это 51 сдвинута€ влево

extern uint16_t num_gl;////
uint8_t rd_value[20] = {0};
uint8_t wr_value[20] = {0x14,0x13,0x12,0x11,0x10,//22
                        0x0F,0x0E,0x0D,0x0C,0x0B,
                        0x0A,0x09,0x08,0x07,0x06,
                        0x05,0x04,0x03,0x02,0x01};

//подготовка к старту и старт условие
void  __attribute__((optimize("O0"))) AT24C_PrepStart ()
{
	  LL_I2C_DisableBitPOS(I2C1);//стандартна€ схема ACK - у текущего байта
	  LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);//включаем генерацию подтверждени€ ACK
	  LL_I2C_GenerateStartCondition(I2C1);//генерируем старт условие
	  while(!LL_I2C_IsActiveFlag_SB(I2C1)){};//ƒождЄмс€ установки бита SB в регистре SR1. ƒанный бит устанавливаетс€ в случае наличие на шине услови€ START
	  //read state
	  (void) I2C1->SR1;//	  —читаем регистр SR1

}


void  __attribute__((optimize("O0"))) AT24C_ReadBytes (uint16_t addr, uint8_t *buf, uint16_t bytes_count)
{  uint16_t i;
	AT24C_PrepStart();

  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_READ);//адрес ведомого отправл€ем в шину
  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};
  LL_I2C_ClearFlag_ADDR(I2C1);
  for(i=0;i<bytes_count;i++)
  {
    if(i<(bytes_count-1))
    {
      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
      buf[i] = LL_I2C_ReceiveData8(I2C1);
    }
    else
    {
      LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
      LL_I2C_GenerateStopCondition(I2C1);
      while(!LL_I2C_IsActiveFlag_RXNE(I2C1)){};
      buf[i] = LL_I2C_ReceiveData8(I2C1);
    }
  }

  asm("nop");
}


void __attribute__((optimize("O0"))) AT24C_WriteBytes (uint16_t addrInMem,uint8_t *buf, uint16_t bytes_count)
{
	  uint16_t i;
	  AT24C_PrepStart();

	  LL_I2C_TransmitData8(I2C1, SLAVE_OWN_ADDRESS | I2C_REQUEST_WRITE);//отправим адрес устройства в шину
	  while(!LL_I2C_IsActiveFlag_ADDR(I2C1)){};//дождЄмс€ установки бита ADDR в регистре SR1, что будет означать окончание передачи адреса SLAVE в шину
	  LL_I2C_ClearFlag_ADDR(I2C1);//ќчистим флаг ADDR, что делаетс€ посредством считывани€ регистров SR1 и SR2

	  //передаем адрес в пам€ти сначала H затем L часть. тут мы их заносим в DR но не передаем реально
//	  LL_I2C_TransmitData8(I2C1, (uint8_t) (addrInMem>>8));//H
//	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?
//	  LL_I2C_TransmitData8(I2C1, (uint8_t) addrInMem);//L
//	  while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};//данные ушли?

	  //передаем данные дл€ записи EEPROM
	  for(i=0;i<bytes_count;i++)
	  {
	    LL_I2C_TransmitData8(I2C1, buf[i]);
	    while(!LL_I2C_IsActiveFlag_TXE(I2C1)){};
	  }

	  LL_I2C_GenerateStopCondition(I2C1);
	  asm("nop");
}

void __attribute__((optimize("O0"))) AT24C_SlaveReceiveBytes()
{
	uint32_t res = 0;
	uint8_t rd = 0;
	//генерировать ACK
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

	//совпал адрес?
	while(res = LL_I2C_IsActiveFlag_ADDR(I2C1)){}

//	res = LL_I2C_IsActiveFlag_STOP(I2C1);
//	while(!res){}

	rd = LL_I2C_ReceiveData8(I2C1);

//	//#1 зафиксировать старт условие
//	LL_I2C_DisableBitPOS(I2C1);//стандартна€ схема ACK - у текущего байта
//	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);//включаем генерацию подтверждени€ ACK
	asm("nop");
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	LED1_ON();LED1_OFF();
	LED2_ON();LED2_OFF();
	LED3_ON();LED3_OFF();
	LED4_ON();LED4_OFF();
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
//  LL_TIM_EnableIT_UPDATE(TIM2);
  LL_TIM_EnableCounter(TIM2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  AT24C_WriteBytes (0x004A, wr_value, 20);
//	  AT24C_ReadBytes (0x004A, rd_value, 20);
	  AT24C_SlaveReceiveBytes();
//	  LL_mDelay(1000);
//	if (!(LL_GPIO_ReadInputPort(GPIOA)&LL_GPIO_PIN_0))
//	{
//		LED1_ON();
//		LL_mDelay(100);
//		LED1_OFF();
//		LED2_ON();
//		LL_mDelay(100);
//		LED2_OFF();
//		LED3_ON();
//		LL_mDelay(100);
//		LED3_OFF();
//		LED4_ON();
//		LL_mDelay(100);
//		LED4_OFF();
//	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(16000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(16000000);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration  
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  /** I2C Initialization 
  */
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 2000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 102;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 200;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 3000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_UPDATE);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
