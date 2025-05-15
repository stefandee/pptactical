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
#pragma hdrstop

#include "Platform.h"

#ifdef PPT_COMPILER_MICROSOFT
#define for if(0) {} else for
#endif

#include "Statics.h"
#include "Serializable.h"

void TMiniSoldier::Serialize(PP::Stream& _a)
{
  _a.writeInt(&proj_res);
  _a.writeInt(&nrg_res);
  _a.writeInt(&plasma_res);
  _a.writeInt(&bullet_res);
  _a.writeInt(&fire_res);
  _a.writeInt(&range);
  _a.writeInt(&precision);
  _a.writeInt(&radius);
  _a.writeInt(&fire_rate);
  _a.write(projectile, MAX_PP_PATH);
  _a.writeInt(&projectile_frames);
  _a.write(effect_bitmap, MAX_PP_PATH);
  _a.writeInt(&morale);
  _a.writeInt(&hp);
  _a.writeInt(&bravery);
  _a.writeInt(&sr_combat);
  _a.writeInt(&sniper_skill);
  _a.write(sprite, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

unsigned __int32 TMiniSoldier::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TMiniSoldier::DeSerialize(PP::Stream& _a)
{
  _a.readInt(&proj_res);
  _a.readInt(&nrg_res);
  _a.readInt(&plasma_res);
  _a.readInt(&bullet_res);
  _a.readInt(&fire_res);
  _a.readInt(&range);
  _a.readInt(&precision);
  _a.readInt(&radius);
  _a.readInt(&fire_rate);
  _a.read(projectile, MAX_PP_PATH);
  _a.readInt(&projectile_frames);
  _a.read(effect_bitmap, MAX_PP_PATH);
  _a.readInt(&morale);
  _a.readInt(&hp);
  _a.readInt(&bravery);
  _a.readInt(&sr_combat);
  _a.readInt(&sniper_skill);
  _a.read(sprite, MAX_PP_PATH);
}
//---------------------------------------------------------------------------

void TStaticWeapon::Serialize(PP::Stream& _a)
{
  _a.write(wepName, 32);
  _a.writeInt(&range);
  _a.writeInt(&precision);
  _a.writeInt(&radius);
  _a.writeInt(&damage);
  _a.write(projectile, MAX_PP_PATH);
  _a.write(effect_bitmap, MAX_PP_PATH);
  _a.writeInt(&fire_rate);
  _a.write(onFire, MAX_PP_PATH),
  _a.write(onHit, MAX_PP_PATH);
  _a.write(&type, sizeof(char));
}
//---------------------------------------------------------------------------

unsigned __int32 TStaticWeapon::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TStaticWeapon::DeSerialize(PP::Stream& _a)
{
  _a.read(wepName, 32);
  _a.readInt(&range);
  _a.readInt(&precision);
  _a.readInt(&radius);
  _a.readInt(&damage);
  _a.read(projectile, MAX_PP_PATH);
  _a.read(effect_bitmap, MAX_PP_PATH);
  _a.readInt(&fire_rate);
  _a.read(onFire, MAX_PP_PATH),
  _a.read(onHit, MAX_PP_PATH);
  _a.read(&type, sizeof(char));
}
//---------------------------------------------------------------------------

void TStaticLayer::Serialize(PP::Stream& _a)
{
  _a.write(mName, 32);
  _a.write(mSprite, 512);

  _a.writeInt(&mFrameDelay);
  _a.writeInt(&mCycleDelay);
  _a.writeInt(&mX);
  _a.writeInt(&mY);

  _a.writeFloat(&mAlpha);

  _a.writeInt(&mSpriteRows);
  _a.writeInt(&mSpriteCols);
}
//---------------------------------------------------------------------------

unsigned __int32 TStaticLayer::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TStaticLayer::DeSerialize(PP::Stream& _a)
{
  _a.read(mName, 32);
  _a.read(mSprite, 512);

  _a.readInt(&mFrameDelay);
  _a.readInt(&mCycleDelay);
  _a.readInt(&mX);
  _a.readInt(&mY);

  _a.readFloat(&mAlpha);

  _a.readInt(&mSpriteRows);
  _a.readInt(&mSpriteCols);
}
//---------------------------------------------------------------------------

void TStaticObject::Serialize(PP::Stream& _a)
{
  _a.write(objName, 32);
  _a.write(objDescription, 256);

  _a.writeInt(&hp);

  _a.writeInt(&proj_res);
  _a.writeInt(&nrg_res);
  _a.writeInt(&plasma_res);
  _a.writeInt(&bullet_res);
  _a.writeInt(&fire_res);

  _a.writeBool(&spreadFire);
  _a.writeBool(&indestructible);
  _a.writeBool(&isLandscape);

  _a.writeBool(&canFire);

  weapon.Serialize(_a);

  _a.writeBool(&canContainSoldiers);
  _a.writeInt(&soldiersNo);
  _a.writeInt(&defendTable);

  for(int i = 0; i < soldiersNo; i++)
  {
    soldiersTable[i].Serialize(_a);
  }

  _a.writeBool(&canContainVehicles);
  _a.writeInt(&vehiclesNo);

  CSerPoint(mDropPoint).Serialize(_a);
  CSerPoint(mFirePoint).Serialize(_a);

  _a.writeInt(&mObsPointsCount);

  for(int i = 0; i < mObsPointsCount; i++)
  {
   mObsPoints[i].Serialize(_a);
  }

  _a.write(&type, sizeof(char));

  _a.write(sprite, MAX_PP_PATH);

  _a.writeInt(&maskX);
  _a.writeInt(&maskY);

  // write the mask
  for(int i = 0; i < maskX; i++)
  {
    for(int j = 0; j < maskY; j++)
    {
      _a.write(&mask[i][j], sizeof(char));
    }
  }

  // write the altitudes
  for(int i = 0; i < maskX; i++)
  {
    for(int j = 0; j < maskY; j++)
    {
      _a.writeInt(&altitude[i][j]);
    }
  }

  // write the layers
  _a.writeInt(&mLayersCount);

  for(int i = 0; i < mLayersCount; i++)
  {
    mLayers[i].Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 TStaticObject::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TStaticObject::DeSerialize(PP::Stream& _a)
{
  _a.read(objName, 32);
  _a.read(objDescription, 256);

  _a.readInt(&hp);

  _a.readInt(&proj_res);
  _a.readInt(&nrg_res);
  _a.readInt(&plasma_res);
  _a.readInt(&bullet_res);
  _a.readInt(&fire_res);

  _a.readBool(&spreadFire);
  _a.readBool(&indestructible);
  _a.readBool(&isLandscape);

  _a.readBool(&canFire);

  weapon.DeSerialize(_a);

  _a.readBool(&canContainSoldiers);
  _a.readInt(&soldiersNo);
  _a.readInt(&defendTable);

  for(int i = 0; i < soldiersNo; i++)
  {
    soldiersTable[i].DeSerialize(_a);
  }

  _a.readBool(&canContainVehicles);
  _a.readInt(&vehiclesNo);

  CSerPoint lSerFirePoint, lSerDropPoint;

  lSerDropPoint.DeSerialize(_a);
  lSerFirePoint.DeSerialize(_a);

  _a.readInt(&mObsPointsCount);

  for(int i = 0; i < mObsPointsCount; i++)
  {
    mObsPoints[i].DeSerialize(_a);
  }

  _a.read(&type, sizeof(char));

  _a.read(sprite, MAX_PP_PATH);

  _a.readInt(&maskX);
  _a.readInt(&maskY);

  for(int i = 0; i < maskX; i++)
  {
    for(int j = 0; j < maskY; j++)
    {
      _a.read(&mask[i][j], sizeof(char));
    }
  }

  // read the altitudes
  for(int i = 0; i < maskX; i++)
  {
    for(int j = 0; j < maskY; j++)
    {
      _a.readInt(&altitude[i][j]);
    }
  }

  // read the layers
  _a.readInt(&mLayersCount);

  for(int i = 0; i < mLayersCount; i++)
  {
    mLayers[i].DeSerialize(_a);
  }

  // setup
  mDropPoint = lSerDropPoint.Get();
  mFirePoint = lSerFirePoint.Get();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
