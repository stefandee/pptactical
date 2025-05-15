//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998 - 2003 Stefan Dicu & Tudor Girba                       //
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

#include <math.h>
#pragma hdrstop

#include "config.h"
#include "PPIG_SubSysOscilator.h"
#include "PP_Timer.h"
//---------------------------------------------------------------------------

CIGSubSysOscilator::CIGSubSysOscilator(CIGBaseObject* _parent) : CIGSubSysBase(_parent)
{
  mAmplitude    = 1.0;
  mStep         = 0.25;
  mCurrentValue = 0.0;
  mUpdatePeriod = 100;
  mCurrentTime  = GetTimerInstance()->GetTime();
}
//---------------------------------------------------------------------------

CIGSubSysOscilator::~CIGSubSysOscilator()
{
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::Update()
{
  if (!GetUpdateEnabled())
  {
    return;
  }

  int lTime = GetTimerInstance()->GetTime();

  if (lTime - mCurrentTime > mUpdatePeriod)
  {
    mCurrentTime = lTime;

    mCurrentValue += mStep;

    if (mCurrentValue > mAmplitude)
    {
      mStep = -mStep;
    }

    if (mCurrentValue < -mAmplitude)
    {
      mStep = -mStep;
    }
  }
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::SetAmplitude(float _v)
{
  if (_v < 0)
  {
    _v = -_v;
  }

  if (_v < mCurrentValue && mCurrentValue > 0)
  {
    mCurrentValue = _v;
  }

  if (-_v > mCurrentValue && mCurrentValue < 0)
  {
    mCurrentValue = -_v;
  }

  mAmplitude = _v;
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::SetCurrentValue(float _v)
{
  mCurrentValue = _v;

  if (mCurrentValue > mAmplitude)
  {
    mCurrentValue = mAmplitude;
  }

  if (mCurrentValue < -mAmplitude)
  {
    mCurrentValue = mAmplitude;
  }
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::SetStep(float _v)
{
  mStep = fabs(_v);
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::SetUpdatePeriod(int _v)
{
  mUpdatePeriod = abs(_v);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// Oscilator serialize/deserialize section
//
//---------------------------------------------------------------------------

void CIGSubSysOscilator::Serialize(PP::Stream& _a)
{
  int lUpdateTime = GetTimerInstance()->GetTime() - mCurrentTime;

  CIGSubSysBase::Serialize(_a);

  _a.writeFloat(&mAmplitude);
  _a.writeFloat(&mStep);
  _a.writeFloat(&mCurrentValue);
  _a.writeInt(&mUpdatePeriod);
  _a.writeInt(&lUpdateTime);
}
//---------------------------------------------------------------------------

unsigned __int32 CIGSubSysOscilator::DataSize()
{
  return CIGSubSysBase::DataSize() + 3 * sizeof(float) + 2 * sizeof(int) + sizeof(bool);
}
//---------------------------------------------------------------------------

void CIGSubSysOscilator::DeSerialize(PP::Stream& _a)
{
  int lUpdateTime = 0;

  CIGSubSysBase::DeSerialize(_a);

  _a.readFloat(&mAmplitude);
  _a.readFloat(&mStep);
  _a.readFloat(&mCurrentValue);
  _a.readInt(&mUpdatePeriod);
  _a.readInt(&lUpdateTime);

  // setup
  mCurrentTime = GetTimerInstance()->GetTime() - lUpdateTime;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

