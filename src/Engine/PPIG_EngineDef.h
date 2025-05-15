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
 application: Pure Power IN GAME

 description: In game - engine defines
 last modify: 10 06 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_EngineDefH
#define PPIG_EngineDefH

#include "basic2d.h"
#include "PP_basetypes.h"

#define PI 3.14159265358979

const int AIRCRAFT_Z_LEVEL = 64;

int gfGetDirFromPoints(CPPoint ptStart, CPPoint ptEnd);
int gfGetDirFromAngle(float Angle);
int gfGetComplexDirFromAngle(float Angle);
int gfGetComplexDirFromPoints(CPPoint ptStart, CPPoint ptEnd);
int gfGetDist2D(CPPoint pt1, CPPoint pt2);

/////////////////////////////////////////////////////////////////////////////
//GLOBAL DATA SECTION
/////////////////////////////////////////////////////////////////////////////
//simple directions
extern CPPoint gDir[8];
extern CPPoint gReverseDir[8];
extern float  gDirAngle[8];
//--------------

//complex directions
extern float  gComplexDirAngle[16];
//--------------

//side colors
extern TColor32 gSideColor[4];
//--------------

/////////////////////////////////////////////////////////////////////////////
//GENERIC SECTION
/////////////////////////////////////////////////////////////////////////////
//Class Names
#define IGCN_BASEOBJECT       "Base Object"
#define IGCN_BASETILE         "Base Tile"
#define IGCN_EFFECTOBJECT     "Effect Object"
#define IGCN_REALOBJECT       "Real Object"
#define IGCN_UNIT             "Unit"

// buildings
#define IGCN_STATICOBJECT     "Static Object"
#define IGCN_FACTORYOBJECT    "Factory Object"

// Karg - CIGGroundUnit has been merged with CIGUnit
//#define IGCN_GROUNDUNIT       "Ground Unit"

#define IGCN_AIRUNIT          "Air Unit"
#define IGCN_FIGHTERUNIT      "Fighter Unit"
#define IGCN_BOMBERUNIT       "Bomber Unit"
#define IGCN_CHOPPERUNIT      "Chopper Unit"
#define IGCN_DROPSHIPUNIT     "Dropship Unit"
#define IGCN_INFANTRYUNIT     "Infantry Unit"
#define IGCN_VEHICLEUNIT      "Vehicle Unit"
#define IGCN_TANKUNIT         "Tank Unit"
#define IGCN_RECONUNIT        "Recon Unit"
#define IGCN_TRANUNIT         "Tran Unit"
#define IGCN_ARTILLERYUNIT    "Artillery Unit"
#define IGCN_DAMAGEOBJECT     "Damage Object"
#define IGCN_BULLETOBJECT     "Bullet Object"
#define IGCN_ROCKETOBJECT     "Rocket Object"
#define IGCN_MISSILEOBJECT    "Missile Object"
#define IGCN_PROJECTILEOBJECT "Projectile Object"
#define IGCN_LASEROBJECT      "Laser Object"
#define IGCN_PLASMAOBJECT     "Plasma Object"
#define IGCN_EFFECTOBJECT     "Effect Object"
#define IGCN_BALLISTICOBJECT  "Ballistic Object"
//--------------

//Stimul types
#define STIMUL_TYPE_COUNT    8
#define ST_BULLET            0
#define ST_EXPLODE           1
#define ST_FIRE              2
#define ST_LASER             3
#define ST_PLASMA            4
#define ST_NOISE_WEAPON_FIRE 5
#define ST_NOISE_WEAPON_HIT  6
#define ST_NOISE_MOVEMENT    7
//--------------

/////////////////////////////////////////////////////////////////////////////
//MISSION SECTION
/////////////////////////////////////////////////////////////////////////////
//Mission object list
#define MISSION_OBJECT_LISTS_COUNT 13
#define MOL_ALLOBJECTS           0
#define MOL_OWNEDUNITS           1
#define MOL_OWNEDSTATICS         2
#define MOL_FRIENDLYUNITS        3
#define MOL_FRIENDLYSTATICS      4
#define MOL_ENEMYUNITS           5
#define MOL_ENEMYSTATICS         6
#define MOL_NEUTRALUNITS         7
#define MOL_NEUTRALSTATICS       8
#define MOL_SELECTEDOWNEDUNITS   9
#define MOL_SELECTEDOWNEDOTHERS  10
#define MOL_SELECTEDOTHERS       11
#define MOL_TRANSPORTEDUNITS     12 // this layer is used for units that may exist
                                    // outside the map (sort of ;); MOL_ALLOBJECTS
                                    // and this layer are complimentary
//--------------

//Mission Flags
#define MISSION_FLAGS_COUNT 10
#define MF_ADDSELECTION    0
#define MF_INVERTSELECTION 1
#define MF_ADDACTION       2
#define MF_CAMERAON        3
#define MF_TEAMON          4
#define MF_HEALTHON        5
#define MF_PRESERVEFORM    6 //preserve formation
//--------------

