VERSION = 6.4.5

OBJC = clang
OBJCFLAGS = -O2 -Wall -fobjc-arc -mmacos-version-min=12.0 -arch x86_64 -arch arm64
CFLAGS = -O2 -Wall -mmacosx-version-min=10.6 -arch x86_64 -arch arm64
CODESIGN = codesign --options runtime -f -s $(FOX_CODESIGNING_IDENTITY)
GIMP_RES = /Applications/GIMP.app/Contents/Resources
GIMP_X86_64_RES = ~/GIMP-x86_64/GIMP.app/Contents/Resources
GIMP_LIBS = gimp-3.0 gimpbase-3.0 gegl-0.4 babl-0.1 gio-2.0 gobject-2.0 glib-2.0

recoil-$(VERSION)-macos.dmg: bin/Applications bin/RECOIL.app/Contents/_CodeSignature/CodeResources bin/GIMP bin/file-recoil/file-recoil bin/XnViewMP bin/Xrecoil.usr bin/bin bin/recoil2png
	hdiutil create -volname recoil-$(VERSION)-macos -srcfolder bin -format UDBZ -fs HFS+ -imagekey bzip2-level=3 -ov $@
	/Applications/Xcode.app/Contents/Developer/usr/bin/notarytool submit --wait --keychain-profile foxnotary $@

bin/Applications:
	mkdir -p $(@D) && ln -s /Applications $@

bin/RECOIL.app/Contents/_CodeSignature/CodeResources: bin/RECOIL.app/Contents/MacOS/RECOILMessage bin/RECOIL.app/Contents/Info.plist bin/RECOIL.app/Contents/Resources/AppIcon.icns \
	bin/RECOIL.app/Contents/PlugIns/RECOILPreview.appex/Contents/MacOS/RECOILPreview bin/RECOIL.app/Contents/PlugIns/RECOILPreview.appex/Contents/Info.plist \
	bin/RECOIL.app/Contents/PlugIns/RECOILThumbnail.appex/Contents/MacOS/RECOILThumbnail bin/RECOIL.app/Contents/PlugIns/RECOILThumbnail.appex/Contents/Info.plist \
	entitlements.xml
ifdef FOX_CODESIGNING_IDENTITY
	$(CODESIGN) --entitlements entitlements.xml bin/RECOIL.app/Contents/PlugIns/RECOILPreview.appex bin/RECOIL.app/Contents/PlugIns/RECOILThumbnail.appex bin/RECOIL.app
endif

bin/RECOIL.app/Contents/MacOS/RECOILMessage: message.m
	mkdir -p $(@D) && $(OBJC) -o $@ $(OBJCFLAGS) $^ -framework Cocoa && strip -x $@

bin/RECOIL.app/Contents/PlugIns/RECOILPreview.appex/Contents/MacOS/RECOILPreview: PreviewProvider.m toCGImage.m ../recoil.c
	mkdir -p $(@D) && $(OBJC) -o $@ $(OBJCFLAGS) -fapplication-extension -I .. $^ -e _NSExtensionMain -framework Quartz -framework CoreGraphics && strip -x $@

bin/RECOIL.app/Contents/PlugIns/RECOILThumbnail.appex/Contents/MacOS/RECOILThumbnail: ThumbnailProvider.m toCGImage.m ../recoil.c
	mkdir -p $(@D) && $(OBJC) -o $@ $(OBJCFLAGS) -fapplication-extension -I .. $^ -e _NSExtensionMain -framework QuickLookThumbnailing -framework CoreGraphics && strip -x $@

bin/RECOIL.app/Contents/Info.plist: Info.plist.xsl ../formats.xml
	mkdir -p $(@D) && xsltproc -o $@ --param target "'app'" $^

bin/RECOIL.app/Contents/PlugIns/RECOILPreview.appex/Contents/Info.plist: Info.plist.xsl ../formats.xml
	mkdir -p $(@D) && xsltproc -o $@ --param target "'preview'" $^

bin/RECOIL.app/Contents/PlugIns/RECOILThumbnail.appex/Contents/Info.plist: Info.plist.xsl ../formats.xml
	mkdir -p $(@D) && xsltproc -o $@ --param target "'thumbnail'" $^

bin/RECOIL.app/Contents/Resources/AppIcon.icns: ../recoil-512x512.png
	mkdir -p icon.iconset $(@D)
	sips -z 16 16 $< -o icon.iconset/icon_16x16.png
	sips -z 32 32 $< -o icon.iconset/icon_16x16@2x.png
	sips -z 128 128 $< -o icon.iconset/icon_128x128.png
	sips -z 256 256 $< -o icon.iconset/icon_128x128@2x.png
	iconutil -c icns icon.iconset -o $@

bin/GIMP:
	mkdir -p $(@D) && ln -s $(GIMP_RES)/lib/gimp/3.0/plug-ins $@

bin/file-recoil/file-recoil: ../file-recoil.c ../formats.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) $(addprefix -I $(GIMP_RES)/, include/gimp-3.0 include/gdk-pixbuf-2.0 include include/gegl-0.4 include/babl-0.1 include/pango-1.0 include/cairo include/harfbuzz include/glib-2.0 lib/glib-2.0/include) \
		-o $@ -I .. ../file-recoil3.c ../recoil-stdio.c ../recoil.c -rpath $(GIMP_RES)/ $(GIMP_LIBS:%=$(GIMP_RES)/lib/lib%.dylib) $(GIMP_LIBS:%=$(GIMP_X86_64_RES)/lib/lib%.dylib)
ifdef FOX_CODESIGNING_IDENTITY
	$(CODESIGN) --entitlements gimp-entitlements.xml $@
endif

install-gimp: bin/file-recoil/file-recoil
	mkdir -p ~/Library/'Application Support'/GIMP/3.0/plug-ins/file-recoil && cp $< ~/Library/'Application Support'/GIMP/3.0/plug-ins/file-recoil/

bin/XnViewMP:
	mkdir -p $(@D) && ln -s /Applications/XnViewMP.app/Contents/MacOS/Plugins $@

bin/Xrecoil.usr: ../Xrecoil.c ../formats.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) -o $@ -I .. -dynamiclib ../Xrecoil.c ../recoil-stdio.c ../recoil.c
ifdef FOX_CODESIGNING_IDENTITY
	$(CODESIGN) $@
endif

../formats.h: ../formats.h.xsl ../formats.xml
	xsltproc -o $@ ../formats.h.xsl ../formats.xml

bin/bin:
	mkdir -p $(@D) && ln -s /usr/local/bin $@

bin/recoil2png: ../recoil2png.c ../pngsave.c ../pngsave.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) -o $@ -I .. -I /usr/local/include ../recoil2png.c ../pngsave.c ../recoil-stdio.c ../recoil.c /usr/local/lib/libpng.a -lz
ifdef FOX_CODESIGNING_IDENTITY
	$(CODESIGN) $@
endif

%.c %.h: %.fu ../altirrantsc.pal ../altirrapal.pal ../atari8.fnt ../c16.pal ../c64.fnt ../zx81.fnt
	fut -o $*.c -I .. $<

clean:
	rm -rf recoil-$(VERSION)-macos.dmg bin icon.iconset ../formats.h

.PHONY: clean install-gimp

.DELETE_ON_ERROR:
