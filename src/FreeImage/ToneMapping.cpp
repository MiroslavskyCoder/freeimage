 
#include "FreeImage.h"
#include "Utilities.h"
 
FIBITMAP * DLL_CALLCONV
FreeImage_ToneMapping(FIBITMAP *dib, FREE_IMAGE_TMO tmo, double first_param, double second_param) {
	if(FreeImage_HasPixels(dib)) {
		switch(tmo) { 
			case FITMO_DRAGO03:
				if((first_param == 0) && (second_param == 0)) { 
					return FreeImage_TmoDrago03(dib, 2.2, 0);
				} else { 
					return FreeImage_TmoDrago03(dib, first_param, second_param);
				}
				break; 
			case FITMO_REINHARD05:
				if((first_param == 0) && (second_param == 0)) { 
					return FreeImage_TmoReinhard05(dib, 0, 0);
				} else { 
					return FreeImage_TmoReinhard05(dib, first_param, second_param);
				}
				break; 
			case FITMO_FATTAL02:
				if((first_param == 0) && (second_param == 0)) { 
					return FreeImage_TmoFattal02(dib, 0.5, 0.85);
				} else { 
					return FreeImage_TmoFattal02(dib, first_param, second_param);
				}
				break;
		}
	}

	return NULL;
}


