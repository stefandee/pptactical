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

#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjStatic.h"
#include "PPIG_ObjDropship.h"
#include "PPIG_ObjTank.h"
#include "PPIG_ObjRecon.h"
#include "PPIG_ObjTran.h"
#include "PPIG_ObjChopper.h"
#include "PPIG_ObjFighter.h"
#include "PPIG_ObjArtillery.h"
#include "PPIG_Mission.h"
#include "PPIG_ObjBase.h"

#include "PPIG_WeaponRules.h"
//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

CIGWeaponRules::CIGWeaponRules()
{
}
//---------------------------------------------------------------------------

CIGWeaponRules::~CIGWeaponRules()
{
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireObject(CIGChopperUnit*  _attacker,  int _weapon, CIGBaseObject* _target)
{
  // these weapons cannot be mounted on a chopper
  switch(_weapon)
  {
    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SAM:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
    case VWT_VEHICLE_MACHINE_GUN:
    case VWT_NONE:
    case VWT_AIR_BOMB:
      return false;
  }

  // a chopper may not fire when standing on ground - change this if you feel
  // your universe allow this ;)
  if (_attacker->GetCoordZ() == 0)
  {
    return false;
  }

  switch(_weapon)
  {
    case VWT_AG_MISSILE:
      if (IsAir(_target) && _target->GetCoordZ() > 0)
      {
        return false;
      }
      break;

    case VWT_AA_MISSILE:
      if (IsGround(_target) || (IsAir(_target) && _target->GetCoordZ() == 0))
      {
        return false;
      }
      break;

    case VWT_AIR_LASER:
    case VWT_AIR_CANNON:
      break;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireObject(CIGFighterUnit*   _attacker,  int _weapon, CIGBaseObject* _target)
{
  // these weapons cannot be mounted on a figher
  switch(_weapon)
  {
    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SAM:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
    case VWT_VEHICLE_MACHINE_GUN:
    case VWT_NONE:
      return false;
  }

  // a figher may not fire when standing on ground - change this if you feel
  // your universe allow this ;)
  // a fighter cannot stand on the ground, for that matter ;)
  if (_attacker->GetCoordZ() == 0)
  {
    return false;
  }

  switch(_weapon)
  {
    case VWT_AG_MISSILE:
    case VWT_AIR_BOMB:
      if (IsAir(_target) && _target->GetCoordZ() > 0)
      {

        return false;
      }
      break;

    case VWT_AA_MISSILE:
      if (IsGround(_target) || (IsAir(_target) && _target->GetCoordZ() == 0))
      {
        return false;
      }
      break;

    case VWT_AIR_LASER:
    case VWT_AIR_CANNON:
      break;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireObject(CIGInfantryUnit* _attacker,  int _weapon, CIGBaseObject* _target)
{
  CPString lClassName = _target->ClassName();

  // an infantry unit may not fire a fighter or bomber unless a rocket is used
  if ((lClassName == CPString(IGCN_FIGHTERUNIT) || lClassName == CPString(IGCN_BOMBERUNIT)) && (_weapon != IWT_ROCKET_LAUNCHER))
  {
    return false;
  }

  // an infantry unit may not fire a chopper unless it is on ground (or a rocket is used)
  if ((lClassName == CPString(IGCN_CHOPPERUNIT) || lClassName == CPString(IGCN_DROPSHIPUNIT)) && _target->GetCoordZ() > 0
      /*(_target->GetCoordZ() > 0 || _weapon != IWT_ROCKET_LAUNCHER)*/)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireObject(CIGTankUnit* _attacker,  int _weapon, CIGBaseObject* _target)
{
  // only these weapons may be mounted on a tank
  switch(_weapon)
  {
    case VWT_SAM:
      if (IsAir(_target) && _target->GetCoordZ() > 0)
      {
        return true;
      }

      break;

    case VWT_VEHICLE_MACHINE_GUN:
      break;

    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
      if (IsGround(_target) ||
          (IsAir(_target) && _target->GetCoordZ() == 0))
      {
        return true;
      }
  }

  return false;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireGround(CIGChopperUnit*  _attacker,  int _weapon)
{
  // these weapons cannot be mounted on a chopper
  switch(_weapon)
  {
    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SAM:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
    case VWT_VEHICLE_MACHINE_GUN:
    case VWT_NONE:
    case VWT_AIR_BOMB:
      return false;
  }

  // a chopper may not fire when standing on ground - change this if you feel
  // your universe allow this ;)
  if (_attacker->GetCoordZ() == 0)
  {
    return false;
  }

  switch(_weapon)
  {
    // a missile is used to fire at a target not at ground
    case VWT_AG_MISSILE:
      return false;

    case VWT_AA_MISSILE:
      return false;

    case VWT_AIR_LASER:
    case VWT_AIR_CANNON:
      return true;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireGround(CIGFighterUnit*   _attacker,  int _weapon)
{
  // these weapons cannot be mounted on a fighter
  switch(_weapon)
  {
    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SAM:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
    case VWT_VEHICLE_MACHINE_GUN:
    case VWT_NONE:
      return false;
  }

  switch(_weapon)
  {
    // a missile is used to fire at a target not at ground
    case VWT_AG_MISSILE:
      return false;

    case VWT_AA_MISSILE:
      return false;

    case VWT_AIR_LASER:
    case VWT_AIR_CANNON:
    case VWT_AIR_BOMB:
      return true;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireGround(CIGInfantryUnit* _attacker,  int _weapon)
{
  // for the moment, all infantry weapons may fire ground
  return true;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireGround(CIGTankUnit* _attacker,  int _weapon)
{
  // only these weapons may be mounted on a tank and all of them may
  // be used to fire ground
  switch(_weapon)
  {
    case VWT_VEHICLE_MACHINE_GUN:
    case VWT_ARTILLERY_SHELL:
    case VWT_SHELL_CANNON:
    case VWT_SSM:
    case VWT_ROCKET:
    case VWT_FLAMER:
    case VWT_PLASMA_FLAMER:
    case VWT_LASER_CANNON:
    case VWT_MORTAR:
      return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireObject(int _attackerId, int _weapon, int _targetId)
{
  // retrieve the units bases on id's
  CIGBaseObject* lAttackerUnit = GetMissionInstance()->GetObjectById(_attackerId);
  CIGBaseObject* lTargetUnit   = GetMissionInstance()->GetObjectById(_targetId);

  if (!lAttackerUnit || !lTargetUnit)
  {
    return false;
  }

  if (lAttackerUnit->GetIsDead() || lTargetUnit->GetIsDead())
  {
    return false;
  }

  // convert to the appropriate class so that we may call the appropirate method
  if (lAttackerUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
  {
    return CanFireObject((CIGInfantryUnit*)lAttackerUnit, _weapon, lTargetUnit);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_CHOPPERUNIT) ||
      lAttackerUnit->IsDerivedFrom(IGCN_CHOPPERUNIT)
     )
  {
    return CanFireObject((CIGChopperUnit*)lAttackerUnit, _weapon, lTargetUnit);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_FIGHTERUNIT))
  {
    return CanFireObject((CIGFighterUnit*)lAttackerUnit, _weapon, lTargetUnit);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_TANKUNIT) ||
      lAttackerUnit->IsDerivedFrom(IGCN_TANKUNIT)
     )
  {
    return CanFireObject((CIGTankUnit*)lAttackerUnit, _weapon, lTargetUnit);
  }

  return false;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::CanFireGround(int _attackerId, int _weapon)
{
  // retrieve the units bases on id's
  CIGBaseObject* lAttackerUnit = GetMissionInstance()->GetObjectById(_attackerId);

  if (!lAttackerUnit)
  {
    return false;
  }

  if (lAttackerUnit->GetIsDead())
  {
    return false;
  }

  // convert to the appropriate class so that we may call the appropirate method
  if (lAttackerUnit->ClassName() == CPString(IGCN_INFANTRYUNIT))
  {
    return CanFireGround((CIGInfantryUnit*)lAttackerUnit, _weapon);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_CHOPPERUNIT) ||
      lAttackerUnit->IsDerivedFrom(IGCN_CHOPPERUNIT)
     )
  {
    return CanFireGround((CIGChopperUnit*)lAttackerUnit, _weapon);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_FIGHTERUNIT))
  {
    return CanFireGround((CIGFighterUnit*)lAttackerUnit, _weapon);
  }

  if (lAttackerUnit->ClassName() == CPString(IGCN_TANKUNIT) ||
      lAttackerUnit->IsDerivedFrom(IGCN_TANKUNIT)
     )
  {
    return CanFireGround((CIGTankUnit*)lAttackerUnit, _weapon);
  }

  return false;
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::IsAir(CIGBaseObject* _obj)
{
  CPString lClassName = _obj->ClassName();

  return _obj->IsDerivedFrom(IGCN_AIRUNIT);
}
//---------------------------------------------------------------------------

bool CIGWeaponRules::IsGround(CIGBaseObject* _obj)
{
  CPString lClassName = _obj->ClassName();

  return (lClassName == CPString(IGCN_INFANTRYUNIT) ||
          _obj->IsDerivedFrom(IGCN_VEHICLEUNIT) ||
          lClassName == CPString(IGCN_STATICOBJECT)
          );
}
//---------------------------------------------------------------------------


