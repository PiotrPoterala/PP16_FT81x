#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "set_screen_tft.h"
#include "touch_screen_tags.h"
#include "colors.h"

static const char escape[] ={120,156,237,210,65,10,0,32,8,68,209,110,238,209,45,41,104,149,140,232,162,98,126,251,103,70,173,177,191,82,81,41,81,172,164,180,148,81,141,146,185,15,149,167,20,181,127,236,31,72,1,58,58,193,156,189,2,91,249,239,135,76,195,162,68,105,75,89,103,74,121,133,221,85,7,3,19,100,142,0};
sBitmapHeader escapeBitmapHeader ={ARGB1555, 36, 36, 72, 80,escape, 0};


void OScreenConstruct(defOScreen *oScreen, PTFTgraph *tft){

	oScreen->idScreen=MENU_SCREEN;
	oScreen->menuScreen=menuScreen;
	oScreen->primitivesScreen=primitivesScreen;
	oScreen->graphicsObjectsScreen=graphicsObjectsScreen;

	loadGraphics(tft);
}

void loadGraphics(PTFTgraph *tft){

	unsigned int firstFreeAdress=tft->loadIcon(&tft->data, &escapeBitmapHeader, RAM_G);

	
}

void displayCircleIconButton(PTFTgraph *tft, PXY position, sBitmapHeader *bitmapHeader, unsigned int color, unsigned int tag){

		(tag==tft->touchTag(&tft->data))?tft->drawCircleIconButton(&tft->data, position, 60, bitmapHeader, tag, color, COLOR_TEXT):tft->drawCircleIconButton(&tft->data, position, 35, bitmapHeader, tag, color, COLOR_TEXT);
}



unsigned int menuScreen(PTFTgraph *tft){

		#define MENU_BUTTON_WIDTH			200
		#define MENU_BUTTON_FONT			29
	
		#define MSC_MARGIN	25
		#define MSC_CONTENT_MARGIN	25
	
		unsigned int buttonHeight=tft->fontHeight(&tft->data, MENU_BUTTON_FONT)+30;									
		unsigned int distLine=buttonHeight+10;

		unsigned int curTag=tft->touchTag(&tft->data);
	
		PXY startCorner={MSC_MARGIN,MSC_MARGIN};
		PXY endCorner={FT_DispWidth-MSC_MARGIN,FT_DispHeight-MSC_MARGIN};

		tft->writePrimitive(&tft->data, CMD_DLSTART);
		tft->writePrimitive(&tft->data, CLEAR_COLOR_HEX(COLOR_BG_LIGHT));//określenie koloru zamalowania całego ekranu (tutaj: szary #555)
		tft->writePrimitive(&tft->data, CLEAR(1,0,1));	//zamalowanie wszystkich składowych (kolorem określonym powyżej) poszczególnych pikseli
		tft->writePrimitive(&tft->data, VERTEX_FORMAT(0));
		
		tft->drawRectangular(&tft->data, startCorner, endCorner, WHITE, 64);
		
		
		tft->writeString(&tft->data, (PXY){(FT_DispWidth/2), startCorner.y+MSC_CONTENT_MARGIN}, 30, OPT_CENTERX, "Examples", COLOR_BG_DARK);
		tft->drawLine(&tft->data, (PXY){((FT_DispWidth-MENU_BUTTON_WIDTH)/2),startCorner.y+75}, (PXY){((FT_DispWidth+MENU_BUTTON_WIDTH)/2),startCorner.y+75}, BLACK, 64);

		PXY buttonPos={((FT_DispWidth-MENU_BUTTON_WIDTH)/2), startCorner.y+100};
		tft->drawButton(&tft->data, buttonPos, MENU_BUTTON_WIDTH, buttonHeight, MENU_BUTTON_FONT, (curTag==TAG_FIRST_BUTTON)?OPT_FLAT:0, "Primitives", TAG_FIRST_BUTTON, COLOR_TEXT, ORANGE_LIGHT, ORANGE_LIGHT);	
		buttonPos.y+=distLine;
		tft->drawButton(&tft->data, buttonPos, MENU_BUTTON_WIDTH, buttonHeight, MENU_BUTTON_FONT, (curTag==TAG_SECOND_BUTTON)?OPT_FLAT:0, "Graphics objects", TAG_SECOND_BUTTON, COLOR_TEXT, ORANGE_LIGHT, ORANGE_LIGHT);

	
		tft->writePrimitive(&tft->data, DISPLAY());
		tft->writePrimitive(&tft->data, CMD_SWAP);

		tft->waitToCMDbuffEmpty(&tft->data);

		return curTag;
}


