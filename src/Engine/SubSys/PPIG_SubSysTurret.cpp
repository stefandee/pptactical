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

#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjRocket.h"
#include "PPIG_ObjBullet.h"
#include "PPIG_ObjLaser.h"
#include "PPIG_ObjPlasma.h"
#include "PPIG_ObjBallistic.h"
#include "PPIG_SpaceSound.h"
#include "PPIG_SubSysTurret.h"
#include "PPIG_SubSysTurretAction.h"
#include "PPIG_ObjTank.h"
//---------------------------------------------------------------------------

CIGSubSysTurret::CIGSubSysTurret(CIGTankUnit* _parent) : CIGSubSysBase(_parent)
{
  // turret specific vaiables
  mDir         = 0;
  mSense       = 0;
  mWantedSense = 0;

  // set update counters
  GetUpdateCounters(OA_ROTATE)           .GetThis()->SetMaxValue(1000); // this will be updated by the turret turn rate
  GetUpdateCounters(OA_ATTACK)           .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKGROUND)     .GetThis()->SetMaxValue(120);  // this will be updated by the weapon fire rate
  GetUpdateCounters(OA_ATTACKMOVE)       .GetThis()->SetMaxValue(1);

  // action step counters
  GetActionStepCounters(OA_STOP)         .SetMaxValue(0);
  GetActionStepCounters(OA_ROTATE)       .GetThis()->SetMaxValue(0);
  GetActionStepCounters(OA_ATTACK)       .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKGROUND) .SetMaxValue(0);
  GetActionStepCounters(OA_ATTACKMOVE)   .SetMaxValue(0);

  // action anim types (the line in sprite)
  SetActionAnimTypes(OA_STOP, 1);
  SetActionAnimTypes(OA_ROTATE, 1);
  SetActionAnimTypes(OA_ATTACKMOVE, 2);
  SetActionAnimTypes(OA_ATTACK, 2);
  SetActionAnimTypes(OA_ATTACKGROUND, 2);

  GetSenseCounter().SetMaxValue(7); //no of senses in the sprite for the turret
  GetAnimCounter().SetMaxValue(7); //no of senses in the sprite for the turret
}
//---------------------------------------------------------------------------

