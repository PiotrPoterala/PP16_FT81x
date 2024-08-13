#ifndef _SET_SCREEN_TFT_H
	#define _SET_SCREEN_TFT_H

	#include "pp_tftgraph_ft8xx.h"

enum{
	MENU_SCREEN=0,
	PRIMITIVES_SCREEN
};


	typedef struct{
			unsigned int idScreen;

			unsigned int (*menuScreen)(PTFTgraph *tft);
			unsigned int (*primitivesScreen)(PTFTgraph *tft);
	}defOScreen;


	void OScreenConstruct(defOScreen *oScreen, PTFTgraph *tft);

	void loadGraphics(PTFTgraph *tft);
	void displayCircleIconButton(PTFTgraph *tft, PXY position, sBitmapHeader *bitmapHeader, unsigned int color, unsigned int tag);
	
	unsigned int menuScreen(PTFTgraph *tft);
	unsigned int primitivesScreen(PTFTgraph *tft);
#endif
