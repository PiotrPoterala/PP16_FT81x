#include "stm32l4_it.h"
#include "pp_tftgraph_ft8xx.h"
#include "touch_screen_tags.h"
#include "platform_io_signals.h"
#include "set_screen_tft.h"
#include "config.h"

extern PExtRam extramFT8xx;
extern defOScreen oScreen;
extern PTFTgraph tft;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{

  while (1)
  {
  }
}


void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


void SysTick_Handler(void)
{

}




void TIM8_UP_IRQHandler(void){
	
		if((TIM8->DIER & TIM_DIER_UIE) && (TIM8->SR & TIM_SR_UIF)){
			TIM8->SR&=~TIM_SR_UIF;

			PIN_TOG(LED_PORT, LED1);
				
		}

}


void OCTOSPI1_IRQHandler(void){
	
	extramFT8xx.portListen(&extramFT8xx.data);
}



void TIM5_IRQHandler(void)
{	

	static unsigned int touchScreenTag=NO_TAG, prevTouchScreenTag=NO_TAG;
	
	if((TIM5->DIER & TIM_DIER_UIE) && (TIM5->SR & TIM_SR_UIF )){
		TIM5->SR &= ~TIM_SR_UIF;	
		
				
			if(touchScreenTag==NO_TAG && prevTouchScreenTag){
				if(prevTouchScreenTag==TAG_ESCAPE){
					oScreen.idScreen=MENU_SCREEN;
				}else if(prevTouchScreenTag==TAG_FIRST_BUTTON){
					oScreen.idScreen=PRIMITIVES_SCREEN;
				}else if(prevTouchScreenTag==TAG_SECOND_BUTTON){
					oScreen.idScreen=GRAPHICS_OBJECTS_SCREEN;
				}
			}
			
			
			prevTouchScreenTag=touchScreenTag;
			
			

			switch(oScreen.idScreen){
				case MENU_SCREEN:
					touchScreenTag=oScreen.menuScreen(&tft);
					break;
				case PRIMITIVES_SCREEN:
					touchScreenTag=oScreen.primitivesScreen(&tft);
					break;
				case GRAPHICS_OBJECTS_SCREEN:
					touchScreenTag=oScreen.graphicsObjectsScreen(&tft);
					break;
								
			}

	}
	
}



