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

 description: Class In Game - Effect Object
 last modify: 21 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjEffect.h"
#include "PPIG_Mission.h"
#include "PPIG_EngineDef.h"
#include "PP_Graphic.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGEffectObject::CIGEffectObject() : CIGBaseObject()
{
   mAnimType      = 0;
   mAnimCyclesCount = -1;
   SetLayer(MOL_AIREFFECTS);
   GetUpdateCounter().GetThis()->SetMaxValue(3);
   mActionStepCounter.Reset();
   mActionStepCounter.GetThis()->SetMaxValue(0);
//   mAnimCounter.SetMaxValue(7);
//   SetActionAnimTypes(OA_DIE, 0);
//   SetActionStepCounters(OA_DIE, 0);
//   TIGObjectActionData lData;
//   lData.mType = OA_DIE;
//   lData.mStartPoint = GetLogicCoord();
 //  lData.mTargetPoint = GetLogicCoord();
 //  SetAction(lData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGEffectObject::~CIGEffectObject()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGEffectObject::IsDerivedFrom (CPString className)
{
   return (className == CIGEffectObject::ClassName() || CIGBaseObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the sprite
---------------------------------------------------------------------------*/
void CIGEffectObject::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  if (GetVisible() == true)
  {
    if (GetSprite() != NULL)
    {
      GetGraphicInstance()->PaintSprite(
        (GetCoord().x + ptViewport.x - ptWindow.x)/zoomLevel,
        (GetCoord().y + ptViewport.y - ptWindow.y)/zoomLevel - GetCoordZ(),
         GetSprite(),
         mAnimCounter(), mAnimType +
         mActionStepCounter());
    }
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick message
---------------------------------------------------------------------------*/
void CIGEffectObject::Update ()
{
   VLOG(9) << "CIGEffectObject::Update ";

   //int lUC = GetUpdateCounter();
   //int lAC = mActionStepCounter;
   //int lCC = GetAnimCyclesCount();

   // no effect
   if (GetUpdateCounter().GetMaxValue() == 0)
   {
     RemoveFromLayerMap();
     GetMissionInstance()->AddToDeleteObjects(this);
     return;
   }

   if ((GetUpdateCounter()++).MaxReached() == true)
   //if ((mActionStepCounter++).MinReached() == true)
   //{
      if ((mAnimCounter++).MaxReached() == true)
      {
         if (GetAnimCyclesCount() > 0)
         {
           SetAnimCyclesCount(GetAnimCyclesCount() - 1);
         }
         else// (GetAnimCyclesCount() == 0)
         {
           RemoveFromLayerMap();
           GetMissionInstance()->AddToDeleteObjects(this);
           return;
         }
   //   }
   }

   VLOG(9) << "CIGEffectObject::Update - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the die action
---------------------------------------------------------------------------*/
void CIGEffectObject::OnActionDie()
{
}
//---------------------------------------------------------------------------

void CIGEffectObject::Serialize(PP::Stream& _a)
{
  CIGBaseObject::Serialize(_a);

  CSerInt     lAnimCyclesCount(mAnimCyclesCount);
  CSerCounter lAnimCounter(mAnimCounter);
  CSerCounter lActionStepCounter(mActionStepCounter);
  CSerInt     lAnimType(mAnimType);

  lAnimCyclesCount.Serialize(_a);
  lAnimCounter.Serialize(_a);
  lActionStepCounter.Serialize(_a);
  lAnimType.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGEffectObject::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGEffectObject::DeSerialize(PP::Stream& _a)
{
  CIGBaseObject::DeSerialize(_a);

  CSerInt     lAnimCyclesCount;
  CSerCounter lAnimCounter;
  CSerCounter lActionStepCounter;
  CSerInt     lAnimType;

  lAnimCyclesCount.DeSerialize(_a);
  lAnimCounter.DeSerialize(_a);
  lActionStepCounter.DeSerialize(_a);
  lAnimType.DeSerialize(_a);

  mAnimCyclesCount   = lAnimCyclesCount.Get();
  mAnimCounter       = lAnimCounter.Get();
  mActionStepCounter = lActionStepCounter.Get();
  mAnimType          = lAnimType.Get();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

