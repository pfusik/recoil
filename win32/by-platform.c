// by-platform.c - organize images into platform directories
//
// Copyright (C) 2017-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#include <stdio.h>
#include <windows.h>

#include "recoil-win32.h"

int main(int argc, char **argv)
{
	WIN32_FIND_DATA find;
	HANDLE h = FindFirstFile("../../examples/*", &find);
	if (h == INVALID_HANDLE_VALUE)
		return 1;
	RECOIL *recoil = RECOIL_New();
	do {
		const char *filename = find.cFileName;
		char path[MAX_PATH];
		snprintf(path, sizeof(path), "../../examples/%s", filename);
		//puts(path);
		if (RECOILWin32_LoadA(recoil, path)) {
			const char *platform = RECOIL_GetPlatform(recoil);
			char target[MAX_PATH];
			snprintf(target, sizeof(target), "../../by-platform/%s", platform);
			CreateDirectory(target, NULL);
			snprintf(target, sizeof(target), "../../by-platform/%s/%s", platform, filename);
			CopyFile(path, target, FALSE);
		}
		else {
			fprintf(stderr, "%s: cannot decode\n", path);
		}
	} while (FindNextFile(h, &find));
	return 0;
}
