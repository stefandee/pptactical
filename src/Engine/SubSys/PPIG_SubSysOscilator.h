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

//-----------------------------------------------------------------------------
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  History           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysOscilatorH
#define PPIG_SubSysOscilatorH
//---------------------------------------------------------------------------
#include "PPIG_SubSysBase.h"

class CIGSubSysOscilator : public CIGSubSysBase
{
  public: // c-d
    CIGSubSysOscilator(CIGBaseObject* _parent);
    virtual ~CIGSubSysOscilator();

  public: // ops
    virtual void Update();

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  public: // get-set
    void  SetAmplitude(float _v);
    float GetAmplitude() { return mAmplitude;  }

    void SetStep(float _v);
    float GetStep() { return mStep; }

    void SetCurrentValue(float _v);
    float GetCurrentValue() { return mCurrentValue; }

    void SetUpdatePeriod(int _v);
    int GetUpdatePeriod() { return mUpdatePeriod; }

  private:
    float mAmplitude;
    float mStep;
    float mCurrentValue;
    int mUpdatePeriod, mCurrentTime;
};

#endif
