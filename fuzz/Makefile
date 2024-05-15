run: Fuzzer.exe
	./Fuzzer.exe ../../examples

Fuzzer.exe: Fuzzer.cs RECOIL.cs
	"C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/csc.exe" -nologo -out:$@ -debug $^

RECOIL.cs: ../recoil.fu
	fut -o $@ -I .. -n Recoil $<
