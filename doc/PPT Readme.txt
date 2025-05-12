Pure Power Tactical Engine Readme
----------------------------------

----->Introduction

Pure Power Tactical (in short PPTactical) is a 2D engine used for tactical/strategy
games. 

The source code provides a flexible framework on which to base the development. 
The source code release version is 1.0.0

The source code is released under GNU Lesser General Public License (see Copying.txt 
for details).


----->Running

IMPORTANT!!!!
Before running the Windows-versions of the engine windowed and the map editor, 
change the desktop color depth to 16 bit.

Just go to $(INSTALL_DIR)/bin and run the PPEngine_SDL.exe. I do not distribute
anymore executables from other projects (vcl, msvc, bcb directx or linux) because
of size isssues.

Use ALT+X to exit the engine in both modes, windowed or fullscreen. See $(INSTALL_DIR)
/docs/PPT Faq.doc for more info.

If you want to change the map, open $(INSTALL_DIR)/bin/PPEngine_Cfg.xml and seek the
TestMap section. Now make the changes you like. The maps must be located in 
$(INSTALL_DIR)/bin/data/Missions folder.

You may chose between windowed and fullscreen mode by using one of the lines
in $(INSTALL_DIR)/PPInGame.cfg:

  Renderer = "DIRECTXWINDOWED"
  or
  Renderer = "DIRECTX"

If you like to use the tools, you may find them in the same folder as the 
engine. There isn't a manual yet for them, so you may have to do a little
exploring by yourself. Or just ask at SourceForge project forums.


----->Compiling

Please note that tools projects are only C++Builder as they are based on VCL. That
is, they will only compile with C++Builder. Since latest version of C++Builder is
able to build executables for a range of platforms, they should be portable.

To compile the engine project on Windows, then:
· install Code::Blocks (http://www.codeblocks.org/) for Windows
· (optional) get the latest mingw compiler (7.2.0, at the time of this writing) from https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/ 
· (optional) in Code::Blocks, go to Settings -> Compiler -> Global Compiler Settings -> Toolchain executables and set the path of the compiler to the newest one
· make sure you have the appropriate SDL libs for Windows and MINGW (SDL2_image-2.0.1, SDL2_mixer-2.0.1, SDL2_ttf-2.0.14, SDL2-2.0.5)
· open $(INSTALL_DIR)/dev/codeblocks and build

To compile the engine project on Linux, then:
· install Code::Blocks (http://www.codeblocks.org/) for your platform
· make sure you have the appropriate SDL libs for your platform (SDL2_image-2.0.1, SDL2_mixer-2.0.1, SDL2_ttf-2.0.14, SDL2-2.0.5)
· open $(INSTALL_DIR)/dev/codeblocks and build
· IMPORTANT NOTE: in order to overcome the endianess problem, the Stream_ANSI_C class has been 
  added endian manipulation; since the data files are in little endian format, on big endian 
  platforms conversion on reading basic data types must be performed. To enable this conversion
  for this reads (readBool, readInt, readFloat, readDouble methods of Stream classes) please
  open Platform.h and uncomment the "#define PPT_BIGENDIAN_IO 1" line

To compile the tools, then:
· go to $(INSTALL_DIR)/bcb/Comps and open "PPTactical Components.bpg"
  with C++Builder
· compile it and install each of the project; this should install the required
  components into "PPTactical" pallete of C++Builder (script popup, open image dialog,
  extended image and http label)
· now open each of the tools project file into C++Builder (meditor.bpr, 
  soulless.bpr, unitsed.bpr, tileed.bpr, spred.bpr)
· perform the same path modifications you made to compile the engine
· by now, you should have all the tools executables in $(INSTALL_DIR)/bin


----->Scripting

There is a script reference manual. You may find it in $(INSTALL_DIR)/docs
folder.

Read the PPT Script Reference if you plan on writing infantry/vehicles/aircrafts bots 
scripts, statics scripts, mission scripts or battlegroup scripts.


----->Developing for PPT Engine

The documents are located in the $(INSTALL_DIR)/docs folder. 

Please read the PPT Engine Specs and PPT Project Layout documents before you 
proceed to write code.

----->Authors

PPTactical Engine was originally developed by Stefan "Karg" Dicu and Doru "grabX" Girba.

The following people left their mark on the code: Mike "tamlin" Nordell, Florin, Traian
"symbol shift" Pop, Adrian "Kafka" Pop, Stelian Serban.

Artwork comes from the original Pure Power project and was created by Dan Hodorog, 
Stefan "Karg" Dicu, Stelian Serban and Stefan "Max" Giurgiu.

----->Contact info

You may, of course, visit http://www.sf.net/projects/pptactical and use 
the forums there. The project administrator is Stefan "Karg" Dicu, and can 
be reached at karg@pirongames.com