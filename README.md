# snakeoil


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

## Build-In External Libraries

Build-in means that these libraries are shipped with the framework code and that you have access to it during development. These libraries are stored in the /externals directory.

[RapidXML](http://rapidxml.sourceforge.net/) 
[RapidJSON](https://github.com/Tencent/rapidjson) 
[LUA](https://www.lua.org/)  
[STB](https://github.com/nothings/stb)  
[Ogg](https://www.xiph.org/ogg/)  
[Vorbis](https://xiph.org/vorbis/)  
[ImGui](https://github.com/ocornut/imgui/)  

## Travis CI (experimental)
https://travis-ci.com/aconstlink/snakeoil.svg?branch=master

## Contribution

TBD

## Examples

TBD

## License

snakeoil is distributed under the MIT license.