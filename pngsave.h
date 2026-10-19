// pngsave.h - save PNG file
//
// Copyright (C) 2009-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net


#ifndef _PNGSAVE_H_
#define _PNGSAVE_H_

#include <stdbool.h>
#include <stdio.h>

#include "recoil.h"

#ifdef __cplusplus
extern "C" {
#endif

bool RECOIL_SavePng(RECOIL *recoil, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif
