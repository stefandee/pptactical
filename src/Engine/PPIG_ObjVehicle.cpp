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

#include <math.h>
#pragma hdrstop

#include "basic3d.h"
#include "basepath.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_EngineDef.h"
//#include "PPIG_CScreen.h"
#include "PP_Stdlib.h"
#include "ZMathLib.h"

/*
#if defined(__BORLANDC__) || defined(__SUNPRO_CC)
#include <strstream>
#elif defined (__GNUG__)
#include <sstream>
#else
#include "strstrea.h"	// TMN: Note use of obsolete header
#endif
*/

#include "PPIG_ObjVehicle.h"
//---------------------------------------------------------------------------

CIGVehicle::CIGVehicle() : CIGUnit()
{
  VLOG(9) << "CIGVehicle::CIGVehicle - start";

  // senses
  //GetSenseCounter().SetMaxValue(7);//no of senses in the sprite

  // script stuff
  mModuleName = "V-C"; // vehicle compiler module name

  // default - derived classes will have to override this
  SetLogicDimX(1);
  SetLogicDimY(1);

  VLOG(9) << "CIGVehicle::CIGVehicle- over";
}
//---------------------------------------------------------------------------

CIGVehicle::~CIGVehicle()
{
  if (mValidCsl)
  {
    delete mCompiler;
  }
}
//---------------------------------------------------------------------------

bool CIGVehicle::IsDerivedFrom (CPString className)
{
  return ((CIGUnit::IsDerivedFrom(className)) || (className == CIGVehicle::ClassName()));
}
//---------------------------------------------------------------------------

void CIGVehicle::SetVehicleUnifiedData(TVehicleUnified& value)
{
  mVehicleUnifiedData = value;

  // setup basic variables
  SetMaxHP(value.hp);
  SetCurrentHP(value.hp);

  // setup the sounds
  // can_transport
}
//---------------------------------------------------------------------------

// Descriere   : for all vehicles, the dimensions in map are dependent of sprite size
// Param       :
// Rezultat    :
// Comentarii  :
void CIGVehicle::SetSprite (CPSprite * value)
{
  CIGUnit::SetSprite(value);

  // now compute the sizes - but only X logic dim will be used when computing the path movement
  // the sprite cell will always have to be a square :)
  if (value)
  {
    SetLogicDimX(value->GetAnimWidth() / MAPCELLX);
    SetLogicDimY(value->GetAnimHeight() / MAPCELLY);
  }

  //logWrite("Size is: "); logWriteILn(GetLogicDimX());
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGVehicle::Serialize(PP::Stream& _a)
{
  // parent class serialize
  CIGUnit::Serialize(_a);

  // write data
  //_a.write(&mVehicleUnifiedData, sizeof(TVehicleUnified));
  mVehicleUnifiedData.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGVehicle::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGVehicle::DeSerialize(PP::Stream& _a)
{
  // parent class deserialize
  CIGUnit::DeSerialize(_a);

  // read data
  //_a.read(&mVehicleUnifiedData, sizeof(TVehicleUnified));
  mVehicleUnifiedData.DeSerialize(_a);

  // setup
  SetVehicleUnifiedData(mVehicleUnifiedData);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif





