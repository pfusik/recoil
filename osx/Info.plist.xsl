<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="xml" indent="yes" doctype-public="-//Apple//DTD PLIST 1.0//EN" doctype-system="http://www.apple.com/DTDs/PropertyList-1.0.dtd" />
	<xsl:param name="target" />
	<xsl:template match="/formats">
		<xsl:comment>Generated automatically from formats.xml and Info.plist.xsl. Do not edit.</xsl:comment>
		<plist version="1.0">
			<dict>
				<key>CFBundleExecutable</key>
				<string>
					<xsl:if test="$target = 'app'">RECOILMessage</xsl:if>
					<xsl:if test="$target = 'preview'">RECOILPreview</xsl:if>
					<xsl:if test="$target = 'thumbnail'">RECOILThumbnail</xsl:if>
				</string>
				<xsl:if test="$target = 'app'">
					<key>CFBundleIconFile</key>
					<string>AppIcon</string>
				</xsl:if>
				<key>CFBundleIdentifier</key>
				<string>
					<xsl:text>net.sf.recoil.qlrecoil</xsl:text>
					<xsl:if test="$target = 'preview'">.RECOILPreview</xsl:if>
					<xsl:if test="$target = 'thumbnail'">.RECOILThumbnail</xsl:if>
				</string>
				<key>CFBundleName</key>
				<string>
					<xsl:if test="$target = 'app'">qlrecoil</xsl:if>
					<xsl:if test="$target = 'preview'">RECOILPreview</xsl:if>
					<xsl:if test="$target = 'thumbnail'">RECOILThumbnail</xsl:if>
				</string>
				<xsl:if test="$target != 'app'">
					<key>CFBundlePackageType</key>
					<string>XPC!</string>
				</xsl:if>
				<key>CFBundleShortVersionString</key>
				<string><xsl:value-of select="@version" /></string>
				<xsl:if test="$target != 'app'">
					<key>CFBundleSupportedPlatforms</key>
					<array>
						<string>MacOSX</string>
					</array>
				</xsl:if>
				<key>CFBundleVersion</key>
				<string><xsl:value-of select="@version" /></string>
				<xsl:if test="$target = 'app'">
					<key>NSHumanReadableCopyright</key>
					<string>Copyright © 2014-2026 Piotr Fusik. All rights reserved.</string>
					<key>UTImportedTypeDeclarations</key>
					<array>
						<xsl:for-each select="platform[not(@skip = 'ql')]">
							<xsl:variable name="platform" select="translate(@name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ+-/ ','abcdefghijklmnopqrstuvwxyzp')" />
							<dict>
								<key>UTTypeConformsTo</key>
								<array>
									<string>public.image</string>
								</array>
								<key>UTTypeDescription</key>
								<string><xsl:value-of select="@name" /> Image</string>
								<key>UTTypeIdentifier</key>
								<string>net.sf.recoil.<xsl:value-of select="$platform" />.image</string>
								<key>UTTypeTagSpecification</key>
								<dict>
									<key>public.filename-extension</key>
									<array>
										<xsl:for-each select="format/ext[not(. = following::ext) and . != 'ASC']">
											<xsl:if test="count(../../../platform[format/ext = current()]) = 1">
												<string><xsl:value-of select="translate(., 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')" /></string>
											</xsl:if>
										</xsl:for-each>
									</array>
									<key>public.mime-type</key>
									<string>image/x-<xsl:value-of select="$platform" /></string>
								</dict>
							</dict>
						</xsl:for-each>
						<dict>
							<key>UTTypeConformsTo</key>
							<array>
								<string>public.image</string>
							</array>
							<key>UTTypeDescription</key>
							<string>Retro Computer Image</string>
							<key>UTTypeIdentifier</key>
							<string>net.sf.recoil.image</string>
							<key>UTTypeTagSpecification</key>
							<dict>
								<key>public.filename-extension</key>
								<array>
									<xsl:for-each select="platform/format/ext[not(. = following::ext) and . != 'RPM']">
										<xsl:if test="count(../../../platform[format/ext = current()]) != 1">
											<string><xsl:value-of select="translate(., 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')" /></string>
										</xsl:if>
									</xsl:for-each>
								</array>
								<key>public.mime-type</key>
								<string>image/x-recoil</string>
							</dict>
						</dict>
					</array>
				</xsl:if>
				<xsl:if test="$target != 'app'">
					<key>LSMinimumSystemVersion</key>
					<string>12.0</string>
					<key>NSExtension</key>
					<dict>
						<key>NSExtensionAttributes</key>
						<dict>
							<xsl:if test="$target = 'preview'">
								<key>QLIsDataBasedPreview</key>
								<true/>
							</xsl:if>
							<key>QLSupportedContentTypes</key>
							<array>
								<xsl:for-each select="platform[not(@skip = 'ql')]">
									<string>net.sf.recoil.<xsl:value-of select="translate(@name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ+-/ ','abcdefghijklmnopqrstuvwxyzp')" />.image</string>
								</xsl:for-each>
								<string>net.sf.recoil.image</string>
							</array>
							<xsl:if test="$target = 'preview'">
								<key>QLSupportsSearchableItems</key>
								<false/>
							</xsl:if>
							<xsl:if test="$target = 'thumbnail'">
								<key>QLThumbnailMinimumDimension</key>
								<integer>16</integer>
							</xsl:if>
						</dict>
						<xsl:if test="$target = 'preview'">
							<key>NSExtensionPointIdentifier</key>
							<string>com.apple.quicklook.preview</string>
							<key>NSExtensionPrincipalClass</key>
							<string>PreviewProvider</string>
						</xsl:if>
						<xsl:if test="$target = 'thumbnail'">
							<key>NSExtensionPointIdentifier</key>
							<string>com.apple.quicklook.thumbnail</string>
							<key>NSExtensionPrincipalClass</key>
							<string>ThumbnailProvider</string>
						</xsl:if>
					</dict>
				</xsl:if>
			</dict>
		</plist>
	</xsl:template>
</xsl:stylesheet>
