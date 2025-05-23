//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     //
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------
 application: PP Mission Editor

 descrition : definitii de tipuri, constante si eventual variabile globale
 last modify: 10 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PPME_DefinesH
#define PPME_DefinesH

#include <system.hpp>

//denumiri de clase
#define CLASSBASEOBJECT    0
#define CLASSSTATICOBJECT  1
#define CLASSGROUNDUNIT    5
#define CLASSINFANTRYUNIT  2
#define CLASSVEHICLEUNIT   3
#define CLASSAIRUNIT       4
//au aceleasi valori ca si object type

//level de zoom
#define MINZOOMLEVEL 1
#define MAXZOOMLEVEL 2

//structura de item de tree
typedef struct {
      AnsiString fullName;    //fisierul dat
      AnsiString Path;        //calea catre dat
      AnsiString objName;     //numele obiectului
      int objType;            //tipul obiectului
      int Index;              //pentru aiunits, index in fisier
      bool isData;
}TTreeItem, TLandscapeTreeItem, TUnitsTreeItem;

//structura de masca
#define MAXMASK      20

typedef struct  {
      char Mask[MAXMASK][MAXMASK];      //masca
      int Width, Height;               //dimensiunile mastii
}TMask;

//Constante
#define APPLICATION_CAPTION "PP Map Editor"
#define NONAME_MAP "Noname Map"

//Object types
#define OT_NONE      -1
#define OT_TILE      0
#define OT_STATIC    1
#define OT_INFANTRY  2
#define OT_VEHICLE   3
#define OT_AIR       4

// file history
const unsigned int HISTORY_MENU_MAX_ITEMS = 5;

//---------------------------------------------------------------------------
#endif
