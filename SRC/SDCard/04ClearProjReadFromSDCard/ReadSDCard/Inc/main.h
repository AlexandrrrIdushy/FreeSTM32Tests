/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED5_Pin GPIO_PIN_13
#define LED5_GPIO_Port GPIOC
#define GSMADC_Pin GPIO_PIN_2
#define GSMADC_GPIO_Port GPIOC
#define PWR_Reserve_CTR_Pin GPIO_PIN_3
#define PWR_Reserve_CTR_GPIO_Port GPIOC
#define PWR_CTR_Pin GPIO_PIN_0
#define PWR_CTR_GPIO_Port GPIOA
#define BATT_CTR_Pin GPIO_PIN_1
#define BATT_CTR_GPIO_Port GPIOA
#define DIN1_Pin GPIO_PIN_6
#define DIN1_GPIO_Port GPIOA
#define DIN2_Pin GPIO_PIN_7
#define DIN2_GPIO_Port GPIOA
#define DIN4_Pin GPIO_PIN_4
#define DIN4_GPIO_Port GPIOC
#define DIN5_Pin GPIO_PIN_5
#define DIN5_GPIO_Port GPIOC
#define DIN3_Pin GPIO_PIN_0
#define DIN3_GPIO_Port GPIOB
#define GSM_PWK_Pin GPIO_PIN_1
#define GSM_PWK_GPIO_Port GPIOB
#define GSM_SEL_Pin GPIO_PIN_2
#define GSM_SEL_GPIO_Port GPIOB
#define RS485_DI_TX_Pin GPIO_PIN_10
#define RS485_DI_TX_GPIO_Port GPIOB
#define RS485_DI_RX_Pin GPIO_PIN_11
#define RS485_DI_RX_GPIO_Port GPIOB
#define SPI_NSS_Pin GPIO_PIN_12
#define SPI_NSS_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOA
#define ButtonCTRL_Pin GPIO_PIN_3
#define ButtonCTRL_GPIO_Port GPIOB
#define ButtonCLBR_Pin GPIO_PIN_4
#define ButtonCLBR_GPIO_Port GPIOB
#define TR_485_Pin GPIO_PIN_5
#define TR_485_GPIO_Port GPIOB
#define USB_Renum_Pin GPIO_PIN_6
#define USB_Renum_GPIO_Port GPIOB
#define GSM_RI_Pin GPIO_PIN_7
#define GSM_RI_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_8
#define Relay_GPIO_Port GPIOB
#define OD_OUT_Pin GPIO_PIN_9
#define OD_OUT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
