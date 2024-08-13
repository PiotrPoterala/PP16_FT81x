#ifndef _PP16_CONFIG_H
	#define _PP16_CONFIG_H
//	#include "stm32.h"

	#define BASE_FREQUENCY_OF_SYS_TICK	10

	#define BASE_FREQUENCY_OF_TIM8	1

	#define BASE_FREQUENCY_OF_TIM5	10 

	


	#define NVIC_PriGroup_0         ((uint32_t)0x7) /*!< 0 bits for pre-emption priority
																															4 bits for subpriority */
	#define NVIC_PriGroup_1         ((uint32_t)0x6) /*!< 1 bits for pre-emption priority
																															3 bits for subpriority */
	#define NVIC_PriGroup_2         ((uint32_t)0x5) /*!< 2 bits for pre-emption priority
																															2 bits for subpriority */
	#define NVIC_PriGroup_3         ((uint32_t)0x4) /*!< 3 bits for pre-emption priority
																															1 bits for subpriority */
	#define NVIC_PriGroup_4         ((uint32_t)0x3) /*!< 4 bits for pre-emption priority
																															0 bits for subpriority */

	#define RCC_PLLCFGR_PLLM_DIV2		(1<<RCC_PLLCFGR_PLLM_Pos)
	#define RCC_PLLCFGR_PLLN_MUL30		(30<<RCC_PLLCFGR_PLLN_Pos)
	#define RCC_PLLCFGR_PLLN_MUL40		(40<<RCC_PLLCFGR_PLLN_Pos)
	#define RCC_PLLCFGR_PLLR_DIV2			(0<<RCC_PLLCFGR_PLLR_Pos)

	#define RCC_PLLSAI1CFGR_PLLSAI1M_DIV2		(1<<RCC_PLLSAI1CFGR_PLLSAI1M_Pos)
	#define RCC_PLLSAI1CFGR_PLLSAI1N_MUL15	(15<<RCC_PLLSAI1CFGR_PLLSAI1N_Pos)
	#define RCC_PLLSAI1CFGR_PLLSAI1Q_DIV2		(0<<RCC_PLLSAI1CFGR_PLLSAI1Q_Pos)

	#define GPIO_AFR_AF0				0x0
	#define GPIO_AFR_AF1				0x1
	#define GPIO_AFR_AF2				0x2
	#define GPIO_AFR_AF3				0x3
	#define GPIO_AFR_AF4				0x4
	#define GPIO_AFR_AF5				0x5
	#define GPIO_AFR_AF6				0x6
	#define GPIO_AFR_AF7				0x7
	#define GPIO_AFR_AF8				0x8
	#define GPIO_AFR_AF9				0x9
	#define GPIO_AFR_AF10				0xA
	#define GPIO_AFR_AF11				0xB
	#define GPIO_AFR_AF12				0xC
	#define GPIO_AFR_AF13				0xD
	#define GPIO_AFR_AF14				0xE
	#define GPIO_AFR_AF15				0xF
        

	#define PVD_IRQ_LEV_PROTECT      	1	
	#define IRQ_DECL_PROTECT(prv)  uint32_t prv
	#define IRQ_PROTECT(prv, lvl) (prv = __get_BASEPRI(), __set_BASEPRI((lvl) << (8 - __NVIC_PRIO_BITS)))		//ustawienie priorytetu powyżej którego przerwania nie są blokowane
	#define IRQ_UNPROTECT(prv)    __set_BASEPRI(prv)


	void RCC_Config(void);
	void NVIC_Config(void);
	void GPIO_Config(void);
	void TIM_Config(void);
	void OCTOSPI_Config(void);

#endif
