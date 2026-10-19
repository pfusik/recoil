// recoil-stdio.h - stdio subclass of RECOIL
//
// Copyright (C) 2015-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#ifndef _RECOILSTDIO_H_
#define _RECOILSTDIO_H_

#include <stdbool.h>

#include "recoil.h"

#ifdef __cplusplus
extern "C" {
#endif

RECOIL *RECOILStdio_New(void);
bool RECOILStdio_Load(RECOIL *self, const char *filename);

#ifdef __cplusplus
}
#endif

#endif
