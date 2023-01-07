 
#include "FreeImage.h"
#include "Utilities.h"
 
static int s_format_id;
 
#define JNG_SIGNATURE_SIZE 8	// size of the signature
 
FIBITMAP* mng_ReadChunks(int format_id, FreeImageIO *io, fi_handle handle, long Offset, int flags = 0);
BOOL mng_WriteJNG(int format_id, FreeImageIO *io, FIBITMAP *dib, fi_handle handle, int flags);
 
static const char * DLL_CALLCONV
Format() {
	return "JNG";
}

static const char * DLL_CALLCONV
Description() {
	return "JPEG Network Graphics";
}

static const char * DLL_CALLCONV
Extension() {
	return "jng";
}

static const char * DLL_CALLCONV
RegExpr() {
	return NULL;
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/x-mng";
}

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
	BYTE jng_signature[8] = { 139, 74, 78, 71, 13, 10, 26, 10 };
	BYTE signature[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	io->read_proc(&signature, 1, JNG_SIGNATURE_SIZE, handle);

	return (memcmp(jng_signature, signature, JNG_SIGNATURE_SIZE) == 0) ? TRUE : FALSE;
}

static BOOL DLL_CALLCONV
SupportsExportDepth(int depth) {
	return (
			(depth == 8) ||
			(depth == 24) ||
			(depth == 32)
		);
}

static BOOL DLL_CALLCONV 
SupportsExportType(FREE_IMAGE_TYPE type) {
	return (type == FIT_BITMAP) ? TRUE : FALSE;
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

static void * DLL_CALLCONV
Open(FreeImageIO *io, fi_handle handle, BOOL read) {
	return NULL;
}

static void DLL_CALLCONV
Close(FreeImageIO *io, fi_handle handle, void *data) {
}

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
	long offset = JNG_SIGNATURE_SIZE;	// move to skip first 8 bytes of signature

	// check the signature (8 bytes)
	if(Validate(io, handle) == FALSE) {
		return NULL;
	}
	
	// parse chunks and decode a jng or mng bitmap
	return mng_ReadChunks(s_format_id, io, handle, offset, flags);
}

static BOOL DLL_CALLCONV
Save(FreeImageIO *io, FIBITMAP *dib, fi_handle handle, int page, int flags, void *data) {

	return mng_WriteJNG(s_format_id, io, dib, handle, flags);
}

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitJNG(Plugin *plugin, int format_id) {
	s_format_id = format_id;

	plugin->format_proc = Format;
	plugin->description_proc = Description;
	plugin->extension_proc = Extension;
	plugin->regexpr_proc = RegExpr;
	plugin->open_proc = Open;
	plugin->close_proc = Close;
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
