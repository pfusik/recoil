// recoil-stdio.c - stdio subclass of RECOIL
//
// Copyright (C) 2015-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "recoil-stdio.h"

typedef struct {
	int (*readFile)(const RECOIL *self, const char *filename, uint8_t *content, int contentLength);
} RECOILVtbl;

static int RECOILStdio_ReadFile(const RECOIL *self, const char *filename, uint8_t *content, int contentLength)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
		return -1;
	contentLength = fread(content, 1, contentLength, fp);
	fclose(fp);
	return contentLength;
}

RECOIL *RECOILStdio_New(void)
{
	RECOIL *self = RECOIL_New();
	if (self != NULL) {
		static const RECOILVtbl vtbl = { RECOILStdio_ReadFile };
		*(const RECOILVtbl **) self = &vtbl;
	}
	return self;
}

bool RECOILStdio_Load(RECOIL *self, const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
		return false;
	if (fseek(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return false;
	}
	long contentLength = ftell(fp);
	if (contentLength > INT_MAX
	 || fseek(fp, 0, SEEK_SET) != 0) {
		 fclose(fp);
		 return false;
	}
	uint8_t *content = (uint8_t *) malloc(contentLength);
	if (content == NULL) {
		fclose(fp);
		return false;
	}
	contentLength = fread(content, 1, contentLength, fp);
	fclose(fp);
	bool ok = RECOIL_Decode(self, filename, content, (int) contentLength);
	free(content);
	return ok;
}
