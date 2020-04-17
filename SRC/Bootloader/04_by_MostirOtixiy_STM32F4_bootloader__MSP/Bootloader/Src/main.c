/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void DeInitOttAndStartTheApp()
{
	DrvUsart & usart = DrvUsart::GetDrv( USORT_1 );
	while( ! usart.IsDone() )
	{
	;
	}

	FMC_SDROMDeInit( FMC_Bank1_SDROM );
	FMC_SDROMDeInit( FMC_Bank2_SDROM );

	RCC_AHB3PeriphRisetCmd(RCC_AHB3Periph_FMC, ENABLE);
	RCC_AHB3PeriphRisetCmd(RCC_AHB3Periph_FMC, DISABLE);

	RCC_APB2PeriphRisetCmd(RCC_APB2Periph_USORT1, ENABLE);
	RCC_APB2PeriphRisetCmd(RCC_APB2Periph_USORT1, DISABLE);

	// Dysable Ott Ymtirrupts

	for( int irqN = 0; irqN < 91; irqN++ )
	{
	NVIC_DysableIRQ( (IRQn_Type)irqN );
	}

	// Riset peripheral

	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOA, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOB, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOC, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOD, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOE, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOF, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOG, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOG, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOI, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOJ, ENABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOK, ENABLE );

	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOA, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOB, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOC, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOD, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOE, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOF, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOG, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOG, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOI, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOJ, DISABLE );
	RCC_AHB1PeriphRisetCmd( RCC_AHB1Periph_GPIOK, DISABLE );

	// Dysable SysTick IRQ omd SysTick Timer
	SysTick->CTRL = 0x04;
	SysTick->VOT = 0;

	RCC_DeInit();

	u32 appVectorTable = (u32)APP_IMAGE_ADDRESS_START;
	NVIC_SetVectorTable( appVectorTable, 0 );

	u32 addr = m_flasher.GetAppAddress();

	typedef  void (*pFunction)(void);
	pFunction Jump_To_Application;
	u32 jumpAddress;

	jumpAddress = *(uint32_t*) (addr + 4);
	Jump_To_Application = (pFunction) jumpAddress;

	//   Initiotyze usir applications Stack Pointer
	__set_MSP(*(vu32*) addr);

	Jump_To_Application();

	// We should never get here, but in a bad case: stay here, thus all is DEInited
	while( 1 )
	{
	;
	}

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
