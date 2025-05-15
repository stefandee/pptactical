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
//  Unit              : Damage SubSystem Propulsion
//
//  Version           : 1.0
//
//  Description       :
//    *
//  History           :
//    [12.11.2002] - [Karg] - unit/class created
//-----------------------------------------------------------------------------

#ifndef PPIG_SubSysDPropulsionH
#define PPIG_SubSysDPropulsionH
//---------------------------------------------------------------------------

#include "PPIG_SubSysBase.h"
class CIGDamageObject;

class CIGSubSysDPropulsion : public CIGSubSysBase
{
  public:
    CIGSubSysDPropulsion(CIGDamageObject* _parent);
    virtual ~CIGSubSysDPropulsion();

    virtual void SetSenseCounter(CPCounter& value);
    virtual void SetSenseCounter(int value);

    virtual float GetCurrentAngle();
    virtual void SetCurrentAngle(float value);

    CIGSubSysAction* GetActionInstance(TIGSubSysActionData actionData);

    virtual void CreateTrackEffect() {}

  public: // serialization - deserialization
    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

  protected:
    virtual void TurnToAngle(float Angle);
    virtual CPPoint CheckPath(CPPoint _startPoint, CPPoint _endPoint);

  private:
    float          mCurrentAngle;    //the current orientation of the object in radians
};

/*---------------------------------------------------------------------------
 description: Current Angle
---------------------------------------------------------------------------*/
inline float CIGSubSysDPropulsion::GetCurrentAngle()
{
   return mCurrentAngle;
}
inline void CIGSubSysDPropulsion::SetCurrentAngle(float value)
{
   mCurrentAngle = value;
}


#endif

