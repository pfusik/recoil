/*
 * StorageFileRECOIL.cs - UWP subclass of RECOIL
 *
 * Copyright (C) 2024  Piotr Fusik
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
			StorageFile file;
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
