# YADMA
Yes Another Download Manager

This is a project I created in 2010 (yes a long time ago).
I fixed it and made It work again (2021).

![diagram](/docs/screenshot.png?raw=true "Screenshot")

## Notes

The Windows support is not ready yet.

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

Running yadma
```bash
./build/bin/yadma
```

Webserver based download manger

## What is YADMA ?

YADMA is a webserver that allow you manage your downloads.

