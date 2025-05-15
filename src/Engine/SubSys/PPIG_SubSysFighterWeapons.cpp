//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
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
//---------------------------------------------------------------------------

#pragma hdrstop

#include "PPIG_SubSysFighterWeapons.h"
//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjPlasma.h"
#include "PPIG_ObjBallistic.h"
#include "PPIG_WeaponRules.h"
#include "PPIG_SpaceSound.h"
//---------------------------------------------------------------------------

CIGSubSysFighterWeapons::CIGSubSysFighterWeapons(CIGFighterUnit* _parent) : CIGSubSysBase(_parent)
{
  // turret specific vaiables
  mDir         = 0;
  mSense       = 0;
  mWantedSense = 0;
  mFlag        = 0;

  // set update counters
  GetUpdateCounters(OA_ATTACK)           .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKGROUND)     .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKMOVE)       .GetThis()->SetMaxValue(1);

  // action step counters
  GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACK)       .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKGROUND) .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);

  // action anim types (the line in sprite)
  //SetActionAnimTypes(OA_STOP, 1);
  //SetActionAnimTypes(OA_ATTACKMOVE, 2);
  //SetActionAnimTypes(OA_ATTACK, 2);
  //SetActionAnimTypes(OA_ATTACKGROUND, 2);
}
//---------------------------------------------------------------------------

