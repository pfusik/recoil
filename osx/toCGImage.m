/*
 * toCGImage.m - RECOIL plugin for macOS Quick Look
 *
 * Copyright (C) 2026  Piotr Fusik
 *
 * This file is part of RECOIL (Retro Computer Image Library),
 * see http://recoil.sourceforge.net
 *
 * RECOIL is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * RECOIL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RECOIL; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#import "toCGImage.h"
#import "recoil.h"

static void releaseRECOIL(void *info, const void *data, size_t size)
{
	RECOIL_Delete(info);
}

CGImageRef toCGImage(NSURL *url, NSError **error)
{
	NSData *data = [NSData dataWithContentsOfURL:url options:0 error:error];
	if (!data)
		return NULL;
	RECOIL *recoil = RECOIL_New();
	if (!RECOIL_Decode(recoil, url.path.UTF8String, data.bytes, (int)data.length)) {
		RECOIL_Delete(recoil);
		*error = [NSError errorWithDomain:@"net.sf.recoil.ErrorDomain" code:1 userInfo:nil];
		return NULL;
	}
	size_t width = RECOIL_GetWidth(recoil);
	size_t height = RECOIL_GetHeight(recoil);
	CGDataProviderRef provider = CGDataProviderCreateWithData(recoil,
		RECOIL_GetPixels(recoil), width * height << 2, releaseRECOIL);
	if (!provider) {
		RECOIL_Delete(recoil);
		*error = [NSError errorWithDomain:@"net.sf.recoil.ErrorDomain" code:2 userInfo:nil];
		return NULL;
	}
	CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
	CGImageRef image = CGImageCreate(width, height, 8, 32, width << 2, space,
		(CGBitmapInfo)kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Host,
		provider, NULL, false, kCGRenderingIntentDefault);
	CGColorSpaceRelease(space);
	CGDataProviderRelease(provider);
	return image;
}
