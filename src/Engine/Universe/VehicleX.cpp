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
//---------------------------------------------------------------------------
#include <string.h>
#pragma hdrstop

#include "VehicleX.h"
#include "Stream.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

void TUVehicle::Serialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

unsigned __int32 TUVehicle::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TUVehicle::DeSerialize(PP::Stream& _a)
{
}
//---------------------------------------------------------------------------

CVehicle::CVehicle()
{
  strcpy(mData.name, "(no vehicle)");
  mNone   = true;
  mUsed   = false;

  mFirst  = new CVWeapon();
  mSecond = new CVWeapon();
}
//---------------------------------------------------------------------------

CVehicle::CVehicle(TUVehicle _vehicle)
{
  mData = _vehicle;
  mNone   = false;
  mUsed   = false;

  mFirst  = new CVWeapon();
  mSecond = new CVWeapon();
}
//---------------------------------------------------------------------------

CVehicle::CVehicle(const CVehicle& _vehicle)
{
  mData   = _vehicle.mData;
  mNone   = _vehicle.mNone;
  mUsed   = _vehicle.mUsed;

  // cred ca e corect
  try
  {
    if (mFirst  != _vehicle.mFirst)
    {
      delete mFirst;
      mFirst = NULL;
      mFirst = new CVWeapon(*_vehicle.mFirst);
    }

    if (mSecond  != _vehicle.mSecond)
    {
      delete mSecond;
      mSecond = NULL;
      mSecond = new CVWeapon(*_vehicle.mSecond);
    }
  }
  catch(...)
  {
    delete mFirst;
    delete mSecond;

    throw;
  }

  /*
  mFirst  = _vehicle.mFirst;
  mSecond = _vehicle.mSecond;
  */
}
//---------------------------------------------------------------------------

/*
bool CVehicle::SetFirstWeapon(CVWeapon* _weapon)
{
  // nu avem ce seta, e vehiculul (none)
  if (mNone)
  {
    return false;
  }

  // verific tipul armei
  struct _VWeapon _vWeapon;
  _weapon->GetData(&_vWeapon);

  // verific tipurile
  if ((_vWeapon.type == mData.basicWepType) || (_vWeapon.type == VWT_NONE))
  {
    mFirst = _weapon;
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CVehicle::SetSecondWeapon(CVWeapon* _weapon)
{
  // nu avem ce seta, e vehiculul (none)
  if (mNone)
  {
    return false;
  }

  // verific tipul armei
  struct _VWeapon _vWeapon;
  _weapon->GetData(&_vWeapon);

  // verific tipurile
  if ((_vWeapon.type == mData.secWepType) || (_vWeapon.type == VWT_NONE))
  {
    mSecond = _weapon;
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------
*/

void CVehicle::SetUsed(bool _used)
{
  if (!mNone)
  {
    mUsed = _used;
  }
}
//---------------------------------------------------------------------------

void CVehicle::GetData(struct _Vehicle* _vehicle)
{
  *_vehicle = mData;
}
//---------------------------------------------------------------------------

void CVehicle::Modify()
{
  // sta pe vine...
}
//---------------------------------------------------------------------------

CVWeapon* CVehicle::GetFirstWeapon()
{
  return mFirst;
}
//---------------------------------------------------------------------------

CVWeapon* CVehicle::GetSecondWeapon()
{
  return mSecond;
}
//---------------------------------------------------------------------------

bool CVehicle::IsUsed()
{
  return mUsed;
}
//---------------------------------------------------------------------------

bool CVehicle::IsNone()
{
  return mNone;
}
//---------------------------------------------------------------------------

TTechResources CVehicle::ResForRepair()
{
  TTechResources lRes;
  int lHp = mData.maxHp - mData.hp;

  switch(mData.vType)
  {
    case tank :
      lRes.mMetal      = 5 * (lHp / 12);
      lRes.mEnergy     = 1 * (lHp / 12);
      lRes.mMech       = 2 * (lHp / 13);
      lRes.mElectronic = 1 * (lHp / 10);
      break;

    case trn  :
      lRes.mMetal      = 1 * (lHp / 15);
      lRes.mEnergy     = 1 * (lHp / 16);
      lRes.mMech       = 2 * (lHp / 11);
      lRes.mElectronic = 2 * (lHp / 13);
      break;

    case artillery :
      lRes.mMetal      = 2 * (lHp / 11);
      lRes.mEnergy     = 1 * (lHp / 17);
      lRes.mMech       = 3 * (lHp / 13);
      lRes.mElectronic = 2 * (lHp / 13);
      break;

    case recon :
      lRes.mMetal      = 1 * (lHp / 11);
      lRes.mEnergy     = 1 * (lHp / 11);
      lRes.mMech       = 1 * (lHp / 11);
      lRes.mElectronic = 2 * (lHp / 11);
      break;

    case aircraft :
      lRes.mMetal      = 1 * (lHp / 9);
      lRes.mEnergy     = 2 * (lHp / 9);
      lRes.mMech       = 1 * (lHp / 9);
      lRes.mElectronic = 5 * (lHp / 9);
      break;
  }

  return lRes;
}
//---------------------------------------------------------------------------

void CVehicle::Serialize(PP::Stream& _a)
{
  //_a.write(&mData,   sizeof(struct _Vehicle));
  mData.Serialize(_a);
  _a.writeBool(&mUsed);
  _a.writeBool(&mNone);

  int lStatus = (int)mStatus;

  _a.writeInt(&lStatus);

  mFirst->Serialize(_a);
  mSecond->Serialize(_a);
}
//---------------------------------------------------------------------------

void CVehicle::DeSerialize(PP::Stream& _a)
{
  //_a.read(&mData,   sizeof(struct _Vehicle));
  mData.DeSerialize(_a);
  
  _a.readBool(&mUsed);
  _a.readBool(&mNone);

  int lStatus;

  _a.readInt(&lStatus);

  mStatus = (TVehicleStatus)lStatus;

  delete mFirst;
  delete mSecond;

  mFirst  = NULL;
  mSecond = NULL;

  try
  {
    mFirst  = new CVWeapon();
    mSecond = new CVWeapon();
  }
  catch(...)
  {
    delete mFirst;
    delete mSecond;
  }

  mFirst->DeSerialize(_a);
  mSecond->DeSerialize(_a);
}
//---------------------------------------------------------------------------

CVehicle::~CVehicle()
{
  delete mFirst;
  delete mSecond;
}
//---------------------------------------------------------------------------

