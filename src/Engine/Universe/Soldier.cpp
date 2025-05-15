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
#include <stdlib.h>
#pragma hdrstop

#include "PP_Stdlib.h"
#include "Soldier.h"
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

void TUInfantry::Serialize(PP::Stream& _a)
{
  _a.write(name, 32);
  _a.write(surname, 32);
  _a.write(description, 256);
  _a.writeInt(&weight);
  _a.writeInt(&height);
  _a.writeInt(&gender);
  _a.writeInt(&leadership);
  _a.writeInt(&experience);
  _a.writeInt(&bravery);
  _a.writeInt(&camo);
  _a.writeInt(&antitank_skill);
  _a.writeInt(&sr_combat);
  _a.writeInt(&sniper_skill);
  _a.writeInt(&high_tech);
  _a.writeInt(&maxCarry);
  _a.writeInt(&crtCarry);
  _a.writeInt(&morale);
  _a.writeInt(&price);
  _a.writeInt(&kills);
  _a.writeInt(&hp);
  _a.writeInt(&maxHp);
  _a.writeBool(&secWepAbility);
  _a.write(face, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUInfantry::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 32);
  _a.read(surname, 32);
  _a.read(description, 256);
  _a.readInt(&weight);
  _a.readInt(&height);
  _a.readInt(&gender);
  _a.readInt(&leadership);
  _a.readInt(&experience);
  _a.readInt(&bravery);
  _a.readInt(&camo);
  _a.readInt(&antitank_skill);
  _a.readInt(&sr_combat);
  _a.readInt(&sniper_skill);
  _a.readInt(&high_tech);
  _a.readInt(&maxCarry);
  _a.readInt(&crtCarry);
  _a.readInt(&morale);
  _a.readInt(&price);
  _a.readInt(&kills);
  _a.readInt(&hp);
  _a.readInt(&maxHp);
  _a.readBool(&secWepAbility);
  _a.read(face, MAX_PP_PATH);
}
//---------------------------------------------------------------------------


CSoldier::CSoldier()
{
  mDaysWeaponUse = 0;
  mDaysMoraleCt  = 0;
  mFirstWeaponIndex  = -1;
  mSecondWeaponIndex = -1;
  mArmorIndex        = -1;
  mSpecialDevice     = -1; // nu e totusi stabilit ce ar putea insemna acesrt device
  mEmbarked          = false;

  // random soldier generator
  strcpy(mData.name,        "Nume");
  strcpy(mData.surname,     "Prenume");
  strcpy(mData.description, "No data available");
  strcpy(mData.face,         "");
  //strcpy(mData.comportament, "");

  mData.weight     = gfRandom(60) + 50;
  mData.height     = 0; // nu, deocamdata
  mData.gender        = gfRandom(1);

  mData.leadership     = gfRandom(100);
  mData.experience     = gfRandom(100);
  mData.bravery        = gfRandom(100);
  mData.camo           = gfRandom(100);
  mData.antitank_skill = gfRandom(100);
  mData.sr_combat      = gfRandom(100);
  mData.sniper_skill   = gfRandom(100);
  mData.high_tech      = gfRandom(100);
  mData.maxCarry       = gfRandom(100);
  mData.crtCarry       = gfRandom(100);
  mData.morale         = 80;
  mData.price          = 10000;
  mData.kills          = gfRandom(10) + 2;
  mData.maxHp          = 20 + gfRandom(100);
  mData.hp             = mData.maxHp;
  mData.secWepAbility  = 0;

  mIWeapon = NULL;
  mIArmor  = NULL;

  mComp.SetName("Soldier's empty comportament");
}
//---------------------------------------------------------------------------

CSoldier::CSoldier(struct _Infantry _data)
{
  memcpy(&mData, &_data, sizeof(struct _Infantry));

  mDaysWeaponUse     = 0;
  mDaysMoraleCt      = 0;
  mFirstWeaponIndex  = -1;
  mSecondWeaponIndex = -1;
  mArmorIndex        = -1;
  mSpecialDevice     = -1; // nu e totusi stabilit ce ar putea insemna acesrt device
  mEmbarked          = false;
  mData.crtCarry     = 0;
  mData.hp           = mData.maxHp;
  mData.morale       = 80;

  mIWeapon = NULL;
  mIArmor  = NULL;

  mComp.SetName("Soldier's empty comportament");
}
//---------------------------------------------------------------------------

CSoldier::CSoldier(const CSoldier &_soldier)
{
  mData              = _soldier.mData;
  mDaysWeaponUse     = _soldier.mDaysWeaponUse;
  mDaysMoraleCt      = _soldier.mDaysMoraleCt;
  mFirstWeaponIndex  = _soldier.mFirstWeaponIndex;
  mSecondWeaponIndex = _soldier.mSecondWeaponIndex;
  mArmorIndex        = _soldier.mArmorIndex;
  mSpecialDevice     = _soldier.mSpecialDevice;
  mEmbarked          = _soldier.mEmbarked;
  mData.crtCarry     = 0;
  mData.hp           = mData.maxHp;
  mData.morale       = 80;

  mIWeapon = NULL;
  mIArmor  = NULL;
  mComp    = _soldier.mComp;
}
//---------------------------------------------------------------------------

