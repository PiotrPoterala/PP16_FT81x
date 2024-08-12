#include "stm32f4_it.h"
#include "it_subroutines.h"
#include "pp_serial_port.h"
#include "platform_io_signals.h"
#include "systems_data.h"

#include "diskio.h"
//#include "usb_hcd_int.h"
#include "pp_usb_msc_host.h"
#include "pp_extram_quadspi.h"



#include "pp_math.h"	
#include "pp_file.h"

#include "it_subroutines.h"
#include "driveEDM.h"
#include "backup.h"
#include "pp_lcdgraph.h"
//#include "set_screen.h"
#include "data_input.h"
#include "platform_io_signals.h"
#include "pp_program_resolver_strategy.h"
#include "at_tags.h"
#include "systems_data.h"
#include "machine_config.h"
#include "pp_alert_dialog.h"
#include "pp_motorslist.h"
#include "pp_encoderslist.h"
#include "configurations.h"

#include "strings.h"
#include "establish_step_speed.h"

#include "pp_semaphore.h"
#include "pp_queue.h"
#include "pp_serial_port.h"
#include "pp_at_command_interpreter.h"
#include "pp_edm_par_device.h"
#include "pp_wifi.h"
#include "pp_togpin.h"
#include "pp_queue_frames.h"
#include "pp_digital_safe_read.h"

#include "touch_screen_tags.h"

#include "pp_wire_head_driver.h"
#include "pp_select_item_in_list_commands.h"
#include "pp_paramlist_commands.h"

extern PTFTgraph tft;
extern defOScreen oScreen;
extern defOMenu oMenu;
extern PAlertDialog dialog;
extern PClock oClockRTC;
extern PTime auxTime;
extern PDate auxDate;
extern PDigitalSafeRead oCheckOthersSignals;
extern PPprogramResolverStrategy progResolverStrategy;
extern PSelectList oAutWorkFunctions;
extern PSelectList oFeatures;
extern PConstMotors2clockList stepMotors;
extern PConstParamsList workParams;

extern PConstParamsList auxCoord;
extern const unsigned int noOfGBase;
extern PConstParamsList gBasePoints[];

extern PConstCharList entryList;
extern PConstLineEditList editLinesList;

extern PTouchScreenKeyboard touchScreenKeyboard;

extern PSelectList oLanguage;
extern PSelectList oAutWorkFunctions;
extern PSelectList oEquipment;
extern PSelectList oFeatures;
extern PSelectList oCurrentBlock;
extern PSelectList oTypeOfMotor;
extern PSelectList oStepPrecision;
extern PSelectList oTypeOfInterpretProg;
extern SEffParams effPar;
extern SElectrode electrode;
extern PConstEffParamsList effParamsList;
extern defOMachineConfig oMachineConfig;
extern PPdrive pDrive;
extern PPDrivePath pAutPathDrive;

extern defODataInputNewGprog oDataNewGprog;;

extern PConstCharList progList;
extern PFileInfo progFileInfo;
extern PFileInfo dispFileInfo;

extern PStringVector fileRaws;
extern PMutex fileRawsReserve;
extern PFile fileToDisplay;

extern EDMparDevice parDevice;

extern PSerialPort commSerialPort;
extern PSerialPort commUSBBserialPort;
extern PSerialPort commExtSerialPort;

extern defOSystemsData oSystemsData;
extern PusbMscHostDevice usbMscHost;
extern HCD_HandleTypeDef hhcd_USB_OTG_FS;

extern PExtRam extramFT8xx;

extern PQueueWithMutex qCommand;

extern PConstMotors2clockList stepMotors;

extern PDigitalSafeRead wireHeadLevarPosSignals;
extern PDCmotDriver wireFeedMotor;
extern PDCmotDriver wireTensionMotor;
extern PPwireHead wireHead;

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
	//HAL_IncTick();
}


void PVD_PVM_IRQHandler(void){

	if(EXTI->PR1 & Pin16_Mask){

		PVDsubroutine();				

		EXTI->PR1=Pin16_Mask;

	}
}

void OTG_FS_IRQHandler(void){

	//	USBH_OTG_ISR_Handler(&USB_OTG_Core);
	 HAL_HCD_IRQHandler(&hhcd_USB_OTG_FS);

}


