//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     //
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: Pure Power

 descrition : all kinds of default paths and filenames
 last modify: 9 12 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PathsH
#define PathsH

#include "PP_String.h"

#define MAX_PP_PATH   32

//Soldiers - SGen
#define PATH_INF_DAT        "Data/Dats/"     // infantry directory
#define PATH_PILOT_DAT      "Data/Dats/"     // pilot directory
#define PATH_CREW_DAT       "Data/Dats/"     // crew directory
#define PATH_CMP_DAT        "Data/Dats/"     // crew directory
#define FILE_INF_DAT        "infantry.dat"     // infantry file
#define FILE_PILOT_DAT      "pilots.dat"       // pilot file
#define FILE_CREW_DAT       "crews.dat"        // crew file
#define FILE_CMP_DAT        "cmps.dat"         // comportament file
#define PATH_CHARACTERS_GFX "Data/Gfx/Characters/"
#define PATH_INFANTRY_GFX   "Data/Gfx/Soldiers/"

// Armories - Armory - folosite in equip.h [Karg]
#define PATH_IWEAPONS_DAT  "Data/Dats/"      // infantry weapon dir
#define PATH_VWEAPONS_DAT  "Data/Dats/"      // infantry weapon dir
#define PATH_IARMORS_DAT   "Data/Dats/"      // infantry weapon dir
#define PATH_VARMORS_DAT   "Data/Dats/"      // infantry weapon dir

#define FILE_IWEAPONS_DAT  "iwep.dat"
#define FILE_VWEAPONS_DAT  "vwep.dat"
#define FILE_IARMORS_DAT   "iarmor.dat"
#define FILE_VARMORS_DAT   "varmor.dat"

#define PATH_IWEAPONS_GFX  "Data/Gfx/Equipment/"
#define PATH_VWEAPONS_GFX  "Data/Gfx/Equipment/"
#define PATH_IARMORS_GFX   "Data/Gfx/Equipment/"
#define PATH_VARMORS_GFX   "Data/Gfx/Equipment/"

//vehicles - Armory - folosite in vehicles.h [Karg]
#define PATH_VEHICLES_GFX  "Data/Gfx/Units/"
#define PATH_VEHICLES_DAT  "Data/Dats/"
#define FILE_VEHICLES_DAT  "vehicles.dat"

// planets/solar systems - folosite in Solarsys.h
#define FILE_SYSTEMS_DAT   "systems.dat"
#define PATH_SYSTEMS_DAT   "Data/Dats/"
#define PATH_PLANETS_GFX   "Data/Gfx/Planets/"
#define FILE_SYSTEM_BMP    "system.bmp"

// enciclopedie - folosite in Enciclopedia.h
#define FILE_ENC_DAT       "ppenc.dat"
#define PATH_ENC_DAT       "Data/Dats/"
#define PATH_ENC_GFX       "Data/Gfx/Enciclopedia/"

// Interface gfx (for ppower)
#define PATH_INTERFACE_GFX "Data/Gfx/Interface/"

// GUI gfx
#define PATH_GUI_GFX "Data/Gfx/gui/"

// Portraits
#define PATH_PORTRAITS_GFX "Data/Gfx/Portraits/"

// Players paths directory
#define PP_PLAYERS_PATH "Players/"

// Scripts
#define PP_SCRIPTS_PATH "Scripts/"

//AI Units - Units Editor
#define PATH_AIINFANTRY    "Data/Dats/"
#define PATH_AIVEHICLE     "Data/Dats/"
#define PATH_AIAIR         "Data/Dats/"
#define FILE_AIINFANTRY    "aiinfantry.dat"
#define FILE_AIVEHICLE     "aivehicle.dat"
#define FILE_AIAIR         "aiair.dat"
#define PATH_INF_GFX       "Data/Gfx/Soldiers/"

// Explosions
#define PATH_EXPLODES_GFX  "Data/Gfx/Explode/"

// Proiectile
#define PATH_PROJECTILES_GFX "Data/Gfx/Projectiles/"

//Sounds
const CPString PATH_SFX = "Data/Sound/Sfx/";
const CPString PATH_MUSIC = "Data/Sound/Music/";

//Mission Editor
#define PATH_LANDSCAPE     "Data/Gfx/Landscape/"
#define FILE_MEDITOR_CFG   "PPME.cfg"
#define FILE_MEDITOR_DESKTOP_CFG   "PPMEDesktop.cfg"
#define PATH_MEDITOR       ""
#define PATH_PPTOOLS       ""

//Engine Test
#define FILE_ENGINETEST          "test.tpm"     // fisierul in care se salveaza
                                                // temporar din meditor

// PP Engine
#define PATH_INGAME              "Exe/"
const CPString PATH_INGAME_SFX = "Data/Sound/Sfx/In-game/";
#define PATH_INGAME_CFG          ""             // in the same dir as the executable
#define FILE_INGAME_CFG          "PPEngine_Cfg.xml"
#define PATH_INGAME_GFX          "Data/Gfx/In-game/"
#define FILE_MOUSE_GFX           "cursors.png"
#define PATH_MISSIONS            "Data/Missions/"

// Fonts
const CPString PATH_FONTS = "Data/Gfx/Fonts/";
const CPString FILE_FONT_SMALL = "EUROCAPS.ttf";
const CPString FILE_FONT_BIG = "MicroExtendFLF-Bold.ttf";

// Engine Scripting
const CPString PATH_SCRIPTS = "Data/Scripts/";
const CPString PATH_SCRIPTS_INFANTRY = "Data/Scripts/Infantry/";
const CPString PATH_SCRIPTS_VEHICLE = "Data/Scripts/Vehicle/";
const CPString PATH_SCRIPTS_AIRCRAFT = "Data/Scripts/Aircraft/";
const CPString PATH_SCRIPTS_STATICS = "Data/Scripts/Statics/";
const CPString PATH_SCRIPTS_BATTLEGROUP = "Data/Scripts/Battlegroup/";
const CPString PATH_SCRIPTS_MISSION = "Data/Scripts/Mission/";

// PP Interface
#define PATH_PPOWER_GFX          "Data/Gfx/PPower/"
#define PATH_PPOWER_SFX          "Data/Sound/Sfx/PPower/"

// GUI gfx
const CPString PATH_MISSION_THUMBNAILS = "Data/Gfx/missions/";

// Out (root for all the files exported by the game: save games, logs, screenshots, profiles). This
// path should be configurable
const CPString PATH_OUT = "out/";

// Saved games (relative to PATH_OUT)
const CPString PATH_SAVEDGAMES = "saves/";
const CPString FILE_QUICK_SAVE = "quick.sav";

// Player Profiles (relative to PATH_OUT)
const CPString PATH_PLAYER_PROFILE = "";
const CPString FILE_DEFAULT_PROFILE = "profile.xml";

// Screenshots (relative to PATH_OUT)
const CPString PATH_SCREENSHOTS = "screenshots/";

// Logs (relative to PATH_OUT)
const CPString PATH_LOG = "logs/";

//---------------------------------------------------------------------------
#endif
