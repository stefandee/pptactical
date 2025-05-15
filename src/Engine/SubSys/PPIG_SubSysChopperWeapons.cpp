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
#include <cmath>
#pragma hdrstop

#include "PPIG_SubSysChopperWeapons.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjPlasma.h"
#include "PPIG_ObjBallistic.h"
#include "PPIG_WeaponRules.h"
#include "PPIG_SpaceSound.h"

//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

CIGSubSysChopperWeapons::CIGSubSysChopperWeapons(CIGChopperUnit* _parent) : CIGSubSysBase(_parent)
{
  // turret specific vaiables
  mDir         = 0;
  mSense       = 0;
  mWantedSense = 0;

  // set update counters
  GetUpdateCounters(OA_ATTACK)           .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKGROUND)     .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKMOVE)       .GetThis()->SetMaxValue(1);

  // action step counters
  GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACK)       .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKGROUND) .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);

  // action anim types (the line in sprite) - these are not used
  //SetActionAnimTypes(OA_STOP, 1);
  //SetActionAnimTypes(OA_ATTACKMOVE, 2);
  //SetActionAnimTypes(OA_ATTACK, 2);
  //SetActionAnimTypes(OA_ATTACKGROUND, 2);
}
//---------------------------------------------------------------------------

CIGSubSysChopperWeapons::~CIGSubSysChopperWeapons()
{
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysChopperWeapons::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();
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

void CIGSubSysChopperWeapons::OnActionStop   ()
{
}
//---------------------------------------------------------------------------

void CIGSubSysChopperWeapons::OnActionAttack ()
{
  CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

  VLOG(9) << "CIGSubSysChopperWeapons::OnActionAttack";

  // if the weapon is not enabled, just stay
  //if (!lParent->GetAircraftUnifiedData().wep1_enabled)
  //{
  //  logWriteLn("CIGSubSysTurret::OnActionAttack - weapon 1 is not enabled.");
  //  return;
  //}

  if (GetCurrentAction()->GetData().mTargetObject->GetIsDead() == true)
  {
    GetCurrentAction()->End();
    return;
  }

  // check if the chopper may fire the object
  if (!CIGWeaponRules().CanFireObject(lParent, lParent->GetAircraftUnifiedData().wep1_type, GetCurrentAction()->GetData().mTargetObject))
  {
    GetCurrentAction()->End();
    return;
  }

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int weaponMaxRange = lParent->GetAircraftUnifiedData().wep1_maxrange;
  int lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord));

  // a weapon has a minimum range and a maximum range - check them before turn the chopper and fire
  if ((lDistance >= lParent->GetAircraftUnifiedData().wep1_minrange) && (lDistance <= weaponMaxRange))
  {
     SetCurrentActionIndex(OA_ATTACK);
     GetActionStepCounters(OA_ATTACK) = 0;

     int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetObject->GetLogicCoord());

     if (lParent->GetSenseCounter() == lTmpSense)
     {
        Fire(); // creates the damage object
        mDir = 0;
     }
     else
     {
        //SetCurrentActionIndex(OA_STOP); // stop and turn to sense
        SetWantedSense(lTmpSense);
        //OnActionRotate();
        ComputeDir();
        Turn();
     }
  }
  else
  {
    // queue a move action in the weapon range
    TIGSubSysActionData moveActionData = GetCurrentAction()->GetData();
    moveActionData.mType = OA_MOVE;

    int moveTargetX, moveTargetY;
    int targetObjectX = moveActionData.mTargetObject->GetLogicCoord().x, targetObjectY = moveActionData.mTargetObject->GetLogicCoord().y;

    if (lCoord.x == targetObjectX)
    {
        moveTargetX = lCoord.x;
        moveTargetY = targetObjectY + std::copysign(weaponMaxRange, -targetObjectY + lCoord.y);
    }
    else if (lCoord.y == targetObjectY)
    {
        moveTargetX = targetObjectX + std::copysign(weaponMaxRange, -targetObjectX + lCoord.x);
        moveTargetY = lCoord.y;
    }
    else
    {
        float delta = 1.0 - (float)(weaponMaxRange - 1) / lDistance;

        moveTargetX = std::floor(lCoord.x + delta * (targetObjectX - lCoord.x));
        moveTargetY = std::floor(lCoord.y + delta * (targetObjectY - lCoord.y));
    }

    moveActionData.mTargetPoint = CPPoint(moveTargetX * MAPCELLX, moveTargetY * MAPCELLY);

    // TODO queue an attack action
    TIGSubSysActionData attackActionData = GetCurrentAction()->GetData();
    attackActionData.mType = OA_ATTACK;
    attackActionData.mTargetPoint = CPPoint(-1, -1);

    //Stop();
    GetCurrentAction()->End();

    // TODO wait for the move action to end
    lParent->SetAction(moveActionData);
  }

  VLOG(9) << "CIGSubSysChopperWeapons::OnActionAttack - over";
}
//---------------------------------------------------------------------------

void CIGSubSysChopperWeapons::OnActionAttackGround()
{
  CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

  VLOG(9) << "CIGSubSysChopperWeapons::OnActionAttack";

  // if the weapon is not enabled, do nothing
  if (!lParent->GetAircraftUnifiedData().wep1_enabled)
  {
    VLOG(2) << "CIGSubSysChopperWeapons::OnActionAttackGround - weapon 1 is not enabled.";
    GetCurrentAction()->End();
    return;
  }

  // check if the chopper may fire the object
  if (!CIGWeaponRules().CanFireGround(lParent, lParent->GetAircraftUnifiedData().wep1_type))
  {
    GetCurrentAction()->End();
    return;
  }

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetPoint);

  // a weapon has a minimum range and a maximum range - check them before turn the chopper and fire
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

  VLOG(9) << "CIGSubSysChopperWeapons::OnActionAttackGround - over";
}
//---------------------------------------------------------------------------

void CIGSubSysChopperWeapons::Turn()
{
  int lSense = ((CIGChopperUnit*)GetParent())->GetSenseCounter();

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
    ((CIGChopperUnit*)GetParent())->SetSenseCounter(lSense);

    //int lV = ((CIGChopperUnit*)GetParent())->GetSenseCounter();
    //lV++;
  }
  else
  {
    mDir = 0;
  }
}
//---------------------------------------------------------------------------

void CIGSubSysChopperWeapons::ComputeDir()
{
  int lSense = ((CIGChopperUnit*)GetParent())->GetSenseCounter();

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

void CIGSubSysChopperWeapons::Fire()
{
  CIGChopperUnit* lParent = (CIGChopperUnit*)GetParent();

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
        lObj->SetSpeed(CPCounter(5));
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

void CIGSubSysChopperWeapons::Serialize(PP::Stream& _a)
{
  CIGSubSysBase::Serialize(_a);

  _a.writeInt(&mSense);
  _a.writeInt(&mWantedSense);
  _a.writeInt(&mDir);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysChopperWeapons::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysChopperWeapons::DeSerialize(PP::Stream& _a)
{
  CIGSubSysBase::DeSerialize(_a);

  _a.readInt(&mSense);
  _a.readInt(&mWantedSense);
  _a.readInt(&mDir);
}
//---------------------------------------------------------------------------
