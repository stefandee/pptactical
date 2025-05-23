# Pure Power Tactical Engine

Pure Power Tactical Engine (in short PPTactical) is a cross-platform 2D engine for real time strategy and real time tactics games.

The engine is currently frozen for major features. However, it's still maintained for minor bug fixes and keeping it up to date with compiler and libraries changes.

## History

The project began as the closed source engine of the first game developed by Piron Games, called [Pure Power](https://www.pirongames.com/games/ppower/ppower.htm) (1998-2001)

The engine was open-sourced and has been under steady development until its latest stable release, version 0.9.6 (2001-2006)

Refactoring, bug fixes, library and compiler/IDE upgrades were done in 2017 and 2025.

Previous releases of the engine may be found at https://sourceforge.net/projects/pptactical/

A detailed history of the project may be found in [doc/PPT Releases.doc](doc/PPT%20Releases.doc)

## General Setup
This project uses submodules and they need to be initialized. 

After cloning the project, run:

```
git submodule init
git submodule update
```

Alternatively, pass --recurse-submodules to git clone (or tick the box in your clone popup if you have a visual git client).

## Engine Setup

### Prerequisites

Get a C++ compiler:
* compiler standard is ISO C++11
* the engine has been tested using [mingw64](https://www.mingw-w64.org/) and x86_64-w64 architecture;
* any architecture should be supported though.

Get SDL 2:
* the following libraries are required: [SDL 2](https://github.com/libsdl-org/SDL/releases), [SDL2 Image](https://github.com/libsdl-org/SDL_image/releases), [SDL2 gfx](https://sourceforge.net/projects/sdl2gfx/), [SDL2 mixer](https://github.com/libsdl-org/SDL_mixer/releases), [SDL2 ttf](https://github.com/libsdl-org/SDL_ttf/releases)
* make sure you use the libraries compatible with the compiler;
* if using mingw, unpack into [lib/SDL2_mingw](lib/SDL2_mingw/) folder or, if you already have them installed, adjust the paths (include/linker) to your installation.

Get Code::Blocks (optional):
* a project is available in [dev/codeblocks/engine_sdl](dev/codeblocks/engine_sdl/)
* if you've installed SDL 2 libraries in another path than [lib/SDL2_mingw](lib/SDL2_mingw/), please adjust the include and library paths;
* go to Settings -> Compiler -> Global Compiler Settings -> Toolchain executables and set the path to the C++ compiler you're planning to use.

The engine should build with Microsoft C++ toolchain (Visual Studio, Code, etc), but no projects are supplied at this time.

The engine should build and run on all platforms where SDL is supported (Windows, Linux, Unix, etc). 

> [!IMPORTANT]
> In order to overcome the endianess problem, the Stream_ANSI_C class has been added endian manipulation; since the data files are in little endian format, on big endian platforms conversion on reading basic data types must be performed. To enable endianess conversion, you must set the PPT_BIGENDIAN_IO define.

### Running
> [!IMPORTANT]
> In order to run, please make sure that these DLLs are present in [bin](bin/) folder or in your path. Please make sure the DLLs are for the correct architecture you're building for (e.g. 64 bit)

```shell
cd bin
ls *.dll

SDL2.dll
SDL2_image.dll
SDL2_mixer.dll
SDL2_ttf.dll
```

## Tools Setup

### Prerequisites

C++ Builder 11.2 or later is required to build the tools.

A project group to conveniently have all the tools in one place is available here: [dev/bcb/tools/PPTacticalEngineTools.groupproj](dev/bcb/tools/PPTacticalEngineTools.groupproj)

### Building

Build and install the custom components from the PironGamesComponents projects.

Please note that the Map Editor is quite large in lines of code and C++ Builder will display a nagware popup everytime you try to build. Apparently, people at Embarcadero can't wrap their heads around large open-source being coded just for fun without making any sort of money.

> [!IMPORTANT]
> Map Editor uses DirectX libraries, which are only available in C:\Program Files (x86)\Embarcadero\Studio\22.0\lib\win32\release\psdk. If your C++ Builder install path is different, please re-add ddraw.lib, dsound.lib and dinput.lib

### Running the Map Editor

The original game used 16 bit graphic modes for performance reasons, so DirectX rendering system was optimized for this. 

This means that Map Editor needs to be run on a desktop with bit depth set to 16 bit to work correctly. As this is no longer possible, some shims are required:
* get DDrawCompat from https://github.com/narzoul/DDrawCompat/releases
* extract ddraw.dll into the path contains the Map Editor executable, either tmp/MapEditor/Win32/Debug if you're running from inside C++ Builder or in [bin](bin/) if you are running stand-alone.
* copy [etc/DDrawCompat/DDrawCompat.ini] into the same path as above.

## Quirks&Limitations

The tools (map editor and the like) are hardcoded to treat all images as PNGs, so PNG is the official format for graphics that are shared between engine and editors. This might be changed in the future by using a generic API to load graphic files in the tools (e.g. GDI+).

The license block at the beginning of some source files is still mentioning GPL. Please treat the license as LGPL.

Map Editor needs a compatibility shim to work on 32 bit desktops, see above. In the future, SDL 2 should be integrated with the Map Editor for consistency.

PNGDib library used by GPngLoad class is a custom version that uses PP::Stream interface. This is only for the DirectX system.

## Documentation

Documentation covering many aspects of the engine is available in [doc](doc/) folder.

Docs also contain an article written in 2006 for a magazine in Poland called Software Developer's Journal which is a tutorial on how to implement a factory building (a building that builds units, the staple of any RTS).

## Credits

Pure Power Tactical Engine was originally developed at [Piron Games](https://www.pirongames.com) as part of the Pure Power project by Doru "grab" Girba and Stefan "Karg" Dicu.

Big thanks to Mike "tamlin" Nordell for massive contributions to the code base.

Additional coding: Adrian "Kafka" Pop, Florin (sorry, forgot your full name).

Assets come from the original Pure Power project and were created by Bogdan "Dahn" Hodorog, Stefan "Karg" Dicu, Stelian Serban and Stefan "Max" Giurgiu.

## Media

| ![PPTactical Screenshot 001](web/0.9.6/ss/0.9.6/easyrecon_dustoff_m.jpg) | ![PPTactical Screenshot 002](web/0.9.6/ss/0.9.6/easyrecon_respiro_m.jpg) |
|:--:|:--:|
| ![PPTactical Screenshot 003](web/0.9.6/ss/0.9.5rc1/ppt_layered_building_and_aircraftsm.jpg) | ![PPTactical Screenshot 004](web/0.9.6/ss/0.9.0rc1/ppt004m.jpg) |
| ![PPTactical Screenshot 005](web/0.9.6/ss/0.8.5/e005m.jpg) | ![PPTactical Screenshot 006](web/0.9.6/ss/0.7.0/e001m.JPG) |
| ![PPTactical Screenshot 007](web/0.9.6/ss/0.7.0/e002m.JPG) | ![PPTactical Screenshot 008](web/0.9.6/ss/0.7.0/e003m.JPG) |
| ![PPTactical Screenshot 009](web/0.9.6/ss/0.4.0/e001m.jpg) | ![PPTactical Screenshot 010](web/0.9.6/ss/0.4.0/e002m.jpg) |
| ![PPTactical Screenshot 011](web/0.9.6/ss/0.4.0/e003m.jpg) | |

## License

Code is licensed under [LGPL license](https://www.gnu.org/licenses/lgpl-3.0.txt).

Please note that CSL license is GPL.

Assets are licensed under [Creative Commons Attribution-NonCommercial-ShareAlike](https://creativecommons.org/licenses/by-nc-sa/4.0/).