//Mission Actions
#define MA_DEFAULT       0
#define MA_STOP          1
#define MA_MOVE          2
#define MA_ATTACK        3
#define MA_GUARD         4
#define MA_PATROL        5
#define MA_HOLD          6
#define MA_CAPTURE       7
#define MA_LOAD          8
#define MA_UNLOAD        9
#define MA_SELECT        10  // the user wants to select
#define MA_SELECTING     11  // means that selecting action begun
#define MA_TAKEOFF       12  // not used at the moment...
#define MA_CANCEL        13  // is the same as default ?
#define MA_SUPPRESS_FIRE 14  // sort of attack ground for infantry units
#define MA_TAKE_COVER    15  // move to the closest hideout
#define MA_UNLOAD_ALL    16  // all units in a transport vehicle get off
#define MA_LAND          17  // an air unit lands action - land and takeoff are complimentary actions

// TEST CODE FOR SDJ
#define MA_BUILD         18  // testing factory build
#define MA_RALLY_POINT   19  // rally point for factory
//--------------

/////////////////////////////////////////////////////////////////////////////
//MAP SECTION
/////////////////////////////////////////////////////////////////////////////
//Map
#define MAXMAPWIDTH     250
#define MAXMAPHEIGHT    250
#define MAPCELLX        32
#define MAPCELLY        32
//--------------

//Map Object Layers
#define OBJECT_LAYERS_COUNT 8 //no of object lists in a map cell
#define MOL_GROUNDOVERLAY  0
#define MOL_GROUNDUNIT     1
#define MOL_GROUNDOBJECT   2
#define MOL_GROUNDEFFECTS  3
#define MOL_GROUNDDAMAGE   4
#define MOL_AIRDAMAGE      5
#define MOL_AIROBJECTS     6
#define MOL_AIREFFECTS     7
#define MOL_BASETILE       10 //this is not an index in the array
//--------------

/////////////////////////////////////////////////////////////////////////////
//OBJECTS SECTION
/////////////////////////////////////////////////////////////////////////////

//Object Actions
const int OBJECT_ACTION_COUNT = 28;
const int OA_DEFAULT      = 0;
const int OA_STOP         = 1;
const int OA_MOVE         = 2;
const int OA_ATTACK       = 3;
const int OA_PATROL       = 4;
const int OA_GUARD        = 5;
const int OA_HOLD         = 6;
const int OA_CAPTURE      = 7;
const int OA_LOAD         = 8;
const int OA_UNLOAD       = 9;
const int OA_INFILTRATE   = 10; //?
const int OA_DIE          = 11;
const int OA_DEAD         = 12;
const int OA_ATTACKMOVE   = 13;
const int OA_ATTACKGROUND = 14;
const int OA_TAKECOVER    = 15;
const int OA_ROTATE       = 16;
const int OA_UNLOAD_ALL   = 17;
const int OA_PICKUP       = 18;
const int OA_LAND         = 19;
const int OA_TAKEOFF      = 20;

// TEST CODE FOR SDJ
const int OA_BUILD        = 21;
const int OA_RALLY_POINT  = 22;
//--------------

const int OA_TACTIC_MOVE         = 23;
const int OA_TACTIC_ATTACK       = 24;
const int OA_TACTIC_ATTACKGROUND = 25;
const int OA_TACTIC_PATROL       = 26;
const int OA_TACTIC_STOP         = 27;

// Object side - these are battle group id's
// #define OS_NEUTRAL   0
// #define OS_OWNED     1
// #define OS_FRIENDLY  2
// #define OS_ENEMY     3
#define OS_LANDSCAPE 1
#define OS_PLAYER    2
#define OS_BGROUP1   3
#define OS_BGROUP2   4
#define OS_BGROUP3   5
#define OS_BGROUP4   6
#define OS_BGROUP5   7
#define OS_BGROUP6   8
#define OS_BGROUP7   9
#define OS_BGROUP8   10
const int OS_UNDEFINED = -1;
//--------------

// Politics
enum TPoliticsRelations { REL_NEUTRAL = 0, REL_FRIENDLY, REL_OWNED, REL_ENEMY, REL_UNKNOWN };
//--------------

//Way related
#define WAY_MAX_LENGTH 100
#define WAY_SEARCHING  -2
#define WAY_OVER       -1
//--------------

// Can (units capabilities)
const int CAN_CARRY_INFANTRY = 1;
const int CAN_CARRY_VEHICLES = 2;
const int CAN_UNLOAD         = 3;
const int CAN_LOAD           = 4;
const int CAN_GUARD          = 5;

/////////////////////////////////////////////////////////////////////////////
//SPRITES SECTION
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#endif

