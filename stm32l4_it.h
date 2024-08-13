

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _STM32L4_IT_H
#define _STM32L4_IT_H


#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void HardFault_Handler(void);
void SysTick_Handler(void);

void TIM5_IRQHandler(void);
void TIM8_UP_IRQHandler(void);
void OCTOSPI1_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
