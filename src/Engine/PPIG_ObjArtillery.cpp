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

#pragma hdrstop

#include "PPIG_ObjArtillery.h"
#include "PPIG_SubSysArtilleryTurret.h"
//---------------------------------------------------------------------------

CIGArtilleryUnit::CIGArtilleryUnit() : CIGTankUnit()
{
  VLOG(5) << "CIGArtilleryUnit::CIGArtilleryUnit";

  // high speed
  SetSpeed(CPCounter(4));

  // visual setup
  //mVisual->SetMaxScanRange(6);
  //mVisual->SetFOV(60);

  // resetup turret
  delete mTurret;

  mTurret = new CIGSubSysArtilleryTurret(this);

  VLOG(5) << "CIGArtilleryUnit::CIGArtilleryUnit - over";
}
//---------------------------------------------------------------------------

CIGArtilleryUnit::~CIGArtilleryUnit()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className
---------------------------------------------------------------------------*/
bool CIGArtilleryUnit::IsDerivedFrom (CPString className)
{
   return ((CIGTankUnit::IsDerivedFrom(className)) || (className == CIGArtilleryUnit::ClassName()));
}
//---------------------------------------------------------------------------

void CIGArtilleryUnit::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddAction(actionData);
      break;

    case OA_STOP:
      mTurret->AddAction(actionData);
      mPropulsion->AddAction(actionData);
      break;

    default:
      mPropulsion->AddAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGArtilleryUnit::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddAction(Action);
      break;

    case OA_STOP:
      mTurret->AddAction(Action);
      mPropulsion->AddAction(Action);
      break;

    default:
      mPropulsion->AddAction(Action);
  }
}
//---------------------------------------------------------------------------

void CIGArtilleryUnit::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->AddActionWithPriority(Action);
      break;

    case OA_STOP:
      mTurret->AddActionWithPriority(Action);
      mPropulsion->AddActionWithPriority(Action);
      break;

    default:
      mPropulsion->AddActionWithPriority(Action);
  }
}
//---------------------------------------------------------------------------

void CIGArtilleryUnit::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_ATTACK:
    case OA_ATTACKGROUND:
      mTurret->SetAction(actionData);
      break;

    case OA_STOP:
      mTurret->SetAction(actionData);
      mPropulsion->SetAction(actionData);
      break;

    default:
      mPropulsion->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

