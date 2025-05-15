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

#include "config.h"
#include "PPIG_SubSysSensors.h"
#include "PPIG_Mission.h"
#include "PP_Timer.h"
//---------------------------------------------------------------------------

CIGSubSysSensors::CIGSubSysSensors(CIGBaseObject* _parent) : CIGSubSysBase(_parent)
{
  // lists
  mAttackers   = new CPList<CIGAuxAttacker>(2);
  mNoises      = new CPList<CIGAuxNoise>(4);

  //mAttackers   = 0;
  //mNoises      = 0;
}
//---------------------------------------------------------------------------

CIGSubSysSensors::~CIGSubSysSensors()
{
  if (mAttackers)
  {
    while(mAttackers->Count() > 0) { mAttackers->Delete(0); };
    delete mAttackers;
    mAttackers = 0;
  }

  if (mNoises)
  {
    while(mNoises->Count() > 0) { mNoises->Delete(0); };
    delete mNoises;
    mNoises = 0;
  }
}
//---------------------------------------------------------------------------

void CIGSubSysSensors::OnStimul(CIGObjectStimul& stimul)
{
   CIGAuxNoise    *lNoise;
   CIGAuxAttacker *lAttacker;

   switch (stimul.GetType())
   {
     case ST_NOISE_WEAPON_FIRE:
     case ST_NOISE_WEAPON_HIT :
     case ST_NOISE_MOVEMENT   :
       if (!stimul.GetSourceObject())
       {
         return;
       }

       // the noise queue is purged very often, so memorize everything
       // the "kill them all, let God sort them out" style
       lNoise = new CIGAuxNoise(
                      stimul.GetSourceObject()->GetLogicCoord(),
                      stimul.GetSourceObject(),
                      stimul.GetType(),
                      GetTimerInstance()->GetTime()
                      );

       GetNoisesList()->Add(lNoise);
       break;

      case ST_EXPLODE:
      case ST_FIRE:
      case ST_BULLET:
      case ST_LASER:
      case ST_PLASMA:
        if (!stimul.GetSourceObject())
        {
          return;
        }

        for(int i = 0; i < GetAttackersList()->Count(); i++)
        {
          // no duplicates
          if (GetAttackersList()->Get(i)->GetAttacker() == stimul.GetSourceObject())
          {
            // attacker was recorded, just update time
            GetAttackersList()->Get(i)->SetTime(GetTimerInstance()->GetTime());
            return;
          }
        }

        lAttacker = new CIGAuxAttacker();

        lAttacker->SetAttacker(stimul.GetSourceObject());
        lAttacker->SetTime(GetTimerInstance()->GetTime());
        GetAttackersList()->Add(lAttacker);
        break;
   }
}
//---------------------------------------------------------------------------

void CIGSubSysSensors::Update()
{
  CIGSubSysBase::Update();

  // remove older attackers
  int lTime = GetTimerInstance()->GetTime();

  for(int i = 0; i < GetAttackersList()->Count(); i++)
  {
    if (lTime - GetAttackersList()->Get(i)->GetTime() > I_ATTACKER_TIMEOUT)
    {
      GetAttackersList()->Delete(i);
      i = 0;
    }
  }

  // remove older noises
  lTime = GetTimerInstance()->GetTime();

  for(int i = 0; i < GetNoisesList()->Count(); i++)
  {
    if (lTime - GetNoisesList()->Get(i)->GetTime() > I_NOISES_TIMEOUT)
    {
      GetNoisesList()->Delete(i);
      i = 0;
    }
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Noises list serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGAuxAttacker::Serialize(PP::Stream& _a)
{
  CSerInt lAttacker(mAttacker->GetId());

  lAttacker.Serialize(_a);
  _a.writeInt(&mTime);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGAuxAttacker::DataSize()
{
  return 2 * sizeof(__int32);
}
//---------------------------------------------------------------------------

void CIGAuxAttacker::DeSerialize(PP::Stream& _a)
{
  CSerInt lAttacker;

  lAttacker.DeSerialize(_a);
  _a.readInt(&mTime);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Attackers list serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGAuxNoise::Serialize(PP::Stream& _a)
{
  CSerPoint lSourcePoint(mSourcePoint);
  CSerInt   lSourceObjectId(mSourceObject->GetId());

  lSourcePoint   .Serialize(_a);
  lSourceObjectId.Serialize(_a);
  _a.writeInt(&mType);
  _a.writeInt(&mTime);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGAuxNoise::DataSize()
{
  return CSerPoint().DataSize() + 3 * sizeof(int);
}
//---------------------------------------------------------------------------

void CIGAuxNoise::DeSerialize(PP::Stream& _a)
{
  CSerPoint lSourcePoint;
  CSerInt   lSourceObjectId;

  lSourcePoint   .DeSerialize(_a);
  lSourceObjectId.DeSerialize(_a);
  _a.readInt(&mType);
  _a.readInt(&mTime);

  mSourcePoint  = lSourcePoint.Get();
  mSourceObject = GetMissionInstance()->GetObjectById(lSourceObjectId.Get());
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Class serialize/deserialize section
//
//---------------------------------------------------------------------------
void CIGSubSysSensors::Serialize(PP::Stream& _a)
{
  int lNoisesCount = GetNoisesList()->Count(), lAttackerCount = GetAttackersList()->Count();

  // parent ser
  CIGSubSysBase::Serialize(_a);

  // write noises count so at deser we know how many to deser
  _a.writeInt(&lNoisesCount);

  for(int i = 0; i < lNoisesCount; i++)
  {
    GetNoisesList()->Get(i)->Serialize(_a);
  }

  // write attackers count so at deser we know how many to deser
  _a.writeInt(&lAttackerCount);

  for(int i = 0; i < lAttackerCount; i++)
  {
    GetAttackersList()->Get(i)->Serialize(_a);
  }
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysSensors::DataSize()
{
  return 0;
}
//---------------------------------------------------------------------------

void CIGSubSysSensors::DeSerialize(PP::Stream& _a)
{
  int lNoisesCount, lAttackerCount;

  // parent deser
  CIGSubSysBase::DeSerialize(_a);

  // clean the noises and attackers lists
  while(mAttackers->Count() > 0) { mAttackers->Delete(0); };
  while(mNoises->Count() > 0) { mNoises->Delete(0); };

  // we know how many noises we had
  _a.readInt(&lNoisesCount);

  for(int i = 0; i < lNoisesCount; i++)
  {
    CIGAuxNoise* lNoise = new CIGAuxNoise();

    lNoise->DeSerialize(_a);
  }

  // we know how many attackers we had
  _a.readInt(&lAttackerCount);

  for(int i = 0; i < lAttackerCount; i++)
  {
    CIGAuxAttacker* lAttacker = new CIGAuxAttacker();

    lAttacker->DeSerialize(_a);
  }
}
//---------------------------------------------------------------------------


#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

