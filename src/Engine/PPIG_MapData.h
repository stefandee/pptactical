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
 application: PP Mission Editor, PP Engine, PP Engine Test

 descrition : map types definitions, in fact it's a kind of mission def
              but that file already exists :(
 last modify: 7 12 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef MapDataH
#define MapDataH

//Object types
#define OBJT_NONE           -1
#define OBJT_TILE           0
#define OBJT_STATIC         1
#define OBJT_INFANTRY       2
#define OBJT_FIGHTER        3
#define OBJT_TANK           4
#define OBJT_ARTILLERY      5
#define OBJT_RECON          6
#define OBJT_TRAN           7
#define OBJT_CHOPPER        8
#define OBJT_DROPSHIP       9
#define OBJT_BOMBER         10
#define OBJT_ROCKET         11
#define OBJT_EFFECT         12
#define OBJT_LASER          13
#define OBJT_BULLET         14
#define OBJT_BALLISTIC      15
#define OBJT_STATIC_FACTORY 16

// Map tile types
enum TTileType { MTT_WATER = 0, MTT_SHORE, MTT_LAND, MTT_UNKNOWN };

//---------------------------------------------------------------------------
#endif
