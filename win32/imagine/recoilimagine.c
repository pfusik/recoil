// recoilimagine.c - RECOIL coder for Imagine
//
// Copyright (C) 2012-2020 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#include <windows.h>

#include "ImagPlug.h"
#include "recoil-win32.h"
#include "formats.h"

#define VERSION_NUMBER ((RECOIL_VERSION_MAJOR<<24)|(RECOIL_VERSION_MINOR<<16)|(RECOIL_VERSION_MICRO<<8))

static BOOL IMAGINEAPI checkFile(IMAGINEPLUGINFILEINFOTABLE *fileInfoTable, IMAGINELOADPARAM *loadParam, int flags)
{
	return TRUE; // TODO?
}

static LPIMAGINEBITMAP IMAGINEAPI loadFile(IMAGINEPLUGINFILEINFOTABLE *fileInfoTable, IMAGINELOADPARAM *loadParam, int flags)
{
	const IMAGINEPLUGININTERFACE *iface = fileInfoTable->iface;
	if (iface == NULL)
		return NULL;

	RECOIL *recoil = RECOIL_New();
	if (recoil == NULL) {
		loadParam->errorCode = IMAGINEERROR_OUTOFMEMORY;
		return NULL;
	}

	bool ok = iface->lpVtbl->IsUnicode()
		? RECOILWin32_DecodeW(recoil, (LPCWSTR) loadParam->fileName, loadParam->buffer, loadParam->length)
		: RECOILWin32_DecodeA(recoil, (const char *) loadParam->fileName, loadParam->buffer, loadParam->length);
	if (!ok) {
		RECOIL_Delete(recoil);
		loadParam->errorCode = IMAGINEERROR_UNSUPPORTEDTYPE;
		return NULL;
	}
	int width = RECOIL_GetWidth(recoil);
	int height = RECOIL_GetHeight(recoil);
	const int *pixels = RECOIL_GetPixels(recoil);

	LPIMAGINEBITMAP bitmap = iface->lpVtbl->Create(width, height, 24, flags);
	if (bitmap == NULL) {
		RECOIL_Delete(recoil);
		loadParam->errorCode = IMAGINEERROR_OUTOFMEMORY;
		return NULL;
	}
	if ((flags & IMAGINELOADPARAM_GETINFO) != 0) {
		RECOIL_Delete(recoil);
		return bitmap;
	}

	IMAGINECALLBACKPARAM param;
	param.dib = bitmap;
	param.param = loadParam->callback.param;
	param.overall = height - 1;
	param.message = NULL;
	for (int y = 0; y < height; y++) {
		LPBYTE dest = iface->lpVtbl->GetLineBits(bitmap, y);
		for (int x = 0; x < width; x++) {
			int rgb = *pixels++;
			/* 0xRRGGBB -> 0xBB 0xGG 0xRR */
			*dest++ = (BYTE) rgb;
			*dest++ = (BYTE) (rgb >> 8);
			*dest++ = (BYTE) (rgb >> 16);
		}
		if ((flags & IMAGINELOADPARAM_CALLBACK) != 0) {
			param.current = y;
			if (!loadParam->callback.proc(&param)) {
				RECOIL_Delete(recoil);
				loadParam->errorCode = IMAGINEERROR_ABORTED;
				return bitmap;
			}
		}
	}
	RECOIL_Delete(recoil);
	return bitmap;
}

static BOOL IMAGINEAPI registerProcA(const IMAGINEPLUGININTERFACE *iface)
{
	static const IMAGINEFILEINFOITEM fileInfoItemA = {
		checkFile,
		loadFile,
		NULL,
		(LPCTSTR) "Retro Computer (RECOIL)",
		(LPCTSTR) IMAGINE_RECOIL_EXTS
	};
	return iface->lpVtbl->RegisterFileType(&fileInfoItemA) != NULL;
}

static BOOL IMAGINEAPI registerProcW(const IMAGINEPLUGININTERFACE *iface)
{
	static const IMAGINEFILEINFOITEM fileInfoItemW = {
		checkFile,
		loadFile,
		NULL,
		(LPCTSTR) L"Retro Computer (RECOIL)",
		(LPCTSTR) L"" IMAGINE_RECOIL_EXTS
	};
	return iface->lpVtbl->RegisterFileType(&fileInfoItemW) != NULL;
}

__declspec(dllexport) BOOL IMAGINEAPI ImaginePluginGetInfoA(IMAGINEPLUGININFOA *dest)
{
	static const IMAGINEPLUGININFOA pluginInfoA = {
		sizeof(pluginInfoA),
		registerProcA,
		VERSION_NUMBER,
		"RECOIL Plugin",
		IMAGINEPLUGININTERFACE_VERSION
	};
	*dest = pluginInfoA;
	return TRUE;
}

__declspec(dllexport) BOOL IMAGINEAPI ImaginePluginGetInfoW(IMAGINEPLUGININFOW *dest)
{
	static const IMAGINEPLUGININFOW pluginInfoW = {
		sizeof(pluginInfoW),
		registerProcW,
		VERSION_NUMBER,
		L"RECOIL Plugin",
		IMAGINEPLUGININTERFACE_VERSION
	};
	*dest = pluginInfoW;
	return TRUE;
}
