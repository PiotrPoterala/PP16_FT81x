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

	loadGraphics(tft);
}

void loadGraphics(PTFTgraph *tft){

	unsigned int firstFreeAdress=tft->loadIcon(&tft->data, &escapeBitmapHeader, RAM_G);

	
}

void displayCircleIconButton(PTFTgraph *tft, PXY position, sBitmapHeader *bitmapHeader, unsigned int color, unsigned int tag){

		(tag==tft->touchTag(&tft->data))?tft->drawCircleIconButton(&tft->data, position, 60, bitmapHeader, tag, color, COLOR_TEXT):tft->drawCircleIconButton(&tft->data, position, 35, bitmapHeader, tag, color, COLOR_TEXT);
}



unsigned int menuScreen(PTFTgraph *tft){
		#define MSCBUTTON_H				45
		#define MSCBUTTON_W				145
		#define MSCBUTTON_DIST_X		150
		#define MSCBUTTON_FONT			29

		unsigned int curTag=tft->touchTag(&tft->data);
	
		PXY startCorner={25,25};
		PXY endCorner={FT_DispWidth-25,FT_DispHeight-25};

		tft->writePrimitive(&tft->data, CMD_DLSTART);
		tft->writePrimitive(&tft->data, CLEAR_COLOR_HEX(COLOR_BG_LIGHT));//określenie koloru zamalowania całego ekranu (tutaj: szary #555)
		tft->writePrimitive(&tft->data, CLEAR(1,0,1));	//zamalowanie wszystkich składowych (kolorem określonym powyżej) poszczególnych pikseli
		tft->writePrimitive(&tft->data, VERTEX_FORMAT(0));
		
		tft->drawRectangular(&tft->data, startCorner, endCorner, WHITE, 64);
		
		
		tft->writeString(&tft->data, (PXY){(FT_DispWidth/2), 160}, 30, OPT_CENTERX, "Examples:", COLOR_BG_DARK);

		
		tft->drawButton(&tft->data, endCorner, MSCBUTTON_W, MSCBUTTON_H, MSCBUTTON_FONT, (curTag==TAG_FIRST_BUTTON)?OPT_FLAT:0, "Primitives", TAG_FIRST_BUTTON, COLOR_TEXT, ORANGE_LIGHT, ORANGE_LIGHT);	
		tft->drawButton(&tft->data, endCorner, MSCBUTTON_W, MSCBUTTON_H, MSCBUTTON_FONT, (curTag==TAG_SECOND_BUTTON)?OPT_FLAT:0, "Graphics objects", TAG_SECOND_BUTTON, COLOR_TEXT, ORANGE_LIGHT, ORANGE_LIGHT);

	
		tft->writePrimitive(&tft->data, DISPLAY());
		tft->writePrimitive(&tft->data, CMD_SWAP);

		tft->waitToCMDbuffEmpty(&tft->data);

		return curTag;
}


unsigned int primitivesScreen(PTFTgraph *tft){

		unsigned int curTag=tft->touchTag(&tft->data);
	
		PXY startCorner={25,25};
		PXY endCorner={FT_DispWidth-25,FT_DispHeight-25};

		tft->writePrimitive(&tft->data, CMD_DLSTART);
		tft->writePrimitive(&tft->data, CLEAR_COLOR_HEX(COLOR_BG_LIGHT));//określenie koloru zamalowania całego ekranu (tutaj: szary #555)
		tft->writePrimitive(&tft->data, CLEAR(1,0,1));	//zamalowanie wszystkich składowych (kolorem określonym powyżej) poszczególnych pikseli
		tft->writePrimitive(&tft->data, VERTEX_FORMAT(0));
		
		tft->drawRectangular(&tft->data, (PXY){25,25}, (PXY){125,50}, WHITE, 64);
		
		tft->drawLine(&tft->data, (PXY){25,75}, (PXY){125,125}, WHITE, 64);
		
		tft->drawPoint(&tft->data, (PXY){50,125}, WHITE, 20);
	
		PXY lineStripTab[3]={{25, 150},{50, 175},{75, 150}};
		tft->drawLineStrip(&tft->data, lineStripTab, sizeof(lineStripTab)/sizeof(PXY), WHITE, LINE_STRIP);
		
		PXY lineStripTab2[3]={{25, 150},{50, 175},{75, 150}};
		tft->drawLineStrip(&tft->data, lineStripTab2, sizeof(lineStripTab2)/sizeof(PXY), WHITE, EDGE_STRIP_B);
		
		displayCircleIconButton(tft, (PXY){20, 440}, &escapeBitmapHeader, ORANGE_LIGHT, TAG_ESCAPE);
		
		tft->writePrimitive(&tft->data, DISPLAY());
		tft->writePrimitive(&tft->data, CMD_SWAP);

		tft->waitToCMDbuffEmpty(&tft->data);

		return curTag;
}