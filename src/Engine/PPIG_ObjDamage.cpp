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

 description: Class Damage Object
 last modify: 10 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjDamage.h"
#include "PPIG_Mission.h"
#include "PPIG_ObjEffect.h"
#include "PPIG_SpaceSound.h"
#include "basic3d.h"
#include <math.h>
#include "PP_Stdlib.h"
#include "PPIG_SubSysDPropulsion.h"
#include "PP_Timer.h"
#include "PP_Graphic.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CIGDamageObject::CIGDamageObject() : CIGRealObject()
{
   VLOG(9) << "CIGDamageObject::CIGDamageObject";
   SetLayer(MOL_GROUNDDAMAGE);
   mStimulType   = -1;
   mSplashRadius = 0;
   mSourceObject = NULL;
   mTargetObject = NULL;
   mCoordZ       = 0;
   mPropulsion   = new CIGSubSysDPropulsion(this);

   SetUpdateTime(GetTimerInstance()->GetTime());
   SetLife(DEFAULT_DAMAGE_TIME_TO_LIVE);

   VLOG(9) << "CIGDamageObject::CIGDamageObject - over";
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGDamageObject::~CIGDamageObject()
{
  if (mPropulsion)
  {
    delete mPropulsion;
    mPropulsion = 0;
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className - the parent class
---------------------------------------------------------------------------*/
bool CIGDamageObject::IsDerivedFrom (CPString className)
{
  return (className == CIGDamageObject::ClassName() || CIGRealObject::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

void CIGDamageObject::AddAction(TIGSubSysActionData actionData)
{
  mPropulsion->AddAction(actionData);
}
//---------------------------------------------------------------------------

void CIGDamageObject::AddAction (CIGSubSysAction *Action)
{
  mPropulsion->AddAction(Action);
}
//---------------------------------------------------------------------------

void CIGDamageObject::AddActionWithPriority (CIGSubSysAction *Action)
{
  mPropulsion->AddActionWithPriority(Action);
}
//---------------------------------------------------------------------------

void CIGDamageObject::SetAction (TIGSubSysActionData actionData)
{
  mPropulsion->SetAction(actionData);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: updates the object
---------------------------------------------------------------------------*/
void CIGDamageObject::Update()
{
//   CIGRealObject::Update();
//  mPropulsion->OnActionAttack();

  // if the time elapsed since object creation has reached the time-to-live of
  // the object, remove it
  if (GetTimerInstance()->GetTime() - GetUpdateTime() > (unsigned)GetLife())
  {
    Explode();
    return;
  }

  // subsystems update
  mPropulsion->Update();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paints the sprite
---------------------------------------------------------------------------*/
void CIGDamageObject::Paint(CPPoint ptViewport, CPPoint ptWindow, int Width, int Height, int zoomLevel)
{
  if (GetVisible() == true)
  {
    if (GetSprite() != NULL)
    {
      GetGraphicInstance()->PaintSprite(GetCoord().x + ptViewport.x - ptWindow.x,
                                GetCoord().y + ptViewport.y - ptWindow.y - GetCoordZ(),
                                GetSprite(),
                                mPropulsion->GetAnimCounter()() / 2,
                                mPropulsion->GetAnimCounter()() % 2);
    }
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description:
---------------------------------------------------------------------------*/
void CIGDamageObject::Begin()
{
   //logWriteLn("CIGDamageObject::Begin");
   //mCurrentAngle = CenterRadianAngle(- GetCoord().x  + GetTargetPoint().x * MAPCELLX,
   //                                 + GetCoord().y  - GetTargetPoint().y * MAPCELLY);
   //mCurrentAngle = CenterRadianAngle(- GetCoord().x  + GetTargetObject()->GetClosestPointTo(GetLogicCoord()).x * MAPCELLX,
   //                                  - GetCoord().y  + GetTargetObject()->GetClosestPointTo(GetLogicCoord()).y * MAPCELLY);
   //logWriteFLn(mCurrentAngle);
   mPropulsion->SetCurrentAngle(CenterRadianAngle(- GetCoord().x  + GetTargetPoint().x * MAPCELLX, -GetCoord().y  + GetTargetPoint().y * MAPCELLY));
   mPropulsion->SetSenseCounter(gfGetComplexDirFromAngle(mPropulsion->GetCurrentAngle()));
   //logWriteILn(gfGetComplexDirFromAngle(GetCurrentAngle()));
}

/*---------------------------------------------------------------------------
 description: creates the target object
---------------------------------------------------------------------------*/
void CIGDamageObject::Explode()
{
   CIGEffectObject *lTargetEffect;
   lTargetEffect = new CIGEffectObject();
   lTargetEffect->SetAnimCyclesCount(0);
   lTargetEffect->SetSprite(GetEffectSprite());
   //lTargetEffect->CenterInPoint(CPPoint(GetLogicCoord().x * MAPCELLX + gfRandom(MAPCELLX), GetLogicCoord().y * MAPCELLY + gfRandom(MAPCELLY)));
   lTargetEffect->CenterInPoint(CPPoint(GetCoord().x, GetCoord().y));
   lTargetEffect->SetCoordZ(GetCoordZ());
//   (lTargetEffect->GetUpdateCounter().GetObject())->SetMaxValue(3);
   (lTargetEffect->GetUpdateCounter().GetThis())->SetMaxValue(3);
   lTargetEffect->NoteInLayerMap();
   GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lTargetEffect);  //???????
   RemoveFromLayerMap();
   SetVisible(false);
   GetMissionInstance()->AddToDeleteObjects(this);

   /*
   try
   {
      CIGSpaceSoundSystem *lSSSI;
      lSSSI = lSSSI->Instance();
      lSSSI->Play(GetSoundOnHit(), GetLogicCoord().x, GetLogicCoord().y);
   }
   catch(...)
   {
   }
   */
}

/*---------------------------------------------------------------------------
 description: creates the related stimul
---------------------------------------------------------------------------*/
void CIGDamageObject::CreateStimul()
{
   CIGObjectStimul lStimul(GetStimulType(), GetPower(), GetSourceObject(), GetSourceObject()->GetLogicCoord());

   // TODO: find a solution for collision
   // this is near the ground
   if (GetCoordZ() < 8)
   {
     GetMissionInstance()->NoteGroundStimul(GetSourceObject(), lStimul, GetLogicCoord().x, GetLogicCoord().y);
     return;
   }


   /*
   if (mSplashRadius == 0)
   {
     GetMissionInstance()->NoteGroundStimul(GetSourceObject(), lStimul, GetLogicCoord().x, GetLogicCoord().y);
     return;
   }

   for(int x = GetLogicCoord().x - mSplashRadius; x < GetLogicCoord().x + mSplashRadius + 1; x++)
   {
     for(int y = GetLogicCoord().y - mSplashRadius; y < GetLogicCoord().y + mSplashRadius + 1; y++)
     {
       GetMissionInstance()->NoteGroundStimul(GetSourceObject(), lStimul, x, y);
     }
   }
   */

  // otherwise notify the air objects
  GetMissionInstance()->NoteAirStimul(GetSourceObject(), lStimul, GetLogicCoord().x, GetLogicCoord().y, GetCoordZ());
}

void CIGDamageObject::Serialize(PP::Stream& _a)
{
  int         lSprEffectIndex = -1,
              lSprTrackIndex  = -1,
              lSprAfterIndex  = -1;
  CSerCounter lLife(mLife);
  CSerPoint   lTargetPoint(mTargetPoint);
  int         mSourceObjectId = -1,
              mTargetObjectId = -1;

  // setup
  if (mEffectSprite)
  {
    lSprEffectIndex = GetGraphicInstance()->GetSpriteIndex(mEffectSprite, SPRL_MISSION);
  }

  if (mTrackEffectSprite)
  {
    lSprTrackIndex = GetGraphicInstance()->GetSpriteIndex(mTrackEffectSprite, SPRL_MISSION);
  }

  if (mAfterEffectSprite)
  {
    lSprTrackIndex = GetGraphicInstance()->GetSpriteIndex(mAfterEffectSprite, SPRL_MISSION);
  }

  if (mSourceObject)
  {
    mSourceObjectId = mSourceObject->GetId();
  }

  if (mTargetObject)
  {
    mTargetObjectId = mTargetObject->GetId();
  }

  // parent class serialize
  CIGRealObject::Serialize(_a);

  // subsys deserialize
  mPropulsion->Serialize(_a);

  // serialize
  _a.writeInt(&lSprEffectIndex);
  _a.writeInt(&lSprTrackIndex);
  _a.writeInt(&lSprAfterIndex);
  _a.writeInt(&mPower);
  _a.writeInt(&mSoundOnHit);
  _a.writeInt(&mStimulType);
  _a.writeInt(&mSplashRadius);
  _a.writeInt(&mTrackPersistency);

  lLife.Serialize(_a);
  lTargetPoint.Serialize(_a);

  _a.writeInt(&mSourceObjectId);
  _a.writeInt(&mTargetObjectId);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGDamageObject::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGDamageObject::DeSerialize(PP::Stream& _a)
{
  int lSprEffectIndex, lSprTrackIndex, lSprAfterIndex;
  CSerCounter lLife;
  CSerPoint   lTargetPoint;
  int mSourceObjectId, mTargetObjectId;

  // parent class deserialize
  CIGRealObject::DeSerialize(_a);

  // subsys deserialize
  mPropulsion->DeSerialize(_a);

  // deserialization
  _a.readInt(&lSprEffectIndex);
  _a.readInt(&lSprTrackIndex);
  _a.readInt(&lSprAfterIndex);
  _a.readInt(&mPower);
  _a.readInt(&mSoundOnHit);
  _a.readInt(&mStimulType);
  _a.readInt(&mSplashRadius);
  _a.readInt(&mTrackPersistency);

  lLife.DeSerialize(_a);
  lTargetPoint.DeSerialize(_a);
  _a.readInt(&mSourceObjectId);
  _a.readInt(&mTargetObjectId);

  // setup
  mEffectSprite      = GetGraphicInstance()->GetSpriteByIndex(lSprEffectIndex, SPRL_MISSION);
  mTrackEffectSprite = GetGraphicInstance()->GetSpriteByIndex(lSprTrackIndex, SPRL_MISSION);
  mAfterEffectSprite = GetGraphicInstance()->GetSpriteByIndex(lSprAfterIndex, SPRL_MISSION);
  mLife              = lLife.Get();
  mTargetPoint       = lTargetPoint.Get();
  mSourceObject      = GetMissionInstance()->GetObjectById(mSourceObjectId);
  mTargetObject      = GetMissionInstance()->GetObjectById(mTargetObjectId);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
