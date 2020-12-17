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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define	DEBUG_VIEW_FREQ_IRQ
//#define	DEBUG_ANY_MANUAL_SIGNAL
#define	NORMAL_MODE
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define	DEBUG_TEST_READ_TEXT_FROM_SD
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t curBufIdx = 0;
uint8_t wavBuf[2][WAV_BUF_SIZE];
uint16_t curBufOffset = 0;
uint8_t wavReadFlag = 0;
uint32_t wavDataSize = 0;
uint32_t curWavIdx = 0;
uint8_t stopFlag = 0;

FATFS fileSystem;
FIL audioFile;
uint32_t readBytes = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef	DEBUG_TEST_READ_TEXT_FROM_SD
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SD_Path[4];  /* SD logical drive path */
#endif
uint16_t _val;
uint16_t _cnt;
#define STEP_VAL	1
#define MAX_VAL		0xFFF
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int __attribute__((optimize("O0"))) main(void)
{
  /* USER CODE BEGIN 1 */
	_val = 0;
	int8_t _flag = 0;
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
  MX_GPIO_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_DAC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
#ifdef	NORMAL_MODE
  FRESULT res;
  FRESULT resTryMount;
  FRESULT resTryOpenFile;
  FRESULT resTryReadFile;
  resTryMount = f_mount(&fileSystem, SDPath, 1);
  uint8_t path[10] = "audio.wav";
//  uint8_t path[10] = "audis.wav";
//  uint8_t path[10] = "audil.wav";
  resTryOpenFile = f_open(&audioFile, (char*)path, FA_READ);
  //#2
  //Таким образом, находим позицию в буфере, которая соответствует символу ‘d’ и прибавляем к этому значению 8 (4 байта для ‘data’ и 4 байта для размера данных):
  uint16_t dataOffset = 0;
  resTryReadFile = f_read(&audioFile, wavBuf[0], WAV_BUF_SIZE, &readBytes);
  for (uint16_t i = 0; i < (WAV_BUF_SIZE - 3); i++)
  {
      if ((wavBuf[0][i] == 'd') && (wavBuf[0][i + 1] == 'a') &&
          (wavBuf[0][i + 2] == 't') && (wavBuf[0][i + 3] == 'a'))
      {
          dataOffset = i + 8;
          break;
      }
  }

  //#3
  //Заголовок обнаружен, перемещаем указатель FatFs для работы с файлом на аудио-данные и заодно определяем количество байт данных. Для этого вычитаем из общего размера файла размер заголовка:
  res = f_lseek(&audioFile, dataOffset);
  wavDataSize = f_size(&audioFile) - dataOffset;//[байт] длина полной последовательности аудио данных
  //#3
  //нужно быстро выводить данные на ЦАП. для этого сделаем буферизацию.
  //в два буфера wavBuf[0] и wavBuf[1] будем пихать массив данных из файла
  //когда выводим на ЦАП буфер 1 заполняем буфер 2 и наоборот
//  Реализуем этот механизм и, первым делом, заполняем оба буфера данными:
    res = f_read(&audioFile, wavBuf[0], WAV_BUF_SIZE, &readBytes);
    res = f_read(&audioFile, wavBuf[1], WAV_BUF_SIZE, &readBytes);
    //#5
#endif
//  Поскольку данные готовы, спокойно включаем DAC и TIM6 на генерацию прерываний:
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
    HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



#ifdef	DEBUG_TEST_READ_TEXT_FROM_SD
  //код проверки чтения SD карты
    uint8_t* bw;
    FRESULT res;
  res = f_mount(&SDFatFs, (TCHAR const*)SD_Path, 1);
	 if(res != FR_OK)
	    Error_Handler();
	  else
	  {
		  uint8_t arr[] = {0, 1, 2};
		  uint8_t arrrd[20];

		  res = f_open(&MyFile, "adr.txt", FA_READ|FA_WRITE);
		  res = f_read(&MyFile, arrrd, 10, bw);
		  res = f_write(&MyFile, arr, 3, bw);
//          if(f_open(&MyFile, ".\adr.txt", FA_READ) != FR_OK)
//                    Error_Handler();
//            else
//            {
//                          f_close(&MyFile);
//            }
	  }
#endif


  while (1)
  {
#ifdef	DEBUG_ANY_MANUAL_SIGNAL
//	  HAL_Delay(10);

//		if(_cnt < 2)
//			_cnt++;
//		else
//			_cnt = 0;
//
//		if(_cnt == 0)
//		{
			if(_flag == 0)
			{
				if(_val < MAX_VAL)
					_val = _val + STEP_VAL;
				else
					_flag = 1;

			}
			else
			{
				if(_val > 0)
					_val = _val - STEP_VAL;
				else
					_flag = 0;
			}

//		}
#endif
#ifdef	NORMAL_MODE
	  if (wavReadFlag == 1)
	  {
//		  все данные из буфера озвучены. буфер полошел к концу. его нужно обновить"

		  //если сейчас озвучивается буфер 1 то заполняется буфер 2, и наоборот
	      uint8_t readBufIdx = 0;
	      if (curBufIdx == 0)
	          readBufIdx = 1;
	      res = f_read(&audioFile, wavBuf[readBufIdx], WAV_BUF_SIZE, &readBytes);
	      wavReadFlag = 0;
	  }
	  //#
	  //Кроме того, сразу же в while(1) помещаем код, который отвечает за окончание воспроизведения, когда файл подошел к концу:
	  if (stopFlag == 1)
	  {
		  //просто закрываем файл
	      res = f_close(&audioFile);
	      stopFlag = 0;
	  }
#endif

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */
  /** DAC Initialization 
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config 
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 99;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
uint16_t _dacData;

void __attribute__((optimize("O0"))) UserTIM6IRQHandler(void)
{
#ifdef	DEBUG_VIEW_FREQ_IRQ
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0xFFFF);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
#endif
#ifdef	DEBUG_ANY_MANUAL_SIGNAL

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, _val);
#endif
#ifdef	NORMAL_MODE

    _dacData = (((wavBuf[curBufIdx][curBufOffset + 1] << 8) | wavBuf[curBufIdx][curBufOffset]) + 32767);
    _dacData /= 16;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, _dacData);

    //на каждом такте используем забираем и озвучиваем 2 байта из массива аудио данных
    curBufOffset += 2;//номер текущего байта в буфере, этот счетчик, соответственно, будет изменяться от 0 до 512.
    curWavIdx += 2;//общий счетчик байт wav-файла. Его мы используем, чтобы обнаружить конец трека и остановить воспроизведение.

    if (curWavIdx >= wavDataSize)
    {
    	//все данные озвучены, останавливаем процесс
        HAL_TIM_Base_Stop_IT(&htim6);
        stopFlag = 1;
    }
    else
    {
    	//#
    	//Если конец файла еще не достигнут, то проверяем счетчик curBufOffset. Если значение равно 512 (WAV_BUF_SIZE), то буфер подошел к концу, а значит надо изменить номер активного буфера и выставить флаг wavReadFlag:

        if (curBufOffset == WAV_BUF_SIZE)
        {
            curBufOffset = 0;

            //смена текущего рабочего буфера из которого нужно озвучивать
            if (curBufIdx == 0)
                curBufIdx = 1;
            else
                curBufIdx = 0;

            wavReadFlag = 1;//флаг = "все данные из буфера озвучены. буфер полошел к концу. его нужно обновить"
        }
    }

#endif
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
