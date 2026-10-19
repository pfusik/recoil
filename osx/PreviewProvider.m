// PreviewProvider.m - RECOIL plugin for macOS Quick Look
//
// Copyright (C) 2026 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

#import "PreviewProvider.h"
#import "toCGImage.h"

@implementation PreviewProvider

- (void)providePreviewForFileRequest:(QLFilePreviewRequest *)request
		completionHandler:(void (^)(QLPreviewReply * _Nullable reply, NSError * _Nullable error))handler
{
	NSError *error = nil;
	CGImageRef image = toCGImage(request.fileURL, &error);
	if (!image) {
		handler(nil, error);
		return;
	}
	id imageOwner = CFBridgingRelease(image);
	CGSize size = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	QLPreviewReply *reply = [[QLPreviewReply alloc]
		initWithContextSize:size
		isBitmap:YES
		drawingBlock:^BOOL(CGContextRef context, QLPreviewReply *replyToUpdate, NSError **outError) {
			CGContextDrawImage(context, CGRectMake(0, 0, size.width, size.height), (__bridge CGImageRef)imageOwner);
			return YES;
		}];
	handler(reply, nil);
}

@end
