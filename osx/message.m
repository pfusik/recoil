#import <Cocoa/Cocoa.h>

int main(int argc, char **argv)
{
	@autoreleasepool {
		[NSApplication sharedApplication];
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:@"RECOIL Quick Look plugin"];
		[alert setInformativeText:@"Retro Computer Image Library (RECOIL) plugin has been installed. Use Finder thumbnails and preview. Thumbnails may not appear until you restart your computer."];
		[alert setAlertStyle:NSAlertStyleInformational];
		[alert addButtonWithTitle:@"OK"];
		[alert runModal];
		[NSApp terminate:nil];
	}
	return 0;
}
