// recoil-win32.h - Win32 API subclass of RECOIL
//
// Copyright (C) 2015-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#ifndef _RECOILWIN32_H_
#define _RECOILWIN32_H_

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include "recoil.h"

#ifdef __cplusplus
extern "C" {
#endif

bool RECOILWin32_IsOurFileW(LPCWSTR filename);
int RECOILWin32_SlurpFileA(const char *filename, uint8_t *buffer, int len);
int RECOILWin32_SlurpFileW(LPCWSTR filename, uint8_t *buffer, int len);
bool RECOILWin32_DecodeA(RECOIL *self, const char *filename, uint8_t const *content, int contentLength);
bool RECOILWin32_DecodeW(RECOIL *self, LPCWSTR filename, uint8_t const *content, int contentLength);
bool RECOILWin32_LoadA(RECOIL *self, const char *filename);
bool RECOILWin32_LoadW(RECOIL *self, LPCWSTR filename);

// Use as follows:
// WCHAR platform[RECOIL_MAX_PLATFORM_LENGTH + 1];
// RECOILWin32_GetPlatformW(recoil, &platform);
void RECOILWin32_GetPlatformW(const RECOIL *self, WCHAR (*platform)[RECOIL_MAX_PLATFORM_LENGTH + 1]);

#ifdef UNICODE
#define RECOILWin32_IsOurFile RECOILWin32_IsOurFileW
#define RECOILWin32_SlurpFile RECOILWin32_SlurpFileW
#define RECOILWin32_Decode RECOILWin32_DecodeW
#define RECOILWin32_Load RECOILWin32_LoadW
#else
#define RECOILWin32_IsOurFile RECOIL_IsOurFile
#define RECOILWin32_SlurpFile RECOILWin32_SlurpFileA
#define RECOILWin32_Decode RECOILWin32_DecodeA
#define RECOILWin32_Load RECOILWin32_LoadA
#endif

#ifdef __cplusplus
}
#endif

#endif
