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
 application: PP IN GAME

 description: Defines-uri referitoare la in game interface
 last modify: 13 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPIG_InterfaceDefH
#define PPIG_InterfaceDefH

#include "basic2d.h"
#include "PP_String.h"

//type mouse buttons
typedef struct
{
   bool Left, Right, Middle;
}TMButtons;

typedef struct
{
   bool leftDown, leftUp, rightDown;
   CPPoint Cursor;
}TMouseMsg;

#define CELLX 32
#define CELLY 32

//constante de sound
#define AMBIENTALMUSIC 200

//cursoare de mouse
#define MOUSE_CURSORS_COUNT 19

#define MC_NONE             -1
#define MC_DEFAULT          0
#define MC_MOVE             1
#define MC_ATTACK           2
#define MC_SCROLLSOUTH      3
#define MC_SCROLLWEST       4
#define MC_SCROLLEAST       5
#define MC_SCROLLNORTH      6
#define MC_SCROLLSOUTHWEST  7
#define MC_SCROLLSOUTHEAST  8
#define MC_SCROLLNORTHWEST  9
#define MC_SCROLLNORTHEAST  10
#define MC_ONOWNED          11
#define MC_ONENEMY          12
#define MC_CAPTURE          13
#define MC_PATROL           14
#define MC_LOAD             15
#define MC_UNLOAD           16
#define MC_GUARD            17
#define MC_LAND             18
//mouse - done...

///////////////////////
//scroll constants
///////////////////////
#define SENSITIVESCROLLCOUNT 12
#define SS_NORTH        0
#define SS_NORTHEAST1   1
#define SS_NORTHEAST2   2
#define SS_EAST         3
#define SS_SOUTHEAST1   4
#define SS_SOUTHEAST2   5
#define SS_SOUTH        6
#define SS_SOUTHWEST1   7
#define SS_SOUTHWEST2   8
#define SS_WEST         9
#define SS_NORTHWEST1   10
#define SS_NORTHWEST2   11

///////////////////////
//constante de butoane
///////////////////////
#define COMMAND_BUTTONS_COUNT 14

//index pentru butoane
#define BTN_CMDSTOP           0
#define BTN_CMDMOVE           1
#define BTN_CMDATTACK         2
#define BTN_CMDPATROL         3
#define BTN_CMDGUARD          4
#define BTN_CMDHOLD           5
#define BTN_CMDCAPTURE        6
#define BTN_CMDLOAD           7
#define BTN_CMDUNLOAD         8
#define BTN_CMDTAKEOFF        9
#define BTN_CMDSUPRESSFIRE    10
#define BTN_CMDTAKECOVER      11

extern CPPoint gMouseHotSpots[MOUSE_CURSORS_COUNT];
extern int    gCommandMasks[COMMAND_BUTTONS_COUNT];
extern CPPoint gCommandBtnsCoord[COMMAND_BUTTONS_COUNT];
extern CPString  gCommandHints[COMMAND_BUTTONS_COUNT];
extern CPString  gCommandBtnsFiles[COMMAND_BUTTONS_COUNT];

//---------------------------------------------------------------------------
#endif