void CSoldier::GetData(struct _Infantry* _data)
{
  *_data = mData;
}
//---------------------------------------------------------------------------

void CSoldier::ModifyMorale(int _step)
{
  mData.morale += _step;
}
//---------------------------------------------------------------------------

void CSoldier::Evolve()
{
}
//---------------------------------------------------------------------------

void CSoldier::SetEmbark(bool _embark)
{
  if (!IsValid())
  {
    return;
  }

  mEmbarked = _embark;
}
//---------------------------------------------------------------------------

bool CSoldier::GetEmbark()
{
  return mEmbarked;
}
//---------------------------------------------------------------------------

bool CSoldier::SetFirstWeapon(CIWeapon* _weapon)
{
  struct _IWeapon lOldIWepData, lNewIWepData;
  int newWeaponWeight, oldWeaponWeight, newCarry;

  if ((_weapon == NULL) || (_weapon->IsNone()))
  {
    newWeaponWeight = 0;
  }
  else
  {
    _weapon->GetData(&lNewIWepData);
    newWeaponWeight = lNewIWepData.weight;
  }

  if (_weapon->IsUsed())
  {
    return false;
  }

  if ((mIWeapon == NULL) || (mIWeapon->IsNone()))
  {
    oldWeaponWeight = 0;
  }
  else
  {
    mIWeapon->GetData(&lOldIWepData);
    oldWeaponWeight = lOldIWepData.weight;
  }

  newCarry = mData.crtCarry - oldWeaponWeight + newWeaponWeight;

  if (newCarry < mData.maxCarry)
  {
    mIWeapon = _weapon;
    mData.crtCarry = newCarry;
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CSoldier::SetArmor(CIArmor* _armor)
{
  struct _IArmor lOldIArmData, lNewIArmData;
  int newArmorWeight, oldArmorWeight, newCarry;

  if ((_armor == NULL) || (_armor->IsNone()))
  {
    newArmorWeight = 0;
  }
  else
  {
    _armor->GetData(&lNewIArmData);
    newArmorWeight = lNewIArmData.weight;
  }

  if (_armor->IsUsed())
  {
    return false;
  }

  if ((mIArmor == NULL) || (mIArmor->IsNone()))
  {
    oldArmorWeight = 0;
  }
  else
  {
    mIArmor->GetData(&lOldIArmData);
    oldArmorWeight = lOldIArmData.weight;
  }

  newCarry = mData.crtCarry - oldArmorWeight + newArmorWeight;

  if (newCarry < mData.maxCarry)
  {
    mIArmor = _armor;
    mData.crtCarry = newCarry;
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

CIWeapon* CSoldier::GetFirstWeapon()
{
  return mIWeapon;
}
//---------------------------------------------------------------------------

CIArmor* CSoldier::GetArmor()
{
  return mIArmor;
}
//---------------------------------------------------------------------------

// Descriere   : verifica daca soldatul e valid pentru imbarcare (valid dpv medical)
// Param       : n/a
// Rezultat    : true/false - e valid/nu
// Comentarii  : nivelul minim de hp admis e SOLDIER_VALID_HP si e influentat si
//               de bravery si moral; variabila lMdf reprezinta modificatorul pe
//               care il produc bravery si moral asupra hp
bool CSoldier::IsValid()
{
  /*
  int lMdf;

  if (mData.morale > 80)
  {
    lMdf = 5;
  }

  if (mData.bravery > 90)
  {
    lMdf = 10;
  }
  else
  {
    if (mData.bravery > 80)
    {
      lMdf = 5;
    }
  }

  if (mData.hp < SOLDIER_VALID_HP - lMdf)
  {
    mEmbarked = false;
    return false;
  }
  */

  return true;
}
//---------------------------------------------------------------------------

void CSoldier::Serialize(PP::Stream& _a)
{
  //_a.Write(&mData,              sizeof(struct _Infantry));
  mData.Serialize(_a);

  _a.writeInt(&mDaysWeaponUse);
  _a.writeInt(&mDaysMoraleCt);
  _a.writeInt(&mFirstWeaponIndex);
  _a.writeInt(&mSecondWeaponIndex);
  _a.writeInt(&mArmorIndex);
  _a.writeInt(&mSpecialDevice);

  _a.writeBool(&mEmbarked);

  int lStatus = (int)mStatus;

  _a.writeInt(&lStatus);

  mComp.Serialize(_a);
}
//---------------------------------------------------------------------------

void CSoldier::DeSerialize(PP::Stream& _a)
{
  //_a.Read(&mData,              sizeof(struct _Infantry));
  mData.DeSerialize(_a);

  _a.readInt(&mDaysWeaponUse);
  _a.readInt(&mDaysMoraleCt);
  _a.readInt(&mFirstWeaponIndex);
  _a.readInt(&mSecondWeaponIndex);
  _a.readInt(&mArmorIndex);
  _a.readInt(&mSpecialDevice);

  _a.readBool(&mEmbarked);

  int lStatus;

  _a.readInt(&lStatus);

  mStatus = (TSoldierStatus)lStatus;

  mComp.DeSerialize(_a);
}
//---------------------------------------------------------------------------

CSoldier::~CSoldier()
{
}
//---------------------------------------------------------------------------


