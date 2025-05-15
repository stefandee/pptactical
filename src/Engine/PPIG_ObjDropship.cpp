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

 description: Class In Game Dropship
 last modify: 13 07 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PPIG_ObjDropship.h"
#include "PPIG_SubSysBayManager.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_ObjVehicle.h"
#include "PPIG_SubSysDropshipPropulsion.h"

/*---------------------------------------------------------------------------
 description: c'tor
---------------------------------------------------------------------------*/
CIGDropship::CIGDropship() : CIGChopperUnit()
{
  // depending on the game, a dropship can do other actions
  // for the moment, inherit from the chopper unit and add a couple of its own
  SetActionMask( GetActionMask() |
                 (1<<MA_UNLOAD_ALL)
                 );

  SetSpeed(CPCounter(8));

  // bay subsystem setup
  mBay = new CIGSubSysBayManager(this);
  mBay->SetCapacity(6);

  mPropulsion = new CIGSubSysDropshipPropulsion(this);
}
//---------------------------------------------------------------------------

void CIGDropship::CreatePropulsion()
{
  if (mPropulsion)
  {
    delete mPropulsion;
  }

  mPropulsion = new CIGSubSysDropshipPropulsion(this);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CIGDropship::~CIGDropship()
{
  delete mBay;
  mBay = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the object is derived from the className
---------------------------------------------------------------------------*/
bool CIGDropship::IsDerivedFrom (CPString className)
{
   return (className == CIGDropship::ClassName() || CIGChopperUnit::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

bool CIGDropship::Load(CIGInfantryUnit* _v)
{
  // cannot load dead units
  if (_v->GetIsDead())
  {
    return false;
  }

  //_v->RemoveFromLogicMap();
  //_v->RemoveFromLayerMap();
  return mBay->LoadUnit(_v);
}
//---------------------------------------------------------------------------

bool CIGDropship::Load(CIGVehicle* _v)
{
  // cannot load dead units
  if (_v->GetIsDead())
  {
    return false;
  }

  return mBay->LoadUnit(_v);
}
//---------------------------------------------------------------------------

bool CIGDropship::Can(int _capability)
{
  switch(_capability)
  {
    case CAN_CARRY_INFANTRY:
    case CAN_CARRY_VEHICLES:
      return true;

    case CAN_UNLOAD:
    case CAN_LOAD:
      if (GetCoordZ() == 0)
      {
        return true;
      }
  }

  return CIGChopperUnit::Can(_capability);
}
//---------------------------------------------------------------------------

void CIGDropship::Update()
{
  mBay->Update();

  CIGChopperUnit::Update();
}
//---------------------------------------------------------------------------

void CIGDropship::SetAircraftUnifiedData(TVehicleUnified& value)
{
  CIGChopperUnit::SetAircraftUnifiedData(value);

  mBay->SetCapacity(value.mBayCapacity);
}
//---------------------------------------------------------------------------

int CIGDropship::GetCapacity()
{
  return mBay->GetCapacity();
}
//---------------------------------------------------------------------------

CIGUnit* CIGDropship::GetUnit(int _index)
{
  return mBay->GetUnit(_index);
}
//---------------------------------------------------------------------------

void CIGDropship::RemoveUnit(CIGUnit* _unit)
{
  mBay->RemoveUnit(_unit);
}
//---------------------------------------------------------------------------

void CIGDropship::AddAction(TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mBay->AddAction(actionData);
      mPropulsion->AddAction(actionData);
      break;

    default:
      CIGChopperUnit::AddAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGDropship::AddAction (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mBay->AddAction(Action);
      mPropulsion->AddAction(Action);
      break;

    default:
      CIGChopperUnit::AddAction(Action);
  }
}
//---------------------------------------------------------------------------

void CIGDropship::AddActionWithPriority (CIGSubSysAction *Action)
{
  switch (Action->GetType())
  {
    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mBay->AddActionWithPriority(Action);
      mPropulsion->AddActionWithPriority(Action);
      break;

    default:
      CIGChopperUnit::AddActionWithPriority(Action);
  }
}
//---------------------------------------------------------------------------

void CIGDropship::SetAction (TIGSubSysActionData actionData)
{
  switch (actionData.mType)
  {
    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mBay->SetAction(actionData);
      mPropulsion->SetAction(actionData);
      break;

    default:
      CIGChopperUnit::SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGDropship::InitScriptingInterface()
{
  VLOG(9) << "CIGAircraft::InitScriptingInterface";

  CIGChopperUnit::InitScriptingInterface();

  addModuleFunc("BayCount()", mHandleBayCount);
}
//---------------------------------------------------------------------------

void CIGDropship::InitHandlers()
{
  CIGChopperUnit::InitHandlers();

  mHandleBayCount.set(this, &CIGDropship:: mcBayCount);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Implementation of script methods follows
//
//---------------------------------------------------------------------------
ZString CIGDropship::mcBayCount(ZCsl* csl)
{
  return mBay->GetCapacity();
}

void CIGDropship::Serialize(PP::Stream& _a)
{
  CIGChopperUnit::Serialize(_a);

  mBay->Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGDropship::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGDropship::DeSerialize(PP::Stream& _a)
{
  CIGChopperUnit::DeSerialize(_a);

  mBay->DeSerialize(_a);
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
