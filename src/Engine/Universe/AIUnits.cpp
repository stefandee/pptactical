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
 application: PP Units Editor, PP Mission Editor

 descrition : structuri unificate de inf, vehicule si avioane
 last modify: 29 11 99
        by Grab
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#include "Platform.h"

#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

//#if defined(__SUNPRO_CC) || defined(__GNUG__)
#if defined(PPT_PLATFORM_LINUX) || defined(PPT_PLATFORM_SOLARIS)
#include <sys/types.h>
#include <unistd.h>
#endif

#ifdef PPT_COMPILER_MICROSOFT
#define for if(0) {} else for
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#pragma hdrstop

#include "config.h"
#include "AIUnits.h"
#include "Serializable.h"

void TUnitObservationPoint::Serialize(PP::Stream& _a)
{
  _a.write(mName, 32);

  _a.writeInt(&mMinScanRange);
  _a.writeInt(&mMaxScanRange);
  _a.writeInt(&mMinScanAngle);
  _a.writeInt(&mMaxScanAngle);
  _a.writeInt(&mFOV);
  _a.writeInt(&mScanAngle);

  _a.writeBool(&mActive);

  for(int i = 0; i < 8; i++)
  {
    CSerPoint(mMountPoints[i]).Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 TUnitObservationPoint::DataSize()
{
  return 32 + 4 * sizeof(int) + 4 + 8 * sizeof(CPPoint);
}
//---------------------------------------------------------------------------

void TUnitObservationPoint::DeSerialize(PP::Stream& _a)
{
  _a.read(mName, 32);

  _a.readInt(&mMinScanRange);
  _a.readInt(&mMaxScanRange);
  _a.readInt(&mMinScanAngle);
  _a.readInt(&mMaxScanAngle);
  _a.readInt(&mFOV);
  _a.readInt(&mScanAngle);

  _a.readBool(&mActive);

  for(int i = 0; i < 8; i++)
  {
    CSerPoint lPoint;

    lPoint.DeSerialize(_a);

    mMountPoints[i] = lPoint.Get();
  }
}
//---------------------------------------------------------------------------

void TInfantryUnified::Serialize(PP::Stream& _a)
{
   _a.write(name,        16);
   _a.write(surname,     16);
   _a.write(description, 192);

   _a.writeInt(&gender);
   _a.writeInt(&type);
   _a.write(kind, 15);
   _a.writeInt(&morale);
   _a.writeInt(&hp);

   _a.write(sprite, 32);
   _a.write(shadow, 32);
   _a.write(mPortrait, 32);

   _a.writeInt(&leadership);
   _a.writeInt(&experience);
   _a.writeInt(&bravery);
   _a.writeInt(&camo);
   _a.writeInt(&antitank);
   _a.writeInt(&sr_combat);
   _a.writeInt(&snipery);
   _a.writeInt(&high_tech);
   _a.writeInt(&stealth);
   _a.writeInt(&influence);
   _a.writeInt(&wisdom);
   _a.writeInt(&mMaxUpSlope);
   _a.writeInt(&mMaxDownSlope);

   _a.writeInt(&proj_res);
   _a.writeInt(&nrg_res);
   _a.writeInt(&plasma_res);
   _a.writeInt(&bullet_res);
   _a.writeInt(&fire_res);

   _a.writeInt(&weptype);
   _a.writeInt(&damage);
   _a.writeInt(&range);
   _a.writeInt(&precision);
   _a.writeInt(&radius);
   _a.writeInt(&fire_rate);
   _a.writeInt(&projectiles_per_shot);
   _a.writeInt(&mTimeToLive);
   _a.writeInt(&mTrackPersistency);

   _a.write(projectile, 32);
   _a.write(effect_bitmap, 32);
   _a.write(aftereffect_bitmap ,32);
   _a.write(onhit, 32);
   _a.write(onfire, 32);

   _a.writeInt(&mObsPointsCount);

   for(int i = 0; i < mObsPointsCount; i++)
   {
     mObsPoints[i].Serialize(_a);
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint(mFireHoles[i]).Serialize(_a);
   }

   _a.write(resp1, 12);
   _a.write(resp2, 12);
   _a.write(resp3, 12);
   _a.write(ack1, 12);
   _a.write(ack2, 12);
   _a.write(ack3, 12);
   _a.write(annoyed1, 12);
   _a.write(annoyed2, 12);
   _a.write(annoyed3, 12);
   _a.write(onhit1, 12);
   _a.write(onhit2, 12);
   _a.write(onkill1, 12);
   _a.write(onkill2, 12);
   _a.write(die1, 12);
   _a.write(die2,12);
   _a.write(onidle1, 12);
   _a.write(onidle2, 12);
}
//---------------------------------------------------------------------------

unsigned __int32 TInfantryUnified::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TInfantryUnified::DeSerialize(PP::Stream& _a)
{
   _a.read(name,        16);
   _a.read(surname,     16);
   _a.read(description, 192);

   _a.readInt(&gender);
   _a.readInt(&type);
   _a.read(kind, 15);
   _a.readInt(&morale);
   _a.readInt(&hp);

   _a.read(sprite, 32);
   _a.read(shadow, 32);
   _a.read(mPortrait, 32);

   _a.readInt(&leadership);
   _a.readInt(&experience);
   _a.readInt(&bravery);
   _a.readInt(&camo);
   _a.readInt(&antitank);
   _a.readInt(&sr_combat);
   _a.readInt(&snipery);
   _a.readInt(&high_tech);
   _a.readInt(&stealth);
   _a.readInt(&influence);
   _a.readInt(&wisdom);
   _a.readInt(&mMaxUpSlope);
   _a.readInt(&mMaxDownSlope);

   _a.readInt(&proj_res);
   _a.readInt(&nrg_res);
   _a.readInt(&plasma_res);
   _a.readInt(&bullet_res);
   _a.readInt(&fire_res);

   _a.readInt(&weptype);
   _a.readInt(&damage);
   _a.readInt(&range);
   _a.readInt(&precision);
   _a.readInt(&radius);
   _a.readInt(&fire_rate);
   _a.readInt(&projectiles_per_shot);
   _a.readInt(&mTimeToLive);
   _a.readInt(&mTrackPersistency);

   _a.read(projectile, 32);
   _a.read(effect_bitmap, 32);
   _a.read(aftereffect_bitmap ,32);
   _a.read(onhit, 32);
   _a.read(onfire, 32);

   _a.readInt(&mObsPointsCount);

   for(int i = 0; i < mObsPointsCount; i++)
   {
     mObsPoints[i].DeSerialize(_a);
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     mFireHoles[i] = lPoint.Get();
   }

   _a.read(resp1, 12);
   _a.read(resp2, 12);
   _a.read(resp3, 12);
   _a.read(ack1, 12);
   _a.read(ack2, 12);
   _a.read(ack3, 12);
   _a.read(annoyed1, 12);
   _a.read(annoyed2, 12);
   _a.read(annoyed3, 12);
   _a.read(onhit1, 12);
   _a.read(onhit2, 12);
   _a.read(onkill1, 12);
   _a.read(onkill2, 12);
   _a.read(die1, 12);
   _a.read(die2,12);
   _a.read(onidle1, 12);
   _a.read(onidle2, 12);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: verifica daca filename este un fisier cu inregistrari corecte
---------------------------------------------------------------------------*/
bool verifyAIInfFile(const char *filename)
{
   int handle = open(filename, O_RDONLY);
   if (handle == -1)
   {
      close(handle);
      return false;
   }

   if ((filelength(handle) % AIInfRecSize) != 0)
   {
      close(handle);
      return false;
   }

   close(handle);
   return true;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 descrition: returneaza numarul de inregistrari din filename
---------------------------------------------------------------------------*/
int getAIInfRecords(const char *filename)
{
   if (!verifyAIInfFile(filename))
      return -1; // invalid file length

   int handle = open(filename, O_RDWR | O_BINARY);

   if (handle == -1)
      return -1;

   int length = filelength(handle);
   if (length == -1)
   {
      close(handle);
      return -1;
   }

   close(handle);
   return (int)(length / AIInfRecSize);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: pune in aiinfantry inregistrarea index din filename
             returneaza false daca este eroare
---------------------------------------------------------------------------*/
bool getAIInfRecord(const char* filename, int index, hInfantryUnified infantry)
{
   if (!verifyAIInfFile(filename))
      return false; // invalid file length

   if ((index < 0) || (index > getAIInfRecords(filename) - 1))
      return false;

   int handle = open(filename, O_RDWR | O_BINARY);

   if (handle == -1)
   {
      close(handle);
      return false;
   }

   if (lseek(handle, index * AIInfRecSize, SEEK_SET) == -1)
   {
      close(handle);
      return false;
   }

   if (read(handle, infantry, AIInfRecSize) == -1)
   {
      close(handle);
      return false;
   }

   close(handle);

   return true;
}
//---------------------------------------------------------------------------

void TVehicleUnified::Serialize(PP::Stream& _a)
{
   VLOG(5) << "TVehicleUnified::Serialize started at: " << _a.tell();

   _a.write(name, 32);
   _a.writeInt(&hp);
   _a.writeInt(&engine_type);

   _a.writeBool(&selfdestruct);
   _a.write(sprite, 32);
   _a.write(shadow, 32);
   _a.write(mPortrait, 32);
   _a.writeInt(&vType);
   _a.writeInt(&mTurretTurnRate);

   _a.write(mExplodeSprite, 32);
   _a.writeInt(&mExplodeCount);

   VLOG(5) << "TVehicleUnified::Serialize before points at: " << _a.tell();

   for(int i = 0; i < 8; i++)
   {
     CSerPoint(body[i]).Serialize(_a);
   }

   for(int i = 0; i < 16; i++)
   {
     CSerPoint(turret[i]).Serialize(_a);
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint(primary[i]).Serialize(_a);
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint(secondary[i]).Serialize(_a);
   }

   VLOG(5) << "TVehicleUnified::Serialize after points at: " << _a.tell();

   _a.writeInt(&bodyLightsNo);

   for(int i = 0; i < VEHICLE_MAXBODYLIGHTS; i++)
   {
     CSerPoint(bodyLights[i]).Serialize(_a);
   }

   _a.writeInt(&turretLightsNo);

   for(int i = 0; i < VEHICLE_MAXTURRETLIGHTS; i++)
   {
     CSerPoint(turretLights[i]).Serialize(_a);
   }

   _a.writeInt(&mObsPointsCount);

   for(int i = 0; i < mObsPointsCount; i++)
   {
     mObsPoints[i].Serialize(_a);
   }

   VLOG(5) << "TVehicleUnified::Serialize after turret lights at: " <<_a.tell();

   _a.writeInt(&mBayCapacity);
   _a.writeInt(&movement);
   _a.writeInt(&mMaxUpSlope);
   _a.writeInt(&mMaxDownSlope);

   _a.write(callsign, 16);
   _a.write(description, 192);
   _a.writeInt(&experience);
   _a.writeInt(&driveskill);
   _a.writeInt(&bravery);
   _a.writeInt(&morale);
   _a.writeInt(&leadership);
   _a.writeInt(&anti_infantry);
   _a.writeInt(&anti_vehicle);
   _a.writeInt(&anti_air);
   _a.writeInt(&cm_skill);
   _a.writeInt(&teamwork);

   _a.writeBool(&wep1_enabled);
   _a.writeInt(&wep1_type);
   _a.writeInt(&wep1_damage);
   _a.writeInt(&wep1_minrange);
   _a.writeInt(&wep1_maxrange);
   _a.writeInt(&wep1_precision);
   _a.writeInt(&wep1_radius);
   _a.writeInt(&wep1_firerate);
   _a.writeInt(&wep1_projectiles_per_shot);
   _a.writeInt(&mWep1_TimeToLive);
   _a.writeInt(&wep1_trackPersistency);

   _a.write(wep1_projectile, 32);
   _a.write(wep1_effectbitmap, 32);
   _a.write(wep1_aftereffectbitmap, 32);
   _a.write(wep1_onhit, 32);
   _a.write(wep1_onfire, 32);

   _a.writeBool(&wep2_enabled);
   _a.writeInt(&wep2_type);
   _a.writeInt(&wep2_damage);
   _a.writeInt(&wep2_minrange);
   _a.writeInt(&wep2_maxrange);
   _a.writeInt(&wep2_precision);
   _a.writeInt(&wep2_radius);
   _a.writeInt(&wep2_firerate);
   _a.writeInt(&wep2_projectiles_per_shot);
   _a.writeInt(&mWep2_TimeToLive);
   _a.writeInt(&wep2_trackPersistency);

   _a.write(wep2_projectile, 32);
   _a.write(wep2_effectbitmap, 32);
   _a.write(wep2_aftereffectbitmap, 32);
   _a.write(wep2_onhit, 32);
   _a.write(wep2_onfire, 32);

   _a.writeInt(&exp_res);
   _a.writeInt(&nrg_res);
   _a.writeInt(&plasma_res);
   _a.writeInt(&bullet_res);
   _a.writeInt(&fire_res);

   _a.write(resp1, 32);
   _a.write(resp2, 32);
   _a.write(resp3, 32);
   _a.write(ack1, 32);
   _a.write(ack2, 32);
   _a.write(ack3, 32);
   _a.write(annoyed1, 32);
   _a.write(annoyed2, 32);
   _a.write(annoyed3, 32);
   _a.write(onhit1, 32);
   _a.write(onhit2, 32);
   _a.write(onkill1, 32);
   _a.write(onkill2, 32);
   _a.write(die1, 32);
   _a.write(die2, 32);
   _a.write(onidle1, 32);
   _a.write(onidle2, 32);

   VLOG(5) << "TVehicleUnified::Serialize ended at: " << _a.tell();
}
//---------------------------------------------------------------------------

unsigned __int32 TVehicleUnified::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void TVehicleUnified::DeSerialize(PP::Stream& _a)
{
   VLOG(5) << "TVehicleUnified::DeSerialize started at: " <<_a.tell();

   _a.read(name, 32);
   _a.readInt(&hp);
   _a.readInt(&engine_type);

   _a.readBool(&selfdestruct);
   _a.read(sprite, 32);
   _a.read(shadow, 32);
   _a.read(mPortrait, 32);
   _a.readInt(&vType);
   _a.readInt(&mTurretTurnRate);

   _a.read(mExplodeSprite, 32);
   _a.readInt(&mExplodeCount);

   VLOG(5) << "TVehicleUnified::DeSerialize before points at: " << _a.tell();

   for(int i = 0; i < 8; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     body[i] = lPoint.Get();
   }

   for(int i = 0; i < 16; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     turret[i] = lPoint.Get();
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     primary[i] = lPoint.Get();
   }

   for(int i = 0; i < 8; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     secondary[i] = lPoint.Get();
   }

   VLOG(5) << "TVehicleUnified::DeSerialize after points at: " << _a.tell();

   _a.readInt(&bodyLightsNo);

   for(int i = 0; i < VEHICLE_MAXBODYLIGHTS; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     bodyLights[i] = lPoint.Get();
   }

   _a.readInt(&turretLightsNo);

   for(int i = 0; i < VEHICLE_MAXTURRETLIGHTS; i++)
   {
     CSerPoint lPoint;

     lPoint.DeSerialize(_a);

     turretLights[i] = lPoint.Get();
   }

   _a.readInt(&mObsPointsCount);

   for(int i = 0; i < mObsPointsCount; i++)
   {
     mObsPoints[i].DeSerialize(_a);
   }

   VLOG(5) << "TVehicleUnified::DeSerialize after turret lights at: " << _a.tell();

   _a.readInt(&mBayCapacity);
   _a.readInt(&movement);
   _a.readInt(&mMaxUpSlope);
   _a.readInt(&mMaxDownSlope);

   _a.read(callsign, 16);
   _a.read(description, 192);
   _a.readInt(&experience);
   _a.readInt(&driveskill);
   _a.readInt(&bravery);
   _a.readInt(&morale);
   _a.readInt(&leadership);
   _a.readInt(&anti_infantry);
   _a.readInt(&anti_vehicle);
   _a.readInt(&anti_air);
   _a.readInt(&cm_skill);
   _a.readInt(&teamwork);

   _a.readBool(&wep1_enabled);
   _a.readInt(&wep1_type);
   _a.readInt(&wep1_damage);
   _a.readInt(&wep1_minrange);
   _a.readInt(&wep1_maxrange);
   _a.readInt(&wep1_precision);
   _a.readInt(&wep1_radius);
   _a.readInt(&wep1_firerate);
   _a.readInt(&wep1_projectiles_per_shot);
   _a.readInt(&mWep1_TimeToLive);
   _a.readInt(&wep1_trackPersistency);

   _a.read(wep1_projectile, 32);
   _a.read(wep1_effectbitmap, 32);
   _a.read(wep1_aftereffectbitmap, 32);
   _a.read(wep1_onhit, 32);
   _a.read(wep1_onfire, 32);

   _a.readBool(&wep2_enabled);
   _a.readInt(&wep2_type);
   _a.readInt(&wep2_damage);
   _a.readInt(&wep2_minrange);
   _a.readInt(&wep2_maxrange);
   _a.readInt(&wep2_precision);
   _a.readInt(&wep2_radius);
   _a.readInt(&wep2_firerate);
   _a.readInt(&wep2_projectiles_per_shot);
   _a.readInt(&mWep2_TimeToLive);
   _a.readInt(&wep2_trackPersistency);

   _a.read(wep2_projectile, 32);
   _a.read(wep2_effectbitmap, 32);
   _a.read(wep2_aftereffectbitmap, 32);
   _a.read(wep2_onhit, 32);
   _a.read(wep2_onfire, 32);

   _a.readInt(&exp_res);
   _a.readInt(&nrg_res);
   _a.readInt(&plasma_res);
   _a.readInt(&bullet_res);
   _a.readInt(&fire_res);

   _a.read(resp1, 32);
   _a.read(resp2, 32);
   _a.read(resp3, 32);
   _a.read(ack1, 32);
   _a.read(ack2, 32);
   _a.read(ack3, 32);
   _a.read(annoyed1, 32);
   _a.read(annoyed2, 32);
   _a.read(annoyed3, 32);
   _a.read(onhit1, 32);
   _a.read(onhit2, 32);
   _a.read(onkill1, 32);
   _a.read(onkill2, 32);
   _a.read(die1, 32);
   _a.read(die2, 32);
   _a.read(onidle1, 32);
   _a.read(onidle2, 32);

   VLOG(5) << "TVehicleUnified::DeSerialize ended at: " << _a.tell();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: verifica daca filename este un fisier cu inregistrari corecte
---------------------------------------------------------------------------*/
bool verifyAIVehFile(const char *filename)
{
   int handle = open(filename, O_RDONLY);
   if (handle == -1)
   {
      close(handle);
      return false;
   }

   if ((filelength(handle) % AIVehRecSize) != 0)
   {
      close(handle);
      return false;
   }

   close(handle);
   return true;
}
//---------------------------------------------------------------------------


/*---------------------------------------------------------------------------
 descrition: returneaza numarul de inregistrari din filename
---------------------------------------------------------------------------*/
int getAIVehRecords(const char *filename)
{
   if (!verifyAIVehFile(filename))
      return -1; // invalid file length

   int handle = open(filename, O_RDWR | O_BINARY);

   if (handle == -1)
      return -1;

   int length = filelength(handle);
   if (length == -1)
   {
      close(handle);
      return -1;
   }

   close(handle);
   return (int)(length / AIVehRecSize);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 descrition: pune in aivehicle inregistrarea index din filename
             returneaza false daca este eroare
---------------------------------------------------------------------------*/
bool getAIVehRecord(const char* filename, int index, hVehicleUnified vehicle)
{
   if (!verifyAIVehFile(filename))
      return false; // invalid file length

   if ((index < 0) || (index > getAIVehRecords(filename) - 1))
      return false;

   int handle = open(filename, O_RDWR | O_BINARY);

   if (handle == -1)
   {
      close(handle);
      return false;
   }

   if (lseek(handle, index * AIVehRecSize, SEEK_SET) == -1)
   {
      close(handle);
      return false;
   }

   if (read(handle, vehicle, AIVehRecSize) == -1)
   {
      close(handle);
      return false;
   }

   close(handle);

   return true;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
