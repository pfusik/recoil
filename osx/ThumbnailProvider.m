// ThumbnailProvider.m - RECOIL plugin for macOS Quick Look
//
// Copyright (C) 2026 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

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
