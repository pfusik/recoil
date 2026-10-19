// BoundsCheck.cs - check out-of-bounds accesses for truncated files
//
// Copyright (C) 2015-2022 Piotr Fusik
// SPDX-License-Identifier: MIT
//
// This file is part of RECOIL (Retro Computer Image Library),
// see https://recoil.sourceforge.net

using System;
using System.IO;
using System.Threading.Tasks;

using Recoil;

class FileRECOIL : RECOIL
{
	protected override int ReadFile(string filename, byte[] content, int contentLength)
	{
		try {
			using (FileStream s = File.OpenRead(filename)) {
				return s.Read(content, 0, contentLength);
			}
		}
		catch (FileNotFoundException) {
			return -1;
		}
	}
}

public class BoundsChecker
{
	public static void Main(string[] args)
	{
		if (args.Length != 1)
			throw new ArgumentException("Usage: BoundsCheck DIRECTORY");
		Parallel.ForEach(Directory.GetFiles(args[0]), filename => {
			RECOIL recoil = new FileRECOIL();
			Console.WriteLine(filename);
			byte[] content = File.ReadAllBytes(filename);
			int length = content.Length;
			if (recoil.Decode(filename, content, length)) {
				if (length > 6000)
					length = 6000;
				while (length > 0) {
					byte[] smaller = new byte[--length];
					Array.Copy(content, smaller, length);
					content = smaller;
					if (content.Length % 2000 == 0)
						Console.WriteLine(length);
					recoil.Decode(filename, content, length);
				}
			}
			else {
				Console.WriteLine("Cannot decode");
			}
		});
	}
}
