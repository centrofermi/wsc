# Introduction
This project aims to provide programs to perform temperature, humidity,
and pressure measurements by means of a BME280 sensor connected to an
Arduino Mega 2560.

It contains both Arduino sketch and GUI-based application for reading out
data and putting into a text file on disk.

The code can be built and run on both Windows and Linux.

# Developer's notes
To build the code you will need a set of developer tools as well as a
suited operating system. The code builds and runs on both Microsoft Windows
and Linux operating systems.

## MS Windows build environment
In the following text `%WSC%` represents the folder where the WSC source
repository is located.

### Requirements
- a recent version of Microsoft Windows 64-bit
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [CMake](https://cmake.org/) v3.10 or newer
- Microsoft [Visual Studio Community Edition](https://visualstudio.microsoft.com/it/downloads/)
- Microsoft [VCPKG](https://github.com/Microsoft/vcpkg)
- [Nullsoft Scriptable Install System (NSIS)](https://nsis.sourceforge.io/Main_Page) (optional, for creating the installer)

### Operating system
A 64-bit MS Windows OS is required<sup>1</sup>. The procedure has been
tested on `Windows 7 Ultimate` and `Windows 10`.

<sup>1</sup> It is actually required to build `VCPKG`, that in turn is
needed to build `nana`. If you find another easy way to do so, you may use
your 32-bit MS Windows system.

### Arduino
1. Install `Arduino IDE`.
1. In `Arduino IDE` go to "Tools->Library management".
1. Install both `Adafruit Unified Sensors` and `Adafruit BME280 Library`.

### Visual Studio
1. Install Visual Studio 2017 Community Edition, that is free to use and
downloadable from [Microsoft Website](https://visualstudio.microsoft.com/it/downloads/).
2. Install at least the following components:
  * C++ Desktop Workload
  * Git for Windows

### VCPKG
Install `vcpkg` and the `nana` library:

```shell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg.exe install nana:x86-windows
vcpkg.exe export nana --raw --output %WSC%\wsc-dependencies\
```

### NSIS
Download NSIS from the [official website](https://nsis.sourceforge.io/Main_Page)
and install it in the default folder.

### Build
In a `Developer Command Prompt for VS 2017`:

```shell
cd %WSC%
mkdir build
cd build
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x86-windows -DCMAKE_TOOLCHAIN_FILE="../wsc-dependencies/scripts/buildsystems/vcpkg.cmake" ..
nmake
REM the following is optional: to build the Windows install program
cpack
```

