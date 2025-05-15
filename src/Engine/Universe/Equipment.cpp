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

#include "Equipment.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

void TUEngine::Serialize(PP::Stream& _a)
{
  _a.write(name, 20);
  _a.write(description, 200);
  _a.writeInt(&max_speed);
  _a.writeInt(&max_carry);
  _a.writeInt(&weight);
  _a.writeInt(&price);
  _a.write(bitmap, MAX_PP_PATH);
  _a.writeInt(&frames);
  _a.write(sound, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUEngine::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 20);
  _a.read(description, 200);
  _a.readInt(&max_speed);
  _a.readInt(&max_carry);
  _a.readInt(&weight);
  _a.readInt(&price);
  _a.read(bitmap, MAX_PP_PATH);
  _a.readInt(&frames);
  _a.read(sound, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUVArmor::Serialize(PP::Stream& _a)
{
  _a.write(name, 32);
  _a.write(description, 256);
  _a.writeInt(&proj_res);
  _a.writeInt(&nrg_res);
  _a.writeInt(&plasma_res);
  _a.writeInt(&bullet_res);
  _a.writeInt(&fire_res);
  _a.writeInt(&type);
  _a.writeInt(&value);
  _a.writeInt(&tech_level);
  _a.write(bitmap, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUVArmor::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 32);
  _a.read(description, 256);
  _a.readInt(&proj_res);
  _a.readInt(&nrg_res);
  _a.readInt(&plasma_res);
  _a.readInt(&bullet_res);
  _a.readInt(&fire_res);
  _a.readInt(&type);
  _a.readInt(&value);
  _a.readInt(&tech_level);
  _a.read(bitmap, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUIArmor::Serialize(PP::Stream& _a)
{
  _a.write(name, 32);
  _a.write(description, 256);
  _a.writeInt(&proj_res);
  _a.writeInt(&nrg_res);
  _a.writeInt(&plasma_res);
  _a.writeInt(&bullet_res);
  _a.writeInt(&fire_res);
  _a.writeInt(&weight);
  _a.writeInt(&value);
  _a.writeInt(&tech_level);
  _a.write(bitmap, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUIArmor::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 32);
  _a.read(description, 256);
  _a.readInt(&proj_res);
  _a.readInt(&nrg_res);
  _a.readInt(&plasma_res);
  _a.readInt(&bullet_res);
  _a.readInt(&fire_res);
  _a.readInt(&weight);
  _a.readInt(&value);
  _a.readInt(&tech_level);
  _a.read(bitmap, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUVWeapon::Serialize(PP::Stream& _a)
{
  _a.write(name, 32);
  _a.write(description, 256);

  int lType, lSideFx;

  lType   = (int)type;
  lSideFx = (int)side_effect;

  _a.writeInt(&lType);
  _a.writeInt(&lSideFx);

  _a.writeInt(&minRange);
  _a.writeInt(&maxRange);
  _a.writeInt(&damage);
  _a.writeInt(&precision);
  _a.writeInt(&speed);
  _a.writeInt(&weight);
  _a.writeInt(&value);
  _a.writeInt(&tech_level);
  _a.writeInt(&radius);
  _a.writeInt(&effect_range);
  _a.writeInt(&fire_rate);

  _a.write(bitmap, MAX_PP_PATH);
  _a.write(projectile, MAX_PP_PATH);
  _a.write(effect_bitmap, MAX_PP_PATH);
  _a.write(onhit, MAX_PP_PATH);
  _a.write(onfire, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUVWeapon::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 32);
  _a.read(description, 256);

  int lType, lSideFx;

  _a.readInt(&lType);
  _a.readInt(&lSideFx);

  type = (TVWeapon)lType;
  side_effect = (TSideFx)lSideFx;

  _a.readInt(&minRange);
  _a.readInt(&maxRange);
  _a.readInt(&damage);
  _a.readInt(&precision);
  _a.readInt(&speed);
  _a.readInt(&weight);
  _a.readInt(&value);
  _a.readInt(&tech_level);
  _a.readInt(&radius);
  _a.readInt(&effect_range);
  _a.readInt(&fire_rate);

  _a.read(bitmap, MAX_PP_PATH);
  _a.read(projectile, MAX_PP_PATH);
  _a.read(effect_bitmap, MAX_PP_PATH);
  _a.read(onhit, MAX_PP_PATH);
  _a.read(onfire, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUIWeapon::Serialize(PP::Stream& _a)
{
  _a.write(name, 32);
  _a.write(description, 256);

  _a.writeInt(&minRange);
  _a.writeInt(&maxRange);
  _a.writeInt(&precision);
  _a.writeInt(&radius);
  _a.writeInt(&value);
  _a.writeInt(&tech_level);
  _a.writeInt(&speed);
  _a.writeInt(&weight);
  _a.writeInt(&damage);
  _a.writeInt(&fire_rate);

  int lType, lSideFx;

  lType = (int)type;
  lSideFx = (int)side_effect;

  _a.writeInt(&lType);
  _a.writeInt(&lSideFx);

  _a.write(bitmap, MAX_PP_PATH);
  _a.write(projectile, MAX_PP_PATH);
  _a.write(effect_bitmap, MAX_PP_PATH);
  _a.write(onhit, MAX_PP_PATH);
  _a.write(onfire, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TUIWeapon::DeSerialize(PP::Stream& _a)
{
  _a.read(name, 32);
  _a.read(description, 256);

  _a.readInt(&minRange);
  _a.readInt(&maxRange);
  _a.readInt(&precision);
  _a.readInt(&radius);
  _a.readInt(&value);
  _a.readInt(&tech_level);
  _a.readInt(&speed);
  _a.readInt(&weight);
  _a.readInt(&damage);
  _a.readInt(&fire_rate);

  int lType, lSideFx;

  _a.readInt(&lType);
  _a.readInt(&lSideFx);
    
  type = (TIWeapon)lType;
  side_effect = (TSideFx)lSideFx;

  _a.read(bitmap, MAX_PP_PATH);
  _a.read(projectile, MAX_PP_PATH);
  _a.read(effect_bitmap, MAX_PP_PATH);
  _a.read(onhit, MAX_PP_PATH);
  _a.read(onfire, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

CIWeapon::CIWeapon()
{
  // creaza arma "no weapon"
  /*
  strcpy(mData.name, "(no weapon)");
  strcpy(mData.description, "No data available.");

  mData.type         = IWT_NONE;
  mData.side_effect  = FX_NONE;
  mData.minRange     = 0;
  mData.maxRange     = 1;
  mData.damage       = 0;
  mData.precision    = 0;
  mData.speed        = 0;
  mData.weight       = 0;
  mData.value        = 1000;
  mData.tech_level   = 0;
  mData.radius       = 0;
  mData.fire_rate    = 0;

  strcpy(mData.bitmap, "");
  strcpy(mData.projectile, "");
  strcpy(mData.effect_bitmap, "");
  strcpy(mData.onhit, "");
  strcpy(mData.onfire, "");
  */

  mUsed      = false;
  //mNone = true;
  mData.type = IWT_NONE;
}
//---------------------------------------------------------------------------

CIWeapon::CIWeapon(TUIWeapon _data)
{
  mData      = _data;
  mUsed      = false;
  mData.type = IWT_NONE;
  //mNone = false;
}
//---------------------------------------------------------------------------

CIWeapon::CIWeapon(const CIWeapon &_weapon)
{
  mData = _weapon.mData;
  mUsed = _weapon.mUsed;
  //mNone = _weapon.mNone;
}
//---------------------------------------------------------------------------

void CIWeapon::SetUsed(bool _used)
{
  if (!IsNone())
  {
    mUsed = _used;
  }
}
//---------------------------------------------------------------------------

void CIWeapon::GetData(TUIWeapon* _data)
{
  *_data = mData;
}
//---------------------------------------------------------------------------

bool CIWeapon::IsUsed()
{
  return mUsed;
}
//---------------------------------------------------------------------------

bool CIWeapon::IsNone()
{
  return (mData.type == IWT_NONE);
}
//---------------------------------------------------------------------------

void CIWeapon::Serialize(PP::Stream& _a)
{
  //_a.write(&mData, sizeof(struct _IWeapon));
  mData.Serialize(_a);

  _a.writeBool(&mUsed);
  //_a.Write(&mNone, sizeof(bool));
}
//---------------------------------------------------------------------------

void CIWeapon::DeSerialize(PP::Stream& _a)
{
  //_a.read(&mData, sizeof(struct _IWeapon));
  mData.DeSerialize(_a);

  _a.readBool(&mUsed);
  //_a.Read(&mNone, sizeof(bool));
}
//---------------------------------------------------------------------------

CIWeapon::~CIWeapon()
{
}
//---------------------------------------------------------------------------

CIArmor::CIArmor()
{
  // creaza armura "no armor"
  strcpy(mData.name, "No Armor");
  strcpy(mData.description, "No data available.");
  mUsed = false;
  mNone = true;
}
//---------------------------------------------------------------------------

CIArmor::CIArmor(struct _IArmor _data)
{
  mUsed = false;
  mNone = false;
  mData = _data;
}
//---------------------------------------------------------------------------

CIArmor::CIArmor(const CIArmor &_armor)
{
  mData = _armor.mData;
  mUsed = _armor.mUsed;
  mNone = _armor.mNone;
}
//---------------------------------------------------------------------------

void CIArmor::SetUsed(bool _used)
{
  if (!mNone)
  {
    mUsed = _used;
  }
}
//---------------------------------------------------------------------------

void CIArmor::GetData(struct _IArmor* _data)
{
  *_data = mData;
}
//---------------------------------------------------------------------------

bool CIArmor::IsUsed()
{
  return mUsed;
}
//---------------------------------------------------------------------------

bool CIArmor::IsNone()
{
  return mNone;
}
//---------------------------------------------------------------------------

void CIArmor::Serialize(PP::Stream& _a)
{
  //_a.write(&mData, sizeof(struct _IArmor));
  mData.Serialize(_a);

  _a.writeBool(&mUsed);
  _a.writeBool(&mNone);
}
//---------------------------------------------------------------------------

void CIArmor::DeSerialize(PP::Stream& _a)
{
  //_a.read(&mData, sizeof(struct _IArmor));
  mData.DeSerialize(_a);

  _a.readBool(&mUsed);
  _a.readBool(&mNone);
}
//---------------------------------------------------------------------------

CIArmor::~CIArmor()
{
}
//---------------------------------------------------------------------------

CVWeapon::CVWeapon()
{
  // creaza arma "no weapon"
  /*
  strcpy(mData.name, "(no weapon)");
  strcpy(mData.description, "No data available.");

  mData.type         = VWT_NONE;
  mData.side_effect  = FX_NONE;
  mData.minRange     = 0;
  mData.maxRange     = 1;
  mData.damage       = 0;
  mData.precision    = 0;
  mData.speed        = 0;
  mData.weight       = 0;
  mData.value        = 1000;
  mData.tech_level   = 0;
  mData.radius       = 0;
  mData.effect_range = 0;
  mData.fire_rate    = 0;

  strcpy(mData.bitmap, "");
  strcpy(mData.projectile, "");
  strcpy(mData.effect_bitmap, "");
  strcpy(mData.onhit, "");
  strcpy(mData.onfire, "");
  */

  mUsed      = false;
  mData.type = VWT_NONE;
  //mNone = true;
}
//---------------------------------------------------------------------------

CVWeapon::CVWeapon(struct _VWeapon _data)
{
  mData      = _data;
  mUsed      = false;
  //mNone = false;

  /*
  if (mData.type == VWT_NONE)
  {
    mNone = true;
  }
  */
}
//---------------------------------------------------------------------------

CVWeapon::CVWeapon(const CVWeapon &_weapon)
{
  mData = _weapon.mData;
  mUsed = _weapon.mUsed;
  //mNone = _weapon.mNone;

  /*
  if (mData.type == VWT_NONE)
  {
    mNone = true;
  }
  */
}
//---------------------------------------------------------------------------

void CVWeapon::SetUsed(bool _used)
{
  if (!IsNone())
  {
    mUsed = _used;
  }
}
//---------------------------------------------------------------------------

void CVWeapon::GetData(struct _VWeapon* _data)
{
  *_data = mData;
}
//---------------------------------------------------------------------------

bool CVWeapon::IsUsed()
{
  return mUsed;
}
//---------------------------------------------------------------------------

bool CVWeapon::IsNone()
{
  return (mData.type == VWT_NONE);
}
//---------------------------------------------------------------------------

void CVWeapon::Serialize(PP::Stream& _a)
{
  //_a.write(&mData, sizeof(struct _VWeapon));
  mData.Serialize(_a);
  _a.writeBool(&mUsed);
  //_a.Write(&mNone, sizeof(bool));
}
//---------------------------------------------------------------------------

void CVWeapon::DeSerialize(PP::Stream& _a)
{
  //_a.read(&mData, sizeof(struct _VWeapon));
  mData.DeSerialize(_a);
  
  _a.readBool(&mUsed);
  //_a.Read(&mNone, sizeof(bool));
}
//---------------------------------------------------------------------------

CVWeapon::~CVWeapon()
{
}
//---------------------------------------------------------------------------