CIGSubSysFighterWeapons::~CIGSubSysFighterWeapons()
{
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysFighterWeapons::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_STOP:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionStop(this, actionData);
         }
         break;
      case OA_ATTACK:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionAttack(this, actionData);
         }
         break;

      case OA_ATTACKGROUND:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionAttackGround(this, actionData);
         }
         break;
   }

   return lTmpAction;
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::OnActionStop   ()
{
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::OnActionAttack ()
{
  CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();

  VLOG(9) << "CIGSubSysFighterWeapons::OnActionAttack";

  // if the weapon is not enabled, just stay
  if (!lParent->GetAircraftUnifiedData().wep1_enabled)
  {
    VLOG(2) << "CIGSubSysFighterWeapons::OnActionAttack - weapon 1 is not enabled.";
    return;
  }

  if (GetCurrentAction()->GetData().mTargetObject->GetIsDead() == true)
  {
    GetCurrentAction()->End();
    return;
  }

  // check if the Fighter may fire the object
  if (!CIGWeaponRules().CanFireObject(lParent, lParent->GetAircraftUnifiedData().wep1_type, GetCurrentAction()->GetData().mTargetObject))
  {
    GetCurrentAction()->End();
    return;
  }

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord));

  // a weapon has a minimum range and a maximum range - check them before turn the Fighter and fire
  if ((lDistance >= lParent->GetAircraftUnifiedData().wep1_minrange) && (lDistance <= lParent->GetAircraftUnifiedData().wep1_maxrange))
  {
     SetCurrentActionIndex(OA_ATTACK);
     GetActionStepCounters(OA_ATTACK) = 0;

     int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetObject->GetLogicCoord());

     if (lParent->GetSenseCounter() == lTmpSense)
     {
        Fire(); // creates the damage object

        mDir  = 0;
        mFlag = 0;
     }
     //else
     //{
        //SetCurrentActionIndex(OA_STOP); // stop and turn to sense
     //   SetWantedSense(lTmpSense);
        //OnActionRotate();
     //   ComputeDir();
     //   Turn();
     //}
  }
  else
  {
    //
    // The fighter is not close enough to fire, relocate it
    // sequence 2 actions: a move and an attack
    //
    if (mFlag != APPROACH)
    {
      CPPoint lTarget = GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord);

      int lRange = lParent->GetAircraftUnifiedData().wep1_minrange + (lParent->GetAircraftUnifiedData().wep1_maxrange - lParent->GetAircraftUnifiedData().wep1_minrange) / 2;

      lTarget.x = (lTarget.x + lRange) * MAPCELLX;
      lTarget.y = (lTarget.y + lRange) * MAPCELLY;

      CIGSubSysAction* lAttackAction = GetCurrentAction();
      CIGSubSysAction* lMoveAction = GetCurrentAction();

      lMoveAction->SetType(OA_MOVE);
      lMoveAction->SetTargetPoint(lTarget);

      GetCurrentAction()->End();
      lParent->SetAction(lMoveAction->GetData());
      lParent->AddAction(lAttackAction->Clone());

      mFlag = APPROACH;
    }
  }

  VLOG(9) << "CIGSubSysFighterWeapons::OnActionAttack - over";
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::OnActionAttackGround()
{
  CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();

  VLOG(9) << "CIGSubSysFighterWeapons::OnActionAttack";

  // if the weapon is not enabled, just stay
  if (!lParent->GetAircraftUnifiedData().wep1_enabled)
  {
    VLOG(2) << "CIGSubSysFighterWeapons::OnActionAttackGround - weapon 1 is not enabled.";
    return;
  }

  // check if the Fighter may fire the object
  if (!CIGWeaponRules().CanFireGround(lParent, lParent->GetAircraftUnifiedData().wep1_type))
  {
    GetCurrentAction()->End();
    return;
  }

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetPoint);

  // a weapon has a minimum range and a maximum range - check them before turn the fighter and fire
  if ((lDistance >= lParent->GetAircraftUnifiedData().wep1_minrange) && (lDistance <= lParent->GetAircraftUnifiedData().wep1_maxrange))
  {
     SetCurrentActionIndex(OA_ATTACKGROUND);
     GetActionStepCounters(OA_ATTACKGROUND) = 0;

     int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetPoint);

     if (lParent->GetSenseCounter() == lTmpSense)
     {
        Fire(); // creates the damage object
        mDir = 0;
     }
     else
     {
        SetWantedSense(lTmpSense);
        ComputeDir();
        Turn();
     }
  }
  else
  {
    Stop();
  }

  VLOG(9) << "CIGSubSysFighterWeapons::OnActionAttackGround - over";
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::Turn()
{
  int lSense = ((CIGFighterUnit*)GetParent())->GetSenseCounter();

  if (mDir == 0)
  {
    return;
  }

  if (lSense != mWantedSense)
  {
    lSense += mDir;

    if (lSense < 0)
    {
      lSense = 7;
    }

    if (lSense > 7)
    {
      lSense = 0;
    }

    // ask the parent to turn
    ((CIGFighterUnit*)GetParent())->SetSenseCounter(lSense);

    //int lV = ((CIGFighterUnit*)GetParent())->GetSenseCounter();
    //lV++;
  }
  else
  {
    mDir = 0;
  }
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::ComputeDir()
{
  int lSense = ((CIGFighterUnit*)GetParent())->GetSenseCounter();

  if (lSense == mWantedSense)
  {
    mDir = 0;
    return;
  }

  if (lSense < mWantedSense)
  {
    if (mWantedSense - lSense < 7 - mWantedSense + lSense)
    {
      mDir = 1;
    }
    else
    {
      mDir = -1;
    }
  }
  else
  {
    if (-mWantedSense + mSense < 7 - lSense + mWantedSense)
    {
      mDir = -1;
    }
    else
    {
      mDir = 1;
    }
  }
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::Fire()
{
  CIGFighterUnit* lParent = (CIGFighterUnit*)GetParent();

  if (GetCurrentActionIndex() == OA_ATTACK)
  {
    if (!GetUpdateCounters(OA_ATTACK).MaxReached())
    {
      return;
    }
  }

  if (GetCurrentActionIndex() == OA_ATTACKGROUND)
  {
    if (!GetUpdateCounters(OA_ATTACKGROUND).MaxReached())
    {
      return;
    }
  }

  CIGDamageObject *lObj = NULL;

  // create projectiles accordingly to the weapon wep1_projectiles_per_shot parameter
  for(int i = 0; i < lParent->GetAircraftUnifiedData().wep1_projectiles_per_shot; i++)
  {
    switch(lParent->GetAircraftUnifiedData().wep1_type)
    {
      case VWT_AG_MISSILE:
        lObj = new CIGRocketObject();
        lObj->SetSpeed(CPCounter(10));
        break;

      case VWT_AIR_BOMB:
        break;

      case VWT_AA_MISSILE:
        lObj = new CIGRocketObject();
        lObj->SetSpeed(CPCounter(10));
        break;

      case VWT_AIR_LASER:
        break;

      case VWT_AIR_CANNON:
        lObj = new CIGBulletObject();
        lObj->SetSpeed(CPCounter(20));
        break;
    }

    if (!lObj)
    {
      return;
    }

    CPPoint lFireHole     = lParent->GetAircraftUnifiedData().primary[lParent->GetSenseCounter()];
    CPPoint lBodyMount    = lParent->GetAircraftUnifiedData().body[lParent->GetSenseCounter()];
    CPPoint lTurretMount  = lParent->GetAircraftUnifiedData().turret[lParent->GetSenseCounter()];
    CPPoint lCoord        = lParent->GetCoord();
    int lSpriteWidth      = 0, lSpriteHeight = 0;
    CPSprite* lProjSprite = lParent->GetProjectileSprite();

    if (lProjSprite)
    {
      lSpriteWidth  = lProjSprite->GetAnimWidth();
      lSpriteHeight = lProjSprite->GetAnimHeight();
    }

    lObj->SetCoord(CPPoint(lCoord.x + lBodyMount.x - lTurretMount.x + lFireHole.x - lSpriteWidth/2,
                           lCoord.y + lBodyMount.y - lTurretMount.y + lFireHole.y - lSpriteHeight/2));
    lObj->SetCoordZ(lParent->GetCoordZ());

    //lObj->SetSenseCounter(0);
    lObj->NoteInLayerMap();
    lObj->SetSourceObject(lParent);
    lObj->SetPower(lParent->GetAircraftUnifiedData().wep1_damage);
    lObj->SetSprite(lParent->GetProjectileSprite());
    lObj->SetEffectSprite(lParent->GetEffectSprite());
    //lObj->SetSoundOnHit(lParent->GetSoundOnHit());
    lObj->SetLife(lParent->GetAircraftUnifiedData().mWep1_TimeToLive);
    lObj->SetTrackPersistency(lParent->GetAircraftUnifiedData().wep1_trackPersistency);

    TIGSubSysActionData lTmpActionData;

    switch(GetCurrentActionIndex())
    {
      case OA_ATTACK:
        lObj->SetTargetObject(GetCurrentAction()->GetData().mTargetObject);
        lObj->SetTargetPoint(GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lParent->GetLogicCoord()));
        lObj->SetTargetCoordZ(GetCurrentAction()->GetData().mTargetObject->GetCoordZ());

        // reset the counter
        GetUpdateCounters(OA_ATTACK) = 0;

        // set the attack action for the damage object...
        // ...this will further go to the propulsion of the damage object
        lTmpActionData.mType = OA_ATTACK;
        lObj->SetAction(lTmpActionData);
        break;

      case OA_ATTACKGROUND:
        lObj->SetTargetObject(NULL);
        lObj->SetTargetPoint(GetCurrentAction()->GetData().mTargetPoint);
        lObj->SetTargetCoordZ(GetMissionInstance()->GetMap()->GetAltitude(GetCurrentAction()->GetData().mTargetPoint.x, GetCurrentAction()->GetData().mTargetPoint.y));

        // reset the counter
        GetUpdateCounters(OA_ATTACKGROUND) = 0;

        // set the attack action for the damage object...
        // ...this will further go to the propulsion of the damage object
        lTmpActionData.mType = OA_ATTACKGROUND;
        lObj->SetAction(lTmpActionData);
        break;
    }

    lObj->Begin();
    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Add(lObj); //!!!!!!!!!!!!!!!
  }

  /*
  try
  {
     CIGSpaceSoundSystem* lSSSI = CIGSpaceSoundSystem::Instance();
     lSSSI->Play(lParent->GetSoundOnFire(), lParent->GetLogicCoord().x, lParent->GetLogicCoord().y);
  }
  catch(...)
  {
  }
  */
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::Serialize(PP::Stream& _a)
{
  CIGSubSysBase::Serialize(_a);

  _a.writeInt(&mSense);
  _a.writeInt(&mWantedSense);
  _a.writeInt(&mDir);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysFighterWeapons::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysFighterWeapons::DeSerialize(PP::Stream& _a)
{
  CIGSubSysBase::DeSerialize(_a);

  _a.readInt(&mSense);
  _a.readInt(&mWantedSense);
  _a.readInt(&mDir);
}
//---------------------------------------------------------------------------

