/*
 * PreviewProvider.m - RECOIL plugin for macOS Quick Look
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
