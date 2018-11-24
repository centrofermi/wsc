# Introduction
This project aims to provide programs to perform temperature, humidity,
and pressure measurements by means of a BME280 sensor connected to an
Arduino Mega 2560.

It contains both Arduino sketch and GUI-based Windows application for the
reading out data and putting into a text file on disk.

# Developer's notes
## Build environment
To build the code you will need a set of developer tools as well as a
suited operating system:

- a recent version of Microsoft Windows 64-bit
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [CMake](https://cmake.org/)
- Microsoft [Visual Studio Community Edition](https://visualstudio.microsoft.com/it/downloads/)
- Microsoft [VCPKG](https://github.com/Microsoft/vcpkg)

### Operating system
A 64-bit MS Windows OS is required. The procedure has been tested on
`Windows 7 Ultimate` and `Windows 10`.

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
3. Install `vcpkg` and:
```shell
cd installation\path\of\vcpkg\
vcpkg.exe install nana:x64-windows nana:x86-windows
```
