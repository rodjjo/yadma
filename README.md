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

