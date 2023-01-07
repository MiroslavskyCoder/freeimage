 
#include "FreeImage.h"
#include "Utilities.h"

void tiff_ConvertLineXYZToRGB(BYTE *target, BYTE *source, double stonits, int width_in_pixels) {
	FIRGBF *rgbf = (FIRGBF*)target;
	float *xyz = (float*)source;
	
	for (int cols = 0; cols < width_in_pixels; cols++) { 
		rgbf->red	= (float)( 2.690*xyz[0] + -1.276*xyz[1] + -0.414*xyz[2]);
		rgbf->green	= (float)(-1.022*xyz[0] +  1.978*xyz[1] +  0.044*xyz[2]);
		rgbf->blue	= (float)( 0.061*xyz[0] + -0.224*xyz[1] +  1.163*xyz[2]);
		
		rgbf++;
		xyz += 3;
	}
}

void tiff_ConvertLineRGBToXYZ(BYTE *target, BYTE *source, int width_in_pixels) {
	FIRGBF *rgbf = (FIRGBF*)source;
	float *xyz = (float*)target;
	
	for (int cols = 0; cols < width_in_pixels; cols++) { 
		xyz[0] =  (float)(0.497*rgbf->red +  0.339*rgbf->green +  0.164*rgbf->blue);
		xyz[1] =  (float)(0.256*rgbf->red +  0.678*rgbf->green +  0.066*rgbf->blue);
		xyz[2] =  (float)(0.023*rgbf->red +  0.113*rgbf->green +  0.864*rgbf->blue);

		rgbf++;
		xyz += 3;
	}
}

