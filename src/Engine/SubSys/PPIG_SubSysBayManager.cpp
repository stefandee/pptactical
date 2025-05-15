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

#include <algorithm>
using namespace std;

#include "PPIG_SubSysBayManager.h"
#include "PPIG_ObjTran.h"
#include "PPIG_Mission.h"
#include "PPIG_Map.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_SerObjList.h"
//---------------------------------------------------------------------------

CIGSubSysBayManager::CIGSubSysBayManager(CIGUnit* _parent) : CIGSubSysBase(_parent)
{
  // bay setup
  mUnitList = new CIGBaseObjectList();
  //mUnitList = new CPList<CIGUnit>(2);
  mCapacity = 6;

  // counters/anim setup
  GetUpdateCounters(OA_UNLOAD).SetMaxValue(10);
  GetUpdateCounters(OA_UNLOAD_ALL).SetMaxValue(1);

  GetActionStepCounters(OA_UNLOAD).SetMaxValue(0);
  GetActionStepCounters(OA_UNLOAD_ALL).SetMaxValue(0);

  SetActionAnimTypes(OA_UNLOAD, 0);
  SetActionAnimTypes(OA_UNLOAD_ALL, 0);
}
//---------------------------------------------------------------------------

CIGSubSysBayManager::~CIGSubSysBayManager()
{
  mUnitList->FreeAll();
  delete mUnitList;
  mUnitList = 0;
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::RemoveUnit(CIGUnit* _unit)
{
  mUnitList->Remove(_unit);
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::OnActionUnload()
{
  CIGUnit* lParent = (CIGUnit*)GetParent();

  // if no units in bay, end action
  if (mUnitList->Count() == 0)
  {
    GetCurrentAction()->End();
    return;
  }

  // drop near the transport (the parent), if the parent does not move
  if (lParent->Can(CAN_UNLOAD))
  {
    // we have a drop point, unload the unit
    CIGUnit* lUnit = (CIGUnit*)mUnitList->GetObject(GetCurrentAction()->GetData().mData[0]);

    // maybe it's invalid?
    if (!lUnit)
    {
      GetCurrentAction()->End();
      return;
    }

    // compute coordinates around parent unit
    CPPoint lDropPoint = GetDropPoint(lUnit);

    // no drop point available
    if (lDropPoint.x == -1)
    {
      // should we end the action? mmm...no
      return;
    }

    // add the unit in the maps and end this action
    lUnit->SetCoord(CPPoint(lDropPoint.x * MAPCELLX, lDropPoint.y * MAPCELLY));

    lUnit->NoteInLogicMap();
    lUnit->NoteInLayerMap();

    // TODO: getuniqueid should also look inside transports
    lUnit->SetId(GetMissionInstance()->GetUniqueUnitId());

    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->AddUnique(lUnit);
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->Remove(lUnit);

    if (lUnit->GetBattleGroupId() == OS_PLAYER)
    {
      GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->AddUnique(lUnit);
    }

    mUnitList->Remove(mUnitList->GetObject(GetCurrentAction()->GetData().mData[0]));

    //SetCurrentActionIndex(OA_STOP);
  }

  // either way, end the action
  GetCurrentAction()->End();
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::OnActionUnloadAll()
{
  CIGUnit* lParent = (CIGUnit*)GetParent();

  // if no units in bay, end action
  VLOG(9) << "dropship unit count: " << mUnitList->Count();

  if (mUnitList->Count() == 0)
  {
    GetCurrentAction()->End();
    return;
  }

  if (!lParent->Can(CAN_UNLOAD))
  {
    return;
  }

  // is the transporter close to the drop point and the parent is not moving?
  if (gfGetDist2D(lParent->GetLogicCoord(), GetCurrentAction()->GetData().mTargetPoint) <= 2 && !lParent->GetMoving())
  {
    // we have a drop point, unload the unit
    CIGUnit* lUnit = (CIGUnit*)mUnitList->GetObject(0);

    // compute coordinates around parent unit
    CPPoint lDropPoint = GetDropPoint(lUnit);

    // no drop point available
    if (lDropPoint.x == -1)
    {
      return;
    }

    lUnit->SetCoord(CPPoint(lDropPoint.x * MAPCELLX, lDropPoint.y * MAPCELLY));
    lUnit->NoteInLogicMap();
    lUnit->NoteInLayerMap();

    // TODO: getuniqueid should also look inside transports
    lUnit->SetId(GetMissionInstance()->GetUniqueUnitId());

    GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->AddUnique(lUnit);
    GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->Remove(lUnit);

    if (lUnit->GetBattleGroupId() == OS_PLAYER)
    {
      GetMissionInstance()->GetObjects(MOL_OWNEDUNITS)->AddUnique(lUnit);
    }

    mUnitList->Remove(lUnit);

    SetCurrentActionIndex(OA_HOLD);
  }
}
//---------------------------------------------------------------------------

// Description : Searches the first free place around the parent
// Param       : n/a
// Result      : the free point
// Comments    : it is not optimal, but who cares?
CPPoint CIGSubSysBayManager::GetDropPoint(CIGUnit* _unit)
{
  CIGUnit* lParent = (CIGUnit*)GetParent();

  CPPoint lCoord = lParent->GetLogicCoord();
  int     lDimX  = lParent->GetLogicDimX();
  int     lDimY  = lParent->GetLogicDimY();

  int lXStart = max(0, lCoord.x - _unit->GetLogicDimX());
  int lXEnd   = min(GetMissionInstance()->GetMap()->GetLogicWidth(), lCoord.x + lDimX + _unit->GetLogicDimX());

  int lYStart = max(0, lCoord.y - _unit->GetLogicDimY());
  int lYEnd   = min(GetMissionInstance()->GetMap()->GetLogicHeight(), lCoord.y + lDimY + _unit->GetLogicDimY());

  for(int x = lXStart; x < lXEnd; x++)
  {
    for(int y = lYStart; y < lYEnd; y++)
    {
      if (!GetMissionInstance()->GetMap()->GetGroundObject(x, y))
      {
        // also check if the unit may be placed on the map
        if (_unit->CanBePlaced(CPPoint(x , y)))
        {
          return CPPoint(x, y);
        }
      }
    }
  }

  return CPPoint(-1, -1);
}
//---------------------------------------------------------------------------

bool CIGSubSysBayManager::LoadUnit(CIGInfantryUnit* _unit)
{
  int lSpaceTaken = 0;

  // compute the space occupied
  for(int i = 0; i < mUnitList->Count(); i++)
  {
    lSpaceTaken += mUnitList->GetObject(i)->GetLogicDimX() * mUnitList->GetObject(i)->GetLogicDimY();
  }

  // now check if there is space left
  int lUnitSpace = _unit->GetLogicDimX() * _unit->GetLogicDimY();

  if (mCapacity <= lSpaceTaken + lUnitSpace)
  {
    return false;
  }

  mUnitList->Add(_unit);

  // remove the unit from the MOL_ALLOBJECTS, the unit will be managed by the
  // transport from now on
  GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(_unit);

  // add the unit to the transported layer
  GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(_unit);

  return true;
}
//---------------------------------------------------------------------------

bool CIGSubSysBayManager::LoadUnit(CIGVehicle* _unit)
{
  int lSpaceTaken = 0;

  // compute the space occupied
  for(int i = 0; i < mUnitList->Count(); i++)
  {
    lSpaceTaken += mUnitList->GetObject(i)->GetLogicDimX() * mUnitList->GetObject(i)->GetLogicDimY();
  }

  // now check if there is space left
  int lUnitSpace = _unit->GetLogicDimX() * _unit->GetLogicDimY();

  if (mCapacity <= lSpaceTaken + lUnitSpace)
  {
    return false;
  }

  mUnitList->Add(_unit);

  // remove the unit from the MOL_ALLOBJECTS, the unit will be managed by the
  // transport from now on
  GetMissionInstance()->GetObjects(MOL_ALLOBJECTS)->Remove(_unit);

  // add the unit to the transported layer
  GetMissionInstance()->GetObjects(MOL_TRANSPORTEDUNITS)->AddUnique(_unit);

  return true;
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::SetCapacity(int _v)
{
  if (_v < mUnitList->Count())
  {
    return;
  }

  mCapacity = _v;
}
//---------------------------------------------------------------------------

CIGSubSysAction* CIGSubSysBayManager::GetActionInstance(TIGSubSysActionData actionData)
{
   CIGSubSysAction *lTmpAction = NULL;
   CIGUnit* lParent = (CIGUnit*)GetParent();
   bool lIsDead = lParent->GetIsDead();

   switch(actionData.mType)
   {
      case OA_STOP:
        if (lIsDead == false)
        {
          lTmpAction = new CIGSubSysActionStop(this, actionData);
        }
        break;

      case OA_UNLOAD:
        if (lIsDead == false)
        {
          lTmpAction = new CIGSubSysActionUnload(this, actionData);
        }
        break;

      case OA_UNLOAD_ALL:
        if (lIsDead == false)
        {
          lTmpAction = new CIGSubSysActionUnloadAll(this, actionData);
        }
        break;
   }

   return lTmpAction;
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::Serialize(PP::Stream& _a)
{
  // parent class serialize
  CIGSubSysBase::Serialize(_a);

  // now serialize the capacity
  _a.writeInt(&mCapacity);

  // and finally the list of contained units
  CIGSerBaseObjectList lObjList(mUnitList);

  lObjList.Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysBayManager::DataSize()
{
  return CIGSubSysBase::DataSize();
}
//---------------------------------------------------------------------------

void CIGSubSysBayManager::DeSerialize(PP::Stream& _a)
{
  // parent class deserialize
  CIGSubSysBase::DeSerialize(_a);
  //GetUpdateCounters(OA_UNLOAD_ALL).GetThis()->SetMaxValue(30);
  GetUpdateCounters(OA_HOLD).GetThis()->SetMaxValue(30);

  _a.readInt(&mCapacity);

  mUnitList->FreeAll();

  CIGSerBaseObjectList lObjList(mUnitList);

  lObjList.DeSerialize(_a);
  //mUnitList = new CPList<CIGUnit>(2);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