void TIM8_UP_IRQHandler(void){
	static unsigned int i=0;
	
		if((TIM8->DIER & TIM_DIER_UIE) && (TIM8->SR & TIM_SR_UIF)){
			TIM8->SR&=~TIM_SR_UIF;
			i++;
			
			signalsSubroutine();
			
			#if defined(WEDM)
			wireHeadLevarPosSignals.timerService(&wireHeadLevarPosSignals.data);
			
			wireHead.data.wireFeedMotor->timerServicePWMpin(&wireHead.data.wireFeedMotor->data);
			wireHead.data.wireTensionMotor->timerServicePWMpin(&wireHead.data.wireTensionMotor->data);
			wireHead.timerServiceBrokenWireDetect(&wireHead.data);
			wireHead.levarPositioning(&wireHead.data, workParams.getParamValue(&workParams.data, 'N'));
			#endif
			
			
			if(!(i%(BASE_FREQUENCY_OF_TIM8/200))){
				usbMscHost.process();
			}
			
			if(!(i%BASE_FREQUENCY_OF_TIM8)){
				if(oSystemsData.timeOfWorkClockActive)oSystemsData.oTimeOfWork.addSecs(&oSystemsData.oTimeOfWork.data, 1);
				if(oSystemsData.overallWorkingTimeClockActive)oSystemsData.oOverallWorkingTime.addSecs(&oSystemsData.oOverallWorkingTime.data, 1);
				oSystemsData.oTurnOnTime.addSecs(&oSystemsData.oTurnOnTime.data, 1);
				PIN_TOG(LED_PORT, LED1);
				
				
				for(int i=0; i<stepMotors.data.size; i++){
					stepMotors.data.driver[i].timerServiceSelCurrentPin(&stepMotors.data.driver[i].behaveData);
				}
				
				
			}
			
			if(!(i%(BASE_FREQUENCY_OF_TIM8/2))){
				oScreen.cursorVisible=(oScreen.cursorVisible)?false:true;
			}

			if(i>=0xFFFFFFF0)i=0;
		}

}


void OCTOSPI1_IRQHandler(void){
	
	extramFT8xx.portListen(&extramFT8xx.data);
}


void TIM1_UP_TIM16_IRQHandler(void){

	encoderSubroutine(TIM1);
}


void TIM2_IRQHandler(void){
	
	driveAutPathSubroutine(TIM2, BASE_FREQUENCY_OF_TIM2);

}


void TIM3_IRQHandler(void){	
	
	handDriveSubroutine(TIM3, BASE_FREQUENCY_OF_TIM3);

}

void TIM6_DAC_IRQHandler(void){	
		
	centeringSubroutine(TIM6, BASE_FREQUENCY_OF_TIM6);
	
}

void TIM4_IRQHandler(void)
{
	driveSubroutine(TIM4, BASE_FREQUENCY_OF_TIM4);

}


void TIM7_IRQHandler(void){

	drivePathSubroutine(TIM7);

}


