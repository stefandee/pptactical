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

 descrition : diferite utilitare
 last modify: 16 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef ToolsH
#define ToolsH

#include "Statics.h"
#include "VehicleX.h"
#include "Soldier.h"
#include "Tile.h"
#include "PPME_Defines.h"
#include "AIUnits.h"

class CIGInfantryUnit;
class CIGVehicle;
class CIGMission;

void setMEError(int error);

int getObjectType(AnsiString file);
AnsiString getObjectName(AnsiString file);
AnsiString getObjectSpriteName(AnsiString file);
AnsiString getObjectSpriteName(AnsiString filename, int objtype, int index);
TMask getObjectMask(AnsiString file);
TMask getObjectMask(hStaticObject objStatic);
CAuxMapChunk* getMapChunk(AnsiString file);
bool getStaticObjectStruct(AnsiString filename, struct _StaticObject* _static);
//hStaticObject getStaticObjectStruct(AnsiString filename);
bool getAIInfantryUnitStruct(AnsiString filename, int index, TInfantryUnified* _inf);
bool getAIVehicleUnitStruct(AnsiString filename, int index, TVehicleUnified* _veh);
AnsiString GetVehicleType(int _type);
//TInfantryUnified getAIInfantryUnitStruct(AnsiString filename, int index);

CIGInfantryUnit* createInfantryUnit(CIGMission* _mission, AnsiString filename, int index, int _unitId, int _bgId, int _scriptId);
CIGVehicle*      createVehicleUnit (CIGMission* _mission, AnsiString filename, int index, int _unitId, int _bgId, int _scriptId);

/////////////////////////////////////////////////////////////////////////////
//GRAFICA////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/*
void drawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart);
void drawBitmap(HDC hdc, HDC hdcSrc, int xStart, int yStart, int width, int height);
void drawBitmap(HDC hdc, HBITMAP hBitmap, HBITMAP hMaskBitmap, int xStart, int yStart);
void drawBitmap(HDC hdc, HDC hdcSrc, HDC hdcMask, int xStart, int yStart, int width, int height);
*/


//---------------------------------------------------------------------------
#endif
