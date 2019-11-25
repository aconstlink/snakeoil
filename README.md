# snakeoil

snakeoil is an experimental audio/visual programming framework written in C++ for creative coding. Its main purpose is to provide an experimental playground for testing, learning and experiencing topics in the realm of real-time interactive audio/visual applications.

# General

## How to build

1. Check out snakeoil (this repo)
2. Generate a Visual Studio 2015/2017 x64 project **outside** of the source tree using CMake  
3. [optional] Uncheck SNAKEOIL_LIBRARY_BUILD_SHARED for easy use with static libraries  
4. Open the generated VS project and hit build for debug or/and release  
5. Finised - snakeoil is ready to be used

## Compiler Support

A C++11 Compiler is required

- Visual Studio 2015 x64
- Visual Studio 2017 x64
- GCC 6.3.1 64 bit

## Platform Support

- Windows 7/8/10 64 bit
- Linux 64 bit (only low level library support) (tested on Fedora 25)  

> Linux is currently only experimental. Base compoments are compiling and are usable though. You can not do rendering or sound on Linux yet.

## Note on externals

At the beginning of this projects several year back, I wanted to experience mostly all of the engine from scratch as in implementing it from the very scratch. Meanwhile, this became too time consuming so I started focusing on topics that are really interesting to me and I will keep doing those from scratch. There are externals used in snakeoil, which can be observed in the externals folder of the project. Those libs are used throughout the engine and should be ussable by the user of snakeoil.

## CI (experimental)

### Travics CI

[![Build Status](https://travis-ci.com/aconstlink/snakeoil.svg?branch=master)](https://travis-ci.com/aconstlink/snakeoil)

### Something in the realm of windows here

## Contribution

TBD

## Examples

TBD

## License

snakeoil is distributed under the MIT license.