unsigned int primitivesScreen(PTFTgraph *tft){

		tft->writePrimitive(&tft->data, CMD_DLSTART);
		tft->writePrimitive(&tft->data, CLEAR_COLOR_HEX(BLACK));
		tft->writePrimitive(&tft->data, CLEAR(1,0,1));	
		tft->writePrimitive(&tft->data, VERTEX_FORMAT(0));
		
		tft->drawRectangular(&tft->data, (PXY){25,25}, (PXY){125,50}, WHITE, 64);
		
		tft->drawLine(&tft->data, (PXY){25,75}, (PXY){125,100}, WHITE, 64);
		
		tft->drawPoint(&tft->data, (PXY){225,250}, GRAY_VERY_LIGHT, 30);
	
		PXY lineStripTab[5]={{25, 125},{50, 150},{75, 125},{100, 150},{125, 125}};
		tft->drawLineStrip(&tft->data, lineStripTab, sizeof(lineStripTab)/sizeof(PXY), WHITE, LINE_STRIP, 32);
		
		PXY lineStripTab2[3]={{200, 200},{225, 175},{250, 200}};
		tft->drawLineStrip(&tft->data, lineStripTab2, sizeof(lineStripTab2)/sizeof(PXY), WHITE, EDGE_STRIP_A, 16);
		
		PXY lineStripTab3[3]={{200, 300},{225, 325},{250, 300}};
		tft->drawLineStrip(&tft->data, lineStripTab3, sizeof(lineStripTab2)/sizeof(PXY), WHITE, EDGE_STRIP_B, 16);
		
		PXY lineStripTab4[3]={{275, 225},{300, 250},{275, 275}};
		tft->drawLineStrip(&tft->data, lineStripTab4, sizeof(lineStripTab2)/sizeof(PXY), WHITE, EDGE_STRIP_R, 16);
		
		PXY lineStripTab5[3]={{175, 225},{150, 250},{175, 275}};
		tft->drawLineStrip(&tft->data, lineStripTab5, sizeof(lineStripTab2)/sizeof(PXY), GRAY_LIGHT, EDGE_STRIP_L, 16);
		
		displayCircleIconButton(tft, (PXY){20, 440}, &escapeBitmapHeader, ORANGE_LIGHT, TAG_ESCAPE);
		
		tft->writePrimitive(&tft->data, DISPLAY());
		tft->writePrimitive(&tft->data, CMD_SWAP);

		tft->waitToCMDbuffEmpty(&tft->data);

		return tft->touchTag(&tft->data);
}

unsigned int graphicsObjectsScreen(PTFTgraph *tft){
		#define TEXT_FONT			30				
	
		unsigned int buttonHeight=tft->fontHeight(&tft->data, MENU_BUTTON_FONT)+30;									
		unsigned int distLine=buttonHeight+10;
		unsigned int curTag=tft->touchTag(&tft->data);

		tft->writePrimitive(&tft->data, CMD_DLSTART);
		tft->writePrimitive(&tft->data, CLEAR_COLOR_HEX(COLOR_BG_LIGHT));//określenie koloru zamalowania całego ekranu (tutaj: szary #555)
		tft->writePrimitive(&tft->data, CLEAR(1,0,1));	//zamalowanie wszystkich składowych (kolorem określonym powyżej) poszczególnych pikseli
		tft->writePrimitive(&tft->data, VERTEX_FORMAT(0));
		
		PXY pos={25, 25};
	
		tft->writeString(&tft->data, pos, 29, 0, "Hello", WHITE);
//		tft->writeFromString(&tft->data, (PXY){50, 25}, 29, 0, "Hello", 3, COLOR_BG_DARK);
		pos.y+=distLine;
		tft->writeNumber(&tft->data, pos, 29, 0, 3, WHITE);
		tft->drawClock(&tft->data, (PXY){FT_DispWidth-75, 75}, 50, OPT_NOBACK, 1, 35, 10, 0, WHITE, 0);
//		void drawButton(PTFTgraphFT8XXData *data, PXY position, unsigned int w, unsigned int h, unsigned int font, unsigned int options, const char* str, unsigned int tag, unsigned int color, unsigned int fgColor, unsigned int gradColor);
//	
//		void drawProgress(PTFTgraphFT8XXData *data, PXY position, unsigned int w, unsigned int h, unsigned int options, unsigned int val, unsigned int range, unsigned int color, unsigned int bgColor);
		pos.y+=distLine;
		tft->drawScrollbar(&tft->data, pos, 400, 30, 0, 50, 30, 100, NO_TAG, COLOR_TEXT, COLOR_BG_DARK);		
		pos.y+=distLine;
		tft->drawKeys(&tft->data, pos, 11*buttonHeight, buttonHeight, TEXT_FONT, curTag, "qwertyuiop_", COLOR_TEXT, ORANGE_DARK, ORANGE_LIGHT);
		pos.y+=2*distLine;
		tft->drawToggle(&tft->data, pos, 60, TEXT_FONT, 0, 0, "off" "\xff" "on", NO_TAG, COLOR_TEXT, ORANGE_DARK, COLOR_BG_DARK);
		tft->drawSpinner(&tft->data, (PXY){FT_DispWidth/2, FT_DispHeight/2}, 1, 0, COLOR_TEXT);

	
		displayCircleIconButton(tft, (PXY){20, 440}, &escapeBitmapHeader, ORANGE_LIGHT, TAG_ESCAPE);
	
		tft->writePrimitive(&tft->data, DISPLAY());
		tft->writePrimitive(&tft->data, CMD_SWAP);

		tft->waitToCMDbuffEmpty(&tft->data);

		return curTag;
}