/*
 * ThumbnailProvider.m - RECOIL plugin for macOS Quick Look
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

#import "ThumbnailProvider.h"
#import "toCGImage.h"

@implementation ThumbnailProvider

- (void)provideThumbnailForFileRequest:(QLFileThumbnailRequest *)request
		completionHandler:(void (^)(QLThumbnailReply * _Nullable reply, NSError * _Nullable error))handler
{
	NSError *error = nil;
	CGImageRef image = toCGImage(request.fileURL, &error);
	if (!image) {
		handler(nil, error);
		return;
	}
	id imageOwner = CFBridgingRelease(image);
	CGSize size = request.maximumSize;
	size_t imageWidth = CGImageGetWidth(image);
	size_t imageHeight = CGImageGetHeight(image);
	if (imageWidth * size.height < imageHeight * size.width)
		size.width = imageWidth * size.height / imageHeight;
	else
		size.height = imageHeight * size.width / imageWidth;
	CGFloat width = size.width * request.scale;
	CGFloat height = size.height * request.scale;
	handler([QLThumbnailReply replyWithContextSize:size
		drawingBlock:^BOOL(CGContextRef context) {
			CGContextDrawImage(context, CGRectMake(0, 0, width, height), (__bridge CGImageRef)imageOwner);
			return YES;
		}], nil);
}

@end
