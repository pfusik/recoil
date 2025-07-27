VERSION = 6.4.5

CFLAGS_MIN = -O2 -Wall
CFLAGS = $(CFLAGS_MIN) -mmacosx-version-min=10.6 -arch x86_64 -arch arm64
CODESIGN = codesign --options runtime -f -s $(PORK_CODESIGNING_IDENTITY)
QLRECOIL = bin/RECOIL.qlgenerator/Contents/MacOS/qlrecoil
PLIST = bin/RECOIL.qlgenerator/Contents/Info.plist
QLCODESIGN = bin/RECOIL.qlgenerator/Contents/_CodeSignature/CodeResources
GIMP_RES = /Applications/GIMP.app/Contents/Resources

recoil-$(VERSION)-macos.dmg: bin/QuickLook $(QLCODESIGN) $(QLRECOIL) $(PLIST) bin/GIMP bin/file-recoil/file-recoil bin/XnViewMP bin/Xrecoil.usr bin/bin bin/recoil2png
	hdiutil create -volname recoil-$(VERSION)-macos -srcfolder bin -format UDBZ -fs HFS+ -imagekey bzip2-level=3 -ov $@
	/Applications/Xcode.app/Contents/Developer/usr/bin/notarytool submit --wait --keychain-profile recoilnotarization $@

bin/QuickLook:
	mkdir -p $(@D) && ln -s /Library/QuickLook $@

$(QLCODESIGN): $(QLRECOIL) $(PLIST) entitlements.xml
ifdef PORK_CODESIGNING_IDENTITY
	$(CODESIGN) --entitlements entitlements.xml bin/RECOIL.qlgenerator
else
	rm -rf $(@D)
endif

$(QLRECOIL): qlrecoil.c ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) -o $@ -I .. -dynamiclib qlrecoil.c ../recoil-stdio.c ../recoil.c -framework QuickLook -framework ApplicationServices -framework CoreFoundation && strip -x $@

$(PLIST): Info.plist.xsl ../formats.xml
	mkdir -p $(@D) && xsltproc -o $@ Info.plist.xsl ../formats.xml

bin/GIMP:
	mkdir -p $(@D) && ln -s $(GIMP_RES)/lib/gimp/3.0/plug-ins $@

bin/file-recoil/file-recoil: ../file-recoil.c ../formats.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS_MIN) $(addprefix -I $(GIMP_RES)/, include/gimp-3.0 include/gdk-pixbuf-2.0 include include/gegl-0.4 include/babl-0.1 include/pango-1.0 include/cairo include/harfbuzz include/glib-2.0 lib/glib-2.0/include) \
		-o $@ -I .. ../file-recoil3.c ../recoil-stdio.c ../recoil.c -rpath $(GIMP_RES)/ -L $(GIMP_RES)/lib -lgimp-3.0 -lgimpbase-3.0 -lgegl-0.4 -lbabl-0.1 -lgio-2.0 -lgobject-2.0 -lglib-2.0
ifdef PORK_CODESIGNING_IDENTITY
	$(CODESIGN) --entitlements gimp-entitlements.xml $@
endif

install-gimp: bin/file-recoil/file-recoil
	mkdir $(GIMP_RES)/lib/gimp/3.0/plug-ins/file-recoil && cp $< $(GIMP_RES)/lib/gimp/3.0/plug-ins/file-recoil/

bin/XnViewMP:
	mkdir -p $(@D) && ln -s /Applications/XnViewMP.app/Contents/MacOS/Plugins $@

bin/Xrecoil.usr: ../Xrecoil.c ../formats.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) -o $@ -I .. -dynamiclib ../Xrecoil.c ../recoil-stdio.c ../recoil.c
ifdef PORK_CODESIGNING_IDENTITY
	$(CODESIGN) $@
endif

../formats.h: ../formats.h.xsl ../formats.xml
	xsltproc -o $@ ../formats.h.xsl ../formats.xml

bin/bin:
	mkdir -p $(@D) && ln -s /usr/local/bin $@

bin/recoil2png: ../recoil2png.c ../pngsave.c ../pngsave.h ../recoil-stdio.c ../recoil-stdio.h ../recoil.c ../recoil.h
	mkdir -p $(@D) && $(CC) $(CFLAGS) -o $@ -I .. -I /usr/local/include ../recoil2png.c ../pngsave.c ../recoil-stdio.c ../recoil.c /usr/local/lib/libpng.a -lz
ifdef PORK_CODESIGNING_IDENTITY
	$(CODESIGN) $@
endif

%.c %.h: %.fu ../altirrantsc.pal ../altirrapal.pal ../atari8.fnt ../c16.pal ../c64.fnt ../zx81.fnt
	fut -o $*.c -I .. $<

clean:
	rm -rf recoil-$(VERSION)-macos.dmg bin  ../formats.h

.PHONY: clean install-gimp

.DELETE_ON_ERROR:
