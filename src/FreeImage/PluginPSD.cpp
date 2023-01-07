 
#include "FreeImage.h"
#include "Utilities.h"
#include "PSDParser.h"
 
static int s_format_id;
 
static const char * DLL_CALLCONV
Format() {
	return "PSD";
}

static const char * DLL_CALLCONV
Description() {
	return "Adobe Photoshop";
}

static const char * DLL_CALLCONV
Extension() {
	return "psd,psb";
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/vnd.adobe.photoshop";
}

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
	BYTE psd_id[] = { 0x38, 0x42, 0x50, 0x53 };
	BYTE signature[4] = { 0, 0, 0, 0 };

	io->read_proc(signature, 1, 4, handle);

	if(memcmp(psd_id, signature, 4) == 0)
		return TRUE;

	return FALSE;
}

static BOOL DLL_CALLCONV
SupportsExportDepth(int depth) {
	return (
			(depth == 1)  ||
			(depth == 8)  ||
			(depth == 24) ||
			(depth == 32)
		);
}

static BOOL DLL_CALLCONV
SupportsExportType(FREE_IMAGE_TYPE type) {
	return (
		(type == FIT_BITMAP)  ||
		(type == FIT_UINT16)  ||
		(type == FIT_INT16)   ||
		(type == FIT_FLOAT)   ||
		(type == FIT_RGB16)   ||
		(type == FIT_RGBA16)  ||
		(type == FIT_RGBF)    ||
		(type == FIT_RGBAF)
	);
}

static BOOL DLL_CALLCONV
SupportsICCProfiles() {
	return TRUE;
}

static BOOL DLL_CALLCONV
SupportsNoPixels() {
	return TRUE;
} 

// ----------------------------------------------------------

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
	if(!handle) {
		return NULL;
	}
	try {
		psdParser parser;

		FIBITMAP *dib = parser.Load(io, handle, s_format_id, flags);
		
		return dib;

	} catch(const char *text) {
		FreeImage_OutputMessageProc(s_format_id, text);
		return NULL;
	}
}

static BOOL DLL_CALLCONV
Save(FreeImageIO *io, FIBITMAP *dib, fi_handle handle, int page, int flags, void *data) {
	if(!handle) {
		return NULL;
	}
	try {
		psdParser parser;

		bool b = parser.Save(io, dib, handle, page, flags, data);
		
		return b;

	} catch(const char *text) {
		FreeImage_OutputMessageProc(s_format_id, text);
		return FALSE;
	}
}

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitPSD(Plugin *plugin, int format_id) {
	s_format_id = format_id;

	plugin->format_proc = Format;
	plugin->description_proc = Description;
	plugin->extension_proc = Extension;
	plugin->regexpr_proc = NULL;
	plugin->open_proc = NULL;
	plugin->close_proc = NULL;
	plugin->pagecount_proc = NULL;
	plugin->pagecapability_proc = NULL;
	plugin->load_proc = Load;
	plugin->save_proc = Save;
	plugin->validate_proc = Validate;
	plugin->mime_proc = MimeType;
	plugin->supports_export_bpp_proc = SupportsExportDepth;
	plugin->supports_export_type_proc = SupportsExportType;
	plugin->supports_icc_profiles_proc = SupportsICCProfiles;
	plugin->supports_no_pixels_proc = SupportsNoPixels; 
}