CIGSubSysTurret::~CIGSubSysTurret()
{
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::Update()
{
  CIGSubSysBase::Update();
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::OnActionAttack()
{
  CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

  VLOG(9) << "CIGSubSysTurret::OnActionAttack";

  // if the weapon is not enabled, just stay
  if (!lParent->GetVehicleUnifiedData().wep1_enabled)
  {
    VLOG(2) << "CIGSubSysTurret::OnActionAttack - weapon 1 is not enabled.";
    return;
  }

  if (GetCurrentAction()->GetData().mTargetObject->GetIsDead() == true)
  {
    GetCurrentAction()->End();
    return;
  }

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int     lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetObject->GetClosestPointTo(lCoord));

  // a weapon has a minimum range and a maximum range - check them before turn the turret and fire
  if ((lDistance >= lParent->GetVehicleUnifiedData().wep1_minrange) && (lDistance <= lParent->GetVehicleUnifiedData().wep1_maxrange))
  {
     SetCurrentActionIndex(OA_ATTACK);
     GetActionStepCounters(OA_ATTACK) = 0;

     int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetObject->GetLogicCoord());
     if (GetSense() == lTmpSense)
     {
        Fire(); // creates the damage object
        mDir = 0;
     }
     else
     {
        SetCurrentActionIndex(OA_STOP); // stop and turn to sense
        SetWantedSense(lTmpSense);
        OnActionRotate();
     }
  }
  else
  {
    Stop();
  }

  VLOG(9) << "CIGSubSysTurret::OnActionAttack - over";
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::OnActionAttackGround()
{
  CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

  // if the weapon is not enabled, just stay
  if (!lParent->GetVehicleUnifiedData().wep1_enabled)
  {
    VLOG(2) << "CIGSubSysTurret::OnActionAttackGround - weapon 1 is not enabled.";
    return;
  }

  VLOG(9) << "CIGSubSysTurret::OnActionAttackGround";

  // get parent map coordinates for fast access
  CPPoint lCoord = GetParent()->GetLogicCoord();

  // compute distance, for fast work
  int     lDistance = gfGetDist2D(lCoord, GetCurrentAction()->GetData().mTargetPoint);

  // a weapon has a minimum range and a maximum range - check them before turn the turret and fire
  if ((lDistance >= lParent->GetVehicleUnifiedData().wep1_minrange) && (lDistance <= lParent->GetVehicleUnifiedData().wep1_maxrange))
  {
     SetCurrentActionIndex(OA_ATTACKGROUND);
     GetActionStepCounters(OA_ATTACKGROUND) = 0;

     int lTmpSense = gfGetDirFromPoints(lCoord, GetCurrentAction()->GetData().mTargetPoint);
     if (GetSense() == lTmpSense)
     {
        // creates the damage object
        Fire();
        mDir = 0;
     }
     else
     {
        // stop and turn to sense
        SetCurrentActionIndex(OA_STOP);
        SetWantedSense(lTmpSense);
        OnActionRotate();
     }
  }
  else
  {
    Stop();
  }

  VLOG(9) << "CIGSubSysTurret::OnActionAttackGround - over";
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::OnActionRotate()
{
  //SetWantedSense(GetParent()->GetSenseCounter()());
  ComputeDir();
  Turn();
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::OnActionStop()
{
  mDir = 0;
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::Turn()
{
  if (mDir == 0)
  {
    return;
  }

  if (GetUpdateCounters(OA_ROTATE).MaxReached())
  {
    if (mSense != mWantedSense)
    {
      mSense += mDir;

      if (mSense < 0)
      {
        mSense = 7;
      }

      if (mSense > 7)
      {
        mSense = 0;
      }

      // reset the counter for turret rotate
      SetCurrentActionIndex(OA_ROTATE);
    }
    else
    {
      mDir = 0;
    }
  }
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::ComputeDir()
{
  if (mSense == mWantedSense)
  {
    mDir = 0;
    return;
  }

  if (mSense < mWantedSense)
  {
    if (mWantedSense - mSense < 7 - mWantedSense + mSense)
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
    if (-mWantedSense + mSense < 7 - mSense + mWantedSense)
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

void CIGSubSysTurret::Fire()
{
  CIGTankUnit* lParent = (CIGTankUnit*)GetParent();

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
  for(int i = 0; i < lParent->GetVehicleUnifiedData().wep1_projectiles_per_shot; i++)
  {
    switch(lParent->GetVehicleUnifiedData().wep1_type)
    {
      case VWT_ARTILLERY_SHELL:
      case VWT_MORTAR:
        // need a ballistic damage object
        lObj = new CIGBallisticObject();
        break;

      case VWT_SHELL_CANNON:
        lObj = new CIGBulletObject();
        lObj->SetSpeed(CPCounter(20));
        break;

      case VWT_VEHICLE_MACHINE_GUN:
        lObj = new CIGBulletObject();
        lObj->SetSpeed(CPCounter(10));
        break;

      case VWT_ROCKET:
        lObj = new CIGBulletObject();
        lObj->SetSpeed(CPCounter(8));
        break;

      case VWT_LASER_CANNON:
         lObj = new CIGLaserObject();
         break;

      case VWT_SAM:
      case VWT_SSM:
         lObj = new CIGRocketObject();
         break;

      case VWT_FLAMER:
      case VWT_PLASMA_FLAMER:
         lObj = new CIGPlasmaObject();
         break;

      case VWT_NONE:
      default:
         // weapon is of the wrong type, aborting method
         return;
    }

    if (!lObj)
    {
      return;
    }

    //lObj->SetCoord(CPPoint(lParent->GetCoord().x + gDir[GetSense()].x * lParent->GetDimX(), lParent->GetCoord().y + gDir[GetSense()].y * lParent->GetDimY()));
    CPPoint lFireHole     = lParent->GetVehicleUnifiedData().primary[GetSense()];
    CPPoint lBodyMount    = lParent->GetVehicleUnifiedData().body[lParent->GetBodySenseCounter()];
    CPPoint lTurretMount  = lParent->GetVehicleUnifiedData().turret[GetSense()];
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
    lObj->SetPower(lParent->GetVehicleUnifiedData().wep1_damage);
    lObj->SetSprite(lParent->GetProjectileSprite());
    lObj->SetEffectSprite(lParent->GetEffectSprite());
    lObj->SetSoundOnHit(lParent->GetSoundOnHit());
    lObj->SetLife(lParent->GetVehicleUnifiedData().mWep1_TimeToLive);
    lObj->SetTrackPersistency(lParent->GetVehicleUnifiedData().wep1_trackPersistency);

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

  try
  {
     CIGSpaceSoundSystem* lSSSI = CIGSpaceSoundSystem::Instance();
     lSSSI->Play(lParent->GetSoundOnFire(), lParent->GetLogicCoord().x, lParent->GetLogicCoord().y);
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysTurret::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGTankUnit* lParent = (CIGTankUnit*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_STOP:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionTurretStop(this, actionData);
         }
         break;
      case OA_ROTATE:
         if (lIsDead == false)
         {
           lTmpAction = new CIGSubSysActionTurretRotate(this, actionData);
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


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysTurret::Serialize(PP::Stream& _a)
{
  // base class serialize
  CIGSubSysBase::Serialize(_a);

  // members
  _a.writeInt(&mSense);
  _a.writeInt(&mWantedSense);
  _a.writeInt(&mDir);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysTurret::DataSize()
{
  return 3 * sizeof(int);
}
//---------------------------------------------------------------------------

void CIGSubSysTurret::DeSerialize(PP::Stream& _a)
{
  // base class deserialize
  CIGSubSysBase::DeSerialize(_a);

  // members
  _a.readInt(&mSense);
  _a.readInt(&mWantedSense);
  _a.readInt(&mDir);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

