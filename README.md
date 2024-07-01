# fonograf

Basic music player in C++.

Uses the [miniaudio](https://github.com/mackron/miniaudio) library for audio
decoding, and [tagparser](https://github.com/Martchus/tagparser) to, well, parse
tags.

# building

Requirements:

- curl
- A C compiler, g++ on Linux/macOS and cl.exe on Windows. The program is likely to compile with other compilers too like clang++ or mingw, however the makefile and `build.bat` make use of g++ and cl.

Clone the repo:

```bash
git clone https://github.com/pes18fan/fonograf.git
```

Now, install the required libraries (miniaudio, boiler.h and tagparser) and build.
If on Linux or macOS:

```bash
make deps # Add vendor deps
make # Build
```

If on Windows, run `build.bat`. Make sure `cl.exe` and `curl.exe` are in your path,
you may have to run the build file on a Visual Studio command prompt for `cl.exe`
to be available.

Enjoy!
