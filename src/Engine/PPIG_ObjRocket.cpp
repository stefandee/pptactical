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
 application: Pure Power In Game

 description: Class In Game Rocket
 last modify: 17 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjRocket.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "basepath.h"
#include "PPIG_EngineDef.h"
#include "PPIG_Mission.h"
#include "PPIG_ObjEffect.h"
#include "basic3d.h"
#include "PPIG_SubSysRPropulsion.h"
#include <math.h>

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGRocketObject::CIGRocketObject() : CIGDamageObject()
{
  CPGIGraphicSystem *lGSI;
  lGSI = GetGraphicInstance();
  SetTrackEffectSprite(lGSI->AddSprite(CPString(PATH_EXPLODES_GFX) + "smoke01.png", 8, 1, SPRL_MISSION));
  SetSpeed(CPCounter(8));
  //GetSenseCounter().SetMaxValue(15); // Karg: modified, was 7
  //GetAnimCounter().SetMaxValue(15);
  SetPower(10);
  SetStimulType(ST_EXPLODE);

  if (mPropulsion)
  {
    delete mPropulsion;
  }

  mPropulsion = new CIGSubSysRPropulsion(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGRocketObject::IsDerivedFrom (CPString className)
{
   return (className == CIGRocketObject::ClassName() || CIGDamageObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

void CIGRocketObject::Begin()
{
  CIGDamageObject::Begin();

  // little hack, you piggy little hack :)
  mPropulsion->GetSenseCounter().GetThis()->SetMaxValue(15);
  mPropulsion->GetAnimCounter().GetThis()->SetMaxValue(15);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the sprite
---------------------------------------------------------------------------*/
void CIGRocketObject::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
   if (GetVisible() == true)
   {
      if (GetSprite() != NULL)
      {
         GetGraphicInstance()->PaintSprite(GetCoord().x + ptViewport.x - ptWindow.x,
                                    GetCoord().y + ptViewport.y - ptWindow.y - GetCoordZ(),
                                    GetSprite(),
                                    //mOrientation / 2, mOrientation % 2);
                                    mPropulsion->GetAnimCounter()() / 2, mPropulsion->GetAnimCounter()() % 2);
      }
   }
}
//---------------------------------------------------------------------------


void CIGRocketObject::Serialize(PP::Stream& _a)
{
  // parent serialize
  CIGDamageObject::Serialize(_a);

  // this serialize
  //_a.writeInt(&mOrientation);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGRocketObject::DataSize()
{
  return sizeof(__int32) + CIGDamageObject::DataSize();
}
//---------------------------------------------------------------------------

void CIGRocketObject::DeSerialize(PP::Stream& _a)
{
  // parent serialize
  CIGDamageObject::DeSerialize(_a);

  // this serialize
  //_a.readInt(&mOrientation);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
