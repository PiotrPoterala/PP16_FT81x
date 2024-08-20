#include <stdio.h>	
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pp_math.h"
#include "config.h"

#include "pp_tftgraph_ft8xx.h"
#include "set_screen_tft.h"

defOScreen oScreen;
PExtRam extramFT8xx;
PTFTgraph tft;




int main(void){
	
		RCC_Config();
		NVIC_Config();
		GPIO_Config();
		TIM_Config();
		OCTOSPI_Config();
		
		volatile int commExtRamFT8xxReceiveTab[256];
		PQueueWithTimeout qCommExtRamFT8xxReceive;
		PQueueWithTimeoutConstruct(&qCommExtRamFT8xxReceive, commExtRamFT8xxReceiveTab, sizeof(commExtRamFT8xxReceiveTab)/sizeof(int));
		volatile int commExtRamFT8xxSendTab[128];
		PQueueWithTimeout qCommExtRamFT8xxSend;
		PQueueWithTimeoutConstruct(&qCommExtRamFT8xxSend, commExtRamFT8xxSendTab, sizeof(commExtRamFT8xxSendTab)/sizeof(int));
		
		PExtRamConstruct(&extramFT8xx, FT8XX, 0x0, 795647, &qCommExtRamFT8xxReceive, &qCommExtRamFT8xxSend);
		extramFT8xx.open(&extramFT8xx.data, ReadWrite);
		
		PTFTgraphFT8XXConstruct(&tft, &extramFT8xx);
		tft.init(&tft.data);


		if(SysTick_Config(SystemCoreClock/BASE_FREQUENCY_OF_SYS_TICK)){while(1);}	
		

		

		tft.drawLogo(&tft.data);	
		OScreenConstruct(&oScreen, &tft);	
		
		TIM8->DIER |= TIM_DIER_UIE;					
		TIM8->CR1 |= TIM_CR1_CEN ;	
		
		TIM5->DIER |= TIM_DIER_UIE;					
		TIM5->CR1 |= TIM_CR1_CEN ;	

		
while(1){
	
	}
}



