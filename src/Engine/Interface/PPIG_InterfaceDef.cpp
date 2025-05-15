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
#include "PP_String.h"
#pragma hdrstop

#include "PPIG_InterfaceDef.h"
#include "PPIG_EngineDef.h"

CPPoint gMouseHotSpots[MOUSE_CURSORS_COUNT]=
                          {CPPoint(0,6),       //default
                           CPPoint(9,10),      //move
                           CPPoint(9,10),      //attack
                           CPPoint(12,23),     //scroll S
                           CPPoint(0,12),      //scroll W
                           CPPoint(23,12),     //scroll E
                           CPPoint(12,0),      //scroll N
                           CPPoint(0,23),      //scroll SW
                           CPPoint(23,23),     //scroll SE
                           CPPoint(0,0),       //scroll NW
                           CPPoint(23,0),      //scroll NE
                           CPPoint(0,6),       //on owned
                           CPPoint(0,6),       //on enemy
                           CPPoint(9,10),      //capture
                           CPPoint(9,10),      //patrol
                           CPPoint(9,10),      //load
                           CPPoint(9,10),      //unload
                           CPPoint(9,10),      // guard
                           CPPoint(9,10)       // land
                           };

int gCommandMasks[COMMAND_BUTTONS_COUNT] = {MA_STOP,      // stop
                                            MA_MOVE,      // move
                                            MA_ATTACK,    // attack
                                            MA_PATROL,    // patrol
                                            MA_GUARD,     // guard
                                            MA_HOLD,      // hold
                                            MA_CAPTURE,   // capture
                                            MA_LOAD,      // load
                                            MA_UNLOAD,    // unload
                                            MA_LAND,      // takeoff and land are complimentary actions
                                            MA_SUPPRESS_FIRE,
                                            MA_TAKE_COVER,
                                            MA_BUILD,
                                            MA_RALLY_POINT
                                            };

CPPoint gCommandBtnsCoord[COMMAND_BUTTONS_COUNT] =
                           {CPPoint(20, 30),
                            CPPoint(60, 30),
                            CPPoint(100, 30),
                            CPPoint(20, 70),
                            CPPoint(60, 70),
                            CPPoint(100, 70),
                            CPPoint(20, 110),
                            CPPoint(60, 110),
                            CPPoint(100, 110),
                            CPPoint(20, 110),
                            CPPoint(60, 110),
                            CPPoint(100, 110),
                            CPPoint(20, 30), // build
                            CPPoint(100, 110) // rally point
                           };

CPString gCommandHints[COMMAND_BUTTONS_COUNT] =
                           {"Stop",
                            "Move",
                            "Attack",
                            "Patrol",
                            "Guard",
                            "Hold",
                            "Capture",
                            "Load",
                            "Unload",
                            "Land",
                            "Supress fire",
                            "Take Cover",
                            "Build",
                            "Set Rally Point"
                           };
CPString gCommandBtnsFiles[COMMAND_BUTTONS_COUNT] =
                            {"btn_cmd_stop.png",
                            "btn_cmd_move.png",
                            "btn_cmd_attack.png",
                            "btn_cmd_patrol.png",
                            "btn_cmd_guard.png",
                            "btn_cmd_hold.png",
                            "btn_cmd_capture.png",
                            "btn_cmd_load.png",
                            "btn_cmd_unload.png",
                            "btn_cmd_land.png",
                            "btn_cmd_suppress.png",
                            "btn_cmd_hide.png",
                            "btn_cmd_capture.png", // build
                            "btn_cmd_move.png" // rally point
                           };
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

