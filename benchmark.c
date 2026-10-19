// benchmark.c - RECOIL benchmark
//
// Copyright (C) 2013-2021 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "recoil-stdio.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "benchmark: no input files\n");
		return 1;
	}
	RECOIL *recoil = RECOILStdio_New();
	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];
		if (strcmp(arg, "--help") == 0) {
			printf("Usage: benchmark FILE...\n");
			return 0;
		}

		clock_t start_time = clock();
		if (!RECOILStdio_Load(recoil, arg)) {
			fprintf(stderr, "benchmark: error decoding %s\n", arg);
			return 1;
		}
		clock_t decode_time = clock();
		int colors = RECOIL_GetColors(recoil);
		clock_t colors_time = clock();
		RECOIL_ToPalette(recoil);
		clock_t palette_time = clock();

		RECOIL_ToPalette(recoil);
		clock_t palette2_time = clock();
		if (RECOIL_GetColors(recoil) != colors) {
			fprintf(stderr, "benchmark: RECOIL_GetColors failed for %s\n", arg);
			return 1;
		}
		clock_t colors2_time = clock();

		printf("%3dx%3d %4d colors Decode=%2ld GetColors=%2ld,%2ld ToPalette=%2ld,%2ld %s\n",
			RECOIL_GetWidth(recoil), RECOIL_GetHeight(recoil), colors,
			(  decode_time -    start_time) * 1000L / CLOCKS_PER_SEC, // RECOIL_Decode time
			(  colors_time -   decode_time) * 1000L / CLOCKS_PER_SEC, // RECOIL_GetColors time
			( colors2_time - palette2_time) * 1000L / CLOCKS_PER_SEC, // RECOIL_GetColors after RECOIL_Palette, should be smaller
			(palette2_time -  palette_time) * 1000L / CLOCKS_PER_SEC, // RECOIL_GetPalette time
			( palette_time -   colors_time) * 1000L / CLOCKS_PER_SEC, // RECOIL_GetPalette after RECOIL_GetColors, should be smaller
			arg);
	}
	return 0;
}
