# snakeoil
My C++ programming framework. I use it mainly for experimentation and learing new things. I always try to implement everything from scratch in order to have the biggest learing effect, but sometimes this is not possible.

snakeoil is supposed to be a game/animation engine written in C++ with much multi-threading in mind. You could already do a lot with it and snakeoil consists of many libraries.

The framework will build out of the box with the most common requirements included like rendering, threading, game devices or fonts. Still, parts are unfortunately missing like the sound library.

At the moment, this is the mirror repository of my private repository where I do everything.

## Compiler Support

A C++11 Compiler is required

- Visual Studio 2015 64 bit
- GCC 6.3.1 64 bit

## Platform Support

- Windows 7/8/10 64 bit
- Linux 64 bit (basic support) (tested on Fedora 25)

## Build-In External Libraries

Buld-in means, that these libraries are shiped with the framework code and that you have access to it during development. These libraries are stored in the /externals directory.

[RapidXML](http://rapidxml.sourceforge.net/)  
[RapidJson](http://rapidjson.org/)  
[LUA](https://www.lua.org/)  
[STB](https://github.com/nothings/stb)  

## Optional/External Libraries

If you opt-in these libraries via CMake, you need to download the official SDKs:

[Maxon Melange SDK](https://developers.maxon.net/)  

Note that the enabled SDK support is encoded within the automatically generated CMake configuration files.

# How to build

The snakeoil programming framework **will build out of the box**, unless you opt-in optional extensions.

## 1. A basic build (no opt-in, no shared)

1. Please check out the snakeoil repository. 
2. Direct CMake to your snakeoil source tree folder
3. Choose a build directory outside of the source tree!
4. Uncheck SNAKEOIL_LIBRARY_BUILD_SHARED for easy use with static libraries
5. Choose SNAKEOIL_TARGET_OS_WIN_NAME for your target operating system
6. Do not bother the extenstions 
7. Just configure and generate VS 2015 64 bit project files via CMake
8. Open the VS project hit build for debug or/and release
9. snakeoil should be build
