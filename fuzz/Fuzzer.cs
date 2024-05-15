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
		Parallel.ForEach(filenames, filename => {
			RECOIL recoil = new FileRECOIL();
			Console.WriteLine(filename);
			byte[] content = File.ReadAllBytes(filename);
			int length = content.Length;
			if (recoil.Decode(filename, content, length)) {
				for (int offset = 0; offset < length && offset < 10000; offset++) {
					if (offset > 0 && offset % 2000 == 0)
						Console.WriteLine(offset);
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
			}
			else {
				Console.WriteLine("Cannot decode");
			}
		});
	}
}
