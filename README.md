# snakeoil
snakeoil is my C++ programming framework that I use mainly for experimentation and learning new things in the realm of real-time 2D/3D engine programming. I always try to implement required components from scratch in order to have the biggest learing effect with some tiny exceptions:)

snakeoil is supposed to be a game/animation engine written in C++ with much multi-threading in mind. The framework will build out of the box with the most common requirements included like rendering, threading, game devices or fonts. There is even a basic audio engine implemented using XAudio2.

Personally, I also want to see snakeoil as a support framework since it provides some features out of the box without using 3rd party support. For example you can use the device library to integrate devices in your application or you can use it to retrieve project cars UDP data and many more scenarios.

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

Windows 10 is not tested. The compilation is based on Windows OS SDKs and I did not have the opportunity to use Windows 10 privatly in order to support this OS. But it should be possible to install the Windows 8.1 SDK and set the SNAKEOIL_TARGET_OS_WIN_NAME to Windows 8.1.

## Build-In External Libraries

Build-in means that these libraries are shipped with the framework code and that you have access to it during development. These libraries are stored in the /externals directory.

[RapidXML](http://rapidxml.sourceforge.net/)  
[RapidJson](http://rapidjson.org/)  
[LUA](https://www.lua.org/)  
[STB](https://github.com/nothings/stb)  
[Ogg](https://www.xiph.org/ogg/)  
[Vorbis](https://xiph.org/vorbis/)  

## Contribution

At the moment, this is a mirror repository and I only accept bug fixes.

## Examples

Please see my other repositories for some example applications. I try to keep it as simple as possible.

## License

snakeoil is distributed under the MIT license.