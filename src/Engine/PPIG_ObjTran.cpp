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

#include "PPIG_ObjTran.h"
#include "PPIG_ObjInfantry.h"
#include "PPIG_Mission.h"

#include "PPIG_SubSysBayManager.h"
#include "PPIG_SubSysTactics.h"
//---------------------------------------------------------------------------

CIGTranUnit::CIGTranUnit()
{
  VLOG(9) << "CIGTranUnit::CIGTranUnit";

  // setup
  SetActionMask((1<<MA_STOP)|
                (1<<MA_MOVE)|
                (1<<MA_ATTACK)|
                (1<<MA_PATROL)|
                (1<<MA_GUARD)|
                (1<<MA_HOLD)|
                (1<<MA_SUPPRESS_FIRE));

  // bay subsystem setup
  mInfantryBay = new CIGSubSysBayManager(this);
  mInfantryBay->SetCapacity(6);

  // visual
  //mVisual->SetFOV(130);
  //mVisual->SetScanRange(8);

  SetSpeed(CPCounter(8));

  VLOG(9) << "CIGTranUnit::CIGTranUnit - over";
}
//---------------------------------------------------------------------------

CIGTranUnit::~CIGTranUnit()
{
  if (mInfantryBay)
  {
    delete mInfantryBay;
    mInfantryBay = 0;
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if this class is derived from className
 parameters : className
---------------------------------------------------------------------------*/
bool CIGTranUnit::IsDerivedFrom (CPString className)
{
   return ((CIGTankUnit::IsDerivedFrom(className)) || (className == CIGTranUnit::ClassName()));
}
//---------------------------------------------------------------------------

void CIGTranUnit::SetVehicleUnifiedData(TVehicleUnified& value)
{
  CIGTankUnit::SetVehicleUnifiedData(value);

  mInfantryBay->SetCapacity(value.mBayCapacity);
}
//---------------------------------------------------------------------------

void CIGTranUnit::Update()
{
  mInfantryBay->Update();
  //logWrite("Tran bay capacity: "); logWriteILn(mInfantryBay->GetCapacity());
  CIGTankUnit::Update();
}
//---------------------------------------------------------------------------

int CIGTranUnit::GetCapacity()
{
  return mInfantryBay->GetCapacity();
}
//---------------------------------------------------------------------------

CIGUnit* CIGTranUnit::GetUnit(int _index)
{
  return mInfantryBay->GetUnit(_index);
}
//---------------------------------------------------------------------------

void CIGTranUnit::RemoveUnit(CIGUnit* _unit)
{
  mInfantryBay->RemoveUnit(_unit);
}
//---------------------------------------------------------------------------

void CIGTranUnit::AddAction(TIGSubSysActionData actionData)
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
      mInfantryBay->AddAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddAction(actionData);
      mPropulsion->CancelCurrentAction();
      mInfantryBay->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddAction(actionData);
      break;

    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mTactics->CancelCurrentAction();
      mInfantryBay->AddAction(actionData);
      mPropulsion->AddAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddAction(actionData);
  }
}
//---------------------------------------------------------------------------

void CIGTranUnit::AddAction (CIGSubSysAction *Action)
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
      mInfantryBay->AddAction(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddAction(Action);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddAction(Action);
      mPropulsion->CancelCurrentAction();
      mInfantryBay->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddAction(Action);
      break;

    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mTactics->CancelCurrentAction();
      mInfantryBay->AddAction(Action);
      mPropulsion->AddAction(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddAction(Action);
  }
}
//---------------------------------------------------------------------------

void CIGTranUnit::AddActionWithPriority (CIGSubSysAction *Action)
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
      mInfantryBay->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->AddActionWithPriority(Action);
      break;

    case OA_TACTIC_STOP:
      mTactics->AddActionWithPriority(Action);
      mPropulsion->CancelCurrentAction();
      mInfantryBay->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->AddActionWithPriority(Action);
      break;

    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mTactics->CancelCurrentAction();
      mInfantryBay->AddActionWithPriority(Action);
      mPropulsion->AddActionWithPriority(Action);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->AddActionWithPriority(Action);
  }
}
//---------------------------------------------------------------------------

void CIGTranUnit::SetAction (TIGSubSysActionData actionData)
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
      mInfantryBay->SetAction(actionData);
      break;

    case OA_TACTIC_MOVE:
    case OA_TACTIC_ATTACK:
    case OA_TACTIC_ATTACKGROUND:
    case OA_TACTIC_PATROL:
      mTactics->SetAction(actionData);
      break;

    case OA_TACTIC_STOP:
      mTactics->SetAction(actionData);
      mPropulsion->CancelCurrentAction();
      mInfantryBay->CancelCurrentAction();
      break;

    case OA_MOVE:
    case OA_PATROL:
      mPropulsion->SetAction(actionData);
      break;

    case OA_UNLOAD_ALL:
    case OA_UNLOAD:
      mTactics->CancelCurrentAction();
      mInfantryBay->SetAction(actionData);
      mPropulsion->SetAction(actionData);
      break;

    default:
      mTactics->CancelCurrentAction();
      mPropulsion->SetAction(actionData);
  }
}
//---------------------------------------------------------------------------

bool CIGTranUnit::Can(int _capability)
{
  switch(_capability)
  {
    case CAN_CARRY_INFANTRY:
      return true;

    // we can unload only if the tran is not moving
    case CAN_UNLOAD:
    case CAN_LOAD:
      return !GetMoving();
  }

  return CIGTankUnit::Can(_capability);
}
//---------------------------------------------------------------------------

bool CIGTranUnit::Load(CIGInfantryUnit* _v)
{
  // cannot load dead units
  if (_v->GetIsDead())
  {
    return false;
  }

  //_v->RemoveFromLogicMap();
  //_v->RemoveFromLayerMap();
  return mInfantryBay->LoadUnit(_v);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGTranUnit::Serialize(PP::Stream& _a)
{
  // parent class serialize
  CIGTankUnit::Serialize(_a);

  // this serialize
  mInfantryBay->Serialize(_a);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGTranUnit::DataSize()
{
  return CIGTankUnit::DataSize() + mInfantryBay->DataSize();
}
//---------------------------------------------------------------------------

void CIGTranUnit::DeSerialize(PP::Stream& _a)
{
  // parent class serialize
  CIGTankUnit::DeSerialize(_a);

  // this serialize
  mInfantryBay->DeSerialize(_a);
}
//---------------------------------------------------------------------------


#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
