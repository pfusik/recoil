using System;
using System.IO;
using System.Threading;
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
		catch (ArgumentException) { // illegal characters in VSC file
			return -1;
		}
	}
}

public class Fuzzer
{
	static bool Check(RECOIL recoil, string filename, byte[] content, int length)
	{
		try {
			recoil.Decode(filename, content, length);
			return true;
		}
		catch (Exception) {
			File.WriteAllBytes("BAD_" + Path.GetFileName(filename), content);
			return false;
		}
	}

	public static void Main(string[] args)
	{
		if (args.Length != 1)
			throw new ArgumentException("Usage: Fuzzer DIRECTORY");
		string[] filenames = Directory.GetFiles(args[0]);
		int count = filenames.Length;
		Parallel.ForEach(filenames, filename => {
			RECOIL recoil = new FileRECOIL();
			Console.WriteLine("{0} {1}", Interlocked.Decrement(ref count), filename);
			byte[] content = File.ReadAllBytes(filename);
			int length = content.Length;
			if (!recoil.Decode(filename, content, length)) {
				Console.WriteLine("Cannot decode {0}", filename);
				return;
			}
			for (int offset = 0; offset < length && offset < 10000; offset++) {
				if (offset > 0 && offset % 2000 == 0)
					Console.WriteLine(offset);
				if (offset + 3 < length) {
					byte b0 = content[offset];
					byte b1 = content[offset + 1];
					byte b2 = content[offset + 2];
					byte b3 = content[offset + 3];
					content[offset + 1] = content[offset] = 0;
					if (!Check(recoil, filename, content, length))
						return;
					content[offset] = 0x7f;
					content[offset + 3] = content[offset + 2] = content[offset + 1] = 0xff;
					if (!Check(recoil, filename, content, length))
						return;
					content[offset] = 0xff;
					content[offset + 3] = 0x7f;
					if (!Check(recoil, filename, content, length))
						return;
					content[offset + 3] = 0xff;
					if (!Check(recoil, filename, content, length))
						return;
					content[offset] = b0;
					content[offset + 1] = b1;
					content[offset + 2] = b2;
					content[offset + 3] = b3;
				}
				for (byte mask = 0x80; mask != 0; mask >>= 1) {
					content[offset] ^= mask;
					if (!Check(recoil, filename, content, length))
						return;
					content[offset] ^= mask;
				}
			}
			if (length > 6000)
				length = 6000;
			while (length > 0) {
				byte[] smaller = new byte[--length];
				Array.Copy(content, smaller, length);
				content = smaller;
				if (content.Length % 2000 == 0)
					Console.WriteLine(length);
				if (!Check(recoil, filename, content, length))
					break;
			}
		});
	}
}
