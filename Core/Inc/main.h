/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOC
#define I_SENSE_VDD_Pin GPIO_PIN_0
#define I_SENSE_VDD_GPIO_Port GPIOA
#define I_SENSE_3V3_Pin GPIO_PIN_1
#define I_SENSE_3V3_GPIO_Port GPIOA
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define LIGHT_BARRIER_LED_Pin GPIO_PIN_4
#define LIGHT_BARRIER_LED_GPIO_Port GPIOA
#define HAL_SIG_GRID_Pin GPIO_PIN_5
#define HAL_SIG_GRID_GPIO_Port GPIOA
#define LIGHT_BARRIER_YR_Pin GPIO_PIN_6
#define LIGHT_BARRIER_YR_GPIO_Port GPIOA
#define LIGHT_BARRIER_YB_Pin GPIO_PIN_7
#define LIGHT_BARRIER_YB_GPIO_Port GPIOA
#define STPR_EN_MAG_R_Pin GPIO_PIN_0
#define STPR_EN_MAG_R_GPIO_Port GPIOB
#define STPR_DIR_MAG_R_Pin GPIO_PIN_1
#define STPR_DIR_MAG_R_GPIO_Port GPIOB
#define STPR_STP_MAG_R_Pin GPIO_PIN_2
#define STPR_STP_MAG_R_GPIO_Port GPIOB
#define STPR_DIR_GRID_Pin GPIO_PIN_10
#define STPR_DIR_GRID_GPIO_Port GPIOB
#define U_SENSE_VDD_Pin GPIO_PIN_11
#define U_SENSE_VDD_GPIO_Port GPIOB
#define BTN_E_STOP_Pin GPIO_PIN_12
#define BTN_E_STOP_GPIO_Port GPIOB
#define BTN_START_Pin GPIO_PIN_13
#define BTN_START_GPIO_Port GPIOB
#define STATUS_LED_Pin GPIO_PIN_9
#define STATUS_LED_GPIO_Port GPIOC
#define STPR_EN_LIFT_Pin GPIO_PIN_8
#define STPR_EN_LIFT_GPIO_Port GPIOA
#define STPR_DIR_LIFT_Pin GPIO_PIN_9
#define STPR_DIR_LIFT_GPIO_Port GPIOA
#define STPR_STP_LIFT_Pin GPIO_PIN_10
#define STPR_STP_LIFT_GPIO_Port GPIOA
#define LIMIT_LIFT_Pin GPIO_PIN_11
#define LIMIT_LIFT_GPIO_Port GPIOA
#define STPR_STP_GRID_Pin GPIO_PIN_12
#define STPR_STP_GRID_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define STPR_EN_MAG_Y_Pin GPIO_PIN_15
#define STPR_EN_MAG_Y_GPIO_Port GPIOA
#define STPR_MS1_Pin GPIO_PIN_10
#define STPR_MS1_GPIO_Port GPIOC
#define STPR_MS2_Pin GPIO_PIN_11
#define STPR_MS2_GPIO_Port GPIOC
#define STPR_DIR_MAG_Y_Pin GPIO_PIN_4
#define STPR_DIR_MAG_Y_GPIO_Port GPIOB
#define STPR_STP_MAG_Y_Pin GPIO_PIN_5
#define STPR_STP_MAG_Y_GPIO_Port GPIOB
#define STPR_EN_MAG_B_Pin GPIO_PIN_6
#define STPR_EN_MAG_B_GPIO_Port GPIOB
#define STPR_DIR_MAG_B_Pin GPIO_PIN_7
#define STPR_DIR_MAG_B_GPIO_Port GPIOB
#define STPR_STP_MAG_B_Pin GPIO_PIN_8
#define STPR_STP_MAG_B_GPIO_Port GPIOB
#define STPR_EN_GRID_Pin GPIO_PIN_9
#define STPR_EN_GRID_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