void TIM5_IRQHandler(void)
{	
	static unsigned int i=0;
	static bool waitScreenDisplay=false;
	static unsigned int touchScreenTag=NO_TAG, prevTouchScreenTag=NO_TAG;
	static bool sendHoldTag=false;
	
	if((TIM5->DIER & TIM_DIER_UIE) && (TIM5->SR & TIM_SR_UIF )){	//jeżeli jest włączone przerwanie od przepełnienia timera	i timer się przepełnił to wykonaj podprogram
	//	TIM5->CR1 &= ~TIM_CR1_CEN;
		TIM5->SR &= ~TIM_SR_UIF;		//wyzerowanie flagi przerwania
		
				
				if(touchScreenTag!=NO_TAG && sendHoldTag==false){
					enqueueBasicFrame(&qCommand, touchScreenTag, qMARK_PRESS_KEY);
					sendHoldTag=true;
				}else if(touchScreenTag==NO_TAG && prevTouchScreenTag){
					bool interpretButton=true;
					
					sendHoldTag=false;
					
					if(prevTouchScreenTag==KEY_IZR){	
						oAutWorkFunctions.item^=fIZR;
					}else if(oScreen.idScreen==idHAND_WORK_SCREEN){
						bool changePar=false;
						
						if(prevTouchScreenTag==HWS_TAG_HAND_JOG){
							oScreen.openTab=(oScreen.openTab==FIRST_TAB)?NO_TAB:FIRST_TAB;
						}else if(prevTouchScreenTag==HWS_TAG_TOOLS){
							oScreen.openTab=(oScreen.openTab==SECOND_TAB)?NO_TAB:SECOND_TAB;
						}else if(prevTouchScreenTag==HWS_TAG_PARAMS){
							oScreen.openTab=(oScreen.openTab==THIRD_TAB)?NO_TAB:THIRD_TAB;
						}else if(oScreen.openTab==SECOND_TAB && (prevTouchScreenTag>=HWS_TAG_ITEM1 && prevTouchScreenTag<HWS_TAG_ITEM1+parDevice.data.tools->nrOfItems)){
							if(parDevice.data.tools->item & (1<<((prevTouchScreenTag-HWS_TAG_ITEM1)))){
								enqueueBasicFrame(parDevice.data.commQueue, prevTouchScreenTag-HWS_TAG_ITEM1+parDevice.data.tools->nrOfItems+1, 0);
								toolOffCommand(1<<((prevTouchScreenTag-HWS_TAG_ITEM1)));
							}else{
								enqueueBasicFrame(parDevice.data.commQueue, prevTouchScreenTag-HWS_TAG_ITEM1+1, 0);
								toolOnCommand(1<<((prevTouchScreenTag-HWS_TAG_ITEM1)));

							}		
						}else if(oScreen.openTab==THIRD_TAB && (prevTouchScreenTag>=HWS_TAG_ITEM1 && prevTouchScreenTag<HWS_TAG_ITEM1+(2*workParams.data.size))){	
							
							oScreen.cursorY.setCursor(&oScreen.cursorY.data, ((prevTouchScreenTag-HWS_TAG_ITEM1)/2)+1);
							
							changePar=incDecWorkParamsAndRunAddAction(oScreen.cursorY.data.cursor-1, (prevTouchScreenTag-HWS_TAG_ITEM1)%2);
							
						}else{
							interpretButton=false;
						}
						
						if(changePar)enqueueParamsListFrame(parDevice.data.commQueue, &workParams, TAG_SEND_PAR, 0);
						
					}else if(oScreen.idScreen==idAUT_WORK_SCREEN){
						bool changePar=false;
						
						if(prevTouchScreenTag==AWS_TAG_A){	
							oAutWorkFunctions.item^=fAUT_ROZ;
						}else if(prevTouchScreenTag==AWS_TAG_VIBRATION){
							oAutWorkFunctions.item^=fWIB;
						}else if(prevTouchScreenTag==AWS_TAG_HAND_JOG){
							oScreen.openTab=(oScreen.openTab==FIRST_TAB)?NO_TAB:FIRST_TAB;
						}else if(prevTouchScreenTag==AWS_TAG_TOOLS){
							oScreen.openTab=(oScreen.openTab==SECOND_TAB)?NO_TAB:SECOND_TAB;
						}else if(prevTouchScreenTag==AWS_TAG_PARAMS){
							oScreen.openTab=(oScreen.openTab==THIRD_TAB)?NO_TAB:THIRD_TAB;
						}else if(prevTouchScreenTag==AWS_TAG_SPEED_PARAM){	
							changePar=setMostEffParamsBaseElWorkSurface(&workParams, &effPar, &electrode, oMachineConfig.workSurfaceMM2);
						}else if(prevTouchScreenTag==AWS_TAG_ROUGH_PARAM){	
							changePar=setParamsBaseRoughness(&workParams, &effPar, &electrode, oMachineConfig.desiredRaNM, oMachineConfig.workSurfaceMM2);
						}else if(oScreen.openTab==SECOND_TAB && (prevTouchScreenTag>=AWS_TAG_ITEM1 && prevTouchScreenTag<AWS_TAG_ITEM1+parDevice.data.tools->nrOfItems)){
							if(parDevice.data.tools->item & (1<<((prevTouchScreenTag-AWS_TAG_ITEM1)))){
								enqueueBasicFrame(parDevice.data.commQueue, prevTouchScreenTag-AWS_TAG_ITEM1+parDevice.data.tools->nrOfItems+1, 0);
								toolOffCommand(1<<((prevTouchScreenTag-AWS_TAG_ITEM1)));
							}else{
								enqueueBasicFrame(parDevice.data.commQueue, prevTouchScreenTag-AWS_TAG_ITEM1+1, 0);
								toolOnCommand(1<<((prevTouchScreenTag-AWS_TAG_ITEM1)));

							}								
							
							
						}else if(oScreen.openTab==THIRD_TAB && (prevTouchScreenTag>=AWS_TAG_ITEM1 && prevTouchScreenTag<AWS_TAG_ITEM1+(2*workParams.data.size))){	
							
							oScreen.cursorY.setCursor(&oScreen.cursorY.data, ((prevTouchScreenTag-AWS_TAG_ITEM1)/2)+1);
							
							changePar=incDecWorkParamsAndRunAddAction(oScreen.cursorY.data.cursor-1, (prevTouchScreenTag-AWS_TAG_ITEM1)%2);			
							
						}else{
							interpretButton=false;
						}
						
						if(changePar)enqueueParamsListFrame(parDevice.data.commQueue, &workParams, TAG_SEND_PAR, 0);
						
					}else if(oScreen.idScreen==idFILE_VIEW_SCREEN){
						if(prevTouchScreenTag==TFS_TAG_TEXTEDIT && oScreen.markEntryField==MARK_TEXT_EDIT){
							prevTouchScreenTag=KEY_ESC;
						}
						interpretButton=false;
					}else{
						interpretButton=false;
					}
					
					if(interpretButton==false){
						enqueueBasicFrame(&qCommand, prevTouchScreenTag, qMARK_SCREEN_TAG);
					}
				}
			
			
			prevTouchScreenTag=touchScreenTag;
			
			if(progResolverStrategy.interpretProgParam.interpret && waitScreenDisplay==false){
				oScreen.displayWaitScreen(&tft);
				waitScreenDisplay=true;
			}else if(progResolverStrategy.interpretProgParam.interpret==false && waitScreenDisplay==true){
				waitScreenDisplay=false;
			}
			
			
			if(waitScreenDisplay==false){
				if(oScreen.previousIdScreen!=oScreen.idScreen){
					oScreen.previousIdScreen=oScreen.idScreen;
					oScreen.markEntryField=0;
					oScreen.cursorX.reset(&oScreen.cursorX.data);
					oScreen.cursorY.reset(&oScreen.cursorY.data);
					oScreen.openTab=NO_TAB;
				}
			
	
				switch(oScreen.idScreen){
					case idHAND_WORK_SCREEN:
						if(oScreen.openTab==NO_TAB){
							oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, oMenu.menuParam.nrOfMenu);
						}else if(oScreen.openTab==SECOND_TAB){
							oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, parDevice.data.tools->nrOfItems);
						}else if(oScreen.openTab==THIRD_TAB){
							oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, workParams.data.size);
						}
						touchScreenTag=oScreen.displayHandWorkScreen(&tft, &stepMotors, &oMenu.menuParam, &oScreen.cursorY, &oClockRTC, oScreen.openTab, &oFeatures, &oCheckOthersSignals, &usbMscHost, &oAutWorkFunctions, &workParams, parDevice.data.tools);
						break;
					case idDRIVE_SCREEN:
						touchScreenTag=oScreen.displayDriveScreen(&tft, &stepMotors, &oMenu.menuParam, &oClockRTC, &oFeatures,	&oCheckOthersSignals, &usbMscHost, &oAutWorkFunctions, &workParams);
						break;
					case idSTEP_DRIVE_SCREEN:
						touchScreenTag=oScreen.displayStepDriveScreen(&tft, &stepMotors, &oMenu.menuParam, &oClockRTC, &oAutWorkFunctions, pDrive.stepDrive);
						break;
					case idAUT_WORK_SCREEN:		
						if(oScreen.openTab==SECOND_TAB){
							oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, parDevice.data.tools->nrOfItems);
						}else{
							oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, workParams.data.size);
						}
						touchScreenTag=displayAutWorkScreen(&tft, &stepMotors, &oMenu.menuParam, &oSystemsData.oTimeOfWork, &oAutWorkFunctions, &oFeatures, 
																	&oCheckOthersSignals, &usbMscHost, &workParams, parDevice.data.tools, &progFileInfo, 
																	&oScreen.cursorY, &oMachineConfig, getRoughnessNMBaseParams(&workParams, &effPar), &oClockRTC, pAutPathDrive.infoDrive, oScreen.openTab);
						break;
					case idINPUT_SCREEN:
						oScreen.cursorY.setMaxScrollElem(&oScreen.cursorY.data, auxCoord.data.size);
						touchScreenTag=oScreen.displayInputParamScreen(&tft, &auxCoord, &oMenu.menuParam, entryList.data.names[0], oScreen.markEntryField, &oAutWorkFunctions, &oScreen.cursorX, oScreen.cursorVisible, &oScreen.cursorY);
						break;
					case idLIST_BASE_COORD:
						touchScreenTag=oScreen.displayListBaseCoordScreen(&tft, gBasePoints, noOfGBase, &oMenu.menuParam, &oScreen.cursorY);
						break;
					case idLIST_PROG:
						touchScreenTag=oScreen.displayListProgScreen(&tft, &oMenu.menuParam, &progList.data, &progFileInfo, &oScreen.cursorY);
						break;
					case idCHANGE_TIME:
						touchScreenTag=oScreen.displayChangeTimeScreen(&tft, &auxTime, &auxDate, &oMenu.menuParam, oScreen.cursorX.data.cursor);
						break;
					case idLANGUAGE:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oLanguage, &oScreen.cursorY);
						break;
					case idEQUIPMENT:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oEquipment, &oScreen.cursorY);
						break;
					case idSELECT_CURRENT_BLOCK:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oCurrentBlock, &oScreen.cursorY);
						break;
					case idSELECT_TYPE_OF_MOTOR:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oTypeOfMotor, &oScreen.cursorY);
						break;
					case idSELECT_LINEARS_MOTORS_PRECISION:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oStepPrecision, &oScreen.cursorY);
						break;
					case idSELECT_TYPE_OF_PROGRAM:
						touchScreenTag=oScreen.displaySelectListScreen(&tft, &oMenu.menuParam, &oTypeOfInterpretProg, &oScreen.cursorY);
						break;
					case idFEATURES:
						touchScreenTag=oScreen.displayFeaturesScreen(&tft, &oMenu.menuParam, &oFeatures, &oScreen.cursorY);
						break;
					case idFILE_VIEW_SCREEN:
						if(fileRawsReserve.tryTake(fileRawsReserve.available)){
							fileRawsReserve.take(&fileRawsReserve.available);
							{
								dispFileInfo.setFile(&dispFileInfo.data, &fileToDisplay);
								touchScreenTag=oScreen.displayTextFileScreen(&tft,&touchScreenKeyboard.data, &oMenu.menuParam, &fileRaws, &oScreen.cursorX, &oScreen.cursorY, &oScreen.textEditCursorX, &oScreen.textEditCursorY, &dispFileInfo, oScreen.markEntryField, oScreen.cursorVisible, &oDataNewGprog.paramNewGprog);
							}
							fileRawsReserve.give(&fileRawsReserve.available);		
						}
						break;
					case idALERT_DIALOG_SCREEN:
						touchScreenTag=oScreen.displayAlertDialogScreen(&tft, &dialog.data);
						break;
					case idLIST_EFF_PAR:
						touchScreenTag=oScreen.displayMaterialListScreen(&tft, &oMenu.menuParam, &effParamsList.data, &effPar, &oScreen.cursorY);
						break;
					case idINFO:
						touchScreenTag=oScreen.displayInfoScreen(&tft, &oSystemsData, oMenu.menuParam.header);
						break;
					case idGET_FILE_NAME:
					case idINPUT_LIST:
						touchScreenTag=oScreen.displayInputListScreen(&touchScreenKeyboard.data, &tft, &oMenu.menuParam, &editLinesList.data, oScreen.markEntryField, &oScreen.cursorX, oScreen.cursorVisible, &oScreen.cursorY);
						break;
				}
				
				touchScreenTag=convertScreenTagToKeys(oScreen.idScreen, touchScreenTag);
			}
//		TIM5->CR1 |= TIM_CR1_CEN;
	}
	
}

void UART4_IRQHandler(void){

	commSerialPort.portListen(&commSerialPort.data);

}


void USART1_IRQHandler(void){
	
	commUSBBserialPort.portListen(&commUSBBserialPort.data);

}


void USART3_IRQHandler(void){
	
	commExtSerialPort.portListen(&commExtSerialPort.data);

}

