# YADMA
Yes Another Download Manager

This is a project I created in 2010 (yes a long time ago).
I fixed it and made It work again (2021).

![diagram](/docs/screenshot.png?raw=true "Screenshot")

## Init

```
git submodule update --init --recursive
```

## Dependencies

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
```
## Windows users

Download and extrac to somewhere the compiler tools [mingw64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z/download).  

Add the bin directory to your path environment variable.

Install [CMAKE](https://github.com/Kitware/CMake/releases/download/v3.19.2/cmake-3.19.2-win64-x64.msi)

Execute build-with-mingw.bat to build the project.

## Build

This project requires cmake in order to build it
```bash
cmake .
cmake --build .
```

Clean and bui1d
```bash
cmake .
cmake --clean-first --build .
```

Create the data directory
```bash
mkdir ./build/data
```

Running yadma
```bash
./build/bin/yadma
```

Running yadma on windows
```bat
.\build\bin\yadma.exe
```

## Usage

The first thing you have to do before start the downloads is to enter the settings and define the download directory and other settings.


## What is YADMA ?

YADMA is a webserver that allow you manage your downloads.

