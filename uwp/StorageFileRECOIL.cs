// StorageFileRECOIL.cs - UWP subclass of RECOIL
//
// Copyright (C) 2014-2026 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Storage;

namespace RECOIL
{
	public class StorageFileRECOIL : Recoil.RECOIL
	{
		readonly IReadOnlyList<StorageFile> Files;

		public StorageFileRECOIL(IReadOnlyList<StorageFile> files)
		{
			this.Files = files;
		}

		protected override int ReadFile(string filename, byte[] content, int contentLength)
		{
			StorageFile? file;
			try {
				file = this.Files.SingleOrDefault(f => f.Path == filename);
			}
			catch (InvalidOperationException) {
				return -1;
			}
			if (file == null)
				return -1;
			try {
				using (Stream stream = file.OpenStreamForReadAsync().GetAwaiter().GetResult()) {
					return stream.Read(content, 0, contentLength);
				}
			}
			catch (IOException) {
				return -1;
			}
		}
	}
}
