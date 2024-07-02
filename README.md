# fonograf

Basic music player in C++.

Uses the [miniaudio](https://github.com/mackron/miniaudio) library for audio
decoding.

# building

Requirements:

- A C compiler, g++ on Linux/macOS and cl.exe on Windows. The program is likely to compile with other compilers too like clang++ or mingw, however the makefile and `build.bat` make use of g++ and cl.

Clone the repo:

```bash
git clone https://github.com/pes18fan/fonograf.git
```

If on Linux or macOS:

```bash
make # Build
```

If on Windows, run `build.bat`. Make sure `cl.exe` is in your path, you may have
to run the build file on a Visual Studio command prompt for `cl.exe` to be 
available.

Enjoy!
