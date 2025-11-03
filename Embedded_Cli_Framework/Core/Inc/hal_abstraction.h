/*
 * File: hal_abstraction.h
 * Description: Hardware Abstraction Layer interface header file.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_ABSTRACTION_H
#define HAL_ABSTRACTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

void Init_lib(void);
void SystemClock_Config(void);
void GPIO_Init(void);
void USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_ABSTRACTION_H